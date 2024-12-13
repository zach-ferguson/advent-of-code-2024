#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define magic_word "XMAS"

int get_line_len(char *buffer) {
	int i = 0, n = 0;
	char c = buffer[i];
	while (
			c != '\0' &&
			c != EOF &&
			c != '\n'
			) {
		i++, n++;
		c = buffer[i];
	}

	return n + 1;
}

int check(int p, int next, char *str, char *match) {
	int n = strlen(match);
	if ((p + (next * (n - 1)) < 0)) return 0;
	for (int i = 0; i < n; i++) {
		if (str[p + (next * i)] != match[i]) return 0;
	}

	return 1;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Not enough arguments. Please provied an input file.");
		return -1;
	}
	if (argv[1] == NULL) {
		printf("Cannot find file.");
		return -1;
	}

	FILE *input = fopen(argv[1], "r");
	if (!input) {
		printf("Failed to open file.");
		return -1;
	}

	long file_size;
	fseek(input, 0, SEEK_END);
	file_size = ftell(input);
	fseek(input, 0, SEEK_SET);

	char *buffer = malloc(file_size * (sizeof(char)));
	size_t bytes = fread(buffer, 1, file_size, input);
	if (bytes != file_size) {
		printf("Failed to read the file.");
		free(buffer);
		return -1;
	}
	fclose(input);
	
	int line_len = get_line_len(buffer);

	int N = (-1) * line_len;
	int S = line_len;
	int W = 1;
	int E = -1;
	int NW = (-1 * line_len) - 1;
	int NE = (-1 * line_len) + 1;
	int SW = line_len - 1;
	int SE = line_len + 1;

	int count = 0;
	for (int i = 0; i < strlen(buffer); i++) {
		count += check(i, N, buffer, magic_word);
		count += check(i, S, buffer, magic_word);
		count += check(i, W, buffer, magic_word);
		count += check(i, E, buffer, magic_word);
		count += check(i, NW, buffer, magic_word);
		count += check(i, NE, buffer, magic_word);
		count += check(i, SW, buffer, magic_word);
		count += check(i, SE, buffer, magic_word);
	}
	
	printf("count: %d\n", count);
	free(buffer);

	return 0;
}
