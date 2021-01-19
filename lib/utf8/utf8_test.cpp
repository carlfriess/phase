//
// Created by Carl Friess on 19/01/21.
//

#include "gtest/gtest.h"
#include "utf8.h"

// Some valid strings
std::string valid1 = "Hi";
std::string valid2 = "\xE2\x80\x8EHi";
std::string valid3 = "Frieß";
std::string valid4 = "Bye\xE2\x80\xA6";
std::string valid5 = "10\xC2\xA0""CHF";

// Some invalid strings
std::string invalid1 = "\xE2\x80Hi";
std::string invalid2 = "10\xC0\x80""CHF";

// Some incomplete strings
std::string incomplete1 = "Bye\xE2\x80";


TEST(Next, Valid1) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = valid1;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'H');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'i');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_INCOMPLETE);

}

TEST(Next, Valid2) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = valid2;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 0x200E);
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'H');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'i');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_INCOMPLETE);

}

TEST(Next, Valid3) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = valid3;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'F');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'r');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'i');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'e');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 0x00DF);
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_INCOMPLETE);

}

TEST(Next, Valid4) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = valid4;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'B');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'y');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'e');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 0x2026);
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_INCOMPLETE);

}

TEST(Next, Valid5) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = valid5;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, '1');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, '0');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 0x00A0);
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'C');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'H');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'F');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_INCOMPLETE);

}

TEST(Next, Invalid1) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = invalid1;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_ILLEGAL);

}

TEST(Next, Invalid2) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = invalid2;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, '1');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, '0');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_ILLEGAL);

}

TEST(Next, Incomplete1) {

    uint32_t code;
    phase::utf8::error err;
    std::string str = incomplete1;
    std::string::const_iterator it = str.begin();

    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'B');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'y');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(code, 'e');
    err = phase::utf8::next(it, str.end(), code);
    ASSERT_EQ(err, phase::utf8::UTF8_INCOMPLETE);

}


TEST(Strlen, Valid1) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(valid1, len);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(len, 2);

}

TEST(Strlen, Valid2) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(valid2, len);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(len, 3);

}

TEST(Strlen, Valid3) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(valid3, len);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(len, 5);

}

TEST(Strlen, Valid4) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(valid4, len);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(len, 4);

}

TEST(Strlen, Valid5) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(valid5, len);
    ASSERT_EQ(err, phase::utf8::UTF8_OK);
    EXPECT_EQ(len, 6);

}

TEST(Strlen, Invalid1) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(invalid1, len);
    ASSERT_EQ(err, phase::utf8::UTF8_ILLEGAL);

}

TEST(Strlen, Invalid2) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(invalid2, len);
    ASSERT_EQ(err, phase::utf8::UTF8_ILLEGAL);

}

TEST(Strlen, Incomplete1) {

    size_t len;
    phase::utf8::error err;

    err = phase::utf8::strlen(incomplete1, len);
    ASSERT_EQ(err, phase::utf8::UTF8_INCOMPLETE);

}
