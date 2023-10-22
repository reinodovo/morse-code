#include <Arduino.h>
#include <PuzzleModule.h>
#include <map>
#include <Rules.h>

#include "display.h"
#include "morse.h"

const int redPin = 22, greenPin = 23;
const int morseLightPin = 33;
const int submitButtonPin = 25;
const int potPin = 32;

const int SEGMENTS = 8;
const int SEGMENT_PINS[SEGMENTS] = {13, 2, 16, 5, 18, 15, 4, 17};
const int DIGITS = 4;
const int DIGIT_PINS[DIGITS] = {12, 14, 27, 26};

void setup() {
  PuzzleModule::StatusLight statusLight;
  statusLight.init(redPin, greenPin);

  pinMode(morseLightPin, OUTPUT);
  pinMode(potPin, INPUT);

  if (!PuzzleModule::init(statusLight)) {
    ESP.restart();
  }

  Display::start(DIGIT_PINS, SEGMENT_PINS);
  Morse::init(morseLightPin, "TEST");
}

int selected_station() {
  int potVal = analogRead(potPin);
  float t = ((double)potVal)/(4096.0f);
  return t * STATIONS;
}

void loop() {
  Display::set_value(stations[selected_station()]);
  PuzzleModule::update();
  Display::update();
  Morse::update();
}