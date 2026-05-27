#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

struct DynamicBuffer {
	char* data;
	int64_t length;
};

void TrimNewLine(char* str) {
	if (str == NULL) return;
	int64_t countlen = (int64_t)strlen(str);

	if (countlen > 0 && str[countlen - 1] == '\n') {
		str[countlen - 1] = '\0';
	}
}
int ResizeBuffer(struct DynamicBuffer* buffer, int64_t new_size) {
	char* arr = realloc(buffer->data, (size_t)(new_size + 1) * sizeof(char));
	if (arr == NULL) {
		printf("Memory allocation error\n");
		return 0;
	}
	buffer->data = arr;
	return 1;
}
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

	if (!ResizeBuffer(buffer, new_length)) {
		return;
	}
	
	if (buffer->length == 0) {
		buffer->data[0] = '\0';
	}
	else
	{
		strcat_s(buffer->data, (size_t)new_length + 1, " ");
	}
	strcat_s(buffer->data, (size_t)new_length + 1, temp_input);
	buffer->length = new_length;
}



void StartTheNewLine(struct DynamicBuffer* buffer) {
	int64_t countlen = 1;
	int64_t new_length = buffer->length + countlen;
	if (!ResizeBuffer(buffer, new_length)) return;


	buffer->data[buffer->length] = '\n';
	buffer->data[new_length] = '\0';
	buffer->length = new_length;
}



void Save(struct DynamicBuffer* buffer) {
	char filename[100];
	printf("Enter the file name for saving: ");
	if (fgets(filename, sizeof(filename), stdin) == NULL) {
		return;
	}

	TrimNewLine(filename);
	FILE* file = NULL;

	fopen_s(&file, filename, "w");
	if (file == NULL) {
		printf("Error opening file for writing!\n");
		return;
	}
	if(buffer->data != NULL){
		fputs(buffer->data, file);
	}
	fclose(file);
	printf("Text has been saved successfully\n");

}
void Load(struct DynamicBuffer* buffer) {
	char filename[100];
	printf("Enter the file name for loading: ");
	if (fgets(filename, sizeof(filename), stdin) == NULL) {
		return;
	}
	TrimNewLine(filename);

	FILE* file = NULL;
	fopen_s(&file, filename, "r");
	if (file == NULL) {
		printf("Error opening file \n");
		return;
	}

	fseek(file, 0, SEEK_END);

	long file_size = ftell(file);
	if (file_size < 0) {
		printf("Error reading file size!\n");
		fclose(file);
		return;
	}

	rewind(file);
	int64_t new_length = (int64_t)file_size;

	if (buffer->data != NULL) {
		free(buffer->data);
		buffer->data = NULL;
		buffer->length = 0;
	}

	if (file_size > 0) {
		if (!ResizeBuffer(buffer, new_length)) {
			fclose(file);
			return;
		}
		size_t bytes_read = fread(buffer->data, sizeof(char), (size_t)file_size, file);
		buffer->length = (int64_t)bytes_read;
	}
	if (buffer->data != NULL) {
		buffer->data[buffer->length] = '\0';
	}
	fclose(file);
	printf("Text has been loaded successfuly(%lld bytes)\n", buffer->length);
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

	int target_line = 0;
	int target_symbol = 0;
	char text_to_insert[120];
	
	printf("Choose line and index: ");
	if (scanf_s("%d %d", &target_line, &target_symbol) != 2) {
		printf("Bad format \n");
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
		return;
	}

	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);

	printf("Enter text to insert: ");
	if (fgets(text_to_insert, sizeof(text_to_insert), stdin) == NULL) {
		return;
	}

	TrimNewLine(text_to_insert);


	int64_t insert_pos = 0;
	int current_line = 0;

	while (current_line < target_line && insert_pos < buffer->length) {
		if (buffer->data[insert_pos] == '\n') {
			current_line++;
		}
		insert_pos++;
	}

	if (current_line < target_line) {
		printf("Error: out of range insert index (Line not found)\n");
		return;
	}

	int current_symbol = 0;
	while (current_symbol < target_symbol && insert_pos < buffer->length) {
		if (buffer->data[insert_pos] == '\n') {
			break;
		}
		current_symbol++;
		insert_pos++;
	}

	if (current_symbol < target_symbol) {
		printf("Error");
		return;
	}

	int64_t insert_len = (int64_t)strlen(text_to_insert);
	if (insert_len == 0) {
		printf("Nothing to insert\n");
		return;
	}
	int64_t new_length = buffer->length + insert_len;

	if (!ResizeBuffer(buffer, new_length)) {
		return;
	}

	int64_t bytes_to_move = buffer->length - insert_pos;

	if (bytes_to_move > 0) {
		memmove(&buffer->data[insert_pos + insert_len], &buffer->data[insert_pos], (size_t)bytes_to_move);
	}
	memcpy(&buffer->data[insert_pos], text_to_insert, (size_t)insert_len);

	buffer->length = new_length;
	buffer->data[buffer->length] = '\0';

	printf("Text has been inserted\n");
}





void Search(struct DynamicBuffer* buffer) {
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	char search_str[128];
	printf("ENter text to search: ");
	if (fgets(search_str, sizeof(search_str), stdin) == NULL) return;
	TrimNewLine(search_str);

	size_t search_len = strlen(search_str);
	if (search_len == 0) return;

	int line_index = 0;
	int symbol_index = 0;
	int found_any = 0;

	for (int64_t i = 0; i < buffer->length; i++) {
		if (strncmp(&buffer->data[i], search_str, search_len) == 0) {
			printf("Text is present in this position: %d %d\n", line_index, symbol_index);
			found_any = 1;
		}

		if (buffer->data[i] == '\n') {
			line_index++;
			symbol_index = 0;
		}
		else
		{
			symbol_index++;
		}
	}
	if (!found_any) {
		printf("Text not found \n ");
	}
}


void ClearConsole() {
	system("cls");
}

int main() {
	

	struct DynamicBuffer my_buffer = { NULL, 0 };
	char command;
	while (1) {
		printf("--------MENU--------\n");
		printf("1.Append text symbols to the end\n2.Start the new line\n3.Use files to save the information\n4.Use files to save the information\n5.Print the current text to console\n");
		printf("6.Insert the text by line and symbol index\n7.Search\n8.Clearing the console\nq/Q - Exit\n\n");
	
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
			printf("You entered 3 - Use files to save the information\n");
			Save(&my_buffer);
		}
		else if (command == '4') {
			printf("You entered 4 - Use files to load the information\n");
			Load(&my_buffer);
		}
		else if (command == '5') {
			printf("You entered 5 -  Print the current text to console\n");
			PrintText(&my_buffer);
		}
		else if (command == '6') {
			printf("You entered 6 -  Insert the text by line and symbol index\n");
			InsertTextByLine(&my_buffer);
		}
		else if (command == '7') {
			printf("You entered 7 -  Search (please note that the text can be found more than once)\n");
			Search(&my_buffer);
		}
		else if (command == '8') {
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