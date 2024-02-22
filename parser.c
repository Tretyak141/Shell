#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define STRING_LENGTH 100

typedef struct Posl{
    char *args;
    bool need_recur;
} posl_elem;

typedef struct iff{
    char *args;
    bool need_recur;
    int success;
} if_elem;

typedef struct conv{
    char *args;
    bool need_recur;
} conv_elem;

typedef struct Posl_arr{
    posl_elem *arr;
    int len;
} Posl_array;

typedef struct If_arr{
    if_elem *arr;
    int len;
} If_array;

typedef struct Conv_arr{
    conv_elem *arr;
    int len;
} Conv_array;


posl_elem string_to_posl_elem(char *s)
{
    int n = strlen(s);
    int counter_breakes=0;
    posl_elem ret;
    ret.args = calloc(STRING_LENGTH,1);
    for (int i=1;i<n-1;i++)
    {
        if (s[i]=='(') counter_breakes++;
        if (s[i]==')') counter_breakes--;
        if (counter_breakes<0)
        {
            counter_breakes = -1;
            break;
        }
    }
    if ((s[0]=='(')&&(!counter_breakes))
    {
        for (int i=1;i<n-1;i++)
        {
            ret.args[i-1] = s[i];
        }
        ret.need_recur = true;
    }
    else
    {
        strcpy(ret.args,s);
        ret.need_recur = false;
    }
    return ret;
}

if_elem string_to_if_elem(char *s,char *operator)
{
    int n = strlen(s);
    while (1)
    {
        if (s[n-1]==' ')
        {
            n--;
            s[n] = '\0';
        }
        else break;
    }
    int counter_breakes=0;
    if_elem ret;
    ret.args = calloc(STRING_LENGTH,1);
    for (int i=1;i<n-1;i++)
    {
        if (s[i]=='(') counter_breakes++;
        if (s[i]==')') counter_breakes--;
        if (counter_breakes<0)
        {
            counter_breakes = -1;
            break;
        }
    }
    if ((s[0]=='(')&&(!counter_breakes))
    {
        for (int i=1;i<n-1;i++)
        {
            ret.args[i-1] = s[i];
        }
        ret.need_recur = true;
    }
    else
    {
        strcpy(ret.args,s);
        ret.need_recur = false;
    }
    if (!strcmp(operator,"&&")) ret.success = 1;
    else ret.success = 0;
    return ret;
}

conv_elem string_to_conv_elem(char *s)
{
    int n = strlen(s);
    while (1)
    {
        if (s[n-1]==' ')
        {
            n--;
            s[n] = '\0';
        }
        else break;
    }
    int counter_breakes=0;
    conv_elem ret;
    ret.args = calloc(STRING_LENGTH,1);
    for (int i=1;i<n-1;i++)
    {
        if (s[i]=='(') counter_breakes++;
        if (s[i]==')') counter_breakes--;
        if (counter_breakes<0)
        {
            counter_breakes = -1;
            break;
        }
    }
    if ((s[0]=='(')&&(!counter_breakes))
    {
        for (int i=1;i<n-1;i++)
        {
            ret.args[i-1] = s[i];
        }
        ret.need_recur = true;
    }
    else
    {
        strcpy(ret.args,s);
        ret.need_recur = false;
    }
    return ret;
}

Posl_array string_parser(char *s)
{
    int n = strlen(s);
    posl_elem *arr = NULL;
    int counter = 0;
    int string_pointer = 0;
    int opened = 0;
    char *elem = calloc(STRING_LENGTH,1);
    for (int i=0;i<n;i++)
    {
        if (s[i]==' ')
        {
            if (i==0)
            {
                continue;
            }
            if ((s[i-1]==' ')||(s[i-1]==')'))
            {
                continue;
            }
            if (!string_pointer)
            {
                continue;
            }
        }
        if (s[i]=='(')
        {
            opened++;
        }

        if (s[i]==')')
        {
            opened--;
        }

        if ((s[i]==';'))
        {
            if (!opened)
            {
                if (elem[string_pointer-1]==' ') elem[string_pointer-1] = '\0';
                posl_elem el = string_to_posl_elem(elem);
                counter++;
                arr = realloc(arr,sizeof(posl_elem)*counter);
                
                arr[counter-1] = el;
                free(elem);
                elem = calloc(STRING_LENGTH,1);
                string_pointer = 0;
                continue;
            }
        }
        if (s[i]!='\n')
        {
            elem[string_pointer] = s[i];
            string_pointer++;
        }
    }
    if (elem[0])
    {
        counter++;
        arr = realloc(arr,sizeof(posl_elem)*(counter));
        arr[counter-1] = string_to_posl_elem(elem);
    }
    Posl_array array;
    array.arr = arr;
    array.len = counter;
    free(elem);
    return array;
}

