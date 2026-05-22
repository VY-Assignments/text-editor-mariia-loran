#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

int main() {
	FILE* file;
	char mystring[100];

	file = fopen("MyFile.txt", "r");
	if (file == NULL) {
		printf("Error opening file");
	}
	else
	{
		if (fgets(mystring, 100, file) != NULL) {
			printf("%s", mystring);
		}
		fclose(file);
	}
	return;
	0;


	char command;
	printf("Choose the command: ");
	scanf_s(" %c", &command);

	if (command == '1') {
		printf("You entered 1 - Append text symbols to the end  ");
	}
	else if (command == '2') {
		printf("You entered 2 - Start the new line  ");
	}
	else if (command == '3') {
		printf("You entered 3 - Use files to load/save the information");
	}
	else if (command == '4') {
		printf("You entered 4 -  Print the current text to console ");
	}
	else if (command == '5') {
		printf("You entered 5 -  PInsert the text by line and symbol index ");
	}
	else if (command == '6') {
		printf("You entered 6 -  Search (please note that the text can be found more than once)");
	}
	else if (command == '7') {
		printf("You entered 7 -  Clearing the console");
	}
	else
	{
		printf("You did not enter right number\n");
	}
	return 0;

}