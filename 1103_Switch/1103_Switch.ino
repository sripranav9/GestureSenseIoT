const int buttonPin = 2; //input for the button
int buttonState; //high or low, read in the loop()
int lastButtonState = LOW; //initialize
unsigned long buttonPressedTime = 0; //to compare with millis() later
bool hasPrinted3SecHold = false;  // flag to avoid multiple "Button held for 3 seconds" prints/triggers

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // If button is currently pressed and was previously not pressed
  if (buttonState == HIGH && lastButtonState == LOW) {
    buttonPressedTime = millis(); // Capture the time button was pressed
    hasPrinted3SecHold = false;   // Reset the flag for 3-second hold message
  }

  // If button has been pressed for more than 3 seconds and we haven't printed the message yet
  if (buttonState == HIGH && (millis() - buttonPressedTime) >= 1800 && !hasPrinted3SecHold) {
    Serial.println("Button held for 3 seconds");
    hasPrinted3SecHold = true; // Set flag so we don't print the message multiple times
  }

  // If button is currently not pressed and was previously pressed
  if (buttonState == LOW && lastButtonState == HIGH) {
    unsigned long buttonReleaseDuration = millis() - buttonPressedTime; // Duration for which button was pressed
    
    // If button was not held for long (less than 3 seconds) and wasn't held for a very short time either
    if (buttonReleaseDuration < 1800 && buttonReleaseDuration > 50) {
      delay(250);  // Wait for a short time to see if it's a double press
      if (digitalRead(buttonPin) == HIGH) { // If button is pressed again within 250ms
        Serial.println("Button double pressed");
        while(digitalRead(buttonPin) == HIGH);  // Wait for button to be released to avoid multiple prints
      } else {
        Serial.println("Button pressed once");
      }
    }
  }
  
  lastButtonState = buttonState; // Save the current state of the button for the next loop
}
