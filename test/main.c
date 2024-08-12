#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024 // Maximum line length
#define MAX_TOKENS 100

typedef struct {
    char **tokens;
    int token_count;
} LexerResult;

typedef struct {
    char **args;
    int arg_count;
} ParseResult;

LexerResult lexer(char *input) {
    LexerResult result;
    result.tokens = malloc(MAX_TOKENS * sizeof(char *));
    result.token_count = 0;

    char *token = strtok(input, " ");
    while (token != NULL) {
        result.tokens[result.token_count++] = token;
        token = strtok(NULL, " ");
    }

    return result;
}

void free_lexer_result(LexerResult result) {
    free(result.tokens);
}

ParseResult parser(LexerResult lexer_result) {
    ParseResult result;
    result.args = lexer_result.tokens;
    result.arg_count = lexer_result.token_count;
    return result;
}

void execute_command(ParseResult parse_result) {
    int pipefds[2], input_fd = 0, output_fd = 1;
    int i = 0;
    pid_t pid;

    while (i < parse_result.arg_count) {
        char *args[MAX_TOKENS];
        int arg_count = 0;
        int redirect_input = 0, redirect_output = 0;
        char *input_file = NULL, *output_file = NULL;

        // Tokenları pipe'lara göre böl
        while (i < parse_result.arg_count && strcmp(parse_result.args[i], "|") != 0) {
            if (strcmp(parse_result.args[i], "<") == 0) {
                redirect_input = 1;
                input_file = parse_result.args[++i];
            } else if (strcmp(parse_result.args[i], ">") == 0) {
                redirect_output = 1;
                output_file = parse_result.args[++i];
            } else {
                args[arg_count++] = parse_result.args[i];
            }
            i++;
        }
        args[arg_count] = NULL;
        i++;

        // Pipe oluştur
        if (i < parse_result.arg_count) {
            if (pipe(pipefds) == -1) {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
            output_fd = pipefds[1];
        } else {
            output_fd = 1;
        }

        // Çocuk süreç oluştur ve komutu çalıştır
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (input_fd != 0) {
                dup2(input_fd, 0);
                close(input_fd);
            }
            if (output_fd != 1) {
                dup2(output_fd, 1);
                close(output_fd);
            }
            if (redirect_input) {
                int fd = open(input_file, O_RDONLY);
                if (fd == -1) {
                    perror("open input file failed");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, 0);
                close(fd);
            }
            if (redirect_output) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("open output file failed");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, 1);
                close(fd);
            }
            execvp(args[0], args);
            perror("exec failed");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            if (input_fd != 0) close(input_fd);
            if (output_fd != 1) close(output_fd);
            input_fd = pipefds[0];
        }
    }
}

int main() {
    char cmd[MAX_LINE]; // Command input

    while (1) {
        printf("minishell> ");
        if (fgets(cmd, MAX_LINE, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }

        // Remove newline character
        cmd[strcspn(cmd, "\n")] = '\0';

        // Exit on "exit" command
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        // Lexical analysis
        LexerResult lexer_result = lexer(cmd);

        // Parsing
        ParseResult parse_result = parser(lexer_result);

        // Execute the command
        execute_command(parse_result);

        // Free lexer result
        free_lexer_result(lexer_result);
    }

    return 0;
}
