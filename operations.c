#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>
#include <fcntl.h>
#include "parser.h"
#include <signal.h>
#include <dirent.h>
#define STRING_LEN 1000
#define ARGS 100

void chdirect(char *dir)
{
    chdir(dir);
}

pid_t simp_command(char *args,int input,int output)
{
    pid_t pid;
    if (args[0]=='c' && args[1]=='d')
    {
        if (args[2]=='\n' || args[2]=='\0') 
        {
            chdir("/");
        }
        char *dir = args+3;
        chdirect(dir);
        return 1;
    }
    if (!(pid=fork()))
    {
        signal(SIGINT,SIG_DFL);
        char **argv;
        argv = calloc(ARGS,sizeof(char*));
        for (int i=0;i<ARGS;i++)
        {
            argv[i] = calloc(STRING_LEN,sizeof(char));
        }
        int n = strlen(args);
        int pointer = 0;
        int ppointer = 0;
        for (int i=0;i<n;i++)
        {
            if (args[i]==' ')
            {
                if (args[i-1]==' ') continue;
                pointer++;
                ppointer = 0;
                continue;
            }
            if (args[i]=='\n') break;
            argv[pointer][ppointer] = args[i];
            ppointer++;
        }
        pointer++;
        for (int i=pointer;i<ARGS;i++) free(argv[i]);
        argv[pointer] = NULL;
        argv = realloc(argv,sizeof(char*)*(pointer+1));
        argv[pointer] = NULL;
        if (input)
        {
            dup2(input,0);
            close(input);
        }
        if (output!=1)
        {
            dup2(output,1);
            close(output);
        }
        execvp(argv[0],argv);
        for (int i=0;i<pointer;i++) free(argv[i]);
        free(argv);
        abort();
    }
    return pid;
}

int convayer(Conv_array arr)
{   
    int n = arr.len;
    int input = 0;
    int output = 1;
    int st=1;
    if (st)
    {
        char *i1 = calloc(50,1),*i2 = calloc(50,1);
        int i=0;
        while ((arr.arr[0].args[i]!=' ')&&(arr.arr[0].args[i]!='\0')&&(arr.arr[0].args[i]!='\n'))
        {
            i1[i] = arr.arr[0].args[i];
            i++;
        }
        while (arr.arr[0].args[i]==' ')
            i++;
        int del1 = i;
        int j=0;
        while ((arr.arr[0].args[i]!=' ')&&(arr.arr[0].args[i]!='\0')&&(arr.arr[0].args[i]!='\n'))
        {
            i2[j] = arr.arr[0].args[i];
            j++;
            i++;
        }
        if (i1[0]=='<')
        {
            for (int z=0;z<strlen(i1);z++) i1[z] = i1[z+1];
            input = open(i1,O_RDONLY);
            for (int z=0;z<strlen(arr.arr[0].args);z++) arr.arr[0].args[z] = arr.arr[0].args[z+del1];
            if (i2[0]=='>')
            {
                int fl = 0;
                int n = strlen(i2);
                for (int z=0;z<strlen(i2);z++) i2[z] = i2[z+1];
                if (i2[0]=='>')
                    {
                        n = strlen(i2);
                        for (int z=0;z<n;z++) 
                        {
                            i2[z] = i2[z+1];
                        }
                        fl = 1;
                    }
                output = open(i2,O_CREAT|O_WRONLY|(O_TRUNC*!fl)|(O_APPEND*fl),0666);
                for (int i=0;i<strlen(arr.arr[0].args);i++)
                {
                    arr.arr[0].args[i] = arr.arr[0].args[i+strlen(i2)+2];
                }
            }
        }
        else if (i1[0]=='>')
        {
            int fl = 0;
            int n = strlen(i1);
            for (int z=0;z<strlen(i1);z++) i1[z] = i1[z+1];
            if (i1[0]=='>')
                {
                    n = strlen(i1);
                    for (int z=0;z<n;z++) 
                    {
                        i1[z] = i1[z+1];
                    }
                    fl = 1;
                }
            output = open(i1,O_CREAT|O_WRONLY|O_TRUNC,0666);
            for (int z=0;z<strlen(arr.arr[0].args);z++) arr.arr[0].args[z] = arr.arr[0].args[z+del1];
            if (i2[0]=='<')
            {
                for (int z=0;z<strlen(i2);z++) i2[z] = i2[z+1];
                printf("%s\n",i2);
                input = open(i2,O_RDONLY);
                for (int i=0;i<strlen(arr.arr[0].args);i++)
                {
                    arr.arr[0].args[i] = arr.arr[0].args[i+strlen(i2)+2];
                }
                printf("%s\n",arr.arr[0].args);
            }
        }
    }

    
    if (arr.len == 1) 
    {
        signal(SIGINT,SIG_IGN);
        pid_t ret = simp_command(arr.arr[0].args,input,output);
        int st;
        waitpid(ret,&st,0);
        signal(SIGINT,SIG_DFL);
        return !(!WIFEXITED(st) || WEXITSTATUS(st));

    }

    int fd[n-1][2];
    pipe(fd[0]);
    int pids[n];
    pids[0]  = simp_command(arr.arr[0].args,input,fd[0][1]);
    close(fd[0][1]);
    for (int i=1;(i<(n-1));i++)
    {
        pipe(fd[i]);
        pids[i] = simp_command(arr.arr[i].args,fd[i-1][0],fd[i][1]);
        close(fd[i][1]);
        close(fd[i-1][0]);
    }
    pids[n-1] = simp_command(arr.arr[n-1].args,fd[n-2][0],output);
    close(fd[n-2][0]);
    for (int i=0;i<n-1;i++)
    {
        if (arr.arr[0].args[0]=='c' && arr.arr[0].args[1]=='d') continue;
        waitpid(pids[i],0,0);
    }
    int st1;
    if (arr.arr[n-1].args[0]=='c' && arr.arr[n-1].args[1]=='d') goto k;
    waitpid(pids[n-1],&st1,0);
    return (!WIFEXITED(st) || WEXITSTATUS(st));
k:
    return 0;
}

