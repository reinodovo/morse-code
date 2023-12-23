#ifndef RULES_H
#define RULES_H

#include <string>
#include <vector>

const int STATIONS = 16;
typedef std::vector<std::string> StationWords;

const int stations[STATIONS] = {
    3505, 3515, 3522, 3532, 3535, 3542, 3545, 3552,
    3555, 3565, 3572, 3575, 3582, 3592, 3595, 3600,
};

StationWords generate_words(int);

#endif // RULES_H