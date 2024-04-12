#include <SoftwareSerial.h>
SoftwareSerial PrimaryArd(0,1);

// define stepper pins
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

const int baudRate = 9600;
int pos = 0;
int step_number = 0;

// Global i, buffer storage, and testing bool
int i = 0;
const int arrSize = 50;
char inputLine[arrSize];
bool stringComplete = false;

// Data being collected
float millisTime(0);
float intTemp(0);
float extTemp(0);
float RH(0);
float pressure(0);
float accelX(0);
float accelZ(0);

// Sets up external Arduino connection
void setup() {
  Serial.begin(baudRate);
  PrimaryArd.begin(baudRate); 

  // Stepper pins for motor
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
}

void loop() {
  // Checks if new input is coming and if we're still available to receive
  while (PrimaryArd.available() > 0 && stringComplete == false) {

    // Start reading
    inputLine[i] = PrimaryArd.read();

    // Check for line break
    if (inputLine[i] == '\n') {
      // Formatting requires N as end for consistency
      // Bad data if doesn't end with N
      if (inputLine[i-1] != 'N') {
        Serial.println("bad string, breaking loop.");
        i = 0;
        PrimaryArd.flush();
        break;
      }

      // We've completed this data!
      stringComplete = true;

      // Clean buffer after line break in array (sets to null)
      for (int k = i + 1; k < arrSize; k++) { 
        inputLine[k] = '\0';
      }
    
    }

    // Commence iteration
    i++;

    // Don't collect greater than buffer allows!
    if (i >= arrSize) {
      i = 0;
      Serial.println("string in danger of overflow, breaking loop.");
      PrimaryArd.flush();
      break;
    }
  }

  // Print to serial once data is collected
  if (stringComplete == true) {

    // Reset to start of buffer
    i = 0;

    // Parse data into variables
    millisTime = getValFromCharArr(inputLine, i,',');
    intTemp = getValFromCharArr(inputLine, i, ',');
    extTemp = getValFromCharArr(inputLine, i, ',');
    RH = getValFromCharArr(inputLine, i, ',');
    pressure = getValFromCharArr(inputLine, i, ',');
    accelX = getValFromCharArr(inputLine, i, ',');
    accelZ = getValFromCharArr(inputLine, i, 'N');

    // Once end of line reached
    if (inputLine[i] == '\n') {
      // Print all data points individually, separated by commas
      Serial.print(millisTime);
      Serial.print(",");
      Serial.print(intTemp);
      Serial.print(",");
      Serial.print(extTemp);
      Serial.print(",");
      Serial.print(RH);
      Serial.print(",");
      Serial.print(pressure);
      Serial.print(",");
      Serial.print(accelX);
      Serial.print(",");
      Serial.println(accelZ);
    } else {
      Serial.println("possible errors, no newline operator at end of string!");
    }

    // Resets string completion, ready to read next string
    stringComplete = false;

    // Reset buffer iterator
    i = 0;
  }
  
  // If accelerometer detects HIGH VALUE, step the motor?
  if (accelX >= 3.0 || accelZ >= 3.0) {
      OneStep(true);
      pos++;
      delay(1);
  } else if (pos > 0) {
      OneStep(false);
      pos--;
      delay(1);
  }

  // Baseline delay in collecting and using data
  delay(10);
}

// Extracts float value from given string
// WARNING: ASSUMES STRING IS FORMATTED CORRECTLY. 
float getValFromCharArr (char arr[], int& idx, char delimiter) {
  String tempStr;
  while(arr[idx] != delimiter) {
    tempStr += arr[idx];
    idx++;
  }
  idx++;
  return tempStr.toFloat();
}

// Stepping motor bullshit control
// Thank you Luke for your service in creating this tomfoolery.
void OneStep(bool dir) {
  if (dir) {
    switch(step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }
  } else {
      switch(step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
    }
  }
  step_number++;
  if (step_number > 3) {
    step_number = 0;
  }
}
