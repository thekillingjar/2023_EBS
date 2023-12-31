/*
 * config.c
 *
 *  Created on: 2023年8月17日
 *      Author: ROG
 */

#include "main.h"
#include "config.h"
const uint8_t _oa_ratio = 4; // 油压-气压比
const int32_t _release_max_oil = 150; // 制动释放时允许的最高油压
const int32_t _max_air = 800;
const int32_t _min_air = 450;
const int32_t _lose_oil = 3;
