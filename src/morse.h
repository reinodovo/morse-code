#ifndef MORSE_H
#define MORSE_H

#include <string.h>
#include <EventQueue.h>

enum MorseTiming
{
    DOT = 1,
    DASH = 3,
    WORD = 7,
};

const int LETTERS = 26;
const int GAP_TIME_MS = 400;
const int MAX_WORD_LENGTH = 10;
const int MAX_MORSE_LENGTH = MAX_WORD_LENGTH * 18 + 8;
const char *MORSE[LETTERS] = {
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-.",
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

namespace Morse
{
    const int MORSE_LIGHT_PIN = 33;

    char morse_word[MAX_MORSE_LENGTH];
    int light_state_sequence_size = 0;
    bool light_state[MAX_MORSE_LENGTH];
    unsigned long startTime = -1;

    bool started() {
        return startTime != -1;
    }

    void setup(const char *w)
    {
        pinMode(MORSE_LIGHT_PIN, OUTPUT);
        digitalWrite(MORSE_LIGHT_PIN, HIGH);

        morse_word[0] = '\0';
        int len = strlen(w);
        for (int i = 0; i < len; i++)
        {
            sprintf(morse_word, "%s%s%s", morse_word, i == 0 ? "" : " ", MORSE[w[i] - 'a']);
        }
        len = strlen(morse_word);
        for (int i = 0; i < len; i++)
        {
            int sz = DOT;
            bool state = HIGH, add_space = false;
            if (morse_word[i] == '-')
            {
                sz = DASH;
            }
            else if (morse_word[i] == ' ')
            {
                sz = DASH;
                state = LOW;
            }
            if ((morse_word[i] == '.' || morse_word[i] == '-') && i + 1 < len && morse_word[i + 1] != ' ')
                add_space = true;
            for (int j = 0; j < sz; j++)
                light_state[light_state_sequence_size++] = state;
            if (add_space)
                light_state[light_state_sequence_size++] = LOW;
        }
        for (int i = 0; i < WORD; i++)
            light_state[light_state_sequence_size++] = LOW;
    }

    void update()
    {
        if (light_state_sequence_size == 0)
            return;
        unsigned long now = millis();
        int index = ((now - startTime) / GAP_TIME_MS) % light_state_sequence_size;
        digitalWrite(MORSE_LIGHT_PIN, light_state[index]);
    }
};

#endif // MORSE_H