#ifndef DISPLAY_H
#define DISPLAY_H

#include <main_module.h>

const uint8_t NUMBERS[10] = {
    0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110,
    0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110,
};

namespace Display {
const unsigned long DELAY = 2;

const int SEGMENTS = 8;
const int SEGMENT_PINS[SEGMENTS] = {13, 2, 16, 5, 18, 15, 4, 17};
const int DIGITS = 4;
const int DIGIT_PINS[DIGITS] = {12, 14, 27, 26};

int digitValue[DIGITS];

void setValue(int v) {
  for (int i = DIGITS - 1; i >= 0; i--) {
    digitValue[i] = v % 10;
    v /= 10;
  }
}

void clear() {
  for (int i = 0; i < DIGITS; i++)
    digitalWrite(DIGIT_PINS[i], LOW);
  for (int j = 0; j < SEGMENTS; j++)
    digitalWrite(SEGMENT_PINS[j], HIGH);
}

void setup() {
  for (int i = 0; i < DIGITS; i++)
    pinMode(DIGIT_PINS[i], OUTPUT);
  for (int i = 0; i < SEGMENTS; i++)
    pinMode(SEGMENT_PINS[i], OUTPUT);
  clear();
}

void update() {
  int digit = (millis() / DELAY) % DIGITS;
  int lastDigit = (digit - 1 + DIGITS) % DIGITS;
  digitalWrite(DIGIT_PINS[lastDigit], LOW);
  uint8_t segments =
      (~(NUMBERS[digitValue[digit]] | (0b00000001 * (digit == 0))));
  for (int i = 0; i < SEGMENTS; i++)
    digitalWrite(SEGMENT_PINS[i], (segments >> (SEGMENTS - i - 1)) & 1);
  digitalWrite(DIGIT_PINS[digit], HIGH);
}
} // namespace Display

#endif // DISPLAY_H