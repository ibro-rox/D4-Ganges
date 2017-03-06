// Define codes for the different commands
#define OP_THRUST 0
#define OP_ROLL 1
#define OP_YAW 2
#define OP_PITCH 3
#define OP_BUTTON 4
#define OP_KP 5
#define OP_KI 6
#define OP_KD 7

// K value size
#define CHAR_MAX 6

// Define data code for the button command
#define BTN_CARGO_HOOK 0
#define	BTN_PID_MODE 1 // Allow setting of K values and disable telemetry
#define	BTN_FLIGHT_MODE 2 // Receive telemetry

// Pin definitions
#define	PIN_THRUST 0
#define	PIN_ROLL 1
#define	PIN_YAW 2
#define	PIN_PITCH 3

// Define bit sizes for the packets
#define DATA_BIT_SIZE 10
#define COMMAND_BIT_SIZE 3
#define ENCRYPT_KEY_BIT_SIZE 3

// Enable comms
#define ENABLE_UPLINK 1
#define ENABLE_DOWNLINK 1

// Enable encryption
#define ENABLE_ENCRYPTION 1

// Enable UI via UART to Putty
#define ENABLE_UI 0

// Enable controls
#define ENABLE_CONTROLS 1

// Enable double transceivers
#define DOUBLE_TRANSCEIVERS 0

// Enable isolated uplink/downlink tests
#define UPLINK_TEST 1
#define DOWNLINK_TEST 0