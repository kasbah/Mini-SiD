/*

   Mini SiD Example Sketch: Simple Scintillation Counter
   Only works on the Raspberry Pi Pico and arduino-pico!

   Triggers on newly detected pulses and displays average cps values.

   2022, NuclearPhoenix. Open Gamma Project.
   https://github.com/Open-Gamma-Project/Mini-SiD

   Hackaday: https://hackaday.io/project/188090-mini-sipm-driver-board/log/213532-simple-scintillation-counter-example

*/

#include <Adafruit_SSD1306.h>  // Used for OLEDs

const uint8_t INT_PIN = 16;  // Signal interrupt pin
const uint8_t LED = 25;      // LED on GP25
const uint16_t RESET = 2;    // Number of seconds after which the OLED stats will be updated

const uint8_t SCREEN_WIDTH = 128;     // OLED display width, in pixels
const uint8_t SCREEN_HEIGHT = 32;     // OLED display height, in pixels
const uint8_t SCREEN_ADDRESS = 0x3C;  // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
volatile uint32_t counts = 0;  // Accumulated counts


void eventInt() {
  digitalWrite(LED, HIGH);  // Activity LED

  counts++;
  delayMicroseconds(5);  // Show the LED for 5 µs

  digitalWrite(LED, LOW);
}


void setup() {
  pinMode(INT_PIN, INPUT);
  pinMode(LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(INT_PIN), eventInt, FALLING);
}


void setup1() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while (true) {  // Don't go further
      Serial.println("Failed communication with the display. Maybe the I2C address is incorrect?");
      delay(500);
    }
  } else {
    display.setTextSize(2);  // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);

    display.clearDisplay();
    display.println("Mini SiD Example");
    display.display();
    delay(1000);
  }
}


void loop() {
  // Empty
}


void loop1() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(round(float(counts) / float(RESET)), 0);
  display.println(" cps");
  display.display();

  counts = 0;

  delay(RESET * 1000);
}
