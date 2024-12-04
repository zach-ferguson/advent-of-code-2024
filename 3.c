#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (!argv[1]) {
    printf("Please provide an input file.");
  }
  FILE *input = fopen(argv[1], "r");
  if (!input) {
    printf("Failed to open file.");
  }


  // load the file into a string
  fseek(input, 0, SEEK_END);
  int length = ftell(input);
  fseek(input, 0, SEEK_SET);
  char *buffer = malloc(length + 1);
  fread(buffer, 1, length, input);

  printf("length: %d\n", length);

  // make the regex
  regex_t regex;
  int result = regcomp(&regex, "mul\\([0-9]{1,3},[0-9]{1,3}\\)", REG_EXTENDED);

  regmatch_t curr_match[1];
  int ret_val = 0;

  // loop through matches
  while ((result = regexec(&regex, buffer, 1, curr_match, 0)) == 0) {
    int start = curr_match[0].rm_so;
    int end = curr_match[0].rm_eo;
    int match_len = end - start;

    buffer += start;

    char the_match[match_len];
    strncpy(the_match, buffer, match_len);

    // grab the two numbers in the match
    regex_t num_regex;
    regmatch_t curr_num[1];
    char *m_cursor = the_match;
    int a, b;
    char *num_str[3];

    int num = regcomp(&num_regex, "[0-9]{1,3}", REG_EXTENDED);

    // this seems like an affront to God.
    num = regexec(&num_regex, m_cursor, 1, curr_num, 0);
    int a_len = curr_num[0].rm_eo - curr_num[0].rm_so;
    char ca[a_len];
    m_cursor += curr_num[0].rm_so;
    strncpy(ca, m_cursor, a_len);
    a = atoi(ca);

    m_cursor += a_len;

    num = regexec(&num_regex, m_cursor, 1, curr_num, 0);
    int b_len = curr_num[0].rm_eo - curr_num[0].rm_so;
    char cb[b_len];
    m_cursor += curr_num[0].rm_so;
    strncpy(cb, m_cursor, b_len);
    b = atoi(cb);

    regfree(&num_regex);
    ret_val += (a * b);
    buffer += match_len;
  }

  printf("Done!\n");
  printf("Result: %d\n", ret_val);

  regfree(&regex);
  fclose(input);

  return 0;
}