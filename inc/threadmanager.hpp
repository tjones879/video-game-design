#pragma once

#include <thread>
#include <string>
#include <array>
#include <map>
#include <memory>
#include <deque>
#include <atomic>
#include <mutex>
#include <type_traits>

enum class MessageType : char {
    INVALID
};

struct Message {
    virtual MessageType getType() const = 0;
};

class ThreadBuffer {
private:
    std::mutex mut;
    std::deque<std::unique_ptr<Message>> buffer;
public:
    ThreadBuffer() = default;
    ThreadBuffer(ThreadBuffer &&rhs)
    {
        std::lock_guard<std::mutex> lock(rhs.mut);
        buffer = std::move(rhs.buffer);
    }

    template <class T>
    void addMessage(std::unique_ptr<T>&& msg)
    {
        static_assert(std::is_base_of<Message, T>::value,
                      "MessageType must be derived from Message");
        std::lock_guard<std::mutex> lock(mut);
        buffer.push_back(std::forward<std::unique_ptr<T>>(msg));
    }

    template <class T>
    std::unique_ptr<T> getMessage()
    {
        std::unique_ptr<T> msg;
        static_assert(std::is_base_of<Message, T>::value,
                      "MessageType must be derived from Message");
        {
            std::unique_lock<std::mutex> lock(mut);
            if (buffer.size() > 0) {
                // Copy the unique ptr and unlock
                auto uniqueMsg = std::move(buffer.back());
                buffer.pop_back();
                lock.unlock();

                // Cast the ABC Message pointer to T.
                auto msgPtr = dynamic_cast<T *>(uniqueMsg.get());
                uniqueMsg.release();
                msg.reset(msgPtr);
            }
        }
        return msg;
    }
};

class ThreadManager {
    std::map<std::thread::id, std::shared_ptr<std::thread>> threads;
    std::map<std::string, std::pair<std::thread::id, ThreadBuffer>> buffers;
public:
    std::atomic<bool> stopThreads; ///< Denote whether or not we want to kill the program.
    ThreadManager() : stopThreads(false) {}

    ~ThreadManager()
    {
        waitAll();
    }

    /**
     * Spawn a thread to be managed.
     */
    template <typename FunType, typename... FunArgs>
    void spawnThread(FunType &&function, FunArgs&&... args)
    {
        auto thread = new std::thread(function,
                                      &stopThreads,
                                      this,
                                      std::forward<FunArgs>(args)...);
        auto threadID = thread->get_id();
        threads.emplace(std::make_pair(threadID, thread));
    }

    void openBuffer(std::string tag)
    {
        std::thread::id id = std::this_thread::get_id();
        buffers.emplace(std::piecewise_construct,
                        std::forward_as_tuple(tag),
                        std::forward_as_tuple(std::make_pair(id, ThreadBuffer())));
    }

    void closeBuffer(std::string tag)
    {
        auto id = std::this_thread::get_id();
        auto bufferLoc = buffers.find(tag);

        if (bufferLoc != buffers.end() && id == bufferLoc->second.first) {
            buffers.erase(bufferLoc);
        }
    }

    void waitAll()
    {
        stopThreads = true;
        for (auto pair : threads) {
            auto threadPtr = pair.second;

            threadPtr->join();
            threads.erase(pair.first);
        }
    }

    template<typename T>
    void sendMessage(std::string tag, std::unique_ptr<T>&& message)
    {
        auto loc = buffers.find(tag);
        if (loc != buffers.end())
            loc->second.second.addMessage(std::move(message));
    }

    template<typename T>
    std::unique_ptr<T> getMessage(std::string tag)
    {
        auto id = std::this_thread::get_id();
        auto bufferLoc = buffers.find(tag);
        if (bufferLoc != buffers.end() && id == bufferLoc->second.first) {
            auto buffer = &bufferLoc->second.second;
            return buffer->getMessage<T>();
        }
        return std::make_unique<T>();
    }
};
