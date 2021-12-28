#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char * fmtpath(char const * path){
    static char buf[DIRSIZ+1];
    memmove(buf, path, strlen(path));
    char *p = buf+strlen(buf);
    if(*p != '/'){
        *p = '/';
        *(p+1) = '\0';
    }
    return buf;
}

void find(int fd,char const * path){
    char buf[512];
    struct dirent de;
    struct stat st;
    int subfd;

    memmove(buf, path, strlen(path));

    if (fd < 0){
        fprintf(2, "Error: cannot find %s\n", path);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(fstat(fd, &st) < 0){
            printf("Error: cannot stat %s\n", path);
            return;
        }

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
}

int main(int argc, char const *argv[])
{
    int fd;
    if (argc < 2)
    {
        fd = open("./",O_RDONLY);
        find(fd,"./");
    }
    else
    {
        fd = open(argv[1],O_RDONLY);
        find(fd,fmtpath(argv[1]));
    }
    close(fd);
    exit(0);
}
