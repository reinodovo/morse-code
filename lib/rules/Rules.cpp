#include <Rules.h>
#include <algorithm>
#include <random>

std::mt19937 rng;

const int WORDS = 32;
const char *all_words[WORDS] = {
    "shell",  "halls", "slick", "trick", "boxes",  "leaks",  "strobe", "bistro",
    "flick",  "bombs", "break", "brick", "steak",  "sting",  "vector", "beats",
    "streak", "brain", "steer", "stays", "straw",  "steal",  "kicks",  "beach",
    "stock",  "stump", "bring", "broke", "bright", "stairs", "stream", "steel"};

void shuffle(std::vector<std::string> &v, std::mt19937 &rng) {
  for (int i = v.size() - 1; i >= 0; i--) {
    int j = rng() % (i + 1);
    std::swap(v[i], v[j]);
  }
}

StationWords generate_words(int code) {
  rng = std::mt19937(code);
  std::vector<std::string> words(all_words, all_words + WORDS);
  shuffle(words, rng);
  return StationWords(words.begin(), words.begin() + STATIONS);
}