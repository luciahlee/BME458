//Declaration of pin values
#define LED1Pin    3
#define LED2Pin    5
#define LED3Pin    9
int flexSensor = A0;
int FSR = A2;
int EMG = A4

//Variables
int grip_strength;
int grip_strength_pwm;
int fsr;
int fsr_pwm;
int emg;
int emg_pwm;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

//Code for Flex Sensor
grip_strength = analogRead(flexSensor);
Serial.print("Flex Sensor:");
Serial.println(grip_strength);
// grip_strength_pwm = map(grip_strength, )
grip_strength > 710 ? analogWrite(LED1Pin, 255) : analogWrite(LED1Pin, 0);

//Code for FSR
fsr = analogRead(FSR);
Serial.print("FSR:");
Serial.println(fsr);
// grip_strength_pwm = map(grip_strength, )
fsr < 500 ? analogWrite(LED2Pin, 255) : analogWrite(LED2Pin, 0);

//Code for EMG
emg = analogRead(EMG);
Serial.print("FSR:");
Serial.println(fsr);
// grip_strength_pwm = map(grip_strength, )
fsr < 500 ? analogWrite(LED2Pin, 255) : analogWrite(LED2Pin, 0);


//Code for OLED

delay(100);
}

//Todo: Write code for OLED Device
//Todo: Attatch flex sensor to dynamometer
//Todo: Create device for CAD
//Todo: Organize Code to be easier to read
//Todo: Organize notebook and document
//Todo: figure out how to read EMGs
