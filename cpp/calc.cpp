#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

struct calc
{
    /* data */
    char airport[100];
    int count = 0;
    double sum, min, max;
};

static int getcity(const char *city, struct calc calc[], int city_count) {
  for (int i = 0; i < city_count; i++) {
    if (strcmp(calc[i].airport, city) == 0) {
      return i;
    }
  }

  return -1;
}

int compare_airports(const void *a, const void *b){
    const struct calc *calc_a = (const struct calc *)a;
    const struct calc *calc_b = (const struct calc *)b;
    return strcmp(calc_a->airport, calc_b->airport);
}

int main(){
    // const char *data_file = "../data/measurements_1_000_000.txt";
    const char *data_file = "../data/measurements_1_000_000_000.txt";
    FILE *data = fopen(data_file, "r");

    if(!data) {
      perror("Error Openeing File");
      exit(EXIT_FAILURE);
    }
   
    struct calc calc[500];
    int data_count = 0;
    char buffer[1024];
    int i = 0;
    while (fgets(buffer, 1023, data)){
        char *line_pos = strchr(buffer, ';');
        *line_pos = 0x0;
        double temp = strtod(line_pos+1, NULL);

        int city_count = getcity(buffer, calc, i);

        if (city_count < 0) {strcpy(calc[i].airport, buffer);
        calc[i].sum = temp;
        calc[i].max = temp;
        calc[i].min = temp;
        calc[i].count = 1;
        i++;
        } else {
            calc[i].sum += temp;
            calc[i].count += 1;
            if (calc[i].min > temp){
                calc[i].min = temp;
            }
            if (calc[i].max < temp){
                calc[i].max = temp;
            }
        }
    }
   
    fclose(data);
    
    qsort(calc, (size_t)i, sizeof(struct calc), compare_airports);

    for (int j = 0; j < i; j++){
    printf("%s=%.1f/%.1f/%.1f\n", calc[j].airport, calc[j].min,
            calc[j].sum / calc[j].count, calc[j].max);
    }
}
