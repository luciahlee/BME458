// main.c File
// Import required libraries
#include <ArducamSSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error
#include <Bounce2.h>        //To debounce the button and reset states 

//Declaration of pin values
#define LED_flexSensor    3
#define LED_FSR           5
#define LED_EMG           12

//Digital pin writes to the Arduino Uno
#define FLEX_COM_POOR  A5
#define FLEX_COM_AVG   2
#define FLEX_COM_EXCL  4
#define FSR_COM_POOR   6
#define FSR_COM_AVG    7
#define FSR_COM_EXCL   8
#define BUTTON_PIN     13
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
byte fsr_code = 0;
volatile int emg;
int EMG_threshold = 500;
byte emg_code = 0;

Bounce2::Button button = Bounce2::Button(); // INSTANTIATE A Bounce2::Button OBJECT
bool ledState = LOW;

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
  // digitalWrite(LED_EMG, ledState);
  pinMode(FLEX_COM_POOR, OUTPUT);
  pinMode(FLEX_COM_AVG, OUTPUT);
  pinMode(FLEX_COM_EXCL, OUTPUT);
  pinMode(FSR_COM_POOR, OUTPUT);
  pinMode(FSR_COM_AVG, OUTPUT);
  pinMode(FSR_COM_EXCL, OUTPUT);
  pinMode(EMG_COM_POOR, OUTPUT);
  pinMode(EMG_COM_EXCL, OUTPUT);
  Serial.println("Time FlexSensor FSR EMG");

  button.attach ( BUTTON_PIN , INPUT );
  button.interval( 5 );
  button.setPressedState( LOW ); 
}

void loop() {
  // digitalWrite(LED, HIGH);
  //need to reimplement 
  time = millis();
  flex = read_flexSensor();
  fsr = read_FSR();
  emg = read_EMG();

  //Button Bounce
  button.update();
  if ( button.pressed() ) {
    // TOGGLE THE LED STATE : 
    ledState = !ledState; // SET ledState TO THE OPPOSITE OF ledState
    digitalWrite(LED_FSR, ledState);
    // flex_code = 0;
    // fsr_code = 0;
    // emg_code = 0;
  
  }
  
  //Send the signal output to Uno
  digital_output();

  //Print Output on one line
  String p1= " ";
  Serial.println(time + p1 + flex + p1 + fsr + p1 + emg);
  // Serial.println(flex_code + p1 + fsr_code + p1 + emg_code);
  delay(100);
}

//ToDo:
//create a general program to read sensors 
int read_sensor();


int read_flexSensor(){
  int val = analogRead(flexSensor);
  val > FLEX_EXCELLENT ? analogWrite(LED_flexSensor, 255) : analogWrite(LED_flexSensor, 0);
  if (val > FLEX_EXCELLENT)
    flex_code = flex_code | 8;
  if (val >= FLEX_POOR && val <= FLEX_AVERAGE)
    flex_code = flex_code | 2;
  if (val < FLEX_POOR)
    flex_code = flex_code | 1;
  return val;
}

int read_FSR(){
  int val = analogRead(FSR);
  val < FSR_EXCELLENT ? analogWrite(LED_FSR, 255) : analogWrite(LED_FSR, 0);
  if (val <= FSR_EXCELLENT)
    fsr_code = fsr_code | 8;
  if (val >= FSR_AVERAGE && val <= FSR_POOR)
    fsr_code = fsr_code | 2;
  if (val > FSR_POOR)
    fsr_code = fsr_code | 1;
  return val;
}

int read_EMG(){
  int val = analogRead(EMG);
  //val < EMG_EXCELLENT ? analogWrite(LED_EMG, 255) : analogWrite(LED_EMG, 0);
  if (val >= EMG_EXCELLENT){
    emg_code = emg_code | 8;
    analogWrite(LED_EMG, 0);
  }
  if (val < EMG_EXCELLENT){
    emg_code = emg_code | 1;
    analogWrite(LED_EMG, 255);
  }
  return val;
}

//Input the code values 
int digital_output(){
  if(flex_code == 11){
    digitalWrite(FLEX_COM_EXCL, HIGH);
    digitalWrite(FLEX_COM_AVG, LOW);
    digitalWrite(FLEX_COM_POOR, LOW);
    // Serial.println("Flex Excellent");
  }
  if(flex_code == 3){
    digitalWrite(FLEX_COM_EXCL, LOW);
    digitalWrite(FLEX_COM_AVG, HIGH);
    digitalWrite(FLEX_COM_POOR, LOW);
    // Serial.println("Flex Average");
  }
  if(flex_code == 1){
    digitalWrite(FLEX_COM_EXCL, LOW);
    digitalWrite(FLEX_COM_AVG, LOW);
    digitalWrite(FLEX_COM_POOR, HIGH);
    // Serial.println("Flex Poor");
  }
  if(fsr_code == 11){
    digitalWrite(FSR_COM_EXCL, HIGH);
    digitalWrite(FSR_COM_AVG, LOW);
    digitalWrite(FSR_COM_POOR, LOW);
    // Serial.println("FSR Excellent");
  }
  if(fsr_code == 3){
    digitalWrite(FSR_COM_EXCL, LOW);
    digitalWrite(FSR_COM_AVG, HIGH);
    digitalWrite(FSR_COM_POOR, LOW);
    // Serial.println("FSR Average");
  }
  if(fsr_code == 1){
    digitalWrite(FSR_COM_EXCL, LOW);
    digitalWrite(FSR_COM_AVG, LOW);
    digitalWrite(FSR_COM_POOR, HIGH);
    // Serial.println("FSR Poor");
  }
  if(emg_code == 9){
    digitalWrite(EMG_COM_EXCL, HIGH);
    digitalWrite(EMG_COM_POOR, LOW);
    // Serial.println("EMG Excellent");
  }
  if(emg_code == 1){
    digitalWrite(EMG_COM_EXCL, LOW);
    digitalWrite(EMG_COM_POOR, HIGH);
    // Serial.println("EMG Poor");
  }
  return 1;
}

//Communicate data with PuTTY
//Source: https://datalab.medium.com/arduino-ide-serial-data-export-by-putty-6a77631a23ea

//Button with interrupt to reset data
//Source: https://projecthub.arduino.cc/ronbentley1/button-switch-using-an-external-interrupt-16d57f
//Debounce - https://github.com/thomasfredericks/Bounce2

//Todo: Create buttons for different screens
//Todo: Training for User with Dynamometer
//Todo: Logic for outputting different digital read values from sensor script
//Todo: Calculating ALS likelihood from oled script
//Todo: Organize notebook and document
//Todo: implement general function to read sensor 