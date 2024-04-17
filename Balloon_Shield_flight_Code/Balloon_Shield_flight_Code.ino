/* Balloon Shield Rev. 1.0
   Test Code - Jesse Austin 11/20/14
   Updated 12/16/2014 - Chris Koehler for CCCE Workshop 2015
      -Modified from WRSG Arduino Workshop Code
   Updated 01/04/2017 for OpenLog   
   Updated 12/20/2019 for yellow led
   
   Sensor Code for:
   - Temperature x2
   - Humidity
   - Pressure
   - Accelerometer
   - 6 LED Visual Display */

#include <Servo.h>

Servo doorServo;
Servo armServo;
int armPos = 0;

#define heaterPin 8
#define cameraPin 12

// Definitions
// Temperature Sensor #1
int temp1;
float temp1Volt;
float temp1C;
float temp1F;

// Temperature Sensor #2
int temp2;
float temp2Volt;
float temp2C;
float temp2F;

// Humidity Sensor
int humidity;
float humidityVolt;
float RH;

// Presure Sensor
int pressure;
float pressureVolt;
float psi;

// Accelerometer X
int accelX;
float accelXVolt;
float accelXG;

// Accelerometer Z
int accelZ;
float accelZVolt;
float accelZG;

// data
int timeOfLastAccel = 0;
bool blueLedOn = false;

// Altitude (in feet)
float altitude = 0;

// Time keeper
// The time stamp used when recording data points
uint32_t timeStamp = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // setup the LED Visual Display
  pinMode(3, OUTPUT);   //Arduino on      (orange)
  pinMode(4, OUTPUT);   //Internal Temp   (blue)
  pinMode(5, OUTPUT);   //External Temp   (green)
  pinMode(6, OUTPUT);   //Humidity        (blue)
  pinMode(7, OUTPUT);   //Pressure        (red)
  pinMode(9, OUTPUT);   //Accels          (yellow)
  
  pinMode(heaterPin, OUTPUT);
  pinMode(cameraPin, OUTPUT);

  // turn on Arduino LED
  digitalWrite(3, HIGH);  // Leave on while power is on

  doorServo.attach(11);
  armServo.attach(10);

  // Print Column Headers
  // Serial.write("Time,Temp1F,Temp2F,RH,Pres,AccX,AccZ,alt\n"); // commented to leave out the header
}

void loop() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // START SERIAL AND SENSOR STUFF
  
  // Turn script running leds OFF at begining of loop
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);

  // delay(100); // idk what this delay is for

  // Log the time
  timeStamp = millis();
  // Serial.print(timeStamp);
  // digitalWrite(4, HIGH);

  temp1 = analogRead(A0);
  temp1Volt = temp1 * (5.0 / 1023);
  temp1C = (temp1Volt - 0.5) / (0.01);
  // temp1F = (temp1C*(9.0/5.0) + 32);
  // Serial.print(",");
  // Serial.print(temp1C, 2);

  temp2 = analogRead(A1);
  temp2Volt = temp2 * (5.0 / 1023);
  temp2C = (temp2Volt - 0.5) / (0.01);
  // temp2F = (temp2C*(9.0/5.0) + 32);
  // Serial.print(",");
  // Serial.print(temp2C, 2);
  // Serial.write("[" + String(temp2Volt) + "]");
  // digitalWrite(5, HIGH);

  humidity = analogRead(A2);
  humidityVolt = humidity * (5.0 / 1023);
  RH = (((humidityVolt / 5.0) - 0.16) / 0.0062);
  // Serial.print(",");
  // Serial.print(RH, 2);
  // Serial.write("[" + String(humidityVolt) + "]");
  // digitalWrite(6, HIGH);

  pressure = analogRead(A3);
  pressureVolt = pressure * (5.0 / 1023);
  psi = (pressureVolt - 0.5) * (15.0 / 4.0);
  // Serial.print(",");
  // Serial.print(psi, 2);
  // Serial.write("[" + String(pressureVolt) + "]");
  // digitalWrite(7, HIGH);

  accelX = analogRead(A4);
  accelXVolt = accelX * (5.0 / 1023);
  accelXG = (accelXVolt - (3.3 / 2)) / (0.330);
  // Serial.print(",");
  // Serial.print(accelXG, 3);
  // Serial.write("[" + String(accelXVolt) + "]");

  accelZ = analogRead(A5);
  accelZVolt = accelZ * (5.0 / 1023);
  accelZG = (accelZVolt - (3.3 / 2)) / (0.330);
  // Serial.print(",");
  // Serial.print(accelZG, 3);
  // Serial.write("[" + String(accelZVolt) + "]");
  // digitalWrite(9, HIGH);

  // CALCULATE APPROXIMATE ALTITUDE IN FEET (https://www.mide.com/air-pressure-at-altitude-calculator) (calculated in m, converted to feet)
  altitude = (0 + (15+273.15)/(-0.0065)*(pow((psi/14.7),(8.314472*0.0065/(9.80665*0.0289644)))-1))*3.28084;
            // also reference (https://www.weather.gov/media/epz/wxcalc/pressureAltitude.pdf)

  if (!blueLedOn) {
    digitalWrite(4,HIGH);
    blueLedOn = true;
  } else {
    digitalWrite(4,LOW);
    blueLedOn = false;
  }
  
  Serial.print(String(timeStamp) + "," + String(temp1C) + "," + String(temp2C) + "," + String(RH) + "," + 
               String(psi) + "," + String(accelXG) + "," + String(accelZG) + "," + String(altitude) + "," + String(armPos) + "\n");

  // END SERIAL AND SENSOR STUFF                                                                                                                
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // CONDITIONS FOR TURNING ON HEATING PAD
  if (temp1C <= 5.0 || temp2C <= -40.0) {
    digitalWrite(heaterPin,HIGH);
  } else {
    digitalWrite(heaterPin,LOW);
  }
  
