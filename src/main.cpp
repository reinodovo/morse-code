#include <Arduino.h>
#include <PuzzleModule.h>
#include <map>
#include <Rules.h>
#include <utils/Button.h>

#include "display.h"
#include "morse.h"
#include "pot.h"

const int redPin = 22, greenPin = 23;
const int morseLightPin = 33;
const int submitButtonPin = 25;

const int STATION_RANGE = 256;
const int STATION_TOLERANCE = 120;

const int SEGMENTS = 8;
const int SEGMENT_PINS[SEGMENTS] = {13, 2, 16, 5, 18, 15, 4, 17};
const int DIGITS = 4;
const int DIGIT_PINS[DIGITS] = {12, 14, 27, 26};

int answer = 0;
int selected_station = -1;

Button tx;

int station();
void submit(State state);

void onSeed(int seed) {
  StationWords words = generate_words(seed);
  answer = esp_random() % STATIONS;
  Morse::init(morseLightPin, words[answer].c_str());
}

void setup() {
  PuzzleModule::StatusLight statusLight;
  statusLight.init(redPin, greenPin);

  pinMode(morseLightPin, OUTPUT);
  pinMode(submitButtonPin, INPUT);

  if (!PuzzleModule::init(statusLight, onSeed))
    ESP.restart();

  tx = Button(submitButtonPin);
  tx.onStateChange = submit;

  Display::start(DIGIT_PINS, SEGMENT_PINS);
  Pot::init();
}

void submit(State state) {
  if (state != State::PRESSED) return;
  if (selected_station == answer) {
    PuzzleModule::solve();
  } else {
    PuzzleModule::strike();
  }
}

int station() {
  float t = ((double)Pot::value())/(4096.0f);
  return t * STATIONS;
}

bool in_range() {
  int lower = selected_station * STATION_RANGE - STATION_TOLERANCE;
  int upper = (selected_station + 1) * STATION_RANGE + STATION_TOLERANCE;
  int potVal = Pot::value();
  return lower <= potVal && potVal <= upper;
}

void loop() {
  if (selected_station == -1 || !in_range())
    selected_station = station();
  if (PuzzleModule::status() == PuzzleModule::ModuleStatus::Started && Morse::startTime == -1)
    Morse::startTime = millis();
  PuzzleModule::update();
  Display::set_value(stations[selected_station]);
  Display::update();
  Morse::update();
  Pot::update();
  tx.update();
}