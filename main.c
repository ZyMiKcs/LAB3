#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 10  // максимальное количество аргументов

int flag = 0;

void onCtrlC(int sig) {
    flag = 1;
    printf("Ctrl+C pressed, stopping the program...\n");
    // получаем список запущенных процессов
    system("ps -e");
    // запрашиваем у пользователя идентификатор процесса
    printf("Enter the PID of the process to stop: ");
    pid_t pid;
    scanf("%d", &pid);
    getchar();
    // останавливаем процесс с помощью команды kill
    kill(pid, SIGTERM);
    printf("Press Enter to continue...");
    // завершаем работу программы
    // exit(0);
}

int main() {
    char input[100];           // буфер для ввода команды
    char *args[MAX_ARGS + 2];  // массив аргументов команды
    int i, status, count;
    pid_t pid;

    signal(SIGINT, onCtrlC);  // обработка сигнала SIGINT

    while (1) {
        printf("> ");                      // приглашение командной строки
        fgets(input, sizeof(input), stdin);  // получаем ввод пользователя
        if (flag == 1) {
            printf("> ");
            fgets(input, sizeof(input), stdin);
            flag = 0;
        }
        count = 0;
        args[count++] = strtok(input, " \n");  // первый аргумент - имя команды
        while ((args[count++] = strtok(NULL, " \n")) != NULL) {
            if (count > MAX_ARGS + 1) {
                printf("Too many arguments!\n");
                count = 0;
                break;
            }
        }
        if (count == 1) {  // отсутствие аргументов
            continue;
        }
        args[count] = NULL;  // последний аргумент - NULL

        if (strcmp(args[0], "ls") == 0) {  // команда "ls"
            if ((pid = fork()) == 0) {
                execvp(args[0], args);
                exit(1);
            }
            wait(&status);
        } else if (strcmp(args[0], "cat") == 0) {  // команда "cat"
            if ((pid = fork()) == 0) {
                execvp(args[0], args);
                exit(1);
            }
            wait(&status);
        } else {  // неизвестная команда
            printf("Unknown command: %s\n", args[0]);
        }
    }

    return 0;
}