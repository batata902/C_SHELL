#ifndef ARGPARSER_H
#define ARGPARSER_H

extern const unsigned int MAX_COMMAND_LINE_SIZE;
extern const unsigned short int MAX_ARGS;

typedef struct command_line {
    int argc;
    char *argv[];
} CMD_LINE;


CMD_LINE* parse_args(char str[]);

unsigned int get_max_cmd_lenght();

void print_line(CMD_LINE *line);

void free_cmd_line(CMD_LINE *line);


#endif