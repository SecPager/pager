#include <AESLib.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

const int CAP_LEN=4;
const int IV_LEN=24;
const int MSG_LEN=48;
const int TOTAL_LEN=CAP_LEN+IV_LEN+MSG_LEN;

const int KEY_LEN=24;
uint8_t key[KEY_LEN] = {0x76, 0x71, 0xc5, 0xd, 0xa0, 0xa6, 0x57, 0x9b, 0xdb, 0x7d, 0x80, 0x5d, 0xc5, 0x2b, 0x76, 0xff, 0x34, 0xa0, 0xf5, 0x48, 0xc8, 0x11, 0x74, 0xd0};
uint8_t cap[CAP_LEN] = {0x00, 0x00, 0x00, 0x01};
uint8_t iv[IV_LEN];
uint8_t data[MSG_LEN];
bool dirty = true;

void setup() {
  Serial.begin(9600);  // Debugging only
  if (!driver.init())
    Serial.println("init failed");
}

void loop() {
  uint8_t buf[TOTAL_LEN];
  uint8_t buflen = 36;
  if (driver.recv(buf, &buflen)) {
    if (buf[0] == 0) {
      dirty = false;
      uint8_t data[MSG_LEN];

      for (int i = 0; i < CAP_LEN; i++) {
        if (cap[i] != buf[1+i]) {
          return;
        }
      }
      for (int i = 0; i < IV_LEN; i++) {
        iv[i] = buf[1+i+CAP_LEN];
      }

    } else {
      if (dirty) {
        return;
      }
      dirty = true;

      for (int i = 0; i < MSG_LEN; i++) {
        data[i] = buf[1+i];
      }
      aes192_cbc_dec(key, iv, data, sizeof(data)); // requires data to be %16 == 0
      {
        int i = 0;
        while (data[i] >= 0x09 && data[i] <= 0x7e) i++;
        data[i] = 0;
        Serial.println((char *)data);
      }
    }
  }
}
