#pragma once

#include "gtest/gtest.h"

TEST(count, surrogateToScaler) {
	std::cout << "test";
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}