pid_t if_commands(If_array arr)
{
    int n = arr.len;
    int exit_st = 1;
    int indicator = 0;
    if (n==1)
    {
        if (arr.arr[0].need_recur) return posl(string_parser(arr.arr[0].args));
        return convayer(conv_parser(arr.arr[0].args));
    }
    for (int i=0;(i<n)&&(arr.arr[i].success==exit_st);i++)
    {
        if (arr.arr[i].need_recur)
        {
            int fd[2];
            pipe(fd);
            char s[50];
            getcwd(s,50);
            if (!fork())
            {
                close(fd[0]);
                exit_st = posl(string_parser(arr.arr[i].args));
                write(fd[1],&exit_st,4);
                exit(0);
            }
            chdir(s);
            close(fd[1]);
            read(fd[0],&exit_st,4);
            close(fd[0]);
            wait(0);
        }
        else exit_st = convayer(conv_parser(arr.arr[i].args));
        indicator = i+1;
    }
    if (indicator!=n) return 0;
    return 1;
}

pid_t posl(Posl_array arr)
{
    int n = arr.len;
    for (int i=0;i<n;i++)
    {
        int st;

        if (arr.arr[i].need_recur)
        {

            char s[50];
            getcwd(s,50);
            int fd[2];
            pipe(fd);
            if (!fork())
            {

                close(fd[0]);
                st = posl(string_parser(arr.arr[i].args));
                write(fd[1],&st,4);
                exit(0);
            }
            chdirect(s);
            close(fd[1]);
            read(fd[0],&st,4);
            close(fd[0]);
            wait(0);
        }
        else
        {

            st = if_commands(iff(arr.arr[i].args));
        }
        if (i==n-1) return st;
        printf("SLEEP between posled commands\n");
        sleep(1);
    }
}