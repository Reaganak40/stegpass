#include <gtest/gtest.h>
#include "backend_testing_environment.hpp"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new BackendTestingEnvironment);

    return RUN_ALL_TESTS();
}