#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

static char buffer[100];

bool validate(std::vector<int> record)
{
    bool inc = record[0] < record[1];

    for (int i = 1; i < record.size(); ++i) {
	int a = record[i - 1];
	int b = record[i];
	int d = abs(a - b);

	// YES IM CRAZY IDC
	if (!(((a < b) && inc) || (!(a < b) && !inc))) {
	    return false;
	}

	if (d < 1 || d > 3) {
	    return false;
	}
    }

    return true;
}

int main()
{
    const char *file_path = "input.txt";
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
	fprintf(stderr, "ERROR: Could not open file: %s: %s", file_path, strerror(errno));
	exit(1);
    }

    int result = 0;
    char *line = (char*) malloc(sizeof(char) * 100);
    size_t len;

    while (getline(&line, &len, f) > 0) {
	std::vector<int> list;
	char *tmp = line;
	
	char *endptr = NULL;
	while (true) {
	    int num = strtol(tmp, &endptr, 10);
	    // WARNING: End of record, I assume there are no zeros in input XD
	    if (num == 0) break;
	    list.push_back(num);

	    tmp = endptr;
	}

	if (validate(list)) {
	    result += 1;
	}
    }

    printf("Result is: %d\n", result);

    return 0;
}
