#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 16384
#define SMALL_BUFFER_SIZE 1024

int test_line(char *line_buffer) {
  char *num = strtok(line_buffer, " ");
  if (!atoi(num)) return 0;

  int curr = atoi(num);
  num = strtok(NULL, " ");
  if (!num) {
    return 1;
  }
  int next = atoi(num);
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

  return safe;
}

int test_line_variants(char *buffer, int line_len) {
  char *untainted = malloc(sizeof(char) * line_len);
  char *str = malloc(sizeof(char) * line_len);
  strcpy(untainted, buffer);
  strcpy(str, buffer);
  char *toks = strtok(str, " ");

  int tok_count = 0;
  while(toks != NULL) {
    tok_count++;
    toks = strtok(NULL, " ");
  }

  int master_safe = 0;

  for (int i = 0; i < tok_count; i++) {
    char *str2 = malloc(sizeof(char) * line_len);
    strcpy(str2, untainted);
    char *new_str = malloc(sizeof(char) * 1);
    strcat(new_str, "\0");
    char *new_toks = strtok(str2, " ");
    int j = 0;
    while(new_toks != NULL) {
      if (i != j) {
        new_str = realloc(new_str, (strlen(new_str) + sizeof(new_toks) + 1) * sizeof(char));
        strcat(new_str, new_toks);
      } 
      j++;
      new_toks = strtok(NULL, " ");
      if (new_toks != NULL && (i+1) != j && i != tok_count) {
        strcat(new_str, " ");
      }
    }

    if (test_line(new_str)) {
      master_safe = 1;
    };
  }

  if (test_line(untainted)) {
    master_safe = 1;
  };

  return master_safe;
}

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
    int safe = test_line_variants(line_buffer, sizeof(line_buffer));
    if (safe) ++safe_count;
  }
 
  printf("Done!\n");
  printf("Safe count: %d\n", safe_count);

  fclose(input);
  return 0;
}