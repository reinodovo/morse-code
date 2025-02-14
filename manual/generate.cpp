#include <manual.h>
#include <rules.h>

int main(int argc, char **argv) {
  int code = atoi(argv[1]);
  StationWords words = generate_words(code);

  manual::json data =
      manual::init("Morse Code", "Morse Code",
                   "An antiquated form of naval communication? What next? At "
                   "least it's genuine Morse Code, so pay attention and you "
                   "might just learn something.",
                   APP_VERSION);

  data["stations"] = manual::json::array();
  for (int i = 0; i < STATIONS; i++) {
    data["stations"].push_back(
        {{"word", words[i]},
         {"frequency", std::to_string(stations[i] / 1000) + "." +
                           std::to_string(stations[i] % 1000)}});
  }

  manual::save(data, code);
}