/*
  Arduino - Switch Configuration for Gesture Sense IoT
  © Sri Pranav Srivatsavai

  This sketch focuses on two main componenets:
  - Acheiving the single press, double press, and hold for 3 second input from the switch
  - Reads the gyroscope values from the LSM6DS3 sensor and prints them to the Serial Monitor or Serial Plotter, 
    as a directional detection of an axis' angular velocity.

  Circuit:
  Arduino Nano 33 IoT

  ChatGPT was used for:
  - debugging the code
  - finding solutions to achieve the 'hold for 3 second' result even after the press wasn't released after 3 seconds.

  Special References:
  - Guides at NYU ITP for physical computing.
  - Professor Johnston for his help and feedback.
*/

#include <Arduino_LSM6DS3.h> // Include the LSM6DS3 library

//global variables for gyroscope
float x, y, z;
int plusThreshold = 100, minusThreshold = -100;
int delayRollPitch = 500; //delay set to 0.5 seconds

//global variables for switch debouncing
const int buttonPin = 2; //input for the button
int buttonState; //high or low, read in the loop()
int lastButtonState = LOW; //initialize
unsigned long buttonPressedTime = 0; //to compare with millis() later
bool hasPrinted3SecHold = false;  // flag to avoid multiple "Button held for 3 seconds" prints/triggers

void setup() {
  Serial.begin(9600);

  //wait for serial monitor
  while (!Serial);
  Serial.println("Started");

  //initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //initialize pin as input for the button
  pinMode(buttonPin, INPUT);

  //Gyroscope readings
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");  
  Serial.println();
  Serial.println("Gyroscope in degrees/second");

}

void loop() {
  //gyroscope code
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z); //reads the gyroscope data and stores it in x,y,z variables (declared globally)
  }
  if(y > plusThreshold)
  {
    Serial.println("Collision front");
    delay(delayRollPitch);
  }
  if(y < minusThreshold)
  {
    Serial.println("Collision back");
    delay(delayRollPitch);
  }
  if(x < minusThreshold)
  {
    Serial.println("Collision left");
    delay(delayRollPitch);
  }
    if(x > plusThreshold)
  {
    Serial.println("Collision right");
    delay(delayRollPitch);
  }

  //switch debouncing code
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
