#include <Servo.h>

#define servoPin A5
Servo myServo;

const int MAX_INPUT = 4;

int inputSpeed;
int servoSpeed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myServo.attach(servoPin);

  Serial.println("<Arduino Ready> Enter a speed (0 to 180): ");

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
//  servoSpeed = map(inputSpeed,-100,100,0,180); // bad code dont use
  
  myServo.write(servoSpeed);
  // just displays data
  Serial.print("Spinning at speed for 1 sec: ");
  Serial.println (data);
  delay(200);
  myServo.write(90); // stop the servo  
  
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
