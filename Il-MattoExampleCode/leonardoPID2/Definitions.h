// Speeds are in microseconds
#define MIN_MOTOR_SPEED 1000  //in ms but not for servo, its kina arbitary
#define MAX_MOTOR_SPEED 2000  //in ms

#define LEFT_FRONT_MOTOR 0
#define RIGHT_FRONT_MOTOR  1
#define LEFT_REAR_MOTOR 2
#define RIGHT_REAR_MOTOR 3

#define TIMER_TOP 40000
//PWM VALUES FOR ESC
//#define PWM_DUTY_MIN 2000
//#define PWM_DUTY_MAX 4000
//PWM for mohameds servo
#define PWM_DUTY_MIN 500
#define PWM_DUTY_MAX 5000

#define SPEED_TO_PWM_GAIN ((PWM_DUTY_MAX-PWM_DUTY_MIN)/(MAX_MOTOR_SPEED-MIN_MOTOR_SPEED))

#define N_ANGLE 51.1
#define N_THROTTLE 0.511
#define DELTA_TIME 0.01
