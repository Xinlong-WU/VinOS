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

char const * getFileName(char const * path){
    static char buf[DIRSIZ+1];
    char const *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void find(int fd,char const * path, char const * patten){
    char * buf = malloc(sizeof(char)*100);
    struct dirent de;
    struct stat st;
    int subfd;
    int enableFilter = (strcmp(patten, "*") != 0);

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
            if(!enableFilter)
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
            find(subfd,fmtpath(buf), patten);
            close(subfd);
            continue;
        }
        close(subfd);

        if (enableFilter)
        {
            if (strcmp(patten,de.name) == 0){
                printf("%s \n",buf);
            }
            continue;
        }
        printf("%s \n",buf);
    }
    free(buf);
}

int main(int argc, char const *argv[])
{
    int fd;
    char * path = malloc(sizeof(char)*100);
    char * patten = malloc(sizeof(char)*100);
    if (argc < 2)
    {
        path = memmove(path,"./\0",strlen("./\0"));
        patten = memmove(patten,"*\0",strlen("*\0"));
    }
    else if (argc > 2)
    {
        printf("Error: Too many Arguments.\n");
        printf("find <directory> [filename]\n");
        free(path);
        free(patten);
        exit(0);
    }
    else
    {
        path = memmove(path,argv[1],strlen(argv[1]));
        patten = memmove(patten,argv[2],strlen(argv[2]));
    }
    fd = open(path,O_RDONLY);
    find(fd,fmtpath(path),patten);
    free(path);
    free(patten);
    close(fd);
    exit(0);
}
