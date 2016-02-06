#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main ()
{
	int r, i;
	FILE *f;
	srand (time(NULL));

	f = fopen("test5000c.txt", "w");

	fprintf (f, "%d\n", 5000);
	for (i = 0; i < 5000; i++)
	{
		r = rand();
		fprintf (f, "%d\n", r);
	}
}
