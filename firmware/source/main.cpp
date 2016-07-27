#include <MicroBit.h>

#include "encryption.h"
#include "gestures.h"
#include "images.h"

#if MICROBIT_BLE_ENABLED
#error This program must be compiled with the Bluetooth Stack disabled.
#endif

#define SCROLL_SPEED    50
#define BUFFER_LEN      32
#define NUM_GESTURES    5

/*************************************************************************
 * Message header format
 * =====================
 *                              .-------------- Message length
 *                              |          .--- Encrypted/not encrypted flag
 *                     _________________   _
 *                    /                 \ / \
 * Byte 1 Bit: 7   6   5   4   3   2   1   0
 *
 *                           .----------------- Fletcher checksum
 *             _____________________________
 *            /                             \
 * Byte 2 Bit: 7   6   5   4   3   2   1   0
 *************************************************************************/

#define HDR_ENCRYPTED_FLAG_POS   0
#define HDR_LENGTH_POS           1

#define HDR_ENCRYPTED_FLAG_MSK   (0x01 << HDR_ENCRYPTED_FLAG_POS)
#define HDR_LENGTH_MSK           (0x1F << HDR_LENGTH_POS)

#define HDR_CREATE(ENC, LEN) \
    (((ENC) << HDR_ENCRYPTED_FLAG_POS) & HDR_ENCRYPTED_FLAG_MSK) | \
    (((LEN) << HDR_LENGTH_POS) & HDR_LENGTH_MSK)

// Global variables
MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX);
gesture_t gestures[NUM_GESTURES];

uint8_t getFletcherChecksum(ManagedString &string)
{
    int32_t sum1 = 0;
    int32_t sum2 = 0;

    int16_t i;
    int16_t len = string.length();
    for (i = 0; i < len; ++i)
    {
        sum1 += (uint8_t) string.charAt(i);
        sum2 += sum1;
    }

    int8_t code1 = (int8_t) (sum1 % 255);
    int8_t code2 = (int8_t) (sum2 % 255);
    int8_t final = (int8_t) (((int16_t) (code1 + code2)) % 255);
    return final;
}

// Callback invoked when micro:bit receives a datagram
void onRecv(MicroBitEvent event)
{
    bool isEncrypted;
    int16_t len;
    uint8_t rcvBuf[BUFFER_LEN + 1];
    uBit.radio.datagram.recv(rcvBuf, sizeof(rcvBuf));

    isEncrypted = (rcvBuf[0] & HDR_ENCRYPTED_FLAG_MSK) >> HDR_ENCRYPTED_FLAG_POS;
    len = (rcvBuf[0] & HDR_LENGTH_MSK) >> HDR_LENGTH_POS;

    rcvBuf[len+2] = 0;

    if (isEncrypted)
    {
        MicroBitImage img(5, 5);
        uBit.display.print(MICROBIT_IMAGE_KEY);
        uBit.sleep(1000);
        decryptString((char*) &rcvBuf[2], len, getShift(gestures, NUM_GESTURES));
    }

    ManagedString s((char*) &rcvBuf[2]);

    serial.send("<< RECV ");
    if (isEncrypted)
        serial.send("(Encrypted) ");
    if (rcvBuf[1] == getFletcherChecksum(s))
      serial.send("(valid) ");
    else
      serial.send("(invalid) ");
    serial.send((char*) &rcvBuf[2]);
    serial.send("\r\n");

    uBit.display.scroll((char*) &rcvBuf[2], SCROLL_SPEED);

    // Parameter unused
    (void) event;
}

bool preparePacketBuffer(ManagedString &string, uint8_t *sndBuf, bool isEncrypted)
{
    int16_t len = string.length();

    if (!len || len > BUFFER_LEN - 1)
        return false;

    sndBuf[0] = HDR_CREATE(isEncrypted, len);
    sndBuf[1] = getFletcherChecksum(string);

    if (isEncrypted) {
      char encryptionBuf[len];
      strcpy(encryptionBuf, string.toCharArray());

      // Encrypt the message
      encryptString(encryptionBuf, len, getShift(gestures, NUM_GESTURES));
      string = ManagedString(encryptionBuf);
    }
    
    strcpy((char*) &sndBuf[2], string.toCharArray());
    sndBuf[len+2] = 0;

    serial.send(">> SEND ");
    if (isEncrypted)
        serial.send("(Encrypted) ");
    serial.send((char*) &sndBuf[2]);
    serial.send("\r\n");

    return true;
}

void generateEncryptionKey(MicroBitEvent event)
{
    // Disable radio listener
    uBit.messageBus.ignore(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onRecv);

    // Record 5 gestures to act as an encryption key
    getGestures(NUM_GESTURES, gestures);

    // Re-enable radio receiver listener
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onRecv);

    // Parameter unused
    (void) event;
}

int main()
{
    ManagedString serialRxBuf;
    uint8_t radioTxBuf[BUFFER_LEN];
    bool sendEncrypted;

    // Initialise the micro:bit runtime.
    uBit.init();

    // Initialise the micro:bit radio
    uBit.radio.enable();

    // Initialise radio receiver listener
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onRecv);

    // Listen for button hold event
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_HOLD, generateEncryptionKey);

    // Android serial app doesn't like baud rates >9600!
    serial.baud(9600);

    while(true)
    {
        // Show smiley face
        uBit.display.print(MICROBIT_IMAGE_SMILE);

        // Read until a carriage return character
        serialRxBuf = serial.readUntil("\r", SYNC_SLEEP);

        // If the string starts with a '!', assume message should be encrypted
        sendEncrypted = serialRxBuf.charAt(0) == '!';

        if (sendEncrypted)
        {
            // length() internally uses strlen(), so does *not* account for NULL terminator
            size_t len = serialRxBuf.length();

            // Remove leading '!'
            serialRxBuf = serialRxBuf.substring(1, len - 1);
            len--;

	    ManagedString string(serialRxBuf);

            // Send message from serial port over the radio
            if (preparePacketBuffer(string, radioTxBuf, sendEncrypted))
                uBit.radio.datagram.send(radioTxBuf, BUFFER_LEN);
        }
        else
        {
            if (preparePacketBuffer(serialRxBuf, radioTxBuf, sendEncrypted))
                uBit.radio.datagram.send(radioTxBuf, BUFFER_LEN);
        }

        // Animate smiley face
        for (int i = 0; i < serialRxBuf.length(); ++i)
        {
            uBit.display.print(i % 2 ? MICROBIT_IMAGE_SMILE_TALK : MICROBIT_IMAGE_SMILE);
            uBit.sleep(100);
        }
    }

    // Simply release this fiber, which will mean we enter the scheduler. Worst
    // case, we then sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

