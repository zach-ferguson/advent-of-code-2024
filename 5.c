#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PAGE_MAX 100

typedef struct Node {
	int value;
	struct Node *next;
} Node;

Node *new_node(int x) {
	Node *node = malloc(sizeof(Node));
	node->value = x;
	node->next = NULL;
}

Node *insert_node(int x, Node *next) {
	Node *node = new_node(x);
	node->next = next;
	return node;
}

int is_in_list(Node **list_arr, int a, int b) {
	if (list_arr[b] == NULL) return 1;
	Node *p = list_arr[b];
	while(p->next != NULL) {
		if (p->value == a) return 1;
		p = p->next;
	}
	return 0;
}

int ordered_middle_num(char *buffer, Node **list_arr) {
	int nums[100];
	int n = 0;
	char *num = strtok(buffer, ",");
	if (!num) printf("Memory error.\n");
	for (int i = 0; i < 100; i++) {
		if (!num) break;
		nums[i] = atoi(num);
		n++;
		num = strtok(NULL, ",");
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			if (is_in_list(list_arr, nums[j], nums[i])) {
				return 0;
			}
		}
	}

	return nums[n/2];
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Not enough arguments. Please provide an input file.");
		return -1;
	}

	FILE *input = fopen(argv[1], "r");
	if (!input) {
		printf("Failed to open the file.");
		return -1;
	}

	Node *list_arr[PAGE_MAX * 2];
	for (int i = 0; i < sizeof(list_arr) / sizeof(Node); i++) {
		list_arr[i] = NULL;
	}

	char line_buffer[8];
	while(fgets(line_buffer, sizeof(line_buffer), input) != NULL) {
		if (line_buffer[0] == '\n') {
			break;
		}
		int a, b;
		char *num = strtok(line_buffer, "|");
		a = atoi(num);
		num = strtok(NULL, "|");
		b = atoi(num);
		if (list_arr[a] == NULL) list_arr[a] = new_node(b);
		else list_arr[a] = insert_node(b, list_arr[a]);
	}
	
	for (int i = 0; i < 100; i++) {
		if (list_arr[i] == NULL) continue;
		Node *curr = list_arr[i];
		while(curr->next != NULL) {
			curr = curr->next;
		}
	}

	char update_buffer[100];
	if (!update_buffer) printf("Memory failure");

	int count = 0;
	while(fgets(update_buffer, sizeof(update_buffer), input) != NULL) {
		count += ordered_middle_num(update_buffer, list_arr);
	}

	printf("Done!\n");
	printf("Count: %d\n", count);

	return 0;
}
