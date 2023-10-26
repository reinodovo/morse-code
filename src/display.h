#ifndef DISPLAY_H
#define DISPLAY_H

#include <MainModule.h>
#include <EventQueue.h>

const uint8_t NUMBERS[10] = {
    0b11111100,
    0b01100000,
    0b11011010,
    0b11110010,
    0b01100110,
    0b10110110,
    0b10111110,
    0b11100000,
    0b11111110,
    0b11110110,
};

namespace Display {
    const int DIGITS = 4;
    const int SEGMENTS = 8;
    const unsigned long DELAY = 2;

    int* digitPins;
    int* segmentPins;

    struct DisplayEvent {
        unsigned long time;
        int value, digit;
        bool dot;
    };

    EventQueue<DisplayEvent> displayQueue;

    int value;
    char cvalue[DIGITS];

    void set_value(int v) {
        for (int i = DIGITS - 1; i >= 0; i--) {
            cvalue[i] = v % 10 + '0';
            v /= 10;
        }
    }

    void insert() {
        unsigned long currentTime = millis();
        for (int i = 0; i < DIGITS; i++) {
            displayQueue.push({currentTime + (i + 1) * DELAY, cvalue[i] - '0', i, i == 0});
        }
    }

    void start(const int* digitPins, const int* segmentPins) {
        Display::digitPins = new int[DIGITS];
        Display::segmentPins = new int[SEGMENTS];
        for (int i = 0; i < DIGITS; i++) {
            Display::digitPins[i] = digitPins[i];
            pinMode(digitPins[i], OUTPUT);
            digitalWrite(digitPins[i], LOW);
        }
        for (int i = 0; i < SEGMENTS; i++) {
            Display::segmentPins[i] = segmentPins[i];
            pinMode(segmentPins[i], OUTPUT);
            digitalWrite(segmentPins[i], HIGH);
        }
        insert();
    }

    void update()
    {
        if (displayQueue.isEmpty())
            return;
        DisplayEvent event = displayQueue.front();
        unsigned long currentTime = millis();
        if (event.time > currentTime)
            return;
        displayQueue.pop();
        int lastDigit = (event.digit - 1 + DIGITS) % DIGITS;
        digitalWrite(digitPins[lastDigit], LOW);
        for (int i = 0; i < SEGMENTS; i++) {
            digitalWrite(segmentPins[i], ((~(NUMBERS[event.value] | (0b00000001 * event.dot))) >> (SEGMENTS - i - 1)) & 1);
        }
        digitalWrite(digitPins[event.digit], HIGH);
        if (event.digit + 1 == DIGITS)
            insert();
    }
}

#endif // DISPLAY_H