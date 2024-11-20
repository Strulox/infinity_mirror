// Purpose : To light up LEDs based on sound levels
// Author : Andrew
// Date : Dec 1, 2023

unsigned long last_event = 0UL;
enum LED { L1 = 2, L2, L3, L4, L5, L6, L7};
int analogPin = A0;
int val = 0;

void setup() {
  for(int i = 1; i <= 7; ++i) 
    pinMode(i, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (millis() - last_event > 200UL) {
    val = analogRead(analogPin);
    last_event = millis();
    Serial.println(val);
    
    if(val > 528) {
      digitalWrite(L7, HIGH);
    } else {
      digitalWrite(L7, LOW);
    }
    if (val > 526) {
      digitalWrite(L6, HIGH);
    } else {
      digitalWrite(L6, LOW);
    } 
    if (val > 524) {
      digitalWrite(L5, HIGH);
    } else {
      digitalWrite(L5, LOW);
    }  
    if (val > 522) {
      digitalWrite(L4, HIGH);
    } else {
      digitalWrite(L4, LOW);
    }  
    if (val > 520) {
      digitalWrite(L3, HIGH);
    } else {
      digitalWrite(L3, LOW);
    }  
    if (val > 518) {
      digitalWrite(L2, HIGH);
    } else {
      digitalWrite(L2, LOW);
    }  
    if (val > 516) {
      digitalWrite(L1, HIGH);
    } else {
      digitalWrite(L1, LOW);
    }  
  }
}
