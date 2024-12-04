#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define SPACE 32
#define MAX_ID_LEN 6
#define LINE_SIZE ((sizeof(char) * MAX_ID_LEN) * 2) + (sizeof(char) * 3)

int comp(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

int hash_func(int k, int n) {
  return k % n;
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
  if (!(id_len < MAX_ID_LEN)) {
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

  if (line_count > 10000) {
    printf("That's too many lines.");
    return -1;
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

  struct id_count {
    int id;
    int count;
  };

  struct id_count **id_count_table[line_count];

  for (int i = 0; i < line_count; i++) {
    int idx = hash_func(group2[i], line_count);
    if (!id_count_table[idx]) {
      struct id_count item = { .id = group2[i], .count = 1 };
      id_count_table[idx] = &item;
      continue;
    } else {
      id_count_table[idx]->count += 1;
    }
  }

  for (int i = 0; i < line_count; i++) {
    printf("%d \n", id_count_table[i]->id);
    // printf("%d, %d\n", id_count_table[i]->id, id_count_table[i]->count);
  }

  // int similarity_score = 0;
  // for (int i = 0; i < line_count; i++) {
  //   similarity_score += group1[i] * id_count_arr[group1[i] + 1].count;
  // }

  // printf("Done!\n");
  // printf("Similarity Score: %d\n", similarity_score);

  free(group1);
  free(group2);


  // answer 27732508
  return 0;
}




// was working on making a hashmap for this to make it more space efficient.