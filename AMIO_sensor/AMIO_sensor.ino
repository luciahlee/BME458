// main.c File
// Import required libraries
#include <ArducamSSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error

//Declaration of pin values
#define LED_flexSensor    3
#define LED_FSR   5
#define LED_EMG    9

#define FLEX_COM_POOR  1
#define FLEX_COM_AVG   2
#define FLEX_COM_EXCL  4
#define FSR_COM_POOR   6
#define FSR_COM_AVG    7
#define FSR_COM_EXCL   8
#define EMG_COM_POOR   10
#define EMG_COM_EXCL   11

// int Led_pins = [FLEX_COM_POOR, FLEX_COM_AVG, FLEX_COM_EXCL, FSR_COM_POOR, FSR_COM_AVG, FSR_COM_EXCL, EMG_COM_POOR, EMG_COM_EXCL];

//Thresholds 
#define FLEX_BASE       640 //baseline value
#define FLEX_POOR       650 //less than 650
#define FLEX_AVERAGE    659 //650 - 659
#define FLEX_EXCELLENT  660 //Above 660

int flexSensor = A0;
int FSR = A2;
int EMG = A4;

//Variables (volatile variables can change)
unsigned long time; 

volatile int flex;
int flex_pwm; //use for mapping
// byte flex_code = 0b0000;
int flex_code = 0;


volatile int fsr;
int fsr_pwm;
int FSR_threshold = 400;
byte fsr_code = 0b0000;

volatile int emg;
int emg_pwm;
int EMG_threshold = 500;
byte emg_code = 0b0000;

//functions
int read_flexSensor();
int read_FSR(int threshold);
int read_EMG(int threshold);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_flexSensor, OUTPUT);
  pinMode(LED_FSR, OUTPUT);
  pinMode(LED_EMG, OUTPUT);
  pinMode(FLEX_COM_POOR, OUTPUT);
  Serial.println("Time FlexSensor FSR EMG");

  //Can write code here to configure threshold values 
}

void loop() {
  digitalWrite(FLEX_COM_AVG, HIGH);
  digitalWrite(FSR_COM_POOR, HIGH);
  digitalWrite(EMG_COM_EXCL, HIGH);
  time = millis();
  flex = read_flexSensor();
  fsr = read_FSR(FSR_threshold);
  emg = read_EMG(EMG_threshold);

  //Print Output on one line
  String p1= " ";
  Serial.println(time + p1 + flex + p1 + fsr + p1 + emg);
  Serial.println(flex_code + p1 + fsr_code + p1 + emg_code);
  delay(1000);
}

int read_flexSensor(){
  // byte A = 0b0000, B = 0b0000, C = 0b0000;
  int a = 0;
  int val = analogRead(flexSensor);
  // flex_pwm = map(flex, )
  val > FLEX_EXCELLENT ? analogWrite(LED_flexSensor, 255) : analogWrite(LED_flexSensor, 0);
  if (val > FLEX_EXCELLENT){
    a = 1 << 3;
  }
  if (val >= FLEX_POOR && val <= FLEX_AVERAGE){
    a = 1 << 2;
  }
  if (val >= FLEX_BASE && val < FLEX_POOR){
    a = 1 << 1;
  }
  flex_code = a;
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

//Todo: Create buttons for different screens
//Todo: Training for User with Dynamometer
//Todo: Logic for outputting different digital read values from sensor script
//Todo: Calculating ALS likelihood from oled script
//Todo: Organize notebook and document