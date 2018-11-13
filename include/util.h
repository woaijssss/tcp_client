/*
 * util.h
 *
 *  Created on: Nov 6, 2018
 *      Author: wenhan
 */

#ifndef INCLUDE_UTIL_H_
#define INCLUDE_UTIL_H_

#include <string>
#include "global.h"

/* base64编码 */
std::string b64Encode(const std::string& s);

/* base64解码 */
std::string b64Decode(const std::string& s);

/* 获取服务的本地时间戳，例： 1970-01-01T08:00:00.000+08:00*/
std::string getServerTime();

/* 获取s的十六进制形式字符串 */
std::string getHexString(GLOBAL::u8* s, const GLOBAL::u32& len);

/* 获取毫秒级时间 */
long getMilliseconds();

#endif /* INCLUDE_UTIL_H_ */
