#ifndef MORSE_H
#define MORSE_H

#include <string.h>
#include <EventQueue.h>

const int LETTERS = 26;
const int GAP_TIME_MS = 30;
const int MAX_WORD_LENGTH = 10;
const char* MORSE[LETTERS] = {
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    ".-..",
    "--.",
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.",
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-",
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--..",
};

namespace Morse {
    struct MorseLightEvent {
        unsigned long time;
        bool state;
    };

    EventQueue<MorseLightEvent> morseQueue;

    char word[MAX_WORD_LENGTH];
    int morseLightPin;

    unsigned long insert_event(unsigned long current, int gaps, bool state) {
        current += gaps * GAP_TIME_MS;
        morseQueue.push({current, !state});
    }

    void insert_events() {
        unsigned long current = millis();
        int len = strlen(word);
        for (int i = 0; i < len; i++) {
            if (word[i] == '.') current = insert_event(current, 3, HIGH);
            if (word[i] == '-') current = insert_event(current, 5, HIGH);
            current = insert_event(current, 3, LOW);
        }
        insert_event(current, 5, LOW);
    }

    void init(int pin, const char* w) {
        morseLightPin = pin;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);

        strcpy(word, w);
        insert_events();
    }

    void update() {
        if (morseQueue.isEmpty())
            return;
        MorseLightEvent event = morseQueue.front();
        unsigned long currentTime = millis();
        if (event.time > currentTime)
            return;
        morseQueue.pop();
        digitalWrite(morseLightPin, event.state);
        if (morseQueue.isEmpty())
            insert_events();
    }
};

#endif // MORSE_H