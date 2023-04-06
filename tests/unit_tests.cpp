// Copyright 2023 Gosha Serbin

#include "Artist.hpp"
#include "InputParser.hpp"
#include "MusicDataBaseHandler.hpp"
#include "OutputFormatter.hpp"

#include <gtest/gtest.h>
#include <sstream>

class ReadArgsTest : public testing::Test {
 protected:
    void SetUp() override {
        argc = 7;
        argv = new char*[argc];
        argv[0] = const_cast<char*>("hw1");
        argv[1] = const_cast<char*>("--artist_file_path");
        argv[2] = const_cast<char*>("artist");
        argv[3] = const_cast<char*>("--artist_name");
        argv[4] = const_cast<char*>("Wildways");
        argv[5] = const_cast<char*>("--gender_file_path");
        argv[6] = const_cast<char*>("--gender");
    }

    void TearDown() override { delete[] argv; }

    char** argv;
    int argc;
};

TEST_F(ReadArgsTest, ReturnDataReceivedValueTest) {
    InputData data;
    std::stringstream ss;
    ReturnValues actual = InputParser::readArgs(argc, argv, data, ss);
    ReturnValues expected = ReturnValues::dataReceived;
    EXPECT_EQ(expected, actual);
}

TEST_F(ReadArgsTest, ReturnDataNotReceivedValueTest) {
    InputData data;
    std::stringstream ss;
    argc = 2;
    ReturnValues actual = InputParser::readArgs(argc, argv, data, ss);
    ReturnValues expected = ReturnValues::dataNotReceived;
    EXPECT_EQ(expected, actual);
}

TEST_F(ReadArgsTest, CheckArtistFilePathTest) {
    InputData data;
    std::stringstream ss;
    InputParser::readArgs(argc, argv, data, ss);
    std::string actual = data.artistFilePath;
    std::string expected = "artist";
    EXPECT_EQ(expected, actual);
}

TEST_F(ReadArgsTest, CheckArtistNameTest) {
    InputData data;
    std::stringstream ss;
    InputParser::readArgs(argc, argv, data, ss);
    std::string actual = data.artistName;
    std::string expected = "Wildways";
    EXPECT_EQ(expected, actual);
}

class MusicDataBaseHandlerTest : public testing::Test {
 protected:
    void SetUp() override {
        artistInfo = "Field 0\tField 1\tMETALLICA\tField 3\t1984";
        genderInfo << "0\tMale\n1\tFemale\n2\tOther";
    }
    void TearDown() override {}

    MusicDataBaseHandler db;
    std::string artistInfo;
    std::stringstream genderInfo;
};

TEST_F(MusicDataBaseHandlerTest, getFieldNameTest) {
    std::string actual(db.getField(MusicDataBaseHandler::fieldPosition::NAME, artistInfo));
    std::string expected("METALLICA");
    EXPECT_EQ(expected, actual);
}

TEST_F(MusicDataBaseHandlerTest, getFieldYearTest) {
    std::string actual(db.getField(MusicDataBaseHandler::fieldPosition::YEAR, artistInfo));
    std::string expected("1984");
    EXPECT_EQ(expected, actual);
}

TEST_F(MusicDataBaseHandlerTest, getNonexistingFieldTest) {
    std::string actual(db.getField(MusicDataBaseHandler::fieldPosition::DAY, artistInfo));
    std::string expected("");
    EXPECT_EQ(expected, actual);
}

TEST_F(MusicDataBaseHandlerTest, fillMapGenderByIDSizeTest) {
    db.fillMapGenderByID(genderInfo);
    size_t actual = db.genderByID.size();
    size_t expected = 3;
    EXPECT_EQ(expected, actual);
}

TEST_F(MusicDataBaseHandlerTest, fillMapGenderByIDMaleTest) {
    db.fillMapGenderByID(genderInfo);
    std::string actual = db.genderByID.at(0);
    std::string expected("Male");
    EXPECT_EQ(expected, actual);
}

TEST_F(MusicDataBaseHandlerTest, fillMapGenderByIDFemaleTest) {
    db.fillMapGenderByID(genderInfo);
    std::string actual = db.genderByID.at(1);
    std::string expected("Female");
    EXPECT_EQ(expected, actual);
}

TEST_F(MusicDataBaseHandlerTest, fillMapGenderByIDOtherTest) {
    db.fillMapGenderByID(genderInfo);
    std::string actual = db.genderByID.at(2);
    std::string expected("Other");
    EXPECT_EQ(expected, actual);
}

class OutputFormatterTest : public testing::Test {
 protected:
    void SetUp() override {
        artist.name = "Muse";
        artist.gender = "Other";
        artist.day = artist.month = artist.year = out.emptyFieldValue;
    }
    void TearDown() override {}

    OutputFormatter out;
    Artist artist;
};

TEST_F(OutputFormatterTest, AllFieldAreEmptyFalseTest) {
    EXPECT_FALSE(out.allFieldsAreEmpty(artist));
}

TEST_F(OutputFormatterTest, AllFieldAreEmptyTrueTest) {
    artist.name = artist.gender = out.emptyFieldValue;
    EXPECT_TRUE(out.allFieldsAreEmpty(artist));
}
