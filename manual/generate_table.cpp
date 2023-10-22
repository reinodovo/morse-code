#include <Rules.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    freopen("./manual/table.html", "w", stdout);
    int seed = atoi(argv[1]);
    StationWords words = generate_words(seed);
    for (int i = 0; i < STATIONS; i++) {
        printf("<tr><td style=\"text-align: center;padding: 0.3em 0.6em 0.2em;\">%s</td><td style=\"text-align: center;direction: ltr;padding: 0.3em 0.6em 0.2em;\">%d.%d MHz</td></tr>\n", words[i].c_str(), stations[i] / 1000, stations[i] % 1000);
    }
}