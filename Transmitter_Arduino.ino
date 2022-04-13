#include <AESLib.h>
#include "base64.hpp"
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

const int MSG_LEN=48;
const int KEY_LEN=24;
const int IV_LEN=24;
const int CAP_LEN=4;
const int TOTAL_LEN=MSG_LEN+KEY_LEN+IV_LEN+CAP_LEN;

// Pin 12
RH_ASK driver;

void setup() {
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
        Serial.println("init failed");
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    char binary[TOTAL_LEN];
    memset(binary, 0, TOTAL_LEN); // array filled with 0x00s
    Serial.readBytes(binary, TOTAL_LEN);

    uint8_t cap[CAP_LEN];
    uint8_t key[KEY_LEN];
    uint8_t iv[IV_LEN];
    char msg[MSG_LEN];
    uint8_t outgoing_0[1+CAP_LEN+IV_LEN];
    outgoing_0[0] = 0x00;
    uint8_t outgoing_1[1+MSG_LEN];
    outgoing_1[0] = 0x01;

    for (int i = 0; i < CAP_LEN; i++) {
      cap[i] = binary[i];
      outgoing_0[1+i] = binary[i];
    }
    for (int i = 0; i < KEY_LEN; i++) {
      key[i] = binary[i+CAP_LEN];
    }
    for (int i = 0; i < IV_LEN; i++) {
      iv[i] = binary[i+CAP_LEN+KEY_LEN];
      outgoing_0[1+i+CAP_LEN] = binary[i+CAP_LEN+KEY_LEN];
    }
    int i = 0;
    for (i = 0; i < MSG_LEN; i++) {
      msg[i] = binary[i+CAP_LEN+KEY_LEN+IV_LEN];
    }

    // padding section - our text string can be up to 255 characters long
    uint8_t dataWithPad[MSG_LEN];
    memset(dataWithPad, 0, sizeof(dataWithPad)); // array filled with 0x00s
    memcpy(dataWithPad, msg, strlen(msg)); // array filled with data and then 0x00s as padding

    aes192_cbc_enc(key, iv, dataWithPad, sizeof(dataWithPad)); // requires data to be %16 == 0

    for (int i = 0; i < MSG_LEN; i++) {
      outgoing_1[1+i] = dataWithPad[i];
    }

    for (int i = 0; i < 1; i++) {
      driver.send(outgoing_0, 1+CAP_LEN+IV_LEN);
      driver.waitPacketSent();
      delay(100);
      driver.send(outgoing_1, 1+MSG_LEN);
      driver.waitPacketSent();
      delay(100);
    }

  }
}
