#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv){
   if (argc == 1){
      
   }
   for(int i = 1; argc > i; i++){
      struct stat fileStats;
      char buf[1024];
      readlink(argv[i], buf, 1024);
      int failcheck = lstat(argv[i], &fileStats);
      if(failcheck == -1){
         printf("here %s\n", argv[i]);
         fprintf(stderr, "%s\n", strerror(errno));
         continue;
      }
      time_t timemod = fileStats.st_mtime;
      time_t timenow = time(NULL);
      struct tm *localtimelastmod = localtime(&timemod);
      char timestring[1024];
      if(timemod > (timenow - (180 * 24 * 60 * 60))){
         strftime(timestring, 1024, "%b %e %R", localtimelastmod);
      } else {
         strftime(timestring, 1024, "%b %e  %Y", localtimelastmod);
      }
      printf("%06o %9zd %s %s\n", fileStats.st_mode, 
         fileStats.st_size, timestring, argv[i]);
   }
}
