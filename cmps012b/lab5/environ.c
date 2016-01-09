#include <stdlib.h>
#include <stdio.h>

extern char **environ;
void main(){
    for (int i = 0; environ[i] != NULL; i++){
        printf("%s\n", environ[i]);
    }
}
