/*
 * SimpleTimerAlarmExample.pde
 *
 * Based on usage example for Time + TimeAlarm libraries
 *
 * A timer is called every 15 seconds
 * Another timer is called once only after 10 seconds
 * A third timer is called 10 times.
 *
 */
 
#include <SimpleTimer.h>
 
// There must be one global SimpleTimer object.
// More SimpleTimer objects can be created and run,
// although there is little point in doing so.
SimpleTimer timer;
 
// function to be called repeatedly
void RepeatTask() {
  Serial.println("15 second timer");        
}
 
// function to be called just once
void OnceOnlyTask() {
  Serial.println("This timer only triggers once");  
}
 
// function to be called exactly 10 times
void TenTimesTask() {
  static int k = 0;
  k++;
  Serial.print("called ");
  Serial.print(k);
  Serial.println(" / 10 times.");
}
 
// print current arduino "uptime" on the serial port
void DigitalClockDisplay() {
  int h,m,s;
  s = millis() / 1000;
  m = s / 60;
  h = s / 3600;
  s = s - m * 60;
  m = m - h * 60;
  Serial.print(h);
  printDigits(m);
  printDigits(s);
  Serial.println();
}
 
//
// utility function for digital clock display:
// prints preceding colon and leading 0
//
void printDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
 
long delayThenPressButton( int callNumber ) {
	if ( callNumber==0 ) {
		return 10000;
	} else if ( callNumber ==1 ) {
		pinMode(10,OUTPUT);
		digitalWrite(10,HIGH);
		return 300;
	} else if ( callNumber ==2 ) {
		digitalWrite(10,LOW);
		pinMode(10,INPUT);
		return 0;
	}	
}
 
void setup() {
  Serial.begin(9600);
 
  // welcome message
  Serial.println("SimpleTimer Example");
  Serial.println("One timer is triggered every 15 seconds");
  Serial.println("Another timer is set to trigger only once after 10 seconds");
  Serial.println("Third Timer Runs Once a second");
  Serial.println("Another timer is set to trigger 10 times");
  Serial.println("Last Timer triggers a single output for 300ms after 10 second delay");
  Serial.println();
 
  // timed actions setup
  timer.setInterval(15000, RepeatTask);
  timer.setTimeout(10000, OnceOnlyTask);
  timer.setInterval(1000, DigitalClockDisplay);
  timer.setTimer(1200, TenTimesTask, 10);
  timer.setVariableTimer(delayThenPressButton);
}
 
void loop() {
  // this is where the "polling" occurs
  timer.run();
}

