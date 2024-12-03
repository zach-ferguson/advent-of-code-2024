#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 16384
#define SMALL_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  if (!argv[1]) {
    printf("Please provide an input file arg.\n");
    return -1;
  }

  FILE *input = fopen(argv[1], "r");
  if (!input) {
    printf("Failed to open intput file.\n");
    return -1;
  }

  char buffer[BUFFER_SIZE];
  int line_count = 0;
  size_t bytes;

  while ((bytes = fread(buffer, 1, sizeof(buffer), input)) > 0) {
    for (size_t i = 0; i < bytes; i++) {
      if (buffer[i] == '\n') {
        ++line_count;
      }
    }
  }
  rewind(input);

  char line_buffer[SMALL_BUFFER_SIZE];
  int safe_count = 0;
  while(fgets(line_buffer, sizeof(line_buffer), input) != NULL) {
    char *num = strtok(line_buffer, " ");
    if (!atoi(num)) continue;

    int curr = atoi(num);
    int next = atoi(strtok(NULL, " "));
    int inc = 0, dec = 0, hilo = 0;

    if (curr < next) inc = 1;
    if (curr > next) dec = 1;

    hilo = curr;
    int safe = 1;

    while(num != NULL) {
      if (curr == next) safe = 0;
      if ((curr - next) < -3) safe = 0;
      if ((curr - next) > 3) safe = 0;
      if (inc && (next < hilo)) safe = 0;
      if (inc && (next > hilo)) hilo = next;
      if (dec && (next > hilo)) safe = 0;
      if (dec && (next < hilo)) hilo = next;
      if (!safe) break;

      curr = next;
      num = strtok(NULL, " ");
      if (!num) break;
      next = atoi(num);
    }

    if (safe) ++safe_count;
  }
  
  printf("Done!\n");
  printf("Safe count: %d\n", safe_count);

  fclose(input);
  return 0;
}