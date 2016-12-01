#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int status;
	printf("system() demo!\n");
	
	status = system("ls -al");
	

	return 0;

}
