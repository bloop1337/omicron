// Omicron, a programming language that compiles into cron
// Copyright (C) 2026 bloop
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// SPDX-License-Identifier: GPL-3.0

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define DYNSTR_IMPLEMENTATION
#include "./thirdparty/dynstr.h/dynstr.h"

#define OMICRON_VERSION "0.1.0"

// ENUMS
typedef enum {
	OMICRON_KEYWORD,
	OMICRON_TYPE,
	OMICRON_NUMBER,
	OMICRON_COLON,
	OMICRON_IDENTIFIER,
	OMICRON_L_RBRACKET,
	OMICRON_R_RBRACKET,
	OMICRON_STR,
} Omicron_Token_Type;

// STRUCTERS
typedef struct {
	int col;
	int row;
} Omicron_Code_Location;

typedef struct {
	Dynamic_String content;
	const Omicron_Token_Type type;
	const Omicron_Code_Location loc;
} Omicron_Token;

typedef struct {
	Omicron_Token* tokens;
	size_t length;
	size_t capacity;
} Omicron_Token_Stack;


// FUNCTIONS
static Omicron_Token_Stack* omicron_lexer_process(const char*);
static bool omicron_init_stack(Omicron_Token_Stack*);
static bool omicron_push_back_stack(Omicron_Token_Stack*, Omicron_Token);
static bool omicron_pop_back_stack(Omicron_Token_Stack*);
static void omicron_free_stack(Omicron_Token_Stack*);

// CONSTANTS
static const char* omicron_output_file = "out.cron";
static const char* omicron_keywords[8] = {
	"func", "end",
	"every", "at", "in", "on", "from", "through",
};

static bool omicron_init_stack(Omicron_Token_Stack* stack) {}
static bool omicron_push_back_stack(Omicron_Token_Stack* stack, Omicron_Token token) {
	char* to_str[9] = {
    	"OMICRON_KEYWORD",
		"OMICRON_TYPE",
		"OMICRON_NUMBER",
		"OMICRON_COLON",
		"OMICRON_IDENTIFIER",
		"OMICRON_L_RBRACKET",
		"OMICRON_R_RBRACKET",
		"OMICRON_STR",
	};
	printf(" %s{\""DYNSTR_FORMAT"\"},", to_str[token.type], token.content.length, token.content.data);
}
static bool omicron_pop_back_stack(Omicron_Token_Stack* stack) {}
static void omicron_free_stack(Omicron_Token_Stack*) {}

static int omicron_find_char(const char* str, const char ch	) {
	int result = -1;
	for (size_t i = 0; i < strlen(str); ++i) if (str[i] == ch) return i;
	return result;
}

static Omicron_Token_Stack* omicron_lexer_process(const char* code) {
	
	Omicron_Token_Stack* stack = {0};
	omicron_init_stack(stack);

	int row = 0;
	int col = 0;	
	printf("{ ");
	while (*code != '\0') {
	
		switch(*code) {
			case ' ':
			case '\r':
			case '\t':
				break;
			case '\n':
				row++;
				col = 0;
				break;

			case '(':
				omicron_push_back_stack(stack, (Omicron_Token){
					.content = dynstr("("),
					.type = OMICRON_L_RBRACKET,
					.loc  = (Omicron_Code_Location){.col = col, .row = row },
				});
				break;
			case ')':
				omicron_push_back_stack(stack, (Omicron_Token){
					.content = dynstr(")"),
					.type = OMICRON_R_RBRACKET,
					.loc  = (Omicron_Code_Location){.col = col, .row = row },
				});
				break;
			case ':':
				omicron_push_back_stack(stack, (Omicron_Token){
					.content = dynstr(":"),
					.type = OMICRON_COLON,
					.loc  = (Omicron_Code_Location){.col = col, .row = row },
				});
				break;
	
			default:
				if (isalpha(*code) || *code == '_') {
					Dynamic_String s;
					dynstr_create_len(&s, 10);
					dynstr_append(&s, *(code));

					while (*(code + 1) != '\0') {
						if (isalpha(*(code + 1)) || *(code + 1) == '_') dynstr_append(&s, *(++code));
						else break;	
					}

					omicron_push_back_stack(stack, (Omicron_Token){
						.content = s,
						.type = OMICRON_IDENTIFIER,
						.loc  = (Omicron_Code_Location){.col = col, .row = row },
					});

					break;
				}

				if (isdigit(*code)) {
					Dynamic_String s;
					dynstr_create_len(&s, 5);
					dynstr_append(&s, *(code));

					while (*(code + 1) != '\0') {
						if (isdigit(*(code + 1))) dynstr_append(&s, *(++code));
						else break;	
					}
					omicron_push_back_stack(stack, (Omicron_Token){
						.content = s,
						.type = OMICRON_NUMBER,
						.loc  = (Omicron_Code_Location){.col = col, .row = row },
					});
					break;
				}
				if (*code == '"') {

					int next = omicron_find_char((code+1), '"');
					if (next == -1) {
						printf("Next not found");	
						exit(1);
						break;
					}
					code++; //skip first "
					Dynamic_String s;
					dynstr_create_len(&s, 10);

					for (int i = 0; i < next; ++i) {
						dynstr_append(&s, *code);
						code++;
					}
					
					omicron_push_back_stack(stack, (Omicron_Token){
						.content = s,
						.type = OMICRON_STR,
						.loc  = (Omicron_Code_Location){.col = col, .row = row },
					});

					break;
				}
			break;
		}
		++col;
		++code;
	}
	
	printf(" }\n");
	return stack;
}

int main(int argc, char** argv) {
	omicron_lexer_process("every monday at 18:00 sh(\"goto nahui\")");
	return 0;
}
