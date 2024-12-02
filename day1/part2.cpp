// std=C++20
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>

#define COL_LEN 1000
// NOTE: Better to use linked list, because we need to find elements in second_col
static int first_col[COL_LEN];
static int second_col[COL_LEN];

void read_columns_from_file(int *first, int *second, const char *file_path)
{
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
	fprintf(stderr, "ERROR: Could not open file: %s: %s", file_path, strerror(errno));
	exit(1);
    }

    int cursor = 0;
    for (size_t i = 1; i <= 1000; ++i) {
	char *line;
	size_t len; 
	getline(&line, &len, f);

	char *endptr;
	int num1 = strtol(line, &endptr, 10);
	int num2 = strtol(endptr, NULL, 10);

	first_col[cursor] = num1;
	second_col[cursor++] = num2;
    }
}

int main()
{
    const char *file_path = "input.txt";
    read_columns_from_file(first_col, second_col, file_path);

    std::unordered_map<int, int> map;
    int result = 0;
    
    for (size_t i = 0; i < COL_LEN; ++i) {
	int first = first_col[i];
	for (size_t j = 0; j < COL_LEN; ++j) {
	    int second = second_col[j];
	    if (first == second) {
		if (map.contains(first)) {
		    map[first] += 1;
		} else {
		    map[first] = 1;
		}
	    }
	}
    }

    for (const auto &[k, v] : map) {
	result += k * v;
    }

    printf("Result is: %d", result);
    
    return 0;
}
