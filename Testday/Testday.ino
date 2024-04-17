#include <SoftwareSerial.h>
SoftwareSerial PrimaryArd(0, 1);
#include <Servo.h>

Servo doorServo;
Servo armServo;

const int baudRate = 9600; // number must Serial.Begin() of Primary Arduino

const int arrSize = 50;
char inputLine[arrSize];
int i = 0;
bool stringComplete = false;
int testOpen=10000;
int testClose=30000;

float millisTime(0);
float intTemp(0);
float extTemp(0);
float RH(0);
float pressure(0);
float accelX(0);
float accelZ(0);

double altitude;
int timeAtAlt = -1;
int timeEncounteredAccel = -1;
bool needToExtend = true;
bool noMoreExtend = false;

float getValFromCharArr (char arr[], int &idx, char delimiter);
void armControl(){
  if(needToExtend=true){armServo.writeMicroseconds(180); delay(500); armServo.writeMicroseconds(90);}
  else if(needToExtend=false){armServo.writeMicroseconds(0); delay(500); armServo.writeMicroseconds(90);}
}
void doorControl(){
  delay(100);
  Serial.println("CONTROLLING DOOR");
  if(needToExtend=true){doorServo.writeMicroseconds(1000); delay(500);}
  else if(needToExtend=false){doorServo.writeMicroseconds(100); delay(700);}
  delay(10);
}

void setup() {
  Serial.begin(baudRate);
  PrimaryArd.begin(baudRate); // delclare a Serial port for Primary Arduino
  armServo.attach(A3);
  digitalWrite(7,HIGH);
  doorServo.attach(A5,0.1,0.2);
}

void loop() {   // put main code here, to run repeatedly:

  //  Serial.println(millis()); // print time, for testing
  
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

    //    for (int j=0; j <= i; j++) { //  this loop prints the input exactly as it is read from PrimaryArd
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

    } 
    else {Serial.println("possible errors, no newline operator at end of string!");}
    stringComplete = false; // reset bool value after string has been printed to serial monitor
    i = 0;
  }

 /* // calculate altitude in feet (https://www.weather.gov/media/epz/wxcalc/pressureAltitude.pdf)
  altitude = 0 + (15+273.15)/(-0.0065)*((pressure/14.7)^(-8.31432*-0.0065/9.80665/0.0289644)-1); 

  if ((altitude >= 15000 && altitude <= 85000) && timeAtAlt > 0) {
    if ((timeAtAlt - millis()) >= 30000) { // 
      needToExtend = true;
    }
  } else if (altitude >= 15000 && altitude <= 85000) {
    timeAtAlt = millis(); // sets the first time within correct altitude range to extend
  } else {
    needToExtend = false;
    timeAtAlt = -1;
    
  }

  if (fabs(accelZ) >= 3.0 || fabs(accelX) >= 3.0) {
    timeEncounteredAccel = millis();
  } else if (timeEncounteredAccel != -1 && (timeEncounteredAccel - millis()) >= 10000) { // checking whether encountered acceleration in last 10 seconds
    timeEncounteredAccel = -1;
  } else {} // still within 10 seconds from large acceleration, do not update

  if (timeEncounteredAccel != -1 || needToExtend == false) { // checks that either accel has been encountered or we are outside altitude range for extending
    // retract the arm, then close the door

  } else {
    // open the door (or keep it open), then extend the arm, or keep it extended 

  }

  delay(10);
}*/
  if (millisTime >= testOpen && millisTime <= testClose && needToExtend==true) {
    //extend arm
    doorControl();
    delay(5);
    armControl();
    needToExtend=false;
}
  if (millisTime >= testClose && needToExtend==false && noMoreExtend==false) {
    // retract the arm, then close the door
    doorControl();
    delay(5);
    armControl();
    noMoreExtend=true;


  }
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
