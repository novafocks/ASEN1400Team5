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
  pinMode(11, OUTPUT);

  pinMode(2, OUTPUT);   //TestMotor       (change color)

  // turn on Arduino LED
  digitalWrite(3, HIGH);  // Leave on while power is on

  // Print Column Headers
  // Serial.write("Time,Temp1F,Temp2F,RH,Pres,AccX,AccZ\n");
}

void loop() {
  // Turn script running leds OFF at begining of loop
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);

  // Motor off by default 
  // digitalWrite(2, LOW);

  // delay(100);

  // if (temp2C < 25) {
  //   digitalWrite(11, LOW);
  //   digitalWrite(5, LOW);
  // }

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

  Serial.print(String(timeStamp) + "," + String(temp1C) + "," + String(temp2C) + "," + String(RH) + "," + String(psi) + "," + String(accelXG) + "," + String(accelZG) + "N\n");

  // if (temp2C >= 25) {
  //   digitalWrite(11, HIGH);
  //   digitalWrite(5, HIGH);
  // }

  // Temp test for motor
  if (temp2C >= 22.0) {
    digitalWrite(2, HIGH);
    delay(3000);
  }

  delay(100);
}
