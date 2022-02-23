#include <tinyECC.h>

tinyECC ecc;

#define MAPPING_TABLE_TYPE  MAPPING_TABLE_256

#define MAPPING_TABLE_SIZE  MAPPING_TABLE_256_SIZE


void setup() {
// put your setup code here, to run once:
Serial.begin(9600);

    //Encryption
    ecc.plaintext="Please report \"the\" to the hospital th th th th th th th th. Baby delivery room 3217";
    Serial.println("Plain Text: "+ecc.plaintext);
    ecc.encrypt();
    Serial.println("Cipher Text: "+ecc.ciphertext);

    //Decryption
    ecc.plaintext = F("");
    //decrypts ecc.ciphertext
    ecc.decrypt();
    ecc.ciphertext = F("");
    Serial.println("Decoded: "+ecc.plaintext);
    

}

void loop() {
  // put your main code here, to run repeatedly:

}
