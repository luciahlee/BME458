
// Import required libraries
#include <ArducamSSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error

/*
HardWare I2C pins
A4   SDA
A5   SCL
*/

// Pin definitions
#define OLED_RESET  16  // Pin 15 -RESET digital signal

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

ArducamSSD1306 display(OLED_RESET); // FOR I2C

int sensorPin = A1;
int sensorValue = 0;


void setup(void)
{
	// Start Serial
	Serial.begin(115200);
  pinMode(sensorPin, INPUT); 

  // SSD1306 Init
  display.begin();  // Switch OLED
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}


void loop() {
  display.setCursor(5,20);
  display.println("BME 458 Device: AMIO");
  display.setCursor(5,30);
  display.println("Click Here to Start");
  display.setCursor(20,40);
  display.print("Value is: ");
  display.print(sensorValue);
  display.display();
  delay(1000);
  display.clearDisplay();
}













