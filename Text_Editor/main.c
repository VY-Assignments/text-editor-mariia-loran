#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

struct DynamicBuffer {
	char* data;
	int64_t length;
};


void Append(struct DynamicBuffer* buffer) {
	char temp_input[100];
	printf("Enter text to append: ");
	if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
		return;
	}

	int64_t countlen = strlen(temp_input);
	if (countlen > 0 && temp_input[countlen - 1] == '\n') {
		temp_input[countlen - 1] = '\0';
		countlen--;
	}
	if (countlen == 0) {
		return;
	}

	int64_t addSpace = (buffer->length > 0) ? 1 : 0;
	int64_t new_length = buffer->length + countlen + addSpace;

	char* arr = realloc(buffer->data, (size_t)(new_length + 1) * sizeof(char));
	if (arr == NULL) {
		return;
	}

	buffer->data = arr;
	if (buffer->length == 0) {
		arr[0] = '\0';
	}
	else
	{
		strcat_s(buffer->data, (size_t)new_length + 1, " ");
	}
	strcat_s(buffer->data, (size_t)new_length + 1, temp_input);
	buffer->length = new_length;
}

void StartTheNewLine(struct DynamicBuffer* buffer) {
	int64_t new_length = buffer->length + 1;
	char* arr = realloc(buffer->data, (size_t)(new_length + 1) * sizeof(char));
	if (arr == NULL) {
		return;
	}
	buffer->data = arr;


	buffer->data[buffer->length] = '\n';
	buffer->data[new_length] = '\0';
	buffer->length = new_length;
}

void LoadSaveFile(struct DynamicBuffer* buffer) {

	//if (file == NULL) {
	//	return;
	//}
}

void PrintText(struct DynamicBuffer* buffer) {
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	printf("%s\n", buffer->data);
}

void InsertTextByLine(struct DynamicBuffer* buffer) {
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	
}

void Search(struct DynamicBuffer* buffer) {
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
}

void ClearConsole() {
	system("cls");
}

int main() {
	FILE* file  = NULL;
	char mystring[100];

	fopen_s(&file, "MyFile.txt", "r");
	if (file == NULL) {
		printf("Error opening file \n");
	}
	else
	{
		if (fgets(mystring, 100, file) != NULL) {
			printf("%s", mystring);
		}
		fclose(file);
	}

	struct DynamicBuffer my_buffer = { NULL, 0 };
	char command;
	while (1) {
		printf("--------MENU--------\n");
		printf("1.Append text symbols to the end\n2.Start the new line\n3.Use files to load/save the information\n4.Print the current text to console\n");
		printf("5.Insert the text by line and symbol index\n6.Search\n7.Clearing the console\nq/Q - Exit\n");
		printf("Choose the command: ");

		scanf_s(" %c", &command,1);
		

		int c;
		while ((c = getchar()) != '\n' && c != EOF);



		if (command == '1') {
			printf("You entered 1 - Append text symbols to the end  \n");
			Append(&my_buffer);
			//printf("%s\n", my_buffer.data);
		}
		else if (command == '2') {
			printf("You entered 2 - Start the new line \n");
			StartTheNewLine(&my_buffer);
		}
		else if (command == '3') {
			printf("You entered 3 - Use files to load/save the information\n");
			LoadSaveFile(&my_buffer);
		}
		else if (command == '4') {
			printf("You entered 4 -  Print the current text to console\n");
			PrintText(&my_buffer);
		}
		else if (command == '5') {
			printf("You entered 5 -  Insert the text by line and symbol index\n");
			InsertTextByLine(&my_buffer);
		}
		else if (command == '6') {
			printf("You entered 6 -  Search (please note that the text can be found more than once)\n");
			Search(&my_buffer);
		}
		else if (command == '7') {
			//printf("You entered 7 -  Clearing the console\n");
			ClearConsole();
		}
		else if (command == 'q' || command == 'Q') {
			printf("Exist");
			break;
		}
		else
		{
			printf("You did not enter right number\n");
		}
	}

	if (my_buffer.data != NULL) {
		free(my_buffer.data);
	}
	return 0;
}