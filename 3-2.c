#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>

void determine_enable(char *buffer, int *curr_enable) {
  char enable_buffer[sizeof(buffer)];
  int buffer_len = strlen(buffer);

  regex_t enable_reg;
  regex_t mul_reg;
  regmatch_t enables[1];
  regmatch_t muls[1];
  char *enable_cursor = buffer;
  char *mul_cursor = buffer;
  int enable = regcomp(&enable_reg, "(do\\(\\))|(don't\\(\\))", REG_EXTENDED);
  int mul = regcomp(&mul_reg, "mul\\([0-9]{1,3},[0-9]{1,3}\\)", REG_EXTENDED);
  enable = regexec(&enable_reg, enable_cursor, 1, enables, 0);
  mul = regexec(&mul_reg, mul_cursor, 1, muls, 0);

  // no mo'
  if (enable != 0) return;
  if (mul != 0) return;

  if (enables[0].rm_so < muls[0].rm_so) {
    int enable_len = enables[0].rm_eo - enables[0].rm_so;
    *curr_enable = (enable_len == 4) ? 1 : 0;
  }

  regfree(&enable_reg);
  regfree(&mul_reg);

}

int mul(char * the_match) {
  // grab the two numbers in the match
  regex_t num_regex;
  regmatch_t curr_num[1];
  char *m_cursor = the_match;
  int a, b;
  char *num_str[3];

  int num = regcomp(&num_regex, "[0-9]{1,3}", REG_EXTENDED);

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

  // multiply the two numbers
  regfree(&num_regex);
  return a * b;
}

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

  // make the regex
  regex_t regex;
  int result = regcomp(&regex, "mul\\([0-9]{1,3},[0-9]{1,3}\\)", REG_EXTENDED);
  regmatch_t curr_match[1];
  int ret_val = 0;

  // do/don't
  int enable = 1;
  determine_enable(buffer, &enable);

  // loop through matches
  while ((result = regexec(&regex, buffer, 1, curr_match, 0)) == 0) {
    int start = curr_match[0].rm_so;
    int end = curr_match[0].rm_eo;
    int match_len = end - start;
    buffer += start;

    char the_match[match_len];
    strncpy(the_match, buffer, match_len);

    if (enable) {
      ret_val += mul(the_match);
    }
    buffer += match_len;
    determine_enable(buffer, &enable);
  }

  printf("Done!\n");
  printf("Result: %d\n", ret_val);

  regfree(&regex);
  fclose(input);

  return 0;
}