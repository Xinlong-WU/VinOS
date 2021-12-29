#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static inline int isChar(char ch){
    return (ch >= '!' && ch <= '~');
}

char * trim(char * str){
    int len = 0;
    char * ps = str;
    while (!isChar(*ps))
        ps++;

    while (ps[len] != '\0'){
        str[len] = ps[len];
        len++;
    }

    while (!isChar(str[len]))
        len--;
    
    str[len+1] = '\0';
    return str;
}

int main(int argc, char *argv[])
{
    char buf[100];
    int argCount = 0;
    char ** tmpArgs = malloc(sizeof(char **) * 50);

    read(0,buf,sizeof(buf));
    trim(buf);

    char *p = buf;
    int split = 1;

    while (*p != '\0'){
        if(isChar(*p)){
            if (split){
                tmpArgs[argCount++] = p;
                split = 0;
            }
        }
        else{
            split = 1;
            *p = '\0';
        }
        p++;
    }

    argCount += argc;
    char ** args = malloc(sizeof(char **) * argCount);
    memmove(args,argv, sizeof(char **) * argc);
    memmove(args+argc,tmpArgs, sizeof(char **) * (argCount - argc));

    exec(args[1],args+1);

    free(args);
    free(tmpArgs);
    exit(0);
}
