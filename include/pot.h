#include <Arduino.h>

#include <queue>

namespace Pot {
const int POT_PIN = 32;
const int POT_SAMPLING = 128;

int pot_sum = 0, pot_count = 0;
std::queue<int> pot_queue;

void setup() { pinMode(POT_PIN, INPUT); }

int value() {
  if (pot_count == 0) return 0;
  return pot_sum / pot_count;
}

void update() {
  int pot_value = analogRead(POT_PIN);
  pot_sum += pot_value;
  pot_queue.push(pot_value);
  pot_count++;
  if (pot_queue.size() > POT_SAMPLING) {
    pot_sum -= pot_queue.front();
    pot_queue.pop();
    pot_count--;
  }
}
};  // namespace Pot