#include<stdio.h>
#include<string.h>
void cikti(char *userInput)
{
    char buffer[20];
    strcpy(buffer, userInput);
    printf("%s\n", buffer);
}

int main(int argc, char **argv)
{
	cikti(argv[1]);
	return 0;
}
