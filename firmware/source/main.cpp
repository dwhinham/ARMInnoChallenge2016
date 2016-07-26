/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"
#include "gestures.hpp"

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
 *                           .--------------- Fletcher checksum
 *             _____________________________
 *            /                             \
 * Byte 2 Bit: 7   6   5   4   3   2   1   0
 *************************************************************************/

#define HDR_ENCRYPTED_FLAG_POS   0
#define HDR_LENGTH_POS           1

#define HDR_ENCRYPTED_FLAG_MSK   (0x01 << HDR_ENCRYPTED_FLAG_POS)
#define HDR_LENGTH_MSK           (0x31 << HDR_LENGTH_POS)

#define HDR_CREATE(ENC, LEN) \
    (((ENC) << HDR_ENCRYPTED_FLAG_POS) & HDR_ENCRYPTED_FLAG_MSK) | \
    (((LEN) << HDR_LENGTH_POS) & HDR_LENGTH_MSK)

MicroBit uBit;
bool hasEncryptionKey;
enum gestures gestureArray[NUM_GESTURES];

// Enable USB serial I/O
MicroBitSerial serial(USBTX, USBRX);

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
    uint8_t rcvBuf[BUFFER_LEN + 1];
    uBit.radio.datagram.recv(rcvBuf, sizeof(rcvBuf));

    isEncrypted = (rcvBuf[0] & HDR_ENCRYPTED_FLAG_MSK) >> HDR_ENCRYPTED_FLAG_POS;

    if (isEncrypted)
    {
        MicroBitImage img(5, 5);
        img.print('!');
        uBit.display.print(img);
        uBit.sleep(1000);
        // Decrypt here
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
    strcpy((char*) &sndBuf[2], string.toCharArray());

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

    // 
    getGestures(NUM_GESTURES, gestureArray);

    hasEncryptionKey= true;

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
        // Read until a carriage return character
        serialRxBuf = serial.readUntil("\r", SYNC_SLEEP);

        // If the string starts with a '!', assume message should be encrypted
        sendEncrypted = serialRxBuf.charAt(0) == '!';

        if (sendEncrypted)
            serialRxBuf = serialRxBuf.substring(1, serialRxBuf.length() - 1);

        // Send message from serial port over the radio
        if (preparePacketBuffer(serialRxBuf, radioTxBuf, sendEncrypted))
            uBit.radio.datagram.send(radioTxBuf, BUFFER_LEN);
    }

    // Simply release this fiber, which will mean we enter the scheduler. Worst
    // case, we then sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

