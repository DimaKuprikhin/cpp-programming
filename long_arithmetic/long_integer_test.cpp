#include <gtest/gtest.h>
#include "long_integer.h"

TEST(LongIntegerTest, ConstructFromStringTest) {
    EXPECT_EQ(LongInteger("455"), 455);
    EXPECT_EQ(LongInteger("+1"), 1);
    EXPECT_EQ(LongInteger("-1000000001"), -1000000001);
}

TEST(LongIntegerTest, OperatorPlusTest) {
    EXPECT_EQ(LongInteger("12") + LongInteger(1000), 1012);
    EXPECT_EQ(LongInteger("-12") + LongInteger(1000), 988);
    EXPECT_EQ(LongInteger("12") + LongInteger(-1000), -988);
    EXPECT_EQ(LongInteger("-12") + LongInteger(-1000), -1012);
}

TEST(LongIntegerTest, OperatorMinusTest) {
    EXPECT_EQ(LongInteger("12") - LongInteger(1000), -988);
    EXPECT_EQ(LongInteger("-12") - LongInteger(1000), -1012);
    EXPECT_EQ(LongInteger("12") - LongInteger(-1000), 1012);
    EXPECT_EQ(LongInteger("-12") - LongInteger(-1000), 988);
}

TEST(LongIntegerTest, OperatorLessTest) {
    EXPECT_EQ(LongInteger("12") < LongInteger(1000), true);
    EXPECT_EQ(LongInteger("-12") < LongInteger(1000), true);
    EXPECT_EQ(LongInteger("12") < LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("-12") < LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("12") < LongInteger(12), false);
    EXPECT_EQ(LongInteger("-12") < LongInteger(-12), false);
}

TEST(LongIntegerTest, OperatorGreaterTest) {
    EXPECT_EQ(LongInteger("12") > LongInteger(1000), false);
    EXPECT_EQ(LongInteger("-12") > LongInteger(1000), false);
    EXPECT_EQ(LongInteger("12") > LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("-12") > LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("12") > LongInteger(12), false);
    EXPECT_EQ(LongInteger("-12") > LongInteger(-12), false);
}

TEST(LongIntegerTest, OperatorLessOrEqualTest) {
    EXPECT_EQ(LongInteger("12") <= LongInteger(1000), true);
    EXPECT_EQ(LongInteger("-12") <= LongInteger(1000), true);
    EXPECT_EQ(LongInteger("12") <= LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("-12") <= LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("12") <= LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12") <= LongInteger(-12), true);
}

TEST(LongIntegerTest, OperatorGreaterOrEqualTest) {
    EXPECT_EQ(LongInteger("12") >= LongInteger(1000), false);
    EXPECT_EQ(LongInteger("-12") >= LongInteger(1000), false);
    EXPECT_EQ(LongInteger("12") >= LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("-12") >= LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("12") >= LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12") >= LongInteger(-12), true);
}

TEST(LongIntegerTest, OperatorEqualTest) {
    EXPECT_EQ(LongInteger("12") == LongInteger(1000), false);
    EXPECT_EQ(LongInteger("-12") == LongInteger(1000), false);
    EXPECT_EQ(LongInteger("12") == LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("-12") == LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("12") == LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12") == LongInteger(-12), true);
    EXPECT_EQ(LongInteger("0") == LongInteger(0), true);
    EXPECT_EQ(LongInteger("-0") == LongInteger(0), true);
    EXPECT_EQ(LongInteger("0") == LongInteger(-0), true);
    EXPECT_EQ(LongInteger("-0") == LongInteger(-0), true);
}

TEST(LongIntegerTest, AbsTest) {
    EXPECT_EQ(LongInteger("12").Abs() == LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12").Abs() == LongInteger(12), true);
}
