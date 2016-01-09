// Peter Greer-Berezovsky pbgreerb
//
// NAME
//   l8stat
//
// SYNOPSIS
//   Accepts any amount of file or directory paths. If no files or 
//   directories are given the current directory us used. Information
//   about each given file or directory is printed on a newline. Each
//   file or directory has its mode printed as 6 octal digits,
//   followed by its size in decimal digits, followed by it the last
//   date it was modified, followed by the file or directory's name.
//   If the file contains a symlink the symlink it contains is 
//   printed.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Receives two char arrays, once containing the name of a path that
// will have its information printed and once containing the program
// name. Uses a buffer and readlink to check if the destination of the
// given path contains a symlink. Uses lstat to get stats about the
// given path. If lstat fails an error message is printed and the 
// program moves onto the next given path. The time(), localtime(),
// and tm functions and struct are used to get the date the file or
// directory was last modified and the current time. The information
// is printed in the programs standard format.

void printStats(char *filename, char *progname){
   char buf[1024];
   ssize_t symlinkcheck = readlink(filename, buf, 1024);
   struct stat fileStats;
   int failcheck = lstat(filename, &fileStats);
   if(failcheck == -1){
      fprintf(stderr, "%s: %s: %s\n", progname, filename,
         strerror(errno));
      return;
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
    printf("%06o %9zd %s %s", fileStats.st_mode,
      fileStats.st_size, timestring, filename);
   if(symlinkcheck != -1){
      printf(" -> ");
      for(ssize_t i = 0; i < symlinkcheck; i++){
         printf("%c", buf[i]);
      }
   } 
   printf("\n");
}

// Calls the printStats function for all the given files/directories
// or calls printStats cone for the current directory.

int main(int argc, char **argv){
   if (argc == 1){
      char *currDir = ".";
      printStats(currDir, argv[0]);
   }
   for(int i = 1; argc > i; i++){
      printStats(argv[i], argv[0]);
   }
}
