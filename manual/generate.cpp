#include <manual.h>
#include <rules.h>

manual::json generate_json_for_code(uint16_t code) {
  StationWords words = generate_words(code);

  manual::json data = manual::init(MODULE_NAME, MODULE_NAME,
                                   "An antiquated form of naval communication? What next? At "
                                   "least it's genuine Morse Code, so pay attention and you "
                                   "might just learn something.",
                                   APP_VERSION);

  data["stations"] = manual::json::array();
  for (int i = 0; i < STATIONS; i++) {
    data["stations"].push_back(
        {{"word", words[i]},
         {"frequency", std::to_string(stations[i] / 1000) + "." + std::to_string(stations[i] % 1000)}});
  }

  return data;
}

int main(int argc, char** argv) { manual::run(argc, argv, generate_json_for_code); }