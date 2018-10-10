//iDog Final - A.L.A.N Robotics 

#include <Servo.h>    	// Include servo library
 
Servo servoLeft;    	// Declare left and right servos
Servo servoRight;

int moveCm=0;    		//determines initial distance for hand gestures
int moveCm2=0;    		//has a second distance so that it can compare the two distances
int counter=0;

void setup() {
	
	Serial.begin(9600); 		//initialize serial communication

	servoLeft.attach(12);		// Attach left signal to pin 12
	servoRight.attach(13);      // Attach left signal to pin 13
}

void loop() {
		
	// establish variables for duration of the ping,
	// and the distance result in inches and centimeters:
	long duration, inches, cm;

	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(7, OUTPUT);
	digitalWrite(7, LOW);
	delayMicroseconds(2);
	digitalWrite(7, HIGH);
	delayMicroseconds(5);
	digitalWrite(7, LOW);

	// The same pin is used to read the signal from the PING))): a HIGH
	// pulse whose duration is the time (in microseconds) from the sending
	// of the ping to the reception of its echo off of an object.
	pinMode(7, INPUT);
	duration = pulseIn(7, HIGH);

	// convert the time into a distance

	inches = microsecondsToInches(duration);
	cm = microsecondsToCentimeters(duration);

	Serial.print(inches);
	Serial.print("in, ");
	Serial.print(cm);
	Serial.print("cm");
	Serial.println();

	//dog bone
	if (cm <= 5){                // if an object is within 5 cm, the iDog will stop and "smell" it
		doNothing();
		moveCm = 0;
	}
	else if (cm <=10){              // if an object is further away than 5cm but within 10 cm the iDog will move forward slowly. 
		slowForward(100);
		moveCm = 0;
	}
	else if (cm <= 20){             // if an object is further away than 10cm but within 20 cm the iDog will move forward faster and bark.
		forward(100);
		bark();
		moveCm = 0;
	}

	//move closer or away
	if(cm <=100 && cm > 20){       	//makes the maximum range 100 and minimum range 20. Hand gesture range. 
		if(moveCm ==0){             //sets initial distance
				moveCm = cm;
	}
	
	if(moveCm != 0){
		moveCm2 = cm;               //when the initial distance is satisfied, the second distance is measured. Then it compares the two.
		if(moveCm > moveCm2){       //means the hand is moved closer
			backward(1000);                         // Back up 1 second
			doNothing();
		}
		if(moveCm < moveCm2){       //means the hand is moved further away
			forward(1000);                         // Back up 1 second
		doNothing();
		}
		if(moveCm == moveCm2){          //means if the iDog senses something at the same range, it will do nothing. 
			doNothing();
		}
		moveCm, moveCm2 = 0;  
	}
}  
delay(1000);                    //we have this in order for each loop to last at least 1 second, that way it can measure time.

if(cm > 100){                   // if the iDOg does not sense anything within 100 cm, it will add 1 to the seconds counter
  counter++;
}

if(counter == 5 ){              // if the iDOg counts to 5, it will turn left in order to scan the environment
  turnLeft(600);
  doNothing();
  delay(10);
}

if(counter == 10){              // if the iDog counts to 10, it will turn right in order to scan the environment
  turnRight(1200);
  doNothing();
  delay(10);
}

if(counter == 15){              // if the iDog counts to 15, it will get bored and chase its tail by turning right for 3 seconds. 
  counter = 0;
  turnRight(3000);
  doNothing();
  delay(10);
}

}
long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}




long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}




void forward(int time)                       // Forward function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}




void slowForward(int time)                   // slower Forward function
{
  servoLeft.writeMicroseconds(1550);          // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);          // Right wheel clockwise
  delay(time);                                // Maneuver for time ms
}




void turnLeft(int time)                      // Left turn function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}




void turnRight(int time)                     // Right turn function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}




void backward(int time)                      // Backward function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}




void doNothing(){                             // Do nothing function
  servoLeft.writeMicroseconds(1500);          // Left wheel stand still
  servoRight.writeMicroseconds(1500);         // Right wheel stand still
  delay(100);                                 // 100ms duration
  Serial.print("doNothing");                  // print to serial monitor. 
}
void bark(){                                  // Speaker function
  tone(4,3000,200);                           // port 4, 3000hz, for 200ms duration. 
}
