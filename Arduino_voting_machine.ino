/*
   Voting Machine Code
   Developed for Kitkraft.in

   This code implements a basic voting system using:
   - 7 buttons (for voting inputs)
   - 7 LEDs (to indicate which button was pressed)
   - 1 buzzer (to provide audio feedback)
   - 1 OLED display (to show voting information)

   Each button corresponds to a candidate or option. Pressing a button registers a vote, 
   activates an LED, and updates the total vote count displayed on the OLED.

   Hardware requirements:
   - Adafruit SSD1306 OLED Display
   - 7 momentary push buttons
   - 7 LEDs with resistors
   - 1 buzzer
   - Arduino-compatible microcontroller
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create an SSD1306 display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin Assignments
const int buttonPins[7] = {2, 3, 4, 5, 6, 7, 8}; // Buttons for voting
const int ledPins[7] = {10, 11, 12, 13, A0, A1, A2}; // LEDs for visual feedback
const int buzzerPin = A3; // Buzzer for audio feedback

// Array to store vote counts for each option
int voteCounts[7] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED initialization failed!"));
    for (;;); // Stop program if OLED fails
  }
  display.clearDisplay();
  display.setTextColor(WHITE); // Set text color to white (for OLED)

  // Configure pins
  for (int i = 0; i < 7; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); // Set buttons as input with pull-up resistors
    pinMode(ledPins[i], OUTPUT);         // Set LEDs as output
    digitalWrite(ledPins[i], LOW);       // Turn off LEDs initially
  }
  pinMode(buzzerPin, OUTPUT);            // Set buzzer as output
  digitalWrite(buzzerPin, LOW);          // Ensure buzzer is off initially

  // Display welcome message
  display.clearDisplay();
  display.setTextSize(2);                // Use large font for better visibility
  display.setCursor(0, 0);
  display.println("Voting");
  display.setCursor(0, 20);
  display.println("Machine");
  display.display();
  delay(2000);                           // Show the message for 2 seconds
}

void loop() {
  // Check all buttons in a loop
  for (int i = 0; i < 7; i++) {
    if (digitalRead(buttonPins[i]) == LOW) { // Detect button press (active LOW)
      voteCounts[i]++;                       // Increment the vote counter for the button

      // Turn on the corresponding LED
      digitalWrite(ledPins[i], HIGH);

      // Activate buzzer for feedback
      digitalWrite(buzzerPin, HIGH);
      delay(500); // Buzzer on for 0.5 second
      digitalWrite(buzzerPin, LOW);

      // Display vote confirmation on OLED
      display.clearDisplay();
      display.setTextSize(2); // Large text for main message
      display.setCursor(0, 0);
      display.println("Vote OK!"); // Confirm vote registration
      display.setTextSize(1); // Smaller text for details
      display.setCursor(0, 32);
      display.print("Button: ");
      display.print(i + 1); // Show the button number
      display.print(" Votes: ");
      display.println(voteCounts[i]); // Display total votes for the option
      display.display();
      delay(2000); // Hold the message for 2 seconds

      // Turn off the LED
      digitalWrite(ledPins[i], LOW);

      // Display "Ready for Next Vote" message
      display.clearDisplay();
      display.setTextSize(2); // Large text for "Ready"
      display.setCursor(0, 16); // Center vertically
      display.println("READY");
      display.display();
      delay(1000); // Brief pause before next vote
    }
  }
}
