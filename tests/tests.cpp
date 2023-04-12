// Copyright 2023 Gosha Serbin

#include "ConsoleCalculator.hpp"
#include "Exceptions.hpp"

#include <cmath>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

class ConsoleCalculatorTest : public testing::Test {
 protected:
    void SetUp() override {
        argc = 2;
        argv = new char*[argc];
        argv[0] = const_cast<char*>("hw2");
    }
    void TearDown() override { delete[] argv; }
    char** argv;
    int argc;
    ConsoleCalculator calculator;
    std::stringstream ss;
};

TEST_F(ConsoleCalculatorTest, Add2NumbersTest) {
    argv[1] = const_cast<char*>("1+1");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "2";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, Add10NumbersTest) {
    argv[1] = const_cast<char*>("1+2+3+4+5+6+7+8+9+10");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "55";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, SubstractNumbersTest) {
    argv[1] = const_cast<char*>("10-10");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "0";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, SubstractAndAddNumbersTest) {
    argv[1] = const_cast<char*>("1+2-2-1");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "0";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, Divide2NumbersTest) {
    argv[1] = const_cast<char*>("1/2");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "0.5";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, DividePriorityTest) {
    argv[1] = const_cast<char*>("1+1/2");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "1.5";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, BracketsTest) {
    argv[1] = const_cast<char*>("1+2-(2+2)");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "-1";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, PriorityTestWithDivideAndBrackets) {
    argv[1] = const_cast<char*>("1+3/(3-1)");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "2.5";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, TestWithUnaryMinus) {
    argv[1] = const_cast<char*>("-1+1");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "0";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, AbsFunctionTest) {
    argv[1] = const_cast<char*>("abs(-10.5)");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "10.5";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, AtanFunctionTest) {
    argv[1] = const_cast<char*>("atan(0.5)");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = std::to_string(std::atan(0.5));
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, NoInputArgumentTest) {
    argc = 1;
    EXPECT_NO_THROW(calculator.run(argc, argv, ss));
}

TEST_F(ConsoleCalculatorTest, NotEnoughBracketsTest) {
    argv[1] = const_cast<char*>("(1+(1+0)");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, TooMuchBracketsTest) {
    argv[1] = const_cast<char*>("(1+(1+0)))");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, InCorrectBracketsTest) {
    argv[1] = const_cast<char*>("(1)+1)+(1+0)");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, EmptyInputTest) {
    argv[1] = const_cast<char*>(" ");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, MissingSecondArgumentTest) {
    argv[1] = const_cast<char*>("1+");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, MissingFirstArgumentTest) {
    argv[1] = const_cast<char*>("+1");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, EmptyArgumentInBracketsTest) {
    argv[1] = const_cast<char*>("1+()");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, EmptyArgumentInFunctionTest) {
    argv[1] = const_cast<char*>("atan()");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, NoArgumentInFunctionTest) {
    argv[1] = const_cast<char*>("abs");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}

TEST_F(ConsoleCalculatorTest, InputWithSpacesTest) {
    argv[1] = const_cast<char*>("1 + 2 -    3 / 1 + atan  ( 0 )");
    calculator.run(argc, argv, ss);
    std::string actual(ss.str());
    std::string expected = "0";
    EXPECT_EQ(expected, actual);
}

TEST_F(ConsoleCalculatorTest, UndefinedSymbolsTest) {
    argv[1] = const_cast<char*>("1+2+3+r+4");
    EXPECT_THROW(calculator.run(argc, argv, ss), Error);
}
