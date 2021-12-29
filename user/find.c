#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char * fmtpath(char * path){
    char *p = path+strlen(path);
    if(*(p-1) != '/'){
        *p = '/';
        *(p+1) = '\0';
    }
    return path;
}

void removeFileName(char * path){
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    *(p) = '\0';
    return;
}

void find(int fd,char const * path){
    char * buf = malloc(sizeof(char)*100);
    struct dirent de;
    struct stat st;
    int subfd;

    memmove(buf, path, strlen(path));

    if (fd < 0){
        fprintf(2, "Error: cannot find %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf("Error: cannot stat %s\n", path);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){

        if(de.inum == 0)
            continue;
        if(strcmp(de.name,".") == 0){
            printf("%s \n",path);
            continue;
        }
        if(strcmp(de.name,"..") == 0)
            continue;

        strconcat(buf,path,de.name);
        subfd = open(buf,O_RDONLY);
        if(fstat(subfd, &st) < 0){
            printf("Error: cannot stat %s\n", buf);
            close(subfd);
            continue;
        }
        if (st.type == T_DIR){
            find(subfd,fmtpath(buf));
            close(subfd);
            continue;
        }
        close(subfd);

        printf("%s \n",buf);
    }
    free(buf);
}

int main(int argc, char const *argv[])
{
    int fd;
    char * path = malloc(sizeof(char)*100);
    if (argc < 2)
    {
        path = memmove(path,"./",strlen("./"));
    }
    else
    {
        path = memmove(path,argv[1],strlen(argv[1]));
    }
    fd = open(path,O_RDONLY);
    find(fd,fmtpath(path));
    free(path);
    close(fd);
    exit(0);
}
