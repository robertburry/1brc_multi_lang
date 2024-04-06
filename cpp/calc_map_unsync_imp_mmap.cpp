#include <iostream>
#include <map>
#include <cmath>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <vector>


using namespace std;

int main() {
    const char* data_file_path = "../data/measurements_1_000_000_000.txt";
    int fd = open(data_file_path, O_RDONLY);
    if (fd == -1) {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        cerr << "Error getting file size: " << strerror(errno) << endl;
        close(fd);
        return EXIT_FAILURE;
    }

    char* mapped = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (mapped == MAP_FAILED) {
        cerr << "Error mapping file: " << strerror(errno) << endl;
        close(fd);
        return EXIT_FAILURE;
    }

    map<string, vector<int>> city_data;
    size_t start = 0;

    for (size_t i = 0; i < sb.st_size; ++i) {
        if (mapped[i] == '\n' || i == sb.st_size - 1) { // Also handle the last line without newline character
            string line(mapped + start, i - start);
            size_t delim_pos = line.find(';');
            if (delim_pos != string::npos) {
                string city = line.substr(0, delim_pos);
                string temp_str = line.substr(delim_pos + 1);
                int temp = static_cast<int>(round(stod(temp_str) * 10));
                
                auto& stats = city_data[city];
                if (stats.empty()) stats.resize(4, 0);
                stats[0] += temp;
                stats[1] = stats[1] == 0 ? temp : min(stats[1], temp);
                stats[2] = max(stats[2], temp);
                stats[3] += 1;
            }
            start = i + 1; // Move start to the character after newline
        }
    }

    munmap(mapped, sb.st_size);
    close(fd);

    for (const auto& [city, stats] : city_data) {
        float average = stats[0] / static_cast<float>(stats[3]); // Correct division for average
        cout << city << " = " << (stats[1] / 10.0) << "/" << average << "/" << (stats[2] / 10.0) << "\n";
    }

    return 0;
}
