#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SPACE 32
#define MAX_ID_LEN 10
#define LINE_SIZE ((sizeof(char) * MAX_ID_LEN) * 2) + (sizeof(char) * 3)

int comp(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please provide an input filename and an id length.");
    return -1;
  }

  if (!(argc > 2)) {
    printf("Too many args.");
    return -1;
  }

  int id_len = atoi(argv[2]);
  printf("%d \n", id_len);
  if (!(id_len < 10)) {
    printf("Id size exceed max allowed (10).");
    return -1;
  }

  // open the file
  char *filename = argv[1];
  FILE *input = NULL;
  input = fopen(filename, "r");
  if (!input) {
    printf("Failed to open file.\n");
    return -1;
  }

  //  get the number of lines
  int line_count = 0;
  int c = 0;
  while((c = getc(input)) != EOF) {
    if (c == '\n') {
      ++line_count;
    }
  }
  rewind(input);

  // create the two arrays of nums
  int *group1 = malloc(sizeof(int) * line_count);
  int *group2 = malloc(sizeof(int) * line_count);

  if (!group1 || !group2) {
    printf("Memory error.\n");
    if (group1) free(group1);
    if (group2) free(group2);
  }

  // deal out the two lists
  char buffer[LINE_SIZE];
  int idx = 0;
  while(fgets(buffer, sizeof(buffer), input) != NULL) {
    char id1[id_len];
    char id2[id_len];
    int line_len = strlen(buffer);
    for(int i = 0; i < id_len; i++) {
      id1[i] = buffer[i];
    }
    for(int i = line_len - id_len - 1, j = 0; i < line_len; i++, j++) {
      id2[j] = buffer[i];
    }
    group1[idx] = atoi(id1);
    group2[idx] = atoi(id2);
    idx++;
  }

  // sort the arrays
  qsort(group1, line_count, sizeof(int), comp);
  qsort(group2, line_count, sizeof(int), comp);

  for(int i = 0; i < line_count; i++) {
    printf("%d ", group1[i]);
    printf("%d \n", group2[i]);
  }

  // compare the arrays, calculate the diff
  int total_diff = 0;
  for(int i = 0; i < line_count; i++) {
    if (group1[i] > group2[i]) {
      total_diff += group1[i] - group2[i];
    }
    if (group2[i] > group1[i]) {
      total_diff += group2[i] - group1[i];
    }
  }
   
  // print out the result.
  printf("Done!\n");
  printf("The difference is: %d\n", total_diff);

  return 0;
}
