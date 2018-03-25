#include "gtest/gtest.h"
#include "inc/threadmanager.hpp"
#include "unistd.h"

TEST(sample_thread_case, sample_test)
{
    ThreadManager manager;
    manager.spawnThread("TEST", [](std::atomic<bool> *flag, int n){
                std::cout << "n: " << n << std::endl;
            }, 5);
    sleep(0);
    EXPECT_EQ(1, manager.threads.size());
}
