#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG(x, ...) printf(x "\n", ##__VA_ARGS__);
#define PANIC(x, ...)                     \
  fprintf(stderr, x "\n", ##__VA_ARGS__); \
  return EXIT_FAILURE;

#define MAX_DISTANCE 3
#define REPORT_BUFSIZ 20

// Calculate the distance (difference) between a and b
int dist(int a, int b)
{
  if (a > b)
    return a - b;
  if (a < b)
    return b - a;
  return 0;
}

// Returns -1 on success or the offending level on failure
int is_safe(int _report[], int remove_level)
{
  size_t pos = 0;
  size_t level = 0;
  bool descending = 0;

  size_t i = 0;
  size_t j = 0;
  int report[REPORT_BUFSIZ] = {0};
  printf("\n[L%d]: ", remove_level);

  // Tolerance
  while(_report[i] != 0) {
    if (i != remove_level) {
      report[j] = _report[i];
      j++;
    }
    i++;
  }
  if (remove_level > 0 && report[remove_level-1] == 0) return 0;

  while (report[pos] != 0)
  {
    printf("%d ", report[pos]);

    // Determine direction
    if (level == 1)
    {
      if (report[level] < report[level - 1])
        descending = 1;
    }

    // Check distance
    if (pos > 0)
    {
      if (dist(report[pos], report[pos - 1]) > MAX_DISTANCE)
      {
        printf("-> distance to high: %d ", report[pos],
               dist(report[pos], report[pos - 1]));
        return is_safe(_report, ++remove_level);
      }

      // Check equal numbers
      if (report[pos] == report[pos - 1])
      {
        printf("-> same number ");
        return is_safe(_report, ++remove_level);
      }

      // Wrong direction
      if ((descending == 0 && report[pos] < report[pos - 1]) ||
          (descending == 1 && report[pos] > report[pos - 1]))
      {
        printf("-> wrong direction ");
        return is_safe(_report, ++remove_level);
      }
    }

    pos++;
    level++;
  }
  printf(" [%s] ", descending ? "DESC" : "ASC");
  return -1;
}

int main(int argc, char *argv[])
{
  // Read argument 1
  if (argc < 2)
  {
    PANIC("pass input file as argument");
  }
  char *inputpath = argv[1];

  // Read argument 1 as file
  FILE *input = fopen(inputpath, "r");
  if (input == NULL)
  {
    PANIC("unable to read input file");
  }
  LOG("Input file opened.");

  // Count lines
  size_t lines = 0;
  size_t bufsiz = BUFSIZ;
  char *line = malloc(bufsiz);

  size_t reports_safe = 0;
  size_t reports_unsafe = 0;

  while (getline(&line, &bufsiz, input) != -1)
  {
    lines++;

    // Tokenize lines
    char *token, *string, *tofree;

    int failed = 0;
    int iteration = 0;

    iteration = 0;
    tofree = string = strdup(line);
    assert(string != NULL);
    int report[20] = {0};

    while ((token = strsep(&string, " ")) != NULL)
    {
      report[iteration] = atoi(token);
      iteration++;
    }

    if (is_safe(report, -1) == -1)
    {
      reports_safe++;
      failed = 0;
    }
    else
    {
      reports_unsafe++;
      failed = 1;
    }

    free(tofree);
    printf("-> report %lu %s", lines, failed ? "FAILED" : "OK");
    // getchar();
  }

  LOG("Lines found in input file: %zu", lines);
  LOG("Reports SAFE: %lu UNSAFE: %lu", reports_safe, reports_unsafe);

  free(line);
  fclose(input);
  LOG("Input file closed.");
}
