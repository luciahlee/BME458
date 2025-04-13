// main.c File
// Import required libraries
#include <ArducamSSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error

//Declaration of pin values
#define LED_flexSensor    3
#define LED_FSR           5
#define LED_EMG           9

//Digital pin writes to the Arduino Uno
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
//General function can be implemented later
//values for poor, average, and excellent 
int flex_thresholds[3] = {640, 659, 660};
int FSR_thresholds[3] = {500, 459, 400};

#define FLEX_POOR       640 //less than 650
#define FLEX_AVERAGE    659 //650 - 659
#define FLEX_EXCELLENT  660 //Above 660

#define FSR_POOR        500
#define FSR_AVERAGE     401
#define FSR_EXCELLENT   400

//These threshold values need to be changed 
#define EMG_POOR        500
#define EMG_EXCELLENT   400

//Sensor Inputs 
int flexSensor = A0;
int FSR = A2;
int EMG = A4;

//Variables (volatile variables can change)
unsigned long time; 
volatile int flex;
byte flex_code = 0;
volatile int fsr;
int fsr_code = 0;
volatile int emg;
int EMG_threshold = 500;
int emg_code = 0;

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
}

void loop() {
  // int flex_code = 0;
  // int fsr_code = 0;
  // int emg_code = 0;

  //need to reimplement 
  time = millis();
  flex = read_flexSensor();
  fsr = read_FSR();
  emg = read_EMG(EMG_threshold);

  //Send the signal output to Uno
  digital_output(flex_code, fsr_code, emg_code);

  //Print Output on one line
  String p1= " ";
  Serial.println(time + p1 + flex + p1 + fsr + p1 + emg);
  Serial.println(flex_code + p1 + fsr_code + p1 + emg_code);
  delay(100);
}

//ToDo:
//create a general program to read sensors 
int read_sensor();


int read_flexSensor(){
  int val = analogRead(flexSensor);
  val > FLEX_EXCELLENT ? analogWrite(LED_flexSensor, 255) : analogWrite(LED_flexSensor, 0);
  if (val > FLEX_EXCELLENT)
    flex_code = flex_code | 0100;
  if (val >= FLEX_POOR && val <= FLEX_AVERAGE)
    flex_code = flex_code | 0010;
  if (val < FLEX_POOR)
    flex_code = flex_code | 0001;
  return val;
}

int read_FSR(){
  int a = 0;
  int val = analogRead(FSR);
  val < FSR_EXCELLENT ? analogWrite(LED_FSR, 255) : analogWrite(LED_FSR, 0);
  if (val <= FSR_EXCELLENT)
    a = 1 << 3;
  if (val >= FSR_AVERAGE && val <= FSR_POOR)
    a = 1 << 2;
  if (val > FSR_POOR)
    a = 1 << 1;
  fsr_code = a;
  return val;
}

//Still need to implement 
int read_EMG(int threshold){
  int val = analogRead(EMG);
  val < threshold ? analogWrite(LED_EMG, 255) : analogWrite(LED_EMG, 0);
  return val;
}

//Input the code values 
void digital_output(int flex, int fsr, int emg){
  if(flex_code == 73){
    digitalWrite(FLEX_COM_EXCL, HIGH);
    Serial.println("Flex Excellent");
  }
  if(flex_code == 9){
    digitalWrite(FLEX_COM_AVG, HIGH);
    Serial.println("Flex Average");
  }
  if(flex_code == 1){
    digitalWrite(FLEX_COM_POOR, HIGH);
    Serial.println("Flex Poor");
  }
}


int preivous_code(){
  int a = 0;
  int val = analogRead(flexSensor);
  val > FLEX_EXCELLENT ? analogWrite(LED_flexSensor, 255) : analogWrite(LED_flexSensor, 0);
  if (val > FLEX_EXCELLENT)
    a = 1 << 3;
  if (val >= FLEX_POOR && val <= FLEX_AVERAGE)
    a = 1 << 2;
  if (val < FLEX_POOR)
    a = 1 << 1;
  flex_code = a;
  return val;
}

//Communicate data with PuTTY
//Source: https://datalab.medium.com/arduino-ide-serial-data-export-by-putty-6a77631a23ea

//Todo: Create buttons for different screens
//Todo: Training for User with Dynamometer
//Todo: Logic for outputting different digital read values from sensor script
//Todo: Calculating ALS likelihood from oled script
//Todo: Organize notebook and document

//Todo: implement general function to read sensor 