#include <math.h>

// Define the dipPin array with proper pin numbers
int dipPins[] = {19, 21, 22, 23};  // Correct pin numbers (remove None)

// Function to calculate the dip value based on the pin readings
int dipVal(int* dipPins){
  int sum = 0;
  for(int i = 0; i < 4; i++){
    sum += digitalRead(dipPins[i]) * pow(2, i);  // Use digitalRead to get actual pin values
  }
  return sum;
}

void setup() {
  // Initialize all DIP pins as INPUT
  for(int i = 0; i < 4; i++){
    pinMode(dipPins[i], INPUT_PULLDOWN);
  }
  
  // Start serial communication
  Serial.begin(115200);
}

void loop() {
  // Call dipVal to get the current DIP switch value
  int val = dipVal(dipPins);

  // Process different DIP switch values using a switch statement
  switch(val) {
    case 0:
      Serial.println("Dip value is 0");
      break;
    case 1:
      Serial.println("Dip value is 1");
      break;
    case 2:
      Serial.println("Dip value is 2");
      break;
    case 3:
      Serial.println("Dip value is 3");
      break;
    case 4:
      Serial.println("Dip value is 4");
      break;
    case 5:
      Serial.println("Dip value is 5");
      break;
    case 6:
      Serial.println("Dip value is 6");
      break;
    case 7:
      Serial.println("Dip value is 7");
      break;
    case 8:
      Serial.println("Dip value is 8");
      break;
    case 9:
      Serial.println("Dip value is 9");
      break;
    case 10:
      Serial.println("Dip value is 10");
      break;
    case 11:
      Serial.println("Dip value is 11");
      break;
    case 12:
      Serial.println("Dip value is 12");
      break;
    case 13:
      Serial.println("Dip value is 13");
      break;
    case 14:
      Serial.println("Dip value is 14");
      break;
    case 15:
      Serial.println("Dip value is 15");
      break;
    default:
      Serial.println("Unknown dip value");
      break;
  }

  // Add a small delay to avoid overloading the serial output
  delay(500);
}
