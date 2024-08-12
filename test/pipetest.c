#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buf;

    // Pipe oluşturuluyor
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // fork ile çocuk işlem yaratılıyor
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Çocuk işlem
        char *home = getenv("SHLVL");
        printf("%s\n", home);
        _exit(EXIT_SUCCESS);

    } else {  // Ebeveyn işlem
        char *home = getenv("SHLVL");
        printf("%s\n", home);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
