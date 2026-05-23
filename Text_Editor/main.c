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
	scanf_s("%99s",temp_input, (unsigned int)sizeof(temp_input));

	
	int64_t countlen = strlen(temp_input);
	int64_t new_length = buffer->length + countlen;

	char* arr = realloc(buffer->data, (size_t)(new_length+1) * sizeof(char));
	if (arr == NULL) {
		return;
	}
	if (buffer->length == 0) {
		arr[0] = '\0';
	}
	

	buffer->data = arr;
	strcat_s(buffer->data,(size_t)new_length+1, temp_input);
	buffer->length = new_length;

	//strcpy(buffer->data+ buffer-> length,countlen+1, temp_input);

}






int main() {
	FILE* file  = NULL;
	char mystring[100];

	fopen_s(&file, "MyFile.txt", "r");
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


	struct DynamicBuffer my_buffer = { NULL, 0 };
	char command;

	while (1) {
		printf("Choose the command: ");
		scanf_s(" %c", &command,1);


		if (command == '1') {
			printf("You entered 1 - Append text symbols to the end  ");
			Append(&my_buffer);
			printf("%s\n", my_buffer.data);
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
//struct Node {
//	uint64_t value;
//	struct Node* next;
//};
//
//struct linked_List
//{
//	struct Node* head;
//};
//
//struct linked_List* create() {}
//{
//	struct linked_List* list = (struct(linked_List*)malloc(sizeof(struct(linked_List));
//	{
//		if (list != NULL) {
//			list->head = NULL;
//		}
//		return list;
//	};
//};

