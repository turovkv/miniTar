#include <sstream>
#include <iostream>
#include <vector>
#include "doctest.h"
#include "BitStream.h"

using namespace BitStream;
using std::stringstream;

TEST_CASE("oBitStream") {
    SUBCASE ("close") {
        stringstream s;
        oBitStream obs(s);
        obs << 1;
        obs << 1;
        obs.close();
        CHECK(s.get() == 3);
    }
    SUBCASE ("<< + destruct 1") {
        stringstream s;
        {
            oBitStream obs(s);
            obs << 1 << 0 << 1;
        }
        CHECK(s.get() == 5);
    }
    SUBCASE ("<< + destruct 2") {
        stringstream s;
        {
            oBitStream obs(s);
            obs << 1 << 0 << 1 << 0 << 0 << 0 << 0 << 0;
        }
        CHECK(s.get() == 5);
    }
    SUBCASE ("<< + destruct 3") {
        stringstream s;
        {
            oBitStream obs(s);
            obs << 1 << 0 << 1 << 0 << 0 << 0 << 0 << 0;
            obs << 1;
        }
        CHECK(s.get() == 5);
        CHECK(s.get() == 1);
    }
    SUBCASE (" write 1") {
        stringstream s;
        oBitStream obs(s);
        uint8_t byte1 = 10;
        obs.write(&byte1, 1);

        CHECK(s.get() == 10);
    }
    SUBCASE (" write 2") {
        stringstream s;
        oBitStream obs(s);
        obs << 1;
        uint8_t byte1 = 10;
        obs.write(&byte1, 1);

        CHECK(s.get() == 21);
    }
    SUBCASE (" write 3") {
        stringstream s;
        oBitStream obs(s);
        obs << 1;
        uint16_t byte1 = 255;
        obs.write(reinterpret_cast<uint8_t *>(&byte1), 2);

        CHECK(s.get() == 255);
        CHECK(s.get() == 1);
    }
    SUBCASE ("clear") {
        stringstream s;
        {
            oBitStream obs(s);
            obs << 1;
            obs.clear();
            obs << 1;
        }
        CHECK(s.get() == 1);
    }
}

TEST_CASE("iBitStream") {
    SUBCASE (">>") {
        stringstream s;
        s.put(2);
        s.put(3);
        s.seekg(0, std::ios::beg);
        iBitStream ibs(s);
        uint8_t byte = 0;
        bool bit = false;
        for (int i = 0; i < 8; i++) {
            ibs >> bit;
            byte += static_cast<uint8_t>(bit) << i;
        }
        CHECK(byte == 2);
        byte = 0;
        for (int i = 0; i < 8; i++) {
            ibs >> bit;
            byte += static_cast<uint8_t>(bit) << i;
        }
        CHECK(byte == 3);
    }
    SUBCASE ("read 1") {
        stringstream s;
        s.put(2);
        s.put(3);
        s.seekg(0, std::ios::beg);
        iBitStream ibs(s);
        uint16_t var = 0;
        ibs.read(reinterpret_cast<uint8_t *>(&var), 2);
        CHECK(var == 2 + 3 * (UINT8_MAX + 1));
    }
    SUBCASE ("read 2") {
        stringstream s;
        s.put(2);
        s.put(3);
        s.seekg(0, std::ios::beg);
        iBitStream ibs(s);
        bool tmp = false;
        ibs >> tmp >> tmp;
        uint8_t var = 0;
        ibs.read(&var, 1);
        CHECK(var == 128 + 64);
    }
    SUBCASE ("clear") {
        stringstream s;
        s.put(3);
        s.put(0);
        s.seekg(0, std::ios::beg);

        iBitStream ibs(s);
        bool bit = false;
        ibs >> bit;
        CHECK(bit == true);
        ibs.clear();
        ibs >> bit;
        CHECK(bit == false);
    }
    SUBCASE ("reopen") {
        stringstream s;
        s.put(2);
        s.seekg(0, std::ios::beg);

        iBitStream ibs(s);
        uint8_t byte = 0;
        ibs.read(&byte, 1);
        CHECK(byte == 2);
        ibs.reopen();
        byte = 0;
        ibs.read(&byte, 1);
        CHECK(byte == 2);
    }
    SUBCASE ("close") {
        // empty;
    }
}

TEST_CASE("iBitStream_iterator") {
    SUBCASE("construct 1") {
        iBitStream_iterator it;
        CHECK((bool)it == false);
    }
    SUBCASE("construct 2") {
        stringstream s("hello");
        iBitStream ibs(s);
        iBitStream_iterator it(ibs);
        CHECK((bool)it == true);
    }
    SUBCASE("operator* ") {
        stringstream s;
        s.put(3);
        iBitStream ibs(s);
        iBitStream_iterator it(ibs);
        CHECK(*it == 1);
    }
    SUBCASE("operator++ 1") {
        stringstream s;
        s.put(10);
        iBitStream ibs(s);
        iBitStream_iterator it(ibs);
        std::vector<bool> tmp;
        for (int i = 0; i < 8; i++) {
            tmp.emplace_back(*it);
            ++it;
        }
        CHECK(tmp == std::vector<bool>{0, 1, 0, 1, 0, 0, 0, 0});
    }
    SUBCASE("operator++ 2") {
        stringstream s;
        s.put(10);
        s.put(-1);
        iBitStream ibs(s);
        iBitStream_iterator it(ibs);
        std::vector<bool> tmp;
        for (int i = 0; i < 8; i++) {
            tmp.emplace_back(*it);
            ++it;
        }
        CHECK(tmp == std::vector<bool>{0, 1, 0, 1, 0, 0, 0, 0});
        tmp.clear();
        for (int i = 0; i < 8; i++) {
            tmp.emplace_back(*it);
            ++it;
        }
        CHECK(tmp == std::vector<bool>{1, 1, 1, 1, 1, 1, 1, 1});
    }
    SUBCASE("operator == 1") {
        stringstream s("hello");
        iBitStream ibs(s);
        iBitStream_iterator i1(ibs);
        iBitStream_iterator i2(ibs);
        CHECK(i1 == i2);
    }
    SUBCASE("operator == 2") {
        iBitStream_iterator i1;
        iBitStream_iterator i2;
        CHECK(i1 == i2);
    }
    SUBCASE("operator != 1") {
        stringstream s("hello");
        iBitStream ibs(s);
        iBitStream_iterator i1(ibs);
        iBitStream_iterator i2;
        CHECK(i1 != i2);
    }
}
