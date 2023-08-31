/*
 * ebs.h
 *
 *  Created on: Dec 31, 2022
 *      Author: thunoerobot
 */
#include "main.h"
#ifndef INC_EBS_H_
#define INC_EBS_H_

#define LV              0
#define Manual_HV       1
#define AS_HV           2
#define AS_Ready        3
#define AS_Driving      4
#define AS_Finished     5
#define AS_Emergency    6
#define AS_Driving_Stop 7

extern uint8_t EBS_error;
extern uint8_t new_state;
extern uint8_t pre_AS_state;
extern uint8_t error_state_transition;
extern uint8_t ebs_check_flag;
extern uint8_t air_lose;
extern uint8_t oil_lf_lose;
extern uint8_t oil_lb_lose;
extern uint8_t oil_rf_lose;
extern uint8_t oil_rb_lose;
extern uint8_t circuit_error;
extern uint8_t air1_path_failure;
extern uint8_t air1_overpressure;
extern uint8_t air1_insufficient;
extern uint8_t air2_path_failure;
extern uint8_t air2_overpressure;
extern uint8_t air2_insufficient;
extern uint8_t timeout;
extern uint8_t ASMS;
extern uint32_t bp[10];
extern uint32_t lose_cnt[10];
extern uint8_t bp_lose[10];
extern uint8_t ebs_ready;
void EBS_FSM();
void ebs_emergency();
void EBS_FSM_Event(uint8_t state);
void circuit_check();

#endif /* INC_EBS_H_ */
