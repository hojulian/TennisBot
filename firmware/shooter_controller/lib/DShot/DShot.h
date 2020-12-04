#include "esp32-hal.h"
#include <inttypes.h>

#define DSHOT_MIN 48
#define DSHOT_MAX 2047
#define DSHOT_IDLE 300

typedef struct
{
	rmt_data_t dshot_packet[16];
	rmt_obj_t *rmt_send;
	volatile uint16_t throttle;
} dshot_t;

void dshotCreateMotor(int pin, dshot_t *motor);
void dshotSetThrottle(uint16_t throttle, dshot_t *motor);
void dshotOutput(uint16_t value, rmt_data_t *dshot_packet, rmt_obj_t *rmt_send);
