// Speeds are in microseconds
#define MIN_THROTTLE_IN 0.0  //in ms but not for servo, its kina arbitary
#define MAX_THROTTLE_IN 256.0  //in ms

#define LEFT_FRONT_MOTOR 0
#define RIGHT_FRONT_MOTOR  1
#define LEFT_REAR_MOTOR 2
#define RIGHT_REAR_MOTOR 3

#define TIMER_TOP 40000
//PWM VALUES FOR ESC
#define PWM_DUTY_MIN 2000.0
#define PWM_DUTY_MAX 4000.0
//PWM for mohameds servo
//#define PWM_DUTY_MIN 500
//#define PWM_DUTY_MAX 5400

#define SPEED_TO_PWM_GAIN ((PWM_DUTY_MAX-PWM_DUTY_MIN)/(MAX_THROTTLE_IN-MIN_THROTTLE_IN))

#define N_ANGLE 25.55 //scalar to convert to correct range for input angles
//#define N_THROTTLE 0.511
#define DELTA_TIME 0.01

#define GYRO_INTERRUPT 7  // use pin 2 on Arduino Uno & most boards. atmega32u4 boards should use pin 7 to avoid conflict with I2C and UART
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
#define SERIAL_INTERRUPT 6
