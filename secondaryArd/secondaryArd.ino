#include <SoftwareSerial.h>
SoftwareSerial PrimaryArd(0, 1);
#include <Servo.h>

// define stepper pins
//#define STEPPER_PIN_1 9
//#define STEPPER_PIN_2 10
//#define STEPPER_PIN_3 11
//#define STEPPER_PIN_4 12

const int INIT_ALT = 1665; // in meters

const int baudRate = 9600; // number must Serial.Begin() of Primary Arduino

int pos = 0;
int step_number = 0;

const int arrSize = 50;
char inputLine[arrSize];
int i = 0;
bool stringComplete = false;

float millisTime(0);
float intTemp(0);
float extTemp(0);
float RH(0);
float pressure(0);
float accelX(0);
float accelZ(0);

double altitude;
int firstTimeAboveAlt;

void setup() {
  Serial.begin(baudRate);
  PrimaryArd.begin(baudRate); // delclare a Serial port for Primary Arduino
  
//  pinMode(STEPPER_PIN_1, OUTPUT);
//  pinMode(STEPPER_PIN_2, OUTPUT);
//  pinMode(STEPPER_PIN_3, OUTPUT);
//  pinMode(STEPPER_PIN_4, OUTPUT);

}

void loop() {   // put main code here, to run repeatedly:
  //  Serial.println(millis());
  // checking for output sent from PrimaryArd
  while (PrimaryArd.available() > 0 && stringComplete == false) {
    inputLine[i] = PrimaryArd.read();

    //    Serial.print("recieved char["); // print the recieved character and what array element it gets assigned to
    //    Serial.print(i);
    //    Serial.print("]: ");
    //    Serial.println(inputLine[i]);
    
    if (inputLine[i] == '\n') {
      if (inputLine[i - 1] != 'N') { // verifying string is correct
        Serial.println("bad string, breaking loop.");
        i = 0;
        Serial.println(inputLine[i]);
        PrimaryArd.flush();
        break;
      }

      stringComplete = true;
      for (int k = i + 1; k < arrSize; k++) { // set the remainder of the char array after \n to null
        inputLine[k] = '\0';
      }
      //        Serial.println("recieved complete string."); // testing print statement
    }
    else
      i++;
    if (i >= arrSize) {
      i = 0;
      Serial.println("string in danger of overflow, breaking loop.");
      PrimaryArd.flush();
      break;
    }
  }

  // print string to serial monitor once string is complete
  if (stringComplete == true) {
    //    Serial.println("printing complete string."); // testing print statement // time, int temp, ext temp, humidity, press, accelX, accelZ (order of read values)

    //    for (int j=0; j <= i; j++) { //   this loop prints the input exactly as it is read from PrimaryArd
    //      Serial.print(inputLine[j]);
    //    }
    i = 0;
    millisTime = getValFromCharArr(inputLine, i, ',');
    intTemp = getValFromCharArr(inputLine, i, ',');
    extTemp = getValFromCharArr(inputLine, i, ',');
    RH = getValFromCharArr(inputLine, i, ',');
    pressure = getValFromCharArr(inputLine, i, ',');
    accelX = getValFromCharArr(inputLine, i, ',');
    accelZ = getValFromCharArr(inputLine, i, 'N');

    if (inputLine[i] == '\n') {
      //      Serial.println("recieved full line, no errors. ");
      //      Serial.print("time: ");
      Serial.print(millisTime);
      //      Serial.print("; intTemp: ");
      Serial.print(",");
      Serial.print(intTemp);
      //      Serial.print("; extTemp: ");
      Serial.print(",");
      Serial.print(extTemp);
      //      Serial.print("; RH: ");
      Serial.print(",");
      Serial.print(RH);
      //      Serial.print("; press: ");
      Serial.print(",");
      Serial.print(pressure);
      //      Serial.print("; accelX: ");
      Serial.print(",");
      Serial.print(accelX);
      //      Serial.print("; accelZ: ");
      Serial.print(",");
      Serial.println(accelZ);

    } else {
      Serial.println("possible errors, no newline operator at end of string!");
    }
    stringComplete = false; // reset bool value after string has been printed to serial monitor
    i = 0;
  }


  altitude = 0 + (15+273.15)/(-0.0065)*((pressure/14.7)^(-8.31432*-0.0065/9.80665/0.0289644)-1); // calculated in ft

  if (altitude >= 85000 && firstTimeAboveAlt > 0) {
    if (firstTimeAboveAlt
  } else if (altitude >=

  if (fabs(accelZ) >= 3.0 || fabs(accelX) >= 3.0 || altitude >= ) {
    //    for (int i = 0; i < 2048/4; i++) {
    //      Serial.println("activating stepper");
    //      OneStep(true);
    //      pos++;
    //      delay(2);
    //    }


  } else if (pos > 0) {
    //    for (int i = 0; i < 2048/4; i++) {
    //      OneStep(false);
    //      pos--;
    //      delay(2);
    //    }


  }

  delay(10);
}

float getValFromCharArr (char arr[], int &idx, char delimiter) {
  String tempStr;
  while (arr[idx] != delimiter) {
    tempStr += arr[idx];
    idx++;
  }
  idx++;
  //  Serial.print("converting: "); // testing print statements
  //  Serial.println(tempStr);
  return tempStr.toFloat();
}

//void OneStep(bool dir) {
//  if (dir) {
//    switch(step_number) {
//      case 0:
//        digitalWrite(STEPPER_PIN_1, HIGH);
//        digitalWrite(STEPPER_PIN_2, LOW);
//        digitalWrite(STEPPER_PIN_3, LOW);
//        digitalWrite(STEPPER_PIN_4, LOW);
//        break;
//      case 1:
//        digitalWrite(STEPPER_PIN_1, LOW);
//        digitalWrite(STEPPER_PIN_2, HIGH);
//        digitalWrite(STEPPER_PIN_3, LOW);
//        digitalWrite(STEPPER_PIN_4, LOW);
//        break;
//      case 2:
//        digitalWrite(STEPPER_PIN_1, LOW);
//        digitalWrite(STEPPER_PIN_2, LOW);
//        digitalWrite(STEPPER_PIN_3, HIGH);
//        digitalWrite(STEPPER_PIN_4, LOW);
//        break;
//      case 3:
//        digitalWrite(STEPPER_PIN_1, LOW);
//        digitalWrite(STEPPER_PIN_2, LOW);
//        digitalWrite(STEPPER_PIN_3, LOW);
//        digitalWrite(STEPPER_PIN_4, HIGH);
//        break;
//    }
//  } else {
//      switch(step_number) {
//      case 0:
//        digitalWrite(STEPPER_PIN_1, LOW);
//        digitalWrite(STEPPER_PIN_2, LOW);
//        digitalWrite(STEPPER_PIN_3, LOW);
//        digitalWrite(STEPPER_PIN_4, HIGH);
//        break;
//      case 1:
//        digitalWrite(STEPPER_PIN_1, LOW);
//        digitalWrite(STEPPER_PIN_2, LOW);
//        digitalWrite(STEPPER_PIN_3, HIGH);
//        digitalWrite(STEPPER_PIN_4, LOW);
//        break;
//      case 2:
//        digitalWrite(STEPPER_PIN_1, LOW);
//        digitalWrite(STEPPER_PIN_2, HIGH);
//        digitalWrite(STEPPER_PIN_3, LOW);
//        digitalWrite(STEPPER_PIN_4, LOW);
//        break;
//      case 3:
//        digitalWrite(STEPPER_PIN_1, HIGH);
//        digitalWrite(STEPPER_PIN_2, LOW);
//        digitalWrite(STEPPER_PIN_3, LOW);
//        digitalWrite(STEPPER_PIN_4, LOW);
//        break;
//    }
//  }
//  step_number++;
//  if (step_number > 3) {
//    step_number = 0;
//  }
//}
