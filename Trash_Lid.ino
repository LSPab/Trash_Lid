#include <NewPing.h>
#include <Servo.h>

#define MAX_DISTANCE 600
#define RELAY 5
#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define ECHO_PIN 7
#define TRIGGER_PIN 6

Servo servo1;
Servo servo2;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

long duration;
int distance;

bool sensorTriggered = false;
bool codeUnlocked = false;

int code[] = {3, 2, 1}; 
int entered[3] = {0, 0, 0}; 
int currentIndex = 0;

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);

  servo1.attach(8); 
  servo2.attach(9);

  servo1.write(90); // Start position
  servo2.write(90);

  digitalWrite(RELAY, HIGH); 

  Serial.begin(9600);
}

void loop() {
  
  if (digitalRead(BUTTON_1) == LOW) {
    handleButton(1);
    delay(250);
  } else if (digitalRead(BUTTON_2) == LOW) {
    handleButton(2);
    delay(250);
  } else if (digitalRead(BUTTON_3) == LOW) {
    handleButton(3);
    delay(250);
  }

  
  if (!codeUnlocked && currentIndex == 3) {
    if (entered[0] == code[0] && entered[1] == code[1] && entered[2] == code[2]) {
      digitalWrite(RELAY, HIGH); // Unlock
      servo1.write(90);
      servo2.write(90);
      codeUnlocked = true;
      Serial.println("Code correct, unlocked.");
      
      
    } else {
      Serial.println("Incorrect code.");
      resetEnteredCode();
    }
  }

  
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  
  if (distance <= 50 && distance != 0 && !sensorTriggered) {
    sensorTriggered = true;
    if (sensorTriggered == true){
    digitalWrite(RELAY, LOW); // Lock again
    Serial.println("Target detected. Locking and rotating servos.");
    codeUnlocked = false;
    sensorTriggered = false;

    servo1.write(0);
    servo2.write(0);
    }
  }

  delay(100);
}

void handleButton(int button) {
  if (currentIndex < 3) {
    entered[currentIndex] = button;
    currentIndex++;
    Serial.print("Button pressed: ");
    Serial.println(button);
  }
}

void resetEnteredCode() {
  for (int i = 0; i < 3; i++) {
    entered[i] = 0;
    codeUnlocked = false;
  }
  currentIndex = 0;
}