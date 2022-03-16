#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoSTL.h>
#include <vector>  
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

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define FONT_SIZE 2 //OLED font size
const int BACK_BUTTON = 2;
int lastStateBack = HIGH;
int currentStateBack;
const int NEXT_BUTTON = 3;
int lastStateNext = HIGH;
int currentStateNext;
const int DELETE_BUTTON = 4;
int lastStateDelete = HIGH;
int currentStateDelete;
int numMessages = 0;
const int BUZZER = 7;


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

std::vector<String> messages;
int currMessageIndex = 0;

void checkReceiverForMessage(){
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
        char * charData = (char *)data;
        String input = String(charData);
        storeMessage(input);
        currMessageIndex = (numMessages - 1);
        digitalWrite(BUZZER, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        delay(100);
        digitalWrite(BUZZER, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        printMessage();
      }
    }
  }
}

void printText(String input) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.print(input);
    display.display();
    Serial.print("displaying message: ");
    Serial.println(input);
}

void storeMessage(String message){
  messages.push_back(message);
  numMessages++;
  //printSerialMessages(); REMOVE ME
}
void printSerialMessages(){
  unsigned int i = 0;
  Serial.println("printing all messages:");
  for (i; i < numMessages; i++){
    Serial.println(messages.at(i));
  }
}
void printMessage(){
  if (numMessages > 0){
    String currMessage = String(currMessageIndex + 1) + "/" + String(numMessages);
    printText(currMessage);
    delay(250);
    display.clearDisplay();
    display.display(); 
    printText(messages.at(currMessageIndex));
  }
  else {
    display.clearDisplay();
    currMessageIndex = 0;
  }
}
void nextMessage(){
  currMessageIndex++;
  if (currMessageIndex == numMessages)
    currMessageIndex = 0;
  printMessage();
}
void previousMessage(){
  currMessageIndex--;
  if (currMessageIndex < 0)
    currMessageIndex = (numMessages - 1);
  printMessage();
}
void deleteMessage(){
  if (numMessages > 0){
    messages.erase(messages.begin() + currMessageIndex);
    numMessages--;
    printText("Message\nDeleted");
    delay(250);
    display.clearDisplay();
    display.display(); 
    currMessageIndex = (numMessages - 1);
    printMessage();
  }
  if (numMessages == 0){
    printText("Inbox\nEmpty");
    delay(2000);
    display.clearDisplay();
    display.display(); 
  }
}

void setUpDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.print("SSD1306 allocation failed");
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(FONT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.clearDisplay();
  display.display(); 
}

void setup(){
    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
    setUpDisplay();
    if (!driver.init())
      Serial.println("init failed");
    pinMode(NEXT_BUTTON, INPUT_PULLUP);
    pinMode(BACK_BUTTON, INPUT_PULLUP);
    pinMode(DELETE_BUTTON, INPUT_PULLUP);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void loop(){
  checkReceiverForMessage();
  delay(20);

  if (numMessages > 0){
    currentStateNext = digitalRead(NEXT_BUTTON);
    currentStateBack = digitalRead(BACK_BUTTON);
    currentStateDelete = digitalRead(DELETE_BUTTON);

    if(lastStateNext == LOW && currentStateNext == HIGH){
      nextMessage();
    }
    if(lastStateBack == LOW && currentStateBack == HIGH){
      previousMessage();
    }
    if(lastStateDelete == LOW && currentStateDelete == HIGH){
      deleteMessage();
    }
    lastStateNext = currentStateNext;
    lastStateBack = currentStateBack;
    lastStateDelete = currentStateDelete;
  }
}
