#include <Rules.h>
#include <random>
#include <algorithm>

std::mt19937 rng;

const int WORDS = 32;
const char* all_words[WORDS] = {
    "shell", "halls", "slick", "trick", "boxes", "leaks", "strobe", "bistro",
    "flick", "bombs", "break", "brick", "steak", "sting", "vector", "beats",
    "streak", "brain", "steer", "stays", "straw", "steal", "kicks", "beach",
    "stock", "stump", "bring", "broke", "bright", "stairs", "stream", "steel"
};

void shuffle() {
    for (int i = WORDS - 1; i >= 0; i--) {
        int j = rng() % (i + 1);
        std::swap(all_words[i], all_words[j]);
    }
}

StationWords generate_words(int seed) {
    rng = std::mt19937(seed);
    std::shuffle(all_words, all_words + WORDS, rng);
    return StationWords(all_words, all_words + STATIONS);
}