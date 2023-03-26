#include <gtest/gtest.h>
#include "utils.hpp"

TEST(getFieldTest, getExistingFields) {
    std::string line("f0\tf1\tf2\t\n");
    EXPECT_EQ(Utils::getField(0, line), "f0");
    EXPECT_EQ(Utils::getField(1, line), "f1");
    EXPECT_EQ(Utils::getField(2, line), "f2");
}

TEST(getFieldTest, getNonExistingFields) {
    EXPECT_EQ(Utils::getField(2, "0\t1"), "");
    EXPECT_EQ(Utils::getField(1, "hello"), "");
    EXPECT_EQ(Utils::getField(5, "h\te\tl\tl\to"), "");
}

TEST(fillMapGenderByIDTest, basicInput) {
    std::unordered_map<size_t, std::string> genderByID;
    std::stringstream ss;
    ss << "0\tMale\n1\tFemale\n2\tOther";
    Utils::fillMapGenderByID(ss, &genderByID);
    EXPECT_EQ(genderByID.size(), 3);
    EXPECT_EQ(genderByID.at(0), "Male");
    EXPECT_EQ(genderByID.at(1), "Female");
    EXPECT_EQ(genderByID.at(2), "Other");
}

TEST(printArtistTest, emptyInput) {
    std::unordered_map<size_t, std::string> genderByID;
    std::string input("0\t1\t2\t3\t\\N\t\\N\t\\N\t7\t8\t9\t10\t11\t\\N");
    std::stringstream ssOut;
    Utils::printArtist(input, genderByID, ssOut);
    std::string out;
    ssOut >> out;
    EXPECT_EQ(out, "");
}