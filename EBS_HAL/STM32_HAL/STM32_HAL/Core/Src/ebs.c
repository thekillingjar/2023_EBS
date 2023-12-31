/*
 * ebs.c
 *
 *  Created on: Dec 31, 2022
 *      Author: thunoerobot
 */
#include "ebs.h"
#include "main.h"
#include "tim.h"
#include "math.h"
#include "adc.h"
#include "config.h"

uint32_t lose_cnt[10];
uint8_t bp_lose[10] = {0};
uint8_t EBS_error = 0;
uint8_t new_state = 0;
uint8_t pre_AS_state = 0;
uint8_t error_state_transition = 0;
uint8_t oil_lf_lose = 0;
uint8_t oil_lb_lose = 0;
uint8_t oil_rf_lose = 0;
uint8_t oil_rb_lose = 0;
uint8_t air_lose = 0;
uint8_t circuit_error = 0;
uint8_t air1_path_failure = 0;
uint8_t air1_insufficient = 0;
uint8_t air1_overpressure = 0;
uint8_t air2_path_failure = 0;
uint8_t air2_insufficient = 0;
uint8_t air2_overpressure = 0;
uint8_t error_state_cnt = 0;
uint8_t timeout = 0;
uint8_t ASMS = 0;
uint32_t bp[10];
uint8_t as_driving_air_check_flag = 0;
uint8_t as_hv_air_check_flag = 0;
uint8_t as_ready_air_check_flag = 0;
uint8_t lv_air_check_flag = 0;
uint8_t ebs_ready = 0;

