/*
 * config.h
 *
 *  Created on: 2023年8月17日
 *      Author: ROG
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

//#define EBS_TEST
#define FRONT_AIR
//#define BACK_AIR
extern const uint8_t _oa_ratio; // 油压-气压比
extern const int32_t _release_max_oil; // 制动释放时允许的最高油压
extern const int32_t _max_air;
extern const int32_t _min_air;
extern const int32_t _lose_oil;
#endif /* INC_CONFIG_H_ */
