#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoSTL.h>
#include <vector>

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


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

std::vector<String> messages;
int currMessageIndex = 0;


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
  messages.erase(messages.begin() + currMessageIndex);
  numMessages--;
  if (numMessages > 0){
    currMessageIndex = (numMessages - 1);
    printMessage();
  }
  else{
    display.clearDisplay();
    currMessageIndex = 0;
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
  display.println("Display Ready");
  display.display(); 
  display.clearDisplay();
}

void setup(){
    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
    setUpDisplay();
    pinMode(NEXT_BUTTON, INPUT_PULLUP);
    pinMode(BACK_BUTTON, INPUT_PULLUP);
    pinMode(DELETE_BUTTON, INPUT_PULLUP);
}

void loop(){
  if (Serial.available() >0){
      String input = Serial.readString();
      storeMessage(input);
      currMessageIndex = (numMessages - 1);
      printMessage();
  }
  delay(20);
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
