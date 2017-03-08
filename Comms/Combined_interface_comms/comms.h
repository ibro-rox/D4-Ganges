// Define codes for the different commands
#define OP_THRUST 0
#define OP_ROLL 1
#define OP_YAW 2
#define OP_PITCH 3
#define OP_BUTTON 4
#define OP_KP_THRUST 5
#define OP_KI_THRUST 6
#define OP_KD_THRUST 7
#define OP_KP_ROLL 8
#define OP_KI_ROLL 9
#define OP_KD_ROLL 10
#define OP_KP_YAW 11
#define OP_KI_YAW 12
#define OP_KD_YAW 13
#define OP_KP_PITCH 14
#define OP_KI_PITCH 15
#define OP_KD_PITCH 16

// K value size
#define CHAR_MAX 6

// Define data code for the button command
#define BTN_CARGO_HOOK_UP 0
#define BTN_CARGO_HOOK_DOWN 1
#define	BTN_PID_MODE 2 // Allow setting of K values and disable telemetry
#define	BTN_FLIGHT_MODE 3 // Receive telemetry

// Pin definitions
#define	PIN_THRUST 0
#define	PIN_ROLL 1
#define	PIN_YAW 2
#define	PIN_PITCH 3

// Define bit sizes for the packets
#define DATA_BIT_SIZE 10
#define COMMAND_BIT_SIZE 5
#define ENCRYPT_KEY_BIT_SIZE 1

// Enable comms
#define ENABLE_UPLINK 1
#define ENABLE_DOWNLINK 1

// Enable encryption
#define ENABLE_ENCRYPTION 0

// Enable/configure UI and setting K values
#define ENABLE_UI 1
#define K_RANGE_MIN 0
#define K_RANGE_MAX 10.23

// Enable controls
#define ENABLE_CONTROLS 0

// Enable double transceivers
#define DOUBLE_TRANSCEIVERS 0

// Enable isolated uplink/downlink tests
#define UPLINK_TEST 0
#define DOWNLINK_TEST 0