//  // CONDITIONS FOR OPENING DOOR, POWERING CAMERA, AND EXTENDING ARM /////////////////////////////////////////////////////
//  if (fabs(accelX) >= 3.0 || fabs(accelZ) >= 3.0) {
//    timeOfLastAccel = millis();
//  }
//  
//  if (altitude <= 15000) {
//    if (altitude <= 14000) { // keep the camera off below 14000 feet
//      digitalWrite(cameraPin,LOW);
//    } else { // turn the camera on a little before the door will open
//      digitalWrite(cameraPin,HIGH);
//    }
//    if (armPos > 0) {
//      doorServo.write(165);
//      armServo.writeMicroseconds(700); 
//      armPos--;
//    } else {
//      doorServo.write(85);
//      armServo.writeMicroseconds(1500);
//    }
//  } // end condition altitude <= 15000
//  
//  else if (altitude <= 80000) { // open the door and start extending the arm
//    digitalWrite(cameraPin,HIGH);
//    doorServo.write(165);
//    
//    if (altitude >= 16000 && (millis() - timeOfLastAccel) >= 120000) { // extend arm above 16000 feet, retract it below
//      if (armPos >= 6000)   { // keep arm stationary (extended)
//        armServo.writeMicroseconds(1500);
//      } else { // extend the arm
//        armServo.writeMicroseconds(2300);
//        armPos++;
//      }
//    } else {
//      if (armPos > 0) { // retract arm
//        armServo.writeMicroseconds(700);
//        armPos--;
//      } else { // keep arm stationary (retracted)
//        armServo.writeMicroseconds(1500);
//      }
//    }
//  } // end altitude <= 80000
//
//  else { // altitude will be greater than 80000 feet
//    
//    if (armPos > 0) { // arm is extended, retract the arm, keep door open
//      doorServo.write(165);
//      armServo.writeMicroseconds(700);
//      armPos--;
//      digitalWrite(cameraPin,HIGH);
//    } else { // arm is retracted, close the door, keep arm in place, turn off camera
//      doorServo.write(85);
//      armServo.write(1500);
//      digitalWrite(cameraPin,LOW);
//    }
//  } // end altitude > 80000 feet

  // TEST CONDITIONS FOR OPENING DOOR, POWERING CAMERA, AND EXTENDING ARM /////////////////////////////////////////////////
  if (accelZ <= 0) {
    timeOfLastAccel = millis();
  }
  
  if (temp2C <= 21.0) {
    if (temp2C <= 20.0) { // keep the camera off below 14000 feet
      digitalWrite(cameraPin,LOW);
    } else { // turn the camera on a little before the door will open
      digitalWrite(cameraPin,HIGH);
    }
    if (armPos > 0) {
      doorServo.write(165);
      armServo.writeMicroseconds(700); 
      armPos--;
    } else {
      doorServo.write(85);
      armServo.writeMicroseconds(1500);
    }
  } // end condition altitude <= 15000
  
  else if (temp2C <= 25.0) { // open the door and start extending the arm
    digitalWrite(cameraPin,HIGH);
    doorServo.write(165);
    
    if (temp2C >= 22.0 && (millis() - timeOfLastAccel) >= 10000) { // extend arm above 16000 feet, retract it below
      if (armPos >= 600)   { // keep arm stationary (extended)
        armServo.writeMicroseconds(1500);
      } else { // extend the arm
        armServo.writeMicroseconds(2300);
        armPos++;
      }
    } else {
      if (armPos > 0) { // retract arm
        armServo.writeMicroseconds(700);
        armPos--;
      } else { // keep arm stationary (retracted)
        armServo.writeMicroseconds(1500);
      }
    }
  } // end altitude <= 80000

  else { // altitude will be greater than 80000 feet
    
    if (armPos > 0) { // arm is extended, retract the arm, keep door open
      doorServo.write(165);
      armServo.writeMicroseconds(700);
      armPos--;
      digitalWrite(cameraPin,HIGH);
    } else { // arm is retracted, close the door, keep arm in place, turn off camera
      doorServo.write(85);
      armServo.write(1500);
      digitalWrite(cameraPin,LOW);
    }
  } // end altitude > 80000 feet  


  // TIME BASED CONTROL OF SERVOS, USE FOR TESTING PURPOSES, COMMENT OUT FOR FINAL UPLOAD
//  if (timeStamp <= 5000) {
//    digitalWrite(5, HIGH);
//    doorServo.write(85);    
//    armServo.writeMicroseconds(1500);
//  } else if (timeStamp > 5000 && timeStamp <= 10000) {
//    digitalWrite(6, HIGH);
//    doorServo.write(165);
//    armServo.writeMicroseconds(2300);
//  } else if (timeStamp > 10000) {
//    digitalWrite(7, HIGH);
//    doorServo.write(85);
//    armServo.writeMicroseconds(800);
//  }

  delay(100);
}
