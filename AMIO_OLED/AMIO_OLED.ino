
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

#define FLEX_COM_POOR  1
#define FLEX_COM_AVG   2
#define FLEX_COM_EXCL  4
#define FSR_COM_POOR   6
#define FSR_COM_AVG    7
#define FSR_COM_EXCL   8
#define EMG_COM_POOR   10
#define EMG_COM_EXCL   11

#define POOR        71
#define AVERAGE     72
#define EXCELLENT   73

ArducamSSD1306 display(OLED_RESET); // FOR I2C

//Function Declarations
void results_display(int val0, int val1, int val2, int val3);
void Diagnosis(int flex, int emg, int FSR);
int performance_calc(int line1, int line2, int line3);
int performance_calc(int line1, int line2);
void print_perf(int value);


void setup(void)
{
	// Start Serial
	Serial.begin(115200);

  // SSD1306 Init
  display.begin();  // Switch OLED
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}


void loop() {
  int EMG_perf = performance_calc(EMG_COM_EXCL, EMG_COM_POOR);
  int flex_perf = performance_calc(FLEX_COM_EXCL, FLEX_COM_AVG, FLEX_COM_POOR);
  int FSR_perf = performance_calc(FSR_COM_EXCL, FSR_COM_AVG, FSR_COM_POOR);

  results_display(0, EMG_perf, flex_perf, FSR_perf);

  delay(1000);
  display.clearDisplay();
}

void initial_display(){
  //put in a function
  display.setCursor(5,20);
  display.println("BME 458 Device: AMIO");
  display.setCursor(5,30);
  display.println("Click Here to Start");
  display.display();
}

//val 0 = ALS diagnosis
//val 1 = EMG performance
//val 2 = FlexSensor performance
//val 3 = FSR performance
void results_display(int val0, int val1, int val2, int val3){
  display.setCursor(0, 0);
  display.println("AMIO Diagnosis Result");
  display.setCursor(10,8);
  display.print("ALS: ");
  display.println("Unlikely");
  //Unlikely, Inconclusive, Likely
  display.setCursor(0,30);
  display.print("EMG: ");
  print_perf(val1);
  display.setCursor(0,40);
  display.print("FlexSensor: ");
  print_perf(val2);
  display.setCursor(0,50);
  display.print("FSR: ");
    print_perf(val3);
  display.display();
}

//To be implemented by Isabel
void Diagnosis(int flex, int emg, int FSR){
  display.println("Likely");
  display.println("Inconclusive");
  display.println("Unlikely");
}

//line1 = digital line read for Excellent 
//line2 = digital line read for Average
//line3 = digital line read for Poor
//Outputs excellet, average, or poor as a macro value
int performance_calc(int line1, int line2, int line3){
  int performance;
  if(digitalRead(line1) == HIGH){
    performance = EXCELLENT;
  }
  else if(digitalRead(line2) == HIGH){
    performance = AVERAGE;
  }
  else if(digitalRead(line3) == HIGH){
    performance = POOR;
  }
  return performance;
}

//line1 = digital line read for Excellent 
//line2 = digital line read for Average
//line3 = digital line read for Poor
//Outputs excellet, average, or poor as a macro value
int performance_calc(int line1, int line2){
  int performance;
  if(digitalRead(line1) == HIGH){
    performance = EXCELLENT;
  }
  else if(digitalRead(line2) == HIGH){
    performance = POOR;
  }
  return performance;
}

//print performance
void print_perf(int value){
  if (value == POOR){
    display.println("Poor");
  }
  if (value == AVERAGE){
    display.println("Average");
  }
  if (value == EXCELLENT){
    display.println("Excellent");
  }
}