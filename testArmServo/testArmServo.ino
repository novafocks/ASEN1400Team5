#include <Servo.h>

Servo myServo;

const int MAX_INPUT = 5;

int servoSpeed;
double pos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myServo.attach(10);

  myServo.write(1500);

  Serial.println("<Arduino Ready> Enter a speed from 700-2300 (1500 is stopped)");

}


void loop() {
  // if serial data available, process it
  while (Serial.available () > 0)
    processIncomingByte (Serial.read ());
    
  

}  // end of loop

// here to process incoming serial data after a terminator received
void process_data (const char * data)
  {
  servoSpeed = atoi(data);
  if (servoSpeed > 2300) {
    servoSpeed = 2300;
  } else if (servoSpeed < 700) {
    servoSpeed = 700;
  } else if (pos == 0 && servoSpeed < 1500) { // guard to prevent negative position
    servoSpeed = 1500; 
  }
  
  pos += (servoSpeed+ (-1500))/800.0*150;
  // just displays data
  Serial.print("Spinning at speed for 15 sec: ");
  Serial.print (servoSpeed);
  Serial.println("; pos: " + String(pos));
  myServo.writeMicroseconds(servoSpeed);
  
  delay(15000);
  myServo.writeMicroseconds(1500);
  
  }  // end of process_data
  
void processIncomingByte (const byte inByte)
  {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
    {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte
      
      // terminator reached! process input_line here ...
      process_data (input_line);
      
      // reset buffer for next time
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
        if (input_pos == MAX_INPUT)
          process_data(input_line);
    
      break;

    }  // end of switch
   
  } // end of processIncomingByte  
