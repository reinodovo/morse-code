#include <Arduino.h>
#include <display.h>
#include <modules/puzzle_module.h>
#include <morse.h>
#include <pot.h>
#include <rules.h>
#include <utils/Button.h>

#include <map>

const int RED_PIN = 22, GREEN_PIN = 23;
PuzzleModule module(StatusLight(RED_PIN, GREEN_PIN));

const int STATION_RANGE = 256;
const int STATION_TOLERANCE = 120;
int selected_station = -1;

int answer = 0;

const int SUBMIT_BUTTON_PIN = 25;
Button submit_button;

int station();
void submit(ButtonState state, ButtonState _);

void start() { Morse::start_time = millis(); }

void restart() {
  Morse::clear();
  Display::clear();
}

void on_manual_code(int code) {
  StationWords words = generate_words(code);
  answer = esp_random() % STATIONS;
  Morse::setup(words[answer].c_str());
}

void setup() {
  module.on_start(start);
  module.on_reset(restart);
  module.on_manual_code(on_manual_code);

  if (!module.setup()) ESP.restart();

  submit_button = Button(SUBMIT_BUTTON_PIN);
  submit_button.on_state_change(submit);

  Display::setup();
  Pot::setup();
}

void submit(ButtonState state, ButtonState _) {
  if (state != ButtonState::Pressed) return;
  if (selected_station == answer)
    module.solve();
  else
    module.strike();
}

int station() {
  float t = ((double)Pot::value()) / (4096.0f);
  return t * STATIONS;
}

bool in_range() {
  int lower = selected_station * STATION_RANGE - STATION_TOLERANCE;
  int upper = (selected_station + 1) * STATION_RANGE + STATION_TOLERANCE;
  int pot_val = Pot::value();
  return lower <= pot_val && pot_val <= upper;
}

void loop() {
  module.update();
  Pot::update();
  if (selected_station == -1 || !in_range()) selected_station = station();
  Display::set_value(stations[selected_station]);
  Display::update();
  if (module.get_state() != PuzzleModuleState::Started) return;
  Morse::update();
  submit_button.update();
}