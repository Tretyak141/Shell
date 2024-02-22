#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "operations.h"
#include "parser.h"

int main()
{
    printf("Hello, it's Andrew's command interpretator\n\n");
    printf("I wanna give you some rules\n\n");
    printf("You can use input/output files ONLY in convayers\n");
    printf("You can't do it wth full Shell command (Really, why do you need it?)\n");
    printf("In one Shell command you can use just one ghost mode: in the and of command\n\n");
    printf("Well, when you've read this instruction, please, input 'Go' to start to use my Shell\n");
    while (1)
    {
        char c[50];
        fgets(c,50,stdin);
        if (!strcmp(c,"Go\n")) break;
    }
    while(1)
    {
        char dir[50];
        getcwd(dir,50);
        printf("My_shell. Input your command \nNow dir: %s$ ",dir);
        char s[256];
        fgets(s,256,stdin);
        if (s[strlen(s)-2]=='&')
        {
            s[strlen(s)-2] = '\0';
            pid_t pid;
            if (!(pid=fork()))
            {
                if (!fork())
                {
                    setpgid(0,0);
                    kill(getppid(),SIGINT);
                    signal(SIGINT,SIG_IGN);
                    int ch;
                    int tmp = open("/dev/null",O_RDONLY);
                    dup2(tmp,0);
                    close(tmp);
                    tmp = open("/dev/null",O_WRONLY);
                    dup2(tmp,1);
                    close(tmp);
                    posl(string_parser(s));
                }
                exit(0);
            }
            waitpid(pid,0,0);
            continue;
        }
        Posl_array posled = string_parser(s);
        posl(posled);
    }
}