#include <iostream>
#include <fstream>
// #include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

double round_to_nearest_tenth(double value) {
    return round(ceil(value * 10)) / 10;
}

int main() {
    std::ios::sync_with_stdio(false);
    const string data_file = "../data/measurements_1_000_000_000.txt";
    ifstream data(data_file);

    if(!data) {
        cerr << "Error Opening File" << endl;
        exit(EXIT_FAILURE);
    }

    // Changing the map to hold a vector of doubles
    map<string, vector<double>> city_data{};

    while (data) {
        string city{}, temp_str{};
        double temp{};
        if (getline(data, city, ';') && getline(data, temp_str)) {
            temp = stod(temp_str);
            auto& stats = city_data[city];
            if (stats.empty()) {
                // Initialize with sum, min, max, count
                stats = vector<double>{temp, temp, temp, 1};
            } else {
                // Update sum, min, max, count
                stats[0] += temp; // Sum
                stats[1] = min(stats[1], temp); // Min
                stats[2] = max(stats[2], temp); // Max
                stats[3] += 1; // Count
            }
        }
    }

    // Outputting the results
    for (const auto& [city, stats] : city_data) {
        double average = round_to_nearest_tenth(stats[0] / stats[3]); // Average = Sum / Count
        cout << city << " = " << round_to_nearest_tenth(stats[1]) 
            << "/" << average << "/" << round_to_nearest_tenth(stats[2]) << " ";
    }
}
