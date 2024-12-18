#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PAGE_MAX 100
#define UPDATE_MAX 100

typedef struct Node {
	int value;
	struct Node *next;
} Node;

Node *list_arr[PAGE_MAX * 2];

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

int is_in_list(int a, int b) {
	if (list_arr[b] == NULL) return 0;
	Node *p = list_arr[b];
	while(p->next != NULL) {
		if (p->value == a) return 1;
		p = p->next;
	}
	return 0;
}

int comp(const void *a, const void *b) {
	int a_in_b = is_in_list(*(int *)a, (*(int *)b));
	int b_in_a = is_in_list(*(int *)b, (*(int *)a));
	return a_in_b ? 1 : b_in_a ? -1 : 0;
}

void sort_pages(int *nums, int n) {
	qsort(nums, n, sizeof(int), comp);
}

int get_middle_num(char *buffer) {
	int nums[UPDATE_MAX + 1];
	int n = 0;
	char *num = strtok(buffer, ",");
	if (!num) printf("Memory error.\n");

	for (int i = 0; i < UPDATE_MAX + 1; i++) {
		if (!num) break;
		nums[i] = atoi(num);
		n++;
		num = strtok(NULL, ",");
	}

	int save = 0;
	start:
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			if (is_in_list(nums[j], nums[i])) {
				save = 1;
				int t = nums[i];
				nums[i] = nums[j];
				nums[j] = t;
				sort_pages(nums, n);
				goto start;
			}
		}
	}

	return save ? nums[n/2] : 0;
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
	
	for (int i = 0; i < 101; i++) {
		if (list_arr[i] == NULL) continue;
		Node *curr = list_arr[i];
		while(curr->next != NULL) {
			curr = curr->next;
		}
	}

	char update_buffer[UPDATE_MAX];
	if (!update_buffer) printf("Memory failure");

	int count = 0;
	while(fgets(update_buffer, sizeof(update_buffer), input) != NULL) {
		count += get_middle_num(update_buffer);
	}

	printf("Done!\n");
	printf("Count: %d\n", count);

	return 0;
}

// missing 20?
// got the answer 3062, but now its not working.
// no idea what I did to break things here. 
// crazy.
