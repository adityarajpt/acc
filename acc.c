#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Token Types
typedef enum {
	TK_RESERVED, // Keywords
	TK_NUM,      // Integers
	TK_EOF,      // End of file token
} TokenKind;

typedef struct Token Token;

// Token Defination
struct Token {
	TokenKind kind;  // Type of token
	Token *next; 	 // Pointer to next token
	int val;	 // if Token is TK_NUM then value
	char *str;	 // string value of token
};

// Input program as String
char *user_input;

// Current Token
Token *token;

// error fuction, takes variable number of arguments and exits
void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, "");		// print pos number of spaces
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);

}

// Consumes the current toke if it matches 'op'.
bool consume(char op) {
	if (token->kind != TK_RESERVED || token->str[0] != op) {
		return false;
	}
	token = token->next;
	return true;
}

// Ensure that the current token is 'op'
void expect(char op) {
	if (token->kind != TK_RESERVED || token->str[0] != op) {
		error_at(token->str, "expected %c", op);
	}
	token = token->next;
}

// Ensures that current token is Integer
int expect_number() {
	if (token->kind != TK_NUM) {
		error_at(token->str, "token kind number expected");
	}
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

// Create a new token and add it as the next token of 'cur'
Token *new_token(TokenKind kind, Token *cur, char *str) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	cur->next = tok;
	return tok;
}

// Tokenize user input and returns new token
Token *tokenize() {
	char *p = user_input;
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {

		// skips whitespace characters
		if (isspace(*p)) {
			p++;
			continue;
		}

		// op's 
		if (*p == '+' || *p == '-') {
			cur = new_token(TK_RESERVED, cur, p++);
			continue;
		}

		// Integer Literas
		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p);
			cur->val = strtol(p, &p, 10);
			continue;
		}

		error_at(p, "expected a number");
	}

	new_token(TK_EOF, cur, p);
	return head.next;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "incorrect number of arguments!");
		return 1;
	}

	user_input = argv[1];
	token = tokenize();

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");
	

	printf("	mov rax, %d\n", expect_number());



	while(!at_eof()) {
		if (consume('+')) {
			printf("	add rax, %d\n", expect_number());
			continue;
		}

		expect('-');
		printf("	sub rax, %d\n", expect_number());
	}

	printf("	ret\n");
	return 0;
}
