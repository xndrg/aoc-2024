#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define COL_LEN 1000
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

int cmp(const void *first, const void *second) 
{
    int f = *((int*)first);
    int s = *((int*)second);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int main(void)
{
    const char *file_path = "input.txt";
    read_columns_from_file(first_col, second_col, file_path);

    qsort(first_col, COL_LEN, sizeof(int), cmp);
    qsort(second_col, COL_LEN, sizeof(int), cmp);

    int result = 0;
    for (size_t i = 0; i < 1000; ++i) {
	int d = abs(first_col[i] - second_col[i]);
	result += d;
    }

    printf("Result is: %d\n", result);
    
    return 0;
}
