#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} Inst;

#define PROGRAM_CAP 1000
static Inst program[PROGRAM_CAP];
static size_t program_cur;

int main(void)
{
    const char *file_path = "input.txt";
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
	fprintf(stderr, "ERROR: Could not open %s: %s", file_path, strerror(errno));
	exit(1);
    }

    for (;;) {
	char ch = fgetc(f);
	while (ch != 'm') {
	    if (ch == EOF) goto end;
	    ch = fgetc(f);
	}
    
	if (fgetc(f) != 'u') continue;
	if (fgetc(f) != 'l') continue;
	if (fgetc(f) != '(') continue;

	char num1[10];
	size_t num1_cur = 0;

	int current_ch = fgetc(f);
	while (isdigit(current_ch)) {
	    num1[num1_cur++] = current_ch;
	    current_ch = fgetc(f);
	}
	if (current_ch != ',') continue;

	char num2[10];
	size_t num2_cur = 0;

	current_ch = fgetc(f);
	while (isdigit(current_ch)) {
	    num2[num2_cur++] = current_ch;
	    current_ch = fgetc(f);
	}
	if (current_ch != ')') continue; 

	num1[num1_cur] = '\0';
	num2[num2_cur] = '\0';
    
	int n1 = atoi(num1);
	int n2 = atoi(num2);

	Inst inst = {n1, n2};
	program[program_cur++] = inst;
    }

 end:
    int result = 0;
    for (size_t i = 0; i < program_cur; ++i) {
	Inst inst = program[i];
	result += (inst.a*inst.b);
    }
    printf("Result is: %d\n", result);
    return 0;
}
