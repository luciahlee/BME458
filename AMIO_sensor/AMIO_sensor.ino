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
void button_interrupt_handler();
bool read_button();

//Definitions for Buttons
#define LED                         LED_BUILTIN  // digital pin connected to LED, for testing of switch code only
bool    led_status  =                        LOW; // start with LED off, for testing of switch code  only

int     button_switch =                       9; // external interrupt  pin

#define switched                            true // value if the button  switch has been pressed
#define triggered                           true // controls  interrupt handler
#define interrupt_trigger_type            RISING // interrupt  triggered on a RISING input
#define debounce                              10  // time to wait in milli secs

volatile  bool interrupt_process_status = {
  !triggered                                     // start with no switch press pending,  ie false (!triggered)
};
bool initialisation_complete =            false;  // inhibit any interrupts until initialisation is complete


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_flexSensor, OUTPUT);
  pinMode(LED_FSR, OUTPUT);
  pinMode(LED_EMG, OUTPUT);
  pinMode(FLEX_COM_POOR, OUTPUT);
  Serial.println("Time FlexSensor FSR EMG");

  //Code for Button
  pinMode(LED, OUTPUT);
  pinMode(button_switch, INPUT);
  attachInterrupt(digitalPinToInterrupt(button_switch),
                  button_interrupt_handler,
                  interrupt_trigger_type);
  initialisation_complete = true; // open interrupt processing for business
}

void loop() {
  // digitalWrite(LED, HIGH);
  //need to reimplement 
  time = millis();
  flex = read_flexSensor();
  fsr = read_FSR();
  emg = read_EMG(EMG_threshold);

  //Send the signal output to Uno
  digital_output();

  //Print Output on one line
  String p1= " ";
  Serial.println(time + p1 + flex + p1 + fsr + p1 + emg);
  Serial.println(flex_code + p1 + fsr_code + p1 + emg_code);
  delay(100);

  // test buton switch and process  if pressed
  if (read_button() == switched) {
    // button on/off cycle now  complete, so flip LED between HIGH and LOW
    led_status = HIGH - led_status;  // toggle state
    digitalWrite(LED, led_status);
  } else {
    // do  other things....

  }
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
  int val = analogRead(FSR);
  val < FSR_EXCELLENT ? analogWrite(LED_FSR, 255) : analogWrite(LED_FSR, 0);
  if (val <= FSR_EXCELLENT)
    fsr_code = fsr_code | 0100;
  if (val >= FSR_AVERAGE && val <= FSR_POOR)
    fsr_code = fsr_code | 0010;
  if (val > FSR_POOR)
    fsr_code = fsr_code | 0001;
  return val;
}

//Still need to implement 
int read_EMG(int threshold){
  int val = analogRead(EMG);
  val < threshold ? analogWrite(LED_EMG, 255) : analogWrite(LED_EMG, 0);
  return val;
}

//Input the code values 
int digital_output(){
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
  if(fsr_code == 73){
    digitalWrite(FSR_COM_EXCL, HIGH);
    Serial.println("FSR Excellent");
  }
  if(fsr_code == 9){
    digitalWrite(FSR_COM_AVG, HIGH);
    Serial.println("FSR Average");
  }
  if(fsr_code == 1){
    digitalWrite(FSR_COM_POOR, HIGH);
    Serial.println("FSR Poor");
  }
  if(emg_code == 73){
    digitalWrite(FSR_COM_AVG, HIGH);
    Serial.println("EMG Excellent");
  }
  if(emg_code == 1){
    digitalWrite(FSR_COM_POOR, HIGH);
    Serial.println("EMG Poor");
  }
  return 1;
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

//Button with interrupt to reset data
//Source: https://projecthub.arduino.cc/ronbentley1/button-switch-using-an-external-interrupt-16d57f

// The sketch is designed  such that button status is only flagged as 'switched' AFTER
// 1. button is pressed  AND then released, AND
// 2. elapse of the debounce period AFTER release
//
//  Note that the associated button interrupt handler function and the button_read()
//  function work together - the interrupt handler starts the on/off process and the
//  button_read() function completes/concludes it.  The interrupt handler can only restart  AFTER
// button reading and debounce is complete.  This ensures that only one  interrupt trigger is
// processed at a time.
//
// The button switch is  wired in a standard configuration with a 10K ohm pull down resister which
//  ensures the digital interrupt pin is kept LOW until the button switch is pressed  and
// raises it to HIGH (+5v).
//
// Operation of the button is demonstrated  by toggling the in built LED on and off.
//


//
// ISR for  handling interrupt triggers arising from associated button switch
//
void  button_interrupt_handler()
{
  if (initialisation_complete == true)
  {  //  all variables are initialised so we are okay to continue to process this interrupt
    if (interrupt_process_status == !triggered) {
      // new interrupt so okay  start a new button read process -
      // now need to wait for button release  plus debounce period to elapse
      // this will be done in the button_read  function
      if (digitalRead(button_switch) == HIGH) {
        // button  pressed, so we can start the read on/off + debounce cycle wich will
        //  be completed by the button_read() function.
        interrupt_process_status  = triggered;  // keep this ISR 'quiet' until button read fully completed
      }
    }
  }
} // end of button_interrupt_handler

bool read_button() {
  int button_reading;
  // static variables because we need to retain old values  between function calls
  static bool     switching_pending = false;
  static  long int elapse_timer;
  if (interrupt_process_status == triggered) {
    //  interrupt has been raised on this button so now need to complete
    // the button  read process, ie wait until it has been released
    // and debounce time elapsed
    button_reading = digitalRead(button_switch);
    if (button_reading == HIGH)  {
      // switch is pressed, so start/restart wait for button relealse, plus  end of debounce process
      switching_pending = true;
      elapse_timer  = millis(); // start elapse timing for debounce checking
    }
    if (switching_pending  && button_reading == LOW) {
      // switch was pressed, now released, so check  if debounce time elapsed
      if (millis() - elapse_timer >= debounce) {
        // dounce time elapsed, so switch press cycle complete
        switching_pending  = false;             // reset for next button press interrupt cycle
        interrupt_process_status  = !triggered; // reopen ISR for business now button on/off/debounce cycle complete
        return switched;                       // advise that switch has been pressed
      }
    }
  }
  return !switched; // either no press request or debounce  period not elapsed
} // end of read_button function



//Todo: Create buttons for different screens
//Todo: Training for User with Dynamometer
//Todo: Logic for outputting different digital read values from sensor script
//Todo: Calculating ALS likelihood from oled script
//Todo: Organize notebook and document

//Todo: implement general function to read sensor 