uint32_t MIN(uint32_t x, uint32_t y) {
	return x < y ? x : y;
}
uint32_t MAX(uint32_t x, uint32_t y) {
	return x > y ? x : y;
}
uint32_t MIN4(uint32_t x, uint32_t y, uint32_t u, int32_t v) {
	uint32_t x1 = MIN(x, y);
	uint32_t y1 = MIN(u, v);
	return x1 < y1 ?  x1 : y1;
}
uint32_t MAX4(uint32_t x, uint32_t y, uint32_t u, int32_t v) {
	uint32_t x1 = MAX(x, y);
	uint32_t y1 = MAX(u, v);
	return x1 > y1 ?  x1 : y1;
}
void EBS_FSM() {
    switch(pre_AS_state) {
    case LV:
        if (ASMS == 1 && adc_f) {
            adc_f = 0;
            if (lv_air_check_flag == 0) {
                lv_air_check_flag = 1;
                air_time_flag = 1;
                air_time = 0;
            }
            else if (lv_air_check_flag == 1) {
            	uint8_t flag1 = 1, flag2 = 1;
#ifdef FRONT_AIR
            	if (bp[1] < _min_air) {
            		flag1 = 0;
            	}
#endif
#ifdef BACK_AIR
            	if (bp[2] < _min_air) {
            		flag2 = 0;
            	}
#endif
            	if (flag1 && flag2) {
            		lv_air_check_flag = 2;
            	}
            }
            else if (lv_air_check_flag == 2) {
                air_time_flag = air_time = 0;
#ifdef FRONT_AIR
                if (bp[1] < _min_air || bp[1] > _max_air || MIN(bp[3], bp[4]) < _oa_ratio * bp[1]) {
                	if (bp[1] < _min_air) air1_insufficient = 1;
                	if (bp[1] > _max_air) air1_overpressure = 1;
                	if (MIN(bp[3], bp[4]) < _oa_ratio * bp[1]) air1_path_failure = 1;
                }
#endif
#ifdef BACK_AIR
                if (bp[2] < _min_air || bp[2] > _max_air || MIN(bp[5], bp[6]) < _oa_ratio * bp[2]) {
                	EBS_error = 1;
                	if (bp[2] < _min_air) air2_insufficient = 1;
                	if (bp[2] > _max_air) air2_overpressure = 1;
                	if (MIN(bp[5], bp[6]) < _oa_ratio * bp[2]) air2_path_failure = 1;
                }
#endif
            }
        }
        if (air_time > 100 && air_time_flag) { // 5s
            air_time = air_time_flag = 0;
            lv_air_check_flag = 2;
            EBS_error = 1;
            timeout = 1;
#ifdef FRONT_AIR
        	if (bp[1] < _min_air) air1_insufficient = 1;
        	if (bp[1] > _max_air) air1_overpressure = 1;
        	if (MIN(bp[3], bp[4]) < _oa_ratio * bp[1]) air1_path_failure = 1;
#endif
#ifdef BACK_AIR
        	if (bp[2] < _min_air) air2_insufficient = 1;
        	if (bp[2] > _max_air) air2_overpressure = 1;
        	if (MIN(bp[5], bp[6]) < _oa_ratio * bp[2]) air2_path_failure = 1;
#endif
        }
        if(new_state == Manual_HV) {
        	error_state_cnt = 0;
        	EBS_FSM_Event(new_state);
            pre_AS_state = new_state;
            new_state = 0xFF;
            EBS_error = 0;
            lv_air_check_flag = 0;
            air_time = air_time_flag = 0;
        }
        else if (new_state == AS_HV) {
        	error_state_cnt = 0;
        	if (lv_air_check_flag == 2) {
				EBS_FSM_Event(new_state);
				pre_AS_state = new_state;
				new_state = 0xFF;
				lv_air_check_flag = 0;
				air_time = air_time_flag = 0;
				as_hv_air_check_flag = 0;
        	}
        }
        else if (new_state == LV) {
        	error_state_cnt = 0;
        	new_state = 0xFF;
        }
        else if (new_state != 0xFF){
        	if (error_state_cnt < 3)
        		error_state_cnt++;
        	new_state = 0xFF;
        }
        break;
    case Manual_HV:
        if (new_state == LV) {
        	error_state_cnt = 0;
        	EBS_FSM_Event(new_state);
            pre_AS_state = new_state;
            new_state = 0xFF;
        }
        else if (new_state == Manual_HV) {
        	error_state_cnt = 0;
        	new_state = 0xFF;
        }
        else if (new_state == AS_HV) {
        	error_state_cnt = 0;
        	EBS_FSM_Event(new_state);
            pre_AS_state = new_state;
            new_state = 0xFF;
        }
        else if (new_state != 0xFF){
        	if (error_state_cnt < 10)
        		error_state_cnt++;
        	new_state = 0xFF;
        }
        break;
    case AS_HV:
        if (adc_f) {
            adc_f = 0;
            if (as_hv_air_check_flag == 0) {
                brake1_OFF;
                brake2_ON; // �ſ��ƶ�1
                as_hv_air_check_flag = 1;
                air_time_flag = 1;
                air_time = 0;
            }
            else if (as_hv_air_check_flag == 1) {
#ifdef FRONT_AIR
            	if (air_time > 10 && air_time_flag) {
            		if (bp[1] >= _min_air && MAX(bp[5], bp[6]) < _release_max_oil && MIN(bp[3], bp[4]) >= bp[1] * _oa_ratio) {
                		air_time = 0;
                		air_time_flag = 1;
                		as_hv_air_check_flag = 2;
                		brake2_OFF; brake1_ON;
            		}
            	}
        		if (air_time > 50) {
                   air_time = air_time_flag = 0;
                   ebs_emergency();
                   EBS_error = 1;
                   timeout = 1;
                   if (bp[1] < _min_air) air1_insufficient = 1;
                   if (MAX(bp[5], bp[6]) >= _release_max_oil) air2_path_failure = 1;
                   if (MIN(bp[3], bp[4]) < bp[1] * _oa_ratio) air1_path_failure = 1;
        		}
#else
            	as_hv_air_check_flag = 2;
            	brake2_OFF; brake1_ON;
#endif
            }
            else if (as_hv_air_check_flag == 2) {
#ifdef BACK_AIR
            	if (air_time > 10 && air_time_flag) {
            		if (bp[2] >= _min_air && MAX(bp[3], bp[4]) < _release_max_oil && MIN(bp[5], bp[6]) >= bp[2] * _oa_ratio) {
                		air_time = 0;
                		air_time_flag = 1;
                		as_hv_air_check_flag = 3;
                		brake1_OFF; brake2_OFF;
            		}
            	}
        		if (air_time > 50) {
                   air_time = air_time_flag = 0;
                   ebs_emergency();
                   EBS_error = 1;
                   timeout = 1;
                   if (bp[2] < _min_air) air2_insufficient = 1;
                   if (MAX(bp[3], bp[4]) >= _release_max_oil) air1_path_failure = 1;
                   if (MIN(bp[5], bp[6]) < bp[2] * _oa_ratio) air2_path_failure = 1;
        		}
#else
            	as_hv_air_check_flag = 3;
            	brake1_OFF; brake2_OFF;
#endif
            }
            else if (as_hv_air_check_flag == 3) {
            	if (air_time > 10 && air_time_flag) {
            		uint8_t flag1 = 1, flag2 = 1;
#ifdef FRONT_AIR
            		if (MIN(bp[3], bp[4]) < bp[1] * _oa_ratio) {
            			flag1 = 0;
            		}
#endif
#ifdef BACK_AIR
            		if (MIN(bp[5], bp[6]) < bp[2] * _oa_ratio) {
            			flag2 = 0;
            		}
#endif
            		if (flag1 && flag2) {
                		air_time = 0;
                		air_time_flag = 0;
                		as_hv_air_check_flag = 4;
            		}
            	}
                if (air_time > 50 && air_time_flag) {
                     air_time = air_time_flag = 0;
                    ebs_emergency();
                    EBS_error = 1;

#ifdef FRONT_AIR
                	if (bp[1] < _min_air) air1_insufficient = 1;
                    if (MAX(bp[3], bp[4]) < bp[1] * _oa_ratio) air1_path_failure = 1;
#endif
#ifdef BACK_AIR
                	if (bp[2] < _min_air) air2_insufficient = 1;
                    if (MAX(bp[5], bp[6]) < bp[2] * _oa_ratio) air2_path_failure = 1;
#endif
                }
            }
            else if (as_hv_air_check_flag == 4) {
            	ebs_ready = 1;
#ifdef FRONT_AIR
            	if (bp[1] < _min_air || bp[1] > _max_air || MIN(bp[3], bp[4]) < _oa_ratio * bp[1]) {
					ebs_emergency();
					EBS_error = 1;
		        	if (bp[1] < _min_air) air1_insufficient = 1;
		        	if (bp[1] > _max_air) air1_overpressure = 1;
		        	if (MIN(bp[3], bp[4]) < _oa_ratio * bp[1]) air1_path_failure = 1;

            	}
#endif
#ifdef BACK_AIR
            	if (bp[2] < _min_air || bp[2] > _max_air || MIN(bp[5], bp[6]) < _oa_ratio * bp[2]) {
					ebs_emergency();
					EBS_error = 1;
		        	if (bp[2] < _min_air) air2_insufficient = 1;
		        	if (bp[2] > _max_air) air2_overpressure = 1;
		        	if (MIN(bp[5], bp[6]) < _oa_ratio * bp[2]) air2_path_failure = 1;
            	}
#endif
            }
        }
        if (new_state == AS_Emergency) {
        	error_state_cnt = 0;
        	ebs_emergency();
        }
        else if (new_state == AS_Ready) {
        	error_state_cnt = 0;
        	if (as_hv_air_check_flag == 4) {
            	EBS_FSM_Event(new_state);
                brake1_OFF; brake2_OFF;
                air_time = 0;
                air_time_flag = 0;
                pre_AS_state = new_state;
                new_state = 0xFF;
        	}
        }
        else if (new_state == AS_HV) {
        	error_state_cnt = 0;
        	new_state = 0xFF;
        }
        else if (new_state != 0xFF){
        	error_state_cnt++;

        	if (error_state_cnt >= 10) {
        		error_state_transition = new_state;
            	EBS_FSM_Event(new_state);
            	ebs_emergency();
        	}
        	new_state = 0xFF;
        }
        break;
    case AS_Ready:
        if (adc_f) {
            adc_f = 0;
#ifdef FRONT_AIR
        	if (bp[1] < _min_air || bp[1] > _max_air || MIN(bp[3], bp[4]) < _oa_ratio * bp[1]) {
				ebs_emergency();
				EBS_error = 1;
	        	if (bp[1] < _min_air) air1_insufficient = 1;
	        	if (bp[1] > _max_air) air1_overpressure = 1;
	        	if (MIN(bp[3], bp[4]) < _oa_ratio * bp[1]) air1_path_failure = 1;
        	}
#endif
#ifdef BACK_AIR
        	if (bp[2] < _min_air || bp[2] > _max_air || MIN(bp[5], bp[6]) < _oa_ratio * bp[2]) {
				ebs_emergency();
				EBS_error = 1;
	        	if (bp[2] < _min_air) air2_insufficient = 1;
	        	if (bp[2] > _max_air) air2_overpressure = 1;
	        	if (MIN(bp[5], bp[6]) < _oa_ratio * bp[2]) air2_path_failure = 1;
        	}
#endif
        }
        if (new_state == AS_Emergency) {
        	error_state_cnt = 0;
        	EBS_FSM_Event(new_state);
            pre_AS_state = new_state;
            new_state = 0xFF;
            air_time = air_time_flag = 0;
        }
        else if (new_state == AS_Driving || new_state == AS_Driving_Stop) {
        	error_state_cnt = 0;
        	EBS_FSM_Event(new_state);
            pre_AS_state = new_state;
            new_state = 0xFF;
            brake1_ON; brake2_ON;
            air_time = 0;
            air_time_flag = 1;
        }
        else if (new_state == AS_Ready) {
        	error_state_cnt = 0;
        	new_state = 0xFF;
        }
        else if (new_state != 0xFF){
        	error_state_cnt++;

        	if (error_state_cnt >= 10) {
        		error_state_transition = new_state;
            	EBS_FSM_Event(new_state);
            	ebs_emergency();
        	}
        	new_state = 0xFF;
        }
        break;
    case AS_Driving:
        if (adc_f) {
            adc_f = 0;
            if (as_driving_air_check_flag == 0) {
            	if (air_time > 4) {
            		uint8_t flag1 = 1, flag2 = 1;
#ifdef FRONT_AIR
            		if (bp[1] < _min_air || bp[1] > _max_air) {
            			flag1 = 0;
            		}
#endif
#ifdef BACK_AIR
            		if (bp[2] < _min_air || bp[2] > _max_air) {
            			flag2 = 0;
            		}
#endif
            		if (flag1 && flag2) {
            			as_driving_air_check_flag = 1;
                		air_time = air_time_flag = 0;
            		}

            	}
                if (air_time > 20 && air_time_flag) {
                    air_time = air_time_flag = 0;
                    ebs_emergency();
                    EBS_error = 1;
#ifdef FRONT_AIR
    	        	if (bp[1] < _min_air) air1_insufficient = 1;
    	        	if (bp[1] > _max_air) air1_overpressure = 1;
#endif
#ifdef BACK_AIR
    	        	if (bp[2] < _min_air) air2_insufficient = 1;
    	        	if (bp[2] > _max_air) air2_overpressure = 1;
#endif
                }
            }
            else if (as_driving_air_check_flag == 1) {
#ifdef FRONT_AIR
            	if (bp[1] < _min_air || bp[1] > _max_air) {
                    ebs_emergency();
                    EBS_error = 1;
    	        	if (bp[1] < _min_air) air1_insufficient = 1;
    	        	if (bp[1] > _max_air) air1_overpressure = 1;

            	}
#endif
#ifdef BACK_AIR
            	if (bp[2] < _min_air || bp[2] > _max_air) {
                    ebs_emergency();
                    EBS_error = 1;
    	        	if (bp[2] < _min_air) air2_insufficient = 1;
    	           	if (bp[2] > _max_air) air2_overpressure = 1;
            	}
#endif
            }
        }
        if (new_state == AS_Finished || new_state == AS_Emergency) {
        	error_state_cnt = 0;
        	EBS_FSM_Event(new_state);
            pre_AS_state = new_state;
            new_state = 0xFF;
            air_time = air_time_flag = 0;
        }
        else if (new_state == AS_Driving || new_state == AS_Driving_Stop) {
        	error_state_cnt = 0;
        	new_state = 0xFF;
        }
        else if (new_state != 0xFF){
        	error_state_cnt++;

        	if (error_state_cnt >= 10) {
        		error_state_transition = new_state;
            	EBS_FSM_Event(new_state);
            	ebs_emergency();
        	}
        	new_state = 0xFF;
        }
        break;
    case AS_Finished:
    	if (new_state == AS_Emergency) {
    		error_state_cnt = 0;
    		ebs_emergency();
            pre_AS_state = new_state;
            new_state = 0xFF;
            air_time = air_time_flag = 0;
    	}
    	else if (new_state == AS_Finished) {
    		error_state_cnt = 0;
    		new_state = 0xFF;
    	}
    	else if (new_state != 0xFF) {
        	error_state_cnt++;

        	if (error_state_cnt >= 10) {
        		error_state_transition = new_state;
            	EBS_FSM_Event(new_state);
            	ebs_emergency();
        	}
        	new_state = 0xFF;
    	}
        break;
    case AS_Emergency:
        break;
    }
}

