#include <Stepper.h>

//Constants
const int baudRate = 9600;
const int stepsPerRevolution = 360;

// Pins
const int trigPin = 9; //Trigger Pin for the HC-SR04 Sensor
const int echoPin = 10; //Echo Pin for the HC-SR04 Sensor
const int buttonPin = 2; // Pin for the button
const int buzzerPin = 3;
const int greenLEDPin = 5;
const int yellowLEDPin = 6;
const int redLEDPin = 11;
const int stepperP1 = 13;
const int stepperP2 = 12;
const int stepperP3 = 8;
const int stepperP4 = 7;

// Variables
float duration, distance;
int buttonState = 0;
int status = 0;

Stepper myStepper(stepsPerRevolution, stepperP1, stepperP3, stepperP2, stepperP4);

void setup()
{
  // Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Buzzer
  pinMode(buzzerPin, OUTPUT);
  
  // LEDs
  pinMode(greenLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  
  // Button
  pinMode(buttonPin, INPUT);
  
  // Monitoring
  Serial.begin(baudRate);
}

void loop()
{
  myStepper.step(1);
  delay(10);  // Adjust based on your motor specs
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == HIGH){
    if(status < 2)
    	status++;
    else
      status = 0;
  }
  Serial.print("Staus: ");
  Serial.println(status);
  
  if(status == 0){
    digitalWrite(buzzerPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
  }
  else if(status == 1){
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(yellowLEDPin, HIGH);
    digitalWrite(redLEDPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(yellowLEDPin, HIGH);
    digitalWrite(redLEDPin, HIGH);
  }
  else{
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(yellowLEDPin, HIGH);
    digitalWrite(redLEDPin, HIGH);
    delay(50);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    delay(50);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(yellowLEDPin, HIGH);
    digitalWrite(redLEDPin, HIGH);
  }
}