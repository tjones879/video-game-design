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
    std::deque<Message> buffer;
public:
    template <class T>
    void addMessage(T&& msg)
    {
        static_assert(std::is_base_of<Message, T>::value,
                      "MessageType must be derived from Message");
        std::lock_guard<std::mutex> lock(mut);
        buffer.push_back(std::forward<T>(msg));
    }

    template <class T>
    T getMessage()
    {
        static_assert(std::is_base_of<Message, T>::value,
                      "MessageType must be derived from Message");
        T msg;
        {
            std::unique_lock<std::mutex> lock(mut);
            if (buffer.size() > 0) {
                msg = static_cast<T>(buffer.back());
                buffer.pop_back();
            }
        }
        return msg;
    }
};

template <typename... Ts>
class ThreadManager {
public:
    std::map<std::string, std::shared_ptr<std::thread>> threads;
    std::map<std::string, ThreadBuffer> buffers;
    std::atomic<bool> stopThreads; ///< Denote whether or not we want to kill the program.
    ThreadManager() : stopThreads(false) {}

    ~ThreadManager()
    {
        waitAll();
    }

    /**
     * Spawn a thread to be managed.
     */
    template <class FunType, class... FunArgs>
    void spawnThread(std::string tag, FunType &&function, FunArgs&&... args)
    {
        buffers.emplace(std::make_pair(tag, ThreadBuffer()));
        auto thread = new std::thread(function, &stopThreads, std::forward<FunArgs>(args)...);
        threads.emplace(std::make_pair(tag, thread));
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
    void sendMessage(std::string tag, T message)
    {
        // TODO: Denote some kind of error condition if tag was not found
        auto loc = buffers.find(tag);
        if (loc != buffers.end())
            loc->second.addMessage(message);
    }

    template<typename T>
    T getMessage(std::string tag)
    {
        // TODO: Check to make sure that calling thread is correct one.
        return T();
    }
};