void ebs_emergency() {
	EBS_FSM_Event(AS_Emergency);
	pre_AS_state = AS_Emergency;
}
void EBS_FSM_Event(uint8_t state) {
	switch(state) {
	case LV:
		AS_mode_ON;
		brake1_OFF;
		brake2_OFF;
		watchdog_f = 1;
		break;
	case Manual_HV:
		AS_mode_ON; // change to manual mode
		brake1_ON;
		brake2_ON;
		watchdog_f = 1;
		break;
	case AS_HV:
		AS_mode_OFF;
		brake1_OFF;
		brake2_OFF;
		watchdog_f = 1;
		break;
	case AS_Ready:
		AS_mode_OFF;
		brake1_OFF;
		brake2_OFF;
		watchdog_f = 1;
		break;
	case AS_Driving:
		AS_mode_OFF;
		brake1_ON;
		brake2_ON;
		watchdog_f = 1;
		break;
	case AS_Emergency:
		AS_mode_OFF;
		brake1_OFF;
		brake2_OFF;
		watchdog_f = 0;
		break;
	case AS_Finished:
		AS_mode_OFF;
		brake1_OFF;
		brake2_OFF;
		watchdog_f = 0;
		break;
	case AS_Driving_Stop:
		AS_mode_OFF;
		brake1_ON;
		brake2_ON;
		watchdog_f = 1;
		break;
	default:
		AS_mode_OFF;
		brake1_OFF;
		brake2_OFF;
	}
}
void circuit_check() {
	AS_mode_OFF;
	HAL_Delay(100);
	if (HAL_GPIO_ReadPin(SDC_AS_PORT, SDC_AS_PIN) != GPIO_PIN_SET) {
		circuit_error = 1;
		EBS_error = 1;
	}
	AS_mode_ON;
	HAL_Delay(100);
}
