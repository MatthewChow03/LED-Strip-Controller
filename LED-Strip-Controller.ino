/**
 ************************************************************************
 *
 * @file:   main.ino          
 * @brief   IR transciever firmware to control an LED strip
 * @author  Matthew Chow
 * 
 ************************************************************************
*/

/* Includes -----------------------------------------------------------*/

#include <IRremote.h>

/* Typedefs --------------------------------------------------------------*/

typedef struct
{
    int short_key;
    String key;
    int val;
    String full_key;
} command;

/* Variables ----------------------------------------------------------*/

// IR transmit and receive
const int RX_PIN = 10;
const int TX_PIN = 11;

// RGB LED feeback
const int LED_RED_PIN = 9;
const int LED_GREEN_PIN = 6;
const int LED_BLUE_PIN = 5;

// Array of all possible command structures
const command commands_lookup[] = {
    {1, "UP", 0x5E, "Change Colour Up"},
    {2, "DOWN", 0x5A, "Change Colour Down"},
    {3, "BRIGHT", 0x0C, "Brightness Up"},
    {4, "DIM", 0x08, "Brightness Down"},
    {5, "WHITE", 0x1C, "Set Colour White"},
    {6, "RED", 0x42, "Set Colour Red"},
    {7, "BLUE", 0x4A, "Set Colour Blue"},
    {8, "GREEN", 0x52, "Set Colour Green"},
    {9, "TOGGLE", 0x45, "Toggle On / Off"}};

#define NUM_COMMANDS sizeof(commands_lookup) / sizeof(command)

/* Function Prototypes ------------------------------------------------*/

int get_key_val(String key);
String get_full_key(int value);
void serial_print_command(int value);
void ir_send_command();
void serial_log_receiver();
void rgb_write(int r, int g, int b);
void led_sequence(int sequence_type);

/* Setup Function -----------------------------------------------------*/

void setup()
{
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    Serial.begin(9600);
    IrReceiver.begin(RX_PIN);
    IrSender.begin(TX_PIN, false); // turn off automatic feedback LED
}

/* Main Loop ----------------------------------------------------------*/

void loop()
{
    // Transmit data input is in the buffer
    while (Serial.available() != 0)
    {
        // Get serial input
        String input = Serial.readStringUntil('\n');
        Serial.print("INPUT: ");
        Serial.println(input);

        // Get the keyval struct member based on inputted string
        int value = get_key_val(input);

        // Not a valid command
        if (value == -1)
        {
            Serial.println("Invalid command. Try again.\n");
            // Red error indicator
            led_sequence(-1);
        }
        // Transmit command
        else
        {
            serial_print_command(value);
            ir_send_command(value);

            // Successful TX green indicator
            led_sequence(0);
        }
    }

    // Additional log for transmitted commands, printed to the serial monitor
    // The log is from received IR data, whereas other logs were generated strictly through software
    // This is a more accurate log of what has been sent over IR
    serial_log_receiver();
}

/* Function Definitions -----------------------------------------------*/

/**
 * Gets the val from a key of a command
 * @param key: string used to identify members of the command struct
 * @return: val member of the command struct
 */
int get_key_val(String key)
{
    for (int index = 0; index < NUM_COMMANDS; index++)
    {
        if (key.equalsIgnoreCase(commands_lookup[index].key))
        {
            return commands_lookup[index].val;
        }
    }
    return -1;
}

/**
 * Gets the full_key from a val of a command
 * @param value: value used to identify members of the command struct
 * @return: full_key member of the command struct
 */
String get_full_key(int value)
{
    for (int index = 0; index < NUM_COMMANDS; index++)
    {
        if (value == commands_lookup[index].val)
        {
            return commands_lookup[index].full_key;
        }
    }
    return "ERROR";
}

/**
 * Prints a command value, formatted for serial
 * @param: command value to be printed
 * @return: none
 */
void serial_print_command(int value)
{
    Serial.print("COMMAND CALLED: ");
    Serial.print(get_full_key(value));
    Serial.print(" (0x");
    Serial.print(value, HEX);
    Serial.print(")");
    Serial.println("\n");
}

/**
 * Sends NEC protocal IR data
 * @param: command to send via IR
 * @return: none
 */
void ir_send_command(int command_value)
{
    // TX values used for the sendNEC() function
    int aAddress = 0;
    uint_fast8_t aNumberOfRepeats = 0;
    bool aIsRepeat = false;

    // Send IR data using the NEC protocol
    IrSender.sendNEC(aAddress, command_value, aNumberOfRepeats, aIsRepeat);
}

/**
 * Receives and prints IR data to the serial monitor
 * @param: none
 * @return: none
 */
void serial_log_receiver()
{
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodedIRData.command != 0)
        {
            //Serial.print("From Receiver: 0x");
            //Serial.print(IrReceiver.decodedIRData.command, HEX);
            Serial.print("RECEIVER: ");
            IrReceiver.printIRResultShort(&Serial);
            Serial.println("\n");

            // Successful RX blue indicator
            led_sequence(1);
        }
        IrReceiver.resume();
    }
}

/**
 * Write values to pins of an RGB LED
 * @param r: red colour strength
 * @param g: green colour strength
 * @param b: blue colour strength
 * @return: none
 */
void rgb_write(int red_val, int green_val, int blue_val)
{
    analogWrite(LED_RED_PIN, red_val);
    analogWrite(LED_GREEN_PIN, green_val);
    analogWrite(LED_BLUE_PIN, blue_val);
}

/**
 * Flash RGB LED to show data has been transmitted, received, or an error occured
 * @param type: -1: error / 0: TX / 1: RX
 * @return: none
 */
void led_sequence(int sequence_type)
{
    switch (sequence_type)
    {
    // Error: flash red 2 times
    case -1:
        for (int i = 0; i < 2; i++)
        {
            rgb_write(255, 0, 0);
            delay(100);
            rgb_write(0, 0, 0);
            delay(100);
        }
        break;

    // TX: green gradual flash
    case 0:
        for (int i = 0; i <= 255; i += 2)
        {
            rgb_write(0, i, 0);
            delay(1);
        }
        for (int i = 0; i <= 255; i += 2)
        {
            rgb_write(0, 255 - i, 0);
            delay(1);
        }
        rgb_write(0, 0, 0);
        break;

    // RX: blue gradual flash
    case 1:
        for (int i = 0; i <= 255; i += 2)
        {
            rgb_write(0, 0, i);
            delay(1);
        }
        for (int i = 0; i <= 255; i += 2)
        {
            rgb_write(0, 0, 255 - i);
            delay(1);
        }
        rgb_write(0, 0, 0);
        break;
    }
}