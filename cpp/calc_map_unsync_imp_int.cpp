#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

int main() {
    std::ios::sync_with_stdio(false);
    fs::path data_file = "../data/measurements_1_000_000_000.txt";
    ifstream data(data_file);

    if (!fs::exists(data_file)) {
        cerr << "Error Opening File" << endl;
        exit(EXIT_FAILURE);
    }

    // Changing the map to hold a vector of doubles
    map<string, vector<int>> city_data{};

    while (data.good()) {
    string city{}, temp_str{};
    int temp{};
    if (getline(data, city, ';') && getline(data, temp_str)) {
        temp = static_cast<int>(round(stod(temp_str) * 10));
        
        auto it = city_data.find(city);
        if (it != city_data.end()) {
            // City exists, so update its stats vector directly.
            auto& stats = it->second;
            stats[0] += temp; // Update sum with the new temperature.
            stats[1] = min(stats[1], temp); // Update minimum temperature if the new one is lower.
            stats[2] = max(stats[2], temp); // Update maximum temperature if the new one is higher.
            stats[3] += 1; // Increment the count of temperature readings for this city.
        } else {
            // City does not exist in the map, so insert a new entry with the initial temperature stats.
            city_data[city] = vector<int>{temp, temp, temp, 1};
        }
    }
}

    // Outputting the results
    for (const auto& [city, stats] : city_data) {
        float average = (stats[0] / 10) / (stats[3] / 10); // Average = Sum / Count
        cout << city << " = " << (stats[1] / 10) 
            << "/" << average << "/" << (stats[2] / 10) << " ";
    }
}
