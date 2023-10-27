#include <Arduino.h>
#include <PuzzleModule.h>
#include <map>
#include <Rules.h>
#include <utils/Button.h>

#include "display.h"
#include "morse.h"
#include "pot.h"

const int RED_PIN = 22, GREEN_PIN = 23;

const int STATION_RANGE = 256;
const int STATION_TOLERANCE = 120;
int selected_station = -1;

int answer = 0;

const int SUBMIT_BUTTON_PIN = 25;
Button submitButton;

int station();
void submit(State state);

void onManualCode(int code)
{
  StationWords words = generate_words(code);
  answer = esp_random() % STATIONS;
  Morse::setup(words[answer].c_str());
}

void setup()
{
  if (!PuzzleModule::setup(PuzzleModule::StatusLight(RED_PIN, GREEN_PIN), onManualCode))
    ESP.restart();

  submitButton = Button(SUBMIT_BUTTON_PIN);
  submitButton.onStateChange = submit;

  Display::setup();
  Pot::setup();
}

void submit(State state)
{
  if (state != State::PRESSED)
    return;
  if (selected_station == answer)
    PuzzleModule::solve();
  else
    PuzzleModule::strike();
}

int station()
{
  float t = ((double)Pot::value()) / (4096.0f);
  return t * STATIONS;
}

bool in_range()
{
  int lower = selected_station * STATION_RANGE - STATION_TOLERANCE;
  int upper = (selected_station + 1) * STATION_RANGE + STATION_TOLERANCE;
  int potVal = Pot::value();
  return lower <= potVal && potVal <= upper;
}

void loop()
{
  if (selected_station == -1 || !in_range())
    selected_station = station();
  if (PuzzleModule::status() == PuzzleModule::ModuleStatus::Started && Morse::startTime == -1)
    Morse::startTime = millis();
  PuzzleModule::update();
  Display::setValue(stations[selected_station]);
  Display::update();
  Morse::update();
  Pot::update();
  submitButton.update();
}