#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char * ErrorStr = 0;
  if(argc < 2){
    ErrorStr = "Error: Too few Argument dosen't match\n";
  }
  else if (argc > 2)
  {
    ErrorStr = "Error: Too many Argument dosen't match\n";
  }
  else if(argv[1][0] < '0' || argv[1][0] > '9'){
    ErrorStr = "Error: Invalid Argument.\n";
  }
  
  if(ErrorStr){
    write(2, ErrorStr, strlen(ErrorStr));
    exit(0);
  }
  int tick = atoi(argv[1]);
  sleep(tick);
  exit(0);
}