#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Simple smoke test to verify Google Test is working
TEST(SmokeTest, GoogleTestIsWorking) {
    EXPECT_EQ(1 + 1, 2);
    ASSERT_TRUE(true);
}

TEST(SmokeTest, GMockIsWorking) {
    // Simple mock example
    class MockCalculator {
    public:
        MOCK_METHOD(int, Add, (int a, int b), ());
    };

    MockCalculator mock_calc;
    EXPECT_CALL(mock_calc, Add(2, 3))
        .WillOnce(::testing::Return(5));

    int result = mock_calc.Add(2, 3);
    EXPECT_EQ(result, 5);
}

// Test version functionality
TEST(VersionTest, VersionHeaderExists) {
    // This test will verify that the version header is accessible
    // You can expand this to test actual version values
    EXPECT_TRUE(true); // Placeholder
}
