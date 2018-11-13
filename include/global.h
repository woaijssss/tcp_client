/*
 * global.h
 *
 *  Created on: Sep 30, 2018
 *      Author: wenhan
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>

namespace GLOBAL {
        typedef uint8_t u8;
        typedef uint16_t u16;
        typedef uint32_t u32;
        typedef uint64_t u64;
        typedef bool Bool;

        enum TF {
                FALSE, TRUE
        };

#define CONNECTED           0
#define DISCONNECTED        1

}        // namespace GLOBAL

#endif /* GLOBAL_H_ */
