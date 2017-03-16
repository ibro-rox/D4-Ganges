#include "../comms.h"

#define BAUD 57600

void Send_data(uint8_t type, uint16_t data);
void Retrieve_data(uint8_t* type, uint16_t* data);

void Encode_data(uint8_t* type, uint8_t* data, uint16_t totalpacket);
void Decode_data(uint8_t* type, uint16_t* data, uint16_t totalpacket);

#if ENABLE_ENCRYPTION
uint16_t Encrypt_data(uint16_t packet);
uint16_t Decrypt_data(uint16_t packet);
uint16_t get_1s(uint8_t num);
#endif

void init_uart1();
void uart_transmit(char data);
void send_string(char *str);

struct euler_angles {
	uint16_t throttle;
	uint16_t yaw;
	uint16_t pitch;
	uint16_t roll;
};
uint8_t is_valid_packet(struct euler_angles *input);
void send_packet(struct euler_angles *input);

void init_adc();
uint16_t adc_read(int n);

void init_timer();

#define SERVO_PWM_DUTY_MAX 20
#define SERVO_PWM_DUTY_MIN 1
#define PWM_PRESCALER 8UL
#define PWM_FREQUENCY 50
#define PWM_OFFSET 0

void init_pwm(void);
void pwm_duty(uint8_t duty);

void init_interrupt();