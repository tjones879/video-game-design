#include "gtest/gtest.h"
#include "inc/threadmanager.hpp"
#include "unistd.h"

TEST(sample_thread_case, sample_test)
{
    ThreadManager manager;
    auto fun = [](std::atomic<bool> *flag, ThreadManager * parent, int n) {
                EXPECT_EQ(1, 1);
            };
    manager.spawnThread(fun, 5);
}

TEST(sample_thread_case, SupportsSendingMessages)
{
    struct TestMessage : Message {
        int num;
        TestMessage(int n) : num(n) {}
        virtual MessageType getType() const override {
            return MessageType::INVALID;
        }
    };
    ThreadManager manager;
    manager.spawnThread([](std::atomic<bool> *flag, ThreadManager *manager, int n) {
                auto tmpMsg = std::make_unique<TestMessage>(5);
                EXPECT_EQ(tmpMsg->num, 5);
                manager->openBuffer("EXAMPLE");
                manager->sendMessage("EXAMPLE", std::move(tmpMsg));
                EXPECT_TRUE(!tmpMsg);
            }, 5);
}

TEST(sample_thread_case, SupportsGettingMessages)
{
    struct TestMessage : Message {
        int num;
        TestMessage() : num(-1) {}
        TestMessage(int n) : num(n) {}
        virtual MessageType getType() const override {
            return MessageType::INVALID;
        }
    };
    ThreadManager manager;
    manager.spawnThread([](std::atomic<bool> *flag, ThreadManager *manager, int n) {
                auto tmpMsg = std::make_unique<TestMessage>(5);
                EXPECT_EQ(tmpMsg->num, 5);
                manager->openBuffer("EXAMPLE");
                manager->sendMessage("EXAMPLE", std::move(tmpMsg));
                EXPECT_TRUE(!tmpMsg);
                auto retMsg = manager->getMessage<TestMessage>("EXAMPLE");
                EXPECT_EQ(retMsg->num, 5);
            }, 5);
}
