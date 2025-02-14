#include <Arduino.h>
#include <display.h>
#include <map>
#include <morse.h>
#include <pot.h>
#include <puzzle_module.h>
#include <rules.h>
#include <utils/Button.h>

const int RED_PIN = 22, GREEN_PIN = 23;

const int STATION_RANGE = 256;
const int STATION_TOLERANCE = 120;
int selected_station = -1;

int answer = 0;

const int SUBMIT_BUTTON_PIN = 25;
Button submitButton;

int station();
void submit(ButtonState state, ButtonState _);

void start() { Morse::startTime = millis(); }

void restart() {
  Morse::clear();
  Display::clear();
}

void onManualCode(int code) {
  StationWords words = generate_words(code);
  answer = esp_random() % STATIONS;
  Morse::setup(words[answer].c_str());
}

void setup() {
  Module::name = "Morse Code";
  Module::onStart = start;
  Module::onRestart = restart;
  Module::onManualCode = onManualCode;

  PuzzleModule::statusLight = PuzzleModule::StatusLight(RED_PIN, GREEN_PIN);

  if (!PuzzleModule::setup())
    ESP.restart();

  submitButton = Button(SUBMIT_BUTTON_PIN);
  submitButton.onStateChange = submit;

  Display::setup();
  Pot::setup();
}

void submit(ButtonState state, ButtonState _) {
  if (state != ButtonState::Pressed)
    return;
  if (selected_station == answer)
    PuzzleModule::solve();
  else
    PuzzleModule::strike();
}

int station() {
  float t = ((double)Pot::value()) / (4096.0f);
  return t * STATIONS;
}

bool in_range() {
  int lower = selected_station * STATION_RANGE - STATION_TOLERANCE;
  int upper = (selected_station + 1) * STATION_RANGE + STATION_TOLERANCE;
  int potVal = Pot::value();
  return lower <= potVal && potVal <= upper;
}

void loop() {
  PuzzleModule::update();
  Pot::update();
  if (selected_station == -1 || !in_range())
    selected_station = station();
  Display::setValue(stations[selected_station]);
  Display::update();
  if (Module::status() != Module::Status::Started)
    return;
  Morse::update();
  submitButton.update();
}