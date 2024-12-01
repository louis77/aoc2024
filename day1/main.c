#include <stdio.h>
#include <stdlib.h>

#define LOG(x, ...)   printf(x "\n", ##__VA_ARGS__);
#define PANIC(x, ...) fprintf(stderr, x "\n", ##__VA_ARGS__); return EXIT_FAILURE;

// Compare v1 and v1 as pointers to int. LT = -1, GT = 1, EQ = 0
int cmp(const void* v1, const void* v2) {
    int* i1 = (int*)v1;
    int* i2 = (int*)v2;
    
    if (*i1 < *i2) return -1;
    if (*i1 > *i2) return 1;
    return 0;
}

// Calculate the distance (difference) between a and b
int dist(int a, int b) {
    if (a > b) return a - b;
    if (a < b) return b - a;
    return 0;
}

// Finds the number of times num appears in nums
size_t occurs(int num, int nums[], size_t nums_length) {
    size_t result = 0;
    for (size_t pos = 0; pos < nums_length; pos++)
        if (nums[pos] == num) result++;
    return result;
}


int main(int argc, char* argv[]) {
    // Read argument 1
    if (argc < 2) {
        PANIC("pass input file as argument");
    }
    char * inputpath = argv[1];

    // Read argument 1 as file
    FILE *input = fopen(inputpath, "r");
    if (input == NULL) {
        PANIC("unable to read input file");
    }
    LOG("Input file opened.");

    // Count lines
    size_t lines = 0;
    size_t bufsiz = BUFSIZ;
    char * line = malloc(bufsiz);
    while (getline(&line, &bufsiz, input) != -1) lines++;
    LOG("Lines found in input file: %zu", lines);
    fseek(input, 0, SEEK_SET);

    // Store both arrays 
    int l_vals[lines];
    int r_vals[lines];
    for (size_t pos = 0; pos < lines; pos++)
        fscanf(input, "%d %d", &l_vals[pos], &r_vals[pos]);

    // Cleanup
    free(line);
    fclose(input);
    LOG("Input file closed.");

    // Sort both array
    LOG("Sorting arrays");
    qsort(l_vals, lines, sizeof(int), cmp);
    qsort(r_vals, lines, sizeof(int), cmp);

    // Calculate distances   
    int64_t distance = 0;
    for (size_t pos = 0; pos < lines; pos++) {
        distance += dist(r_vals[pos], l_vals[pos]);
    }

    // Print result
    LOG("DISTANCE: %lld", distance);

    // Calculate Similarity
    size_t similarity = 0;
    for (size_t pos = 0; pos < lines; pos++) {
        size_t occ = occurs(l_vals[pos], r_vals, lines);
        similarity += l_vals[pos] * occ;
    }

    LOG("SIMILARITY: %lu", similarity);
}
