#pragma once

#include <thread>
#include <string>
#include <array>
#include <map>
#include <memory>
#include <deque>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <type_traits>
#include "inc/messagetypes.hpp"

class ThreadBuffer {
private:
    std::recursive_mutex mut;
    typedef std::unique_lock<std::recursive_mutex> LockGuard;
    std::deque<std::unique_ptr<Message>> buffer;
public:
    ThreadBuffer() = default;
    ThreadBuffer(ThreadBuffer &&rhs)
    {
        LockGuard lock(rhs.mut);
        buffer = std::move(rhs.buffer);
    }

    template <class T>
    void addMessage(std::unique_ptr<T>&& msg)
    {
        static_assert(std::is_base_of<Message, T>::value,
                      "MessageType must be derived from Message");
        LockGuard lock(mut);
        buffer.push_back(std::forward<std::unique_ptr<T>>(msg));
    }

    template <class T>
    std::unique_ptr<T> getMessage()
    {
        std::unique_ptr<T> msg;
        static_assert(std::is_base_of<Message, T>::value,
                      "MessageType must be derived from Message");
        {
            LockGuard lock(mut);
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

    size_t getSize() {
        LockGuard lock(mut);
        return buffer.size();
    }
};

class ThreadManager {
    std::map<std::thread::id, std::shared_ptr<std::thread>> threads;
    std::map<std::string, std::pair<std::thread::id, ThreadBuffer>> buffers;
    /**
     * This mutex allows multiple threads to read and write to buffers,
     * but only one open or close them.
     */
    std::shared_timed_mutex bufferList;
public:
    std::atomic<bool> stopThreads; ///< Denote whether or not we want to kill the program.
    ThreadManager() : stopThreads(false) {}

    ~ThreadManager()
    {
        waitAll();
    }

    /**
     * Spawn a thread to be managed.
     *
     * @param function A callable object that accepts a std::atomic<bool> *, ThreadManager *
     *                 as the first two arguments.
     * @param args     The arguments that should be passed to the function.
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
        std::unique_lock<std::shared_timed_mutex> lock(bufferList);
        buffers.emplace(std::piecewise_construct,
                        std::forward_as_tuple(tag),
                        std::forward_as_tuple(std::make_pair(id, ThreadBuffer())));
    }

    void closeBuffer(std::string tag)
    {
        auto id = std::this_thread::get_id();
        std::unique_lock<std::shared_timed_mutex> lock(bufferList);
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
        auto optional = getReadableBuffer(tag);
        if (optional.first)
            return optional.second->getMessage<T>();

        return std::make_unique<T>();
    }

    bool newMessages(std::string tag)
    {
        auto optional = getReadableBuffer(tag);
        if (optional.first)
            return optional.second->getSize();

        return false;
    }
private:
    /**
     * Get a buffer if the thread has ownership of it.
     * @return A pair with
     *      child one denoting if is possible to access the given thread.
     *      child two pointing to the buffer iff the first child is true.
     */
    std::pair<bool, ThreadBuffer *> getReadableBuffer(std::string tag)
    {
        std::shared_lock<std::shared_timed_mutex> lock(bufferList);
        auto id = std::this_thread::get_id();
        auto bufferLoc = buffers.find(tag);
        if (bufferLoc != buffers.end() && id == bufferLoc->second.first) {
            return std::make_pair(true, &bufferLoc->second.second);
        }

        return std::make_pair(false, nullptr);
    }
};
