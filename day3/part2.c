#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define UNUSED(value) (void) (value)
#define UNIMPLEMENTED() do {					  \
	fprintf(stderr, "ERROR: Unimplemented %s()\n", __func__); \
	exit(1);						  \
    } while (0);

typedef enum {
    INST_NONE = 0,
    INST_MUL,
    INST_DO,
    INST_DONT,
} Inst_Type;

typedef struct {
    Inst_Type type;
    int a;
    int b;
} Inst;

typedef struct {
    const char *content;
    size_t size;
    size_t cursor;
} Lexer;

bool lexer_expect_char(Lexer *l, char ch)
{
    return l->content[l->cursor] == ch;
}

bool lexer_is_empty(Lexer *l)
{
    return (l->cursor >= l->size);
}

void lexer_chop_char(Lexer *l)
{
    if (!lexer_is_empty(l)) {
	l->cursor += 1;
    }
}

Inst lexer_next_inst(Lexer *l)
{
    Inst result = {0};

    for (;;) {
	char *allowed_inst[] = {"mul", "do", "don't"};
#define allowed_inst_sz 3
	char inst_name[10] = {0};
	size_t cursor = 0;

    
	while (l->content[l->cursor] != 'm' && l->content[l->cursor] != 'd') {
	    lexer_chop_char(l);
	    if (lexer_is_empty(l)) return result;
	}

	while (isalpha(l->content[l->cursor]) || l->content[l->cursor] == '\'') {
	    inst_name[cursor++] = l->content[l->cursor];
	    lexer_chop_char(l);
	}

	// Compare found instruction with allowed instructions
	for (size_t i = 0; i < allowed_inst_sz; ++i) {
	    if (strcmp(inst_name, allowed_inst[i]) == 0) {
		result.type = i + 1;
	    }

	    char *substr = strstr(inst_name, allowed_inst[i]);
	    if (substr != NULL) {
		result.type = i + 1;
	    }
	}

	if (result.type == INST_NONE) continue;

	if (result.type == INST_MUL) {
	    if (!lexer_expect_char(l, '(')) continue;
	    lexer_chop_char(l);

	    char num1[10] = {0};
	    size_t num1_cur = 0;

	    while (isdigit(l->content[l->cursor])) {
		num1[num1_cur++] = l->content[l->cursor];
		lexer_chop_char(l);
	    }

	    if (!lexer_expect_char(l, ',')) continue;
	    lexer_chop_char(l);

	    char num2[10] = {0};
	    size_t num2_cur = 0;

	    while (isdigit(l->content[l->cursor])) {
		num2[num2_cur++] = l->content[l->cursor];
		lexer_chop_char(l);
	    }

	    if (!lexer_expect_char(l, ')')) continue;

	    int n1 = atoi(num1);
	    int n2 = atoi(num2);

	    result.a = n1;
	    result.b = n2;
	    break;
	}

	if (!lexer_expect_char(l, '(')) continue;
	lexer_chop_char(l);

	if (!lexer_expect_char(l, ')')) continue;
	lexer_chop_char(l);
	break;
    }

    return result;
}

int main(void)
{
    const char *file_path = "input.txt";
    FILE *f = fopen(file_path, "rb");
    fseek(f, 0, SEEK_END);
    int input_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *buffer = malloc(input_size);
    fread(buffer, 1, input_size, f);
    Lexer l = {
	.content = buffer,
	.size = input_size,
	.cursor = 0,
    };
    
    int result = 0;
    bool flag = true;
    
    for (;;) {
	Inst inst = lexer_next_inst(&l);

	switch (inst.type) {
	case INST_NONE: {
	    goto end;
	} break;
	case INST_MUL: {
	    if (flag) {
		result += (inst.a*inst.b);
	    } else {
		continue;
	    }
	} break;
	case INST_DO: {
	    flag = true;
	} break;
	case INST_DONT: {
	    flag = false;
	} break;
	}
	
    }

 end:
    printf("Result is: %d\n", result);
    
    return 0;
}
