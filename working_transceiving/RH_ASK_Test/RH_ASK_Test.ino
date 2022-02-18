// working ASK receiving on 433 MHz
// Commented code is for displaying to a OLED Screen


#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
/*#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define FONT_SIZE 2 //OLED font size

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
*/
RH_ASK driver(2000, D7, D6);

/*void printText(String MessageInput) {
    display.clearDisplay();
    display.setCursor(0, 10);
    String input = MessageInput;
    display.print(input);
    display.display();
    delay(1000);
    display.clearDisplay();
    //Serial.print(input);
}

void setUpDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.print("SSD1306 allocation failed");
    for(;;);
  }
  delay(2000);
  Serial.print("READY TO RUMBLE");
  display.clearDisplay();
  display.setTextSize(FONT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Display Ready");
  display.display(); 
  display.clearDisplay();
}
*/

void setup()
{
    Serial.begin(9600);  // Debugging only
    if (!driver.init())
         Serial.println("init failed");
    //setUpDisplay();
    Serial.println("Done setting up!");
}

void loop()
{
    uint8_t buf[20];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      //int i;
      // Message with a good checksum received, dump it.
      Serial.print("Message: ");
      Serial.println((char*)buf);
      /*String message = (char*)buf;
      printText(message);
      delay(2000);*/         
    }
    //Serial.println(".");
    //
    //printText("No Message");
}
