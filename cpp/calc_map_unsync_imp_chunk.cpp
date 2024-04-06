#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

inline float round_to_nearest_tenth(float value) {
    return ceil(value * 10) / 10;
}

void processLine(const string& line, map<string, vector<float>>& cityData) {
    string city, tempStr;
    float temp;
    istringstream iss(line);
    if (getline(iss, city, ';') && getline(iss, tempStr)) {
        temp = stof(tempStr);
        auto it = cityData.find(city);
        if (it != cityData.end()) {
            auto& stats = it->second;
            stats[0] += temp; // Sum
            stats[1] = min(stats[1], temp); // Min
            stats[2] = max(stats[2], temp); // Max
            stats[3] += 1; // Count
        } else {
            cityData[city] = vector<float>{temp, temp, temp, 1};
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    fs::path data_file = "../data/measurements_1_000_000.txt";

    if (!fs::exists(data_file)) {
        cerr << "Error Opening File" << endl;
        return EXIT_FAILURE;
    }

    ifstream data(data_file, ios::binary);
    //const size_t chunkSize = 1024 * 1024 * 64; // 64 MB
    //vector<char> buffer(chunkSize + 512); // Extra space for partial records
    const std::streamsize chunkSize = static_cast<std::streamsize> (1LL * 1024 * 1024 * 1024); //3 GB
    vector<char> buffer(chunkSize);

    string partialRecord;
    map<string, vector<float>> city_data;

    while (data) {
        data.read(buffer.data() + partialRecord.size(), chunkSize - partialRecord.size());
        streamsize bytesRead = data.gcount();
        string_view chunk(buffer.data(), bytesRead + partialRecord.size());
        partialRecord.clear();

        size_t lastNewLine = chunk.rfind('\n');
        if (lastNewLine != string_view::npos) {
            // Process complete records
            istringstream stream(string(chunk.substr(0, lastNewLine)));
            string line;
            while (getline(stream, line)) {
                processLine(line, city_data);
            }
            // Save partial record for next chunk
            partialRecord = string(chunk.substr(lastNewLine + 1));
        } else {
            // The chunk is a partial record
            partialRecord = string(chunk);
        }

        // Prepend the partial record to the buffer for the next iteration
        copy(partialRecord.begin(), partialRecord.end(), buffer.begin());
    }

    // Process remaining partial record, if any
    if (!partialRecord.empty()) {
        processLine(partialRecord, city_data);
    }

    // Outputting the results
    for (const auto& [city, stats] : city_data) {
        float average = round_to_nearest_tenth(stats[0] / stats[3]); // Average = Sum / Count
        cout << city << " = " << round_to_nearest_tenth(stats[1]) 
             << "/" << average << "/" << round_to_nearest_tenth(stats[2]) << " ";
    }
}
