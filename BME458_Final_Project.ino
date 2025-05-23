// main.c File
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
#define LED_flexSensor    3
#define LED_FSR   5
#define LED_EMG    9

int flexSensor = A0;
int FSR = A2;
int EMG = A4;

#define OLED_RESET  16  // Pin 15 -RESET digital signal

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

ArducamSSD1306 display(OLED_RESET); // FOR I2C

//Variables (volatile variables can change)
unsigned long time; 

volatile int grip_strength;
int grip_strength_pwm; //use for mapping
int flex_threshold = 660;

volatile int fsr;
int fsr_pwm;
int FSR_threshold = 255;

volatile int emg;
int emg_pwm;
int EMG_threshold = 500;

//functions
int read_flexSensor(int threshold);
int read_FSR(int threshold);
int read_EMG(int threshold);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_flexSensor, OUTPUT);
  pinMode(LED_FSR, OUTPUT);
  pinMode(LED_EMG, OUTPUT);
  Serial.println("Time FlexSensor FSR EMG");

  //Can write code here to configure threshold values 
  

  // SSD1306 Init
  display.begin();  // Switch OLED

  // Clear the buffer.
  display.clearDisplay();
/*
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("BME 458 Device: AMIO");
*/
}

void loop() {
time = millis();
grip_strength = read_flexSensor(flex_threshold);
fsr = read_FSR(FSR_threshold);
emg = read_EMG(EMG_threshold);

//Print Output on one line
String p1= " ";
Serial.println(time + p1 + grip_strength + p1 + fsr + p1 + emg);

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

int read_flexSensor(int threshold){
  int val = analogRead(flexSensor);
  // grip_strength_pwm = map(grip_strength, )
  val > threshold ? analogWrite(LED_flexSensor, 255) : analogWrite(LED_flexSensor, 0);
  return val;
}

int read_FSR(int threshold){
  int val = analogRead(FSR);
  val < threshold ? analogWrite(LED_FSR, 255) : analogWrite(LED_FSR, 0);
  return val;
}

int read_EMG(int threshold){
  int val = analogRead(EMG);
  val < threshold ? analogWrite(LED_EMG, 255) : analogWrite(LED_EMG, 0);
  return val;
}

//Communicate data with PuTTY
//Source: https://datalab.medium.com/arduino-ide-serial-data-export-by-putty-6a77631a23ea

//Todo: Write code for OLED Device
//Todo: Organize notebook and document
//Todo: figure out how to read EMGs
