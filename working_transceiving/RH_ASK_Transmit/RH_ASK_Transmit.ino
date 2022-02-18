// Working ASK transmitting code
// Commented out code is attempts to get GCM AES256 Working
// Range is about 11 cm using arduino d1 minis and 433 MHz modules

/*#include <Crypto.h>
#include <AES.h>
#include <GCM.h> // encryption!!!*/
#include <RH_ASK.h> // Amplitude Shift key transmission
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000, D7, D6); // (bps, Rx, Tx)

int isOpen = 0;
/*GCM<AES256> gcm;
const uint8_t key = "five";
const uint8_t iv = "seven";
const char *msg = "Hello World!";
String adata = "adata";
String tag = "tag";*/

void setup()
{
    pinMode(D1, INPUT_PULLUP);
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("init failed");

    /*/ GCM initialization
    gcm.setKey(key, sizeof(key));
    gcm.setIV(iv, sizeof(iv));
    gcm.addAuthData(adata, sizeof(adata));
    gcm.encrypt("hello world", (uint8_t *)msg, sizeof((uint8_t *)msg));
    gcm.computeTag(tag, sizeof(tag));*/
}

void loop()
{
    isOpen = digitalRead(D1);
    if(isOpen == LOW) {
      const char* message = "Hello World!";
      driver.send((uint8_t *)message, strlen(message));
      driver.waitPacketSent();
      //delay(1000);
    }
}
