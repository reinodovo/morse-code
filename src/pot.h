#include <Arduino.h>
#include <queue>

namespace Pot {
    const int POT_PIN = 32;
    const int POT_SAMPLING = 128;

    int potSum = 0, potCount = 0;
    std::queue<int> potQueue;

    void init() {
        pinMode(POT_PIN, INPUT);
    }

    int value() {
        if (potCount == 0) return 0;
        return potSum / potCount;
    }

    void update() {
        int potValue = analogRead(POT_PIN);
        potSum += potValue;
        potQueue.push(potValue);
        potCount++;
        if (potQueue.size() > POT_SAMPLING) {
            potSum -= potQueue.front();
            potQueue.pop();
            potCount--;
        }
    }
};