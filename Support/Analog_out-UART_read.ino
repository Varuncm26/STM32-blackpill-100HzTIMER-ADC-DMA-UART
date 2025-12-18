#include <Arduino.h>


#define DAC_PIN 25          
#define STEP_TIME 5000      

uint8_t dacValues[] = {
  0,     
  77,   
  155,   
  232    
};

const char* voltageLabel[] = {
  "0V", "1V", "2V", "3V"
};

int stepIndex = 0;
unsigned long lastStepTime = 0;

String inputBuffer = "";


void setup() {
  Serial.begin(115200);     
  delay(1000);
  dacWrite(DAC_PIN, dacValues[stepIndex]);
  
}

void loop() {

  unsigned long currentTime = millis();

  if (currentTime - lastStepTime >= STEP_TIME) {
    lastStepTime = currentTime;

    stepIndex++;
    if (stepIndex >= 4) stepIndex = 0;

    dacWrite(DAC_PIN, dacValues[stepIndex]);
   
  }

  while (Serial.available()) {
    char c = (char)Serial.read();

    if (c == '\n') {
  
      Serial.println(inputBuffer); 
      inputBuffer = ""; 
    } 
    else if (c != '\r') { 
      inputBuffer += c;
    }
  }

}
