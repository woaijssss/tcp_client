/*
 * util.cpp
 *
 *  Created on: Nov 6, 2018
 *      Author: wenhan
 */

#include <sstream>
#include <iomanip>
#include <sys/time.h>
#include "util.h"

using namespace std;
using namespace GLOBAL;

const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz0123456789+/";

static size_t index(u32 x)
{
        return (x & 0xfc0000) >> 18;
}

static u8 _chr(u32 x)
{
        return (x & 0xff0000) >> 16;
}

static u8 base64(u8 c)
{
        u8 result = 0;
        if (isupper(c)) {
                result = c - 'A';
        } else if (islower(c)) {
                result = c - 'a' + 26;
        } else if (isdigit(c)) {
                result = c - '0' + 52;
        } else if (c == '+') {
                result = 62;
        } else if (c == '/') {
                result = 63;
        }
        return result;
}

string b64Encode(const string &s)
{
        size_t i = 3;
        string ret;
        for (; i <= s.size(); i += 3) {
                const uint8_t a = s[i - 3], b = s[i - 2], c = s[i - 1];
                uint32_t x = (a << 16) | (b << 8) | c;
                for (int j = 0; j < 4; ++j, x <<= 6) {
                        ret += chars[index(x)];
                }
        }
        const size_t k = i - s.size();
        if (k == 2) {
                const u8 a = s[i - 3];
                u32 x = a << 16;
                for (int j = k; j < 4; ++j, x <<= 6) {
                        ret += chars[index(x)];
                }
                ret += "==";
        } else if (k == 1) {
                const u8 a = s[i - 3], b = s[i - 2];
                u32 x = a << 16 | b << 8;
                for (int j = k; j < 4; ++j, x <<= 6) {
                        ret += chars[index(x)];
                }
                ret += '=';
        }
        return ret;
}

string b64Decode(const string &s)
{
        if (s.empty()) return "";
        string ret;
        size_t i = 4, m = s.size();
        while (s[m - 1] == '=') {
                --m;
        }
        for (; i <= m; i += 4) {
                const u8 a = base64(s[i - 4]), b = base64(s[i - 3]);
                const u8 c = base64(s[i - 2]), d = base64(s[i - 1]);
                u32 x = (a << 18) | (b << 12) | (c << 6) | d;
                for (int j = 0; j < 3; ++j, x <<= 8) {
                        ret += _chr(x);
                }
        }
        if (m + 1 == s.size()) {
                const u8 a = base64(s[m - 3]), b = base64(s[m - 2]);
                u32 x = (a << 18) | (b << 12) | (base64(s[m - 1]) << 6);
                ret += _chr(x);
                ret += _chr(x << 8);
        } else if (m + 2 == s.size()) {
                const u8 a = base64(s[m - 2]), b = base64(s[m - 1]);
                u32 x = (a << 18) | (b << 12);
                ret += _chr(x);
        }
        return ret;
}

string getServerTime()
{
        char time_str[64] = {0};
        time_t t;
        time(&t);
        const char* format = "%Y-%m-%dT%H:%M:%S.000+08:00";
        strftime(time_str, sizeof(time_str), format, localtime(&t));
        return string(time_str);
}

string getHexString(u8* s, const u32& len)
{
        std::ostringstream out;
        out << std::hex << std::setw(2) << std::setfill('0');
        for (u32 i = 0; i < len; i++) {
                out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(s[i])
                                << " ";
        }
        return out.str();
}

long getMilliseconds()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