If_array iff(char *s)
{
    if_elem *arr = NULL;
    int counter = 0;
    int opened = 0;
    char *word = calloc(STRING_LENGTH,1);
    char *ans = calloc(STRING_LENGTH,1);
    int n = strlen(s);
    int pointer=0;
    char buf[3] = "&&";
    for (int i=0;i<n;i++)
    {
        if (s[i]==' ')
        {
            if (!i) continue;
            if (s[i-1] != ' ')
            {
                bool check1 = strcmp(word,"&&");
                bool check2 = strcmp(word,"||");
                if ((!check1 || !check2)&&(!opened))
                {
                    if_elem el;
                    el = string_to_if_elem(ans,buf);
                    strcpy(buf,word);
                    counter++;
                    arr = realloc(arr,counter*sizeof(if_elem));
                    arr[counter-1] = el;
                    free(word);
                    free(ans);
                    word = calloc(STRING_LENGTH,1);
                    ans = calloc(STRING_LENGTH,1);
                    pointer = 0;
                    continue;
                }
                if (ans[0]) strcat(ans," ");
                strcat(ans,word);
                free(word);
                word = calloc(STRING_LENGTH,1);
                pointer = 0;
                continue;
            }
            else continue;
        }
        if (s[i]==')')
        {
            opened--;
            word[pointer] = s[i];
            if (ans[0]) strcat(ans," ");
            strcat(ans,word);
            free(word);
            word = calloc(STRING_LENGTH,1);
            pointer = 0;
            continue;
        }
        if (s[i]=='(')
        {
                bool check1 = strcmp(word,"&&");
                bool check2 = strcmp(word,"||");
                if ((!check1 || !check2)&&(!opened))
                {
                    if_elem el;
                    el = string_to_if_elem(ans,buf);
                    strcpy(buf,word);
                    counter++;
                    arr = realloc(arr,counter*sizeof(if_elem));
                    arr[counter-1] = el;
                    free(word);
                    free(ans);
                    word = calloc(STRING_LENGTH,1);
                    ans = calloc(STRING_LENGTH,1);
                    pointer = 0;
                    word[pointer] = s[i];
                    pointer++;
                    opened++;
                    continue;
                }
                opened++;
                word[pointer] = s[i];
                pointer++;
                continue;
        }
        if (s[i]!='\n')
        {
            word[pointer] = s[i];
            pointer++;
        }
        //printf("%s\n",word);
    }
    if (ans[0]) strcat(ans," ");
    strcat(ans,word);
    if (ans[0])
    {
        counter++;
        arr = realloc(arr,sizeof(if_elem)*counter);
        arr[counter-1] = string_to_if_elem(ans,buf);
    }
    If_array array;
    array.arr = arr;
    array.len = counter;
    free(ans);
    free(word);
    return array;
}

Conv_array conv_parser(char *s)
{
    conv_elem *arr = NULL;
    int counter = 0;
    int opened = 0;
    char *word = calloc(STRING_LENGTH,1);
    char *ans = calloc(STRING_LENGTH,1);
    int n = strlen(s);
    int pointer=0;
    for (int i=0;i<n;i++)
    {
        if (s[i]==' ')
        {
            if (!i) continue;
            if (s[i-1] != ' ')
            {
                bool check = strcmp(word,"|");
                if ((!check)&&(!opened))
                {
                    conv_elem el = string_to_conv_elem(ans);
                    counter++;
                    arr = realloc(arr,counter*sizeof(conv_elem));
                    arr[counter-1] = el;
                    free(word);
                    free(ans);
                    word = calloc(STRING_LENGTH,1);
                    ans = calloc(STRING_LENGTH,1);
                    pointer = 0;
                    continue;
                }
                if (ans[0]) strcat(ans," ");
                strcat(ans,word);
                free(word);
                word = calloc(STRING_LENGTH,1);
                pointer = 0;
                continue;
            }
            else continue;
        }
        if (s[i]==')')
        {
            opened--;
            word[pointer] = s[i];
            if (ans[0]) strcat(ans," ");
            strcat(ans,word);
            free(word);
            word = calloc(STRING_LENGTH,1);
            pointer = 0;
            continue;
        }
        if (s[i]=='(')
        {
                bool check = strcmp(word,"|");
                if ((!check)&&(!opened))
                {
                    conv_elem el = string_to_conv_elem(ans);
                    counter++;
                    arr = realloc(arr,counter*sizeof(conv_elem));
                    arr[counter-1] = el;
                    free(word);
                    free(ans);
                    word = calloc(STRING_LENGTH,1);
                    ans = calloc(STRING_LENGTH,1);
                    pointer = 0;
                    word[pointer] = s[i];
                    pointer++;
                    opened++;
                    continue;
                }
                opened++;
                word[pointer] = s[i];
                pointer++;
                continue;
        }
        if (s[i]!='\n')
        {
            word[pointer] = s[i];
            pointer++;
        }
        //printf("%s\n",word);
    }
    if (ans[0]) strcat(ans," ");
    strcat(ans,word);
    if (ans[0])
    {
        counter++;
        arr = realloc(arr,sizeof(conv_elem)*counter);
        arr[counter-1] = string_to_conv_elem(ans);
    }
    Conv_array array;
    array.arr = arr;
    array.len = counter;
    for (int i=0;i<counter;i++)
    {
        printf("%s\n",array.arr[i].args);
    }
    free(ans);
    free(word);
    return array;
}
