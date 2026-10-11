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
#include <string.h>

#define OMICRON_VERSION "0.1.0"

typedef enum {
	OMICRON_KEYWORD,
	OMICRON_TYPE,
	OMICRON_NUMBER,
	OMICRON_TIME,
	OMICRON_IDENTIFIER,
	OMICRON_L_BRACKET,
	OMICRON_R_BRACKET,
	OMICRON_STR_BEGIN,
	OMICRON_STR_END,
} Omicron_Token_Type;

typedef struct {
	int row;
	int col;
} Omicron_Code_Location;

typedef struct {
	const char* content;
	const Omicron_Token_Type type;
	const Omicron_Code_Location loc;
} Omicron_Token;

typedef struct {
	const char* id;
	const char* desc;
	void (*exec)(int, char**);
} Omicron_Flag;

static void omicron_print_help(int, char**);
static void omicron_print_version(int, char**);
static void omicron_set_output_file(int, char**);
static void omicron_run(int, char**);
static int omicron_max_flag_len(void);
static Omicron_Token* omicron_lexer_process();

static *char omicron_output_file = "out.cron";

static const Omicron_Flag FLAG_HELP =    { .id = "help", .desc = "Display this information", .exec = omicron_print_help };
static const Omicron_Flag FLAG_VERSION = { .id = "version", .desc = "Display compiler version information", .exec = omicron_print_version };
static const Omicron_Flag FLAG_OUT =     { .id = "o", .desc = "Place the output into <file>.", .exec = NULL };

static const Omicron_Flag* CMD_FLAGS[] = {
	&FLAG_HELP,
	&FLAG_VERSION,
	&FLAG_OUT,
	NULL,
};

static void omicron_print_version(int argc, char** argv) {
	printf("Omicrion %s a programming language that compiles into cron\n", OMICRON_VERSION);
	printf("Copyright (C) 2026 bloop");
	exit(0);
}

static void omicron_print_help(int argc, char** argv) {
	printf("Usage: omc [options] file...\n\n");
	
	int max_len = omicron_max_flag_len();
	int padding = 15;

	int i = 0;
	while (1) {
		const Omicron_Flag *flag = CMD_FLAGS[i];
		if (flag == NULL) break;
		
		int flag_len = strlen(flag->id);
		int desc_len = strlen(flag->desc);

		if (flag_len > 1) {
			printf("  --%s", flag->id);
			printf("%*s\n",  ((max_len - flag_len) + desc_len + padding), flag->desc);
		} else {
			printf("  -%s",  flag->id);
			printf("%*s\n",  ((max_len - flag_len) + desc_len + padding + 1), flag->desc);
		}
		++i;
	}
	exit(0);
}

static int omicron_max_flag_len() {
	int max = 0;

	int i = 0;
	while(1) {	
		if (CMD_FLAGS[i] == NULL) break;
		if (strlen(CMD_FLAGS[i]->id) > max) 
			max = strlen(CMD_FLAGS[i]->id);
		++i;
	}

	return max;
}

static void omicron_set_output_file(int argc, char** argv) {
	int output_file_index = 0;

	for (int i = 0; i < argc; ++i) {
		if (argv[i]) == "-o" output_file_index = i + 1;
	}
	
	if (output_file_index >= argc) {
		fprintf(stderr, "syntax error, use: -o <file>");
		exit(1);
	}
	omicron_output_file = argv[output_file_index]; 
}

static void omicron_run(int argc, char** argv) {
	for (int i = 0; i < argc; ++i) {
	}
}

int main(int argc, char** argv) {
	
	omicron_run(argc, argv);	

	return 0;
}
