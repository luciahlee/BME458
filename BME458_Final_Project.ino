// Import required libraries
#include <ArducamSSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error

/*
HardWare I2C pins (only applicable to UNO)
A4   SDA
A5   SCL
*/

//Declaration of pin values
#define LED1Pin    3
#define LED2Pin    5
#define LED3Pin    9
int flexSensor = A0;
int FSR = A2;
int EMG = A4;

#define OLED_RESET  16  // Pin 15 -RESET digital signal

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

ArducamSSD1306 display(OLED_RESET); // FOR I2C

//Variables
int grip_strength;
int grip_strength_pwm;
int fsr;
int fsr_pwm;
int emg;
int emg_pwm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED1Pin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
  pinMode(LED3Pin, OUTPUT);
  Serial.println("Flex Sensor FSR EMG");

  //Can write code here to configure threshold values 
  
/*
  // SSD1306 Init
  display.begin();  // Switch OLED
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("BME 458 Device: AMIO");
*/
}

void loop() {
  // put your main code here, to run repeatedly:

//Code for Flex Sensor
grip_strength = analogRead(flexSensor);
// grip_strength_pwm = map(grip_strength, )
grip_strength > 660 ? analogWrite(LED1Pin, 255) : analogWrite(LED1Pin, 0);

//Code for FSR
fsr = analogRead(FSR);
// grip_strength_pwm = map(grip_strength, )
fsr < 500 ? analogWrite(LED2Pin, 255) : analogWrite(LED2Pin, 0);

//Code for EMG
emg = analogRead(EMG);
// grip_strength_pwm = map(grip_strength, )
emg < 500 ? analogWrite(LED3Pin, 255) : analogWrite(LED3Pin, 0);

//Print Output on one line
//Source: https://forum.arduino.cc/t/print-multiple-variables-amount-in-one-line/604071/6
// Serial.println(grip_strength, fsr, emg);
// Serial.print(grip_strength);
// Serial.print(" ");
// Serial.print(fsr);
// Serial.print(" ");
// Serial.println(emg);

String p1= " ";
Serial.println(grip_strength + p1 + fsr + p1 + emg);

//Code for OLED
/*
display.setCursor(5,30);
display.println("Click Here to Start");
display.setCursor(20,40);
display.print("Value is: ");
display.print(emg);
display.display();
delay(1000);
display.clearDisplay();
*/
}

//Communicate data with PuTTY
//Source: https://datalab.medium.com/arduino-ide-serial-data-export-by-putty-6a77631a23ea

//Todo: Write code for OLED Device
//Todo: Attatch flex sensor to dynamometer
//Todo: Create device for CAD
//Todo: Organize Code to be easier to read
//Todo: Organize notebook and document
//Todo: figure out how to read EMGs
