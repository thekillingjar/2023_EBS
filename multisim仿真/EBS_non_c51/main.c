
// use UTF-8 encode
#include <reg51.h>
#define shutdown_circuit 0 // 1->sdc_close
#define reset 1 // 0->reset
#define watchdog_out 2 // 0->watchdog triggle
#define AS_mode 3 // 0->as_hv
#define brake1 4 // 1->no brake
#define brake2 5
void delay(int k) {
	int i, j;
	while (k--) {
		for (i = 0; i < 0x0f; ++i)
			for (j = 0; j < 0x2f; ++j);
	}
}
// 
void init_state() {
	P0 = (0 << shutdown_circuit) | (0 << reset) | (1 << watchdog_out) | (1 << AS_mode) | (1 << brake1) | (1 << brake2);
}
//测试低压->无人高压->低压->无人/有人高压
void LV_state() {
	P0 = (0 << shutdown_circuit) | (1 << reset) | (1 << watchdog_out) | (1 << AS_mode) | (1 << brake1) | (1 << brake2);
}
void AS_HV_state() {
	P0 = (1 << shutdown_circuit) | (1 << reset) | (1 << watchdog_out) | (1 << AS_mode) | (1 << brake1) | (1 << brake2);
	delay(1);
	P0 = (1 << shutdown_circuit) | (1 << reset) | (1 << watchdog_out) | (0 << AS_mode) | (1 << brake1) | (1 << brake2);
}
void AS_Emergency_state1() {
	P0 = (0 << shutdown_circuit) | (1 << reset) | (1 << watchdog_out) | (0 << AS_mode) | (1 << brake1) | (1 << brake2);
}
void AS_Emergency_state2() {
	P0 = (0 << shutdown_circuit) | (1 << reset) | (0 << watchdog_out) | (0 << AS_mode) | (1 << brake1) | (1 << brake2);
}
void M_HV_State() {
	P0 = (1 << shutdown_circuit) | (1 << reset) | (1 << watchdog_out) | (1 << AS_mode) | (1 << brake1) | (1 << brake2);
}
void watchdog_triggle() {
	P0 = (1 << shutdown_circuit) | (1 << reset) | (0 << watchdog_out) | (1 << AS_mode) | (1 << brake1) | (1 << brake2);
}
int main()
{
/* Insert your code here. */
	init_state();
	delay(1);
	LV_state();
	delay(1);
	M_HV_State();
	delay(1);
	watchdog_triggle();
//AS_HV_state();
//delay(1);
//AS_Emergency_state2();
	while (1) {
	}
}
