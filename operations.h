#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_
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

pid_t simp_command(char *,int,int);
int convayer(Conv_array); 
pid_t if_commands(If_array);
pid_t posl(Posl_array);


#endif