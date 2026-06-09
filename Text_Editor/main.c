#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#define Max_Undo 15
struct DynamicBuffer {
	char* data;
	int64_t length;
};
struct History
{
	char* h_addres[Max_Undo];
	int64_t h_length[Max_Undo];
	int top;
};



void Push(struct History* h, char* value)
{
	char* text_save = (value == NULL) ? "" : value;
	if (h->top < Max_Undo - 1) {
		h->top++;
	}
	else {
		return;
	}
	size_t len = strlen(text_save);
	char* alloc_text = malloc(len + 1);
	if (alloc_text != NULL) {

		h->h_addres[h->top] = alloc_text;
		h->h_length[h->top] = (int64_t)len;
		strcpy_s(h->h_addres[h->top], len + 1, text_save);
	}
	else {
		h->top--;
	}

}
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
void Append(struct DynamicBuffer* buffer, struct History* history) {
	Push(history, buffer->data);
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
	int64_t addSpace = (buffer->length > 0 && buffer->data[buffer->length -1] != '\n') ? 1 : 0;
	int64_t new_length = buffer->length + countlen + addSpace;
	if (!ResizeBuffer(buffer, new_length)) {
		return;
	}
	int64_t current_pos = buffer->length;
	if (addSpace) {
		buffer->data[current_pos] = ' ';
		memcpy(&buffer->data[current_pos + 1], temp_input, (size_t)countlen);
	}
	else {
		memcpy(&buffer->data[current_pos], temp_input, (size_t)countlen);
	}
	buffer->length = new_length;
	buffer->data[buffer->length] = '\0';
}



void StartTheNewLine(struct DynamicBuffer* buffer, struct History* history) {
	Push(history, buffer->data);
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
		if (buffer->data == NULL) {
			printf("Error: problem with buffer");
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
	printf("Text has been loaded successfully (%lld bytes)\n", buffer->length);
}




void PrintText(struct DynamicBuffer* buffer) {
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	printf("%s\n", buffer->data);
}











void Cordinates_for_insert(int* line, int* symbol, char* text , int64_t text_size) {
	printf("Choose line and index: ");
	if (scanf_s("%d %d", line, symbol) != 2) {
		printf("Bad format \n");
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
		return;
	}
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
	printf("Enter text to insert: ");
	if (fgets(text, (int)text_size, stdin) == NULL) {
		return;
	}
	TrimNewLine(text);
}
void Cordinates_for_delete(int* line, int* symbol, int* number_of_symbols) {
	printf("Choose line, index and number of symbols: ");
	if (scanf_s("%d %d %d", line, symbol, number_of_symbols) != 3) {
		printf("Bad format \n");
		*number_of_symbols = 0;	
	}
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
} 

int64_t FindInserPosition(struct DynamicBuffer* buffer, int line, int symbol) {
	int64_t insert_pos = 0;
	int current_line = 0;
	while (current_line < line && insert_pos < buffer->length) {
		if (buffer->data[insert_pos] == '\n') {
			current_line++;
		}
		insert_pos++;
	}
	if (current_line < line) {
		printf("Error:Line not found\n");
		return -1;
	}
	int current_symbol = 0;
	while (current_symbol < symbol && insert_pos < buffer->length && buffer->data[insert_pos] != '\n') {
		current_symbol++;
		insert_pos++;
	}
	if (current_symbol < symbol) {
		printf("Error: Symbol index exceeds line length\n");
		return -1;
	}
	return insert_pos;
}


void InserText(struct DynamicBuffer* buffer, int64_t insert_pos, char* text) {
	int64_t insert_len = (int64_t)strlen(text);
	if (insert_len == 0) {
		printf("Nothing to insert\n");
		return;
	}
	int64_t old_length = buffer->length;
	int64_t new_length = buffer->length + insert_len;
	if (!ResizeBuffer(buffer, new_length)) {
		return;
	}
	int64_t bytes_to_move = old_length - insert_pos;
	if (bytes_to_move > 0) {
		memmove(&buffer->data[insert_pos + insert_len], &buffer->data[insert_pos], (size_t)bytes_to_move);
	}
	memcpy(&buffer->data[insert_pos], text, (size_t)insert_len);
	buffer->length = new_length;
	buffer->data[buffer->length] = '\0';
	printf("Text has been inserted\n");
}





void InsertTextByLine(struct DynamicBuffer* buffer, struct History* history) {
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	int target_line = 0;
	int target_symbol = 0;
	char text_to_insert[120];
	Cordinates_for_insert(&target_line, &target_symbol, text_to_insert, sizeof(text_to_insert));
	int64_t find_insert_position = FindInserPosition(buffer, target_line, target_symbol);
	if (find_insert_position == -1) {
		return;
	}
	Push(history, buffer->data);
	InserText(buffer, find_insert_position, text_to_insert);
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
	int64_t search_len = strlen(search_str);
	if (search_len == 0) return;
	int line_index = 0;
	int symbol_index = 0;
	int found_any = 0;
	for (int64_t i = 0; i <= buffer->length - search_len; i++) {
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



// HW2

void DeleteTheCommand(struct DynamicBuffer* buffer, struct History* history) {
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	int target_line = 0;
	int target_symbol = 0;
	int number_of_symbols = 0;
	Cordinates_for_delete(&target_line, &target_symbol, &number_of_symbols);
	if (number_of_symbols <= 0) {
		printf("Nothing to delete\n");
		return;
	} 
	int64_t delete_position = FindInserPosition(buffer, target_line, target_symbol);
	if (delete_position == -1) {
		return;
	}
	int64_t del_symbols = 0;
	while (del_symbols < number_of_symbols && (delete_position + del_symbols) < buffer->length)
	{
		del_symbols++;
	}
	if (del_symbols == 0 ){
		printf("No chaacters to delete");
		return;
	}
	Push(history, buffer->data);
	int64_t old_length = buffer->length;
	int64_t bytes_to_move = old_length - (delete_position + del_symbols);

	if (bytes_to_move > 0) {
		memmove(&buffer->data[delete_position ], &buffer->data[delete_position + del_symbols], (size_t)bytes_to_move);
	}
	int64_t new_length = old_length- del_symbols;;
	ResizeBuffer(buffer, new_length);
	buffer->length = new_length;
	buffer->data[buffer->length] = '\0';
	printf("Text has been deleted\n");
}





void UndoCommand(struct DynamicBuffer* buffer, struct History* history)
{
	if (history->top == - 1) {
		printf("Nothing to undo\n");
		return; 
	}
	if (buffer->data != NULL) {
		free(buffer->data);
	}
	buffer->length = history->h_length[history->top];
	buffer->data = history->h_addres[history->top];

	history->h_addres[history->top] = NULL;
	history->h_length[history->top] = 0;

	history->top--;
	printf("Success\n");
}

void RedoCommand(struct DynamicBuffer* buffer) 
{

}


void CutCommand(struct DynamicBuffer* buffer) {}
void CopyCommand(struct DynamicBuffer* buffer) {}
void PutCommand(struct DynamicBuffer* buffer) {}
void Insert_with_replacement(struct DynamicBuffer* buffer) {}
void Cursor_Based_Logic(struct DynamicBuffer* buffer) {}

int main() {
	
	struct History history = { {NULL}, {0}, -1 };
	struct DynamicBuffer my_buffer = { NULL, 0 };
	char command;
	int a = 1;
	while (a) {
		printf("--------MENU--------\n");
		printf("1.Append text symbols to the end\n2.Start the new line\n3.Use files to save the information\n4.Use files to load the information\n5.Print the current text to console\n");
		printf("6.Insert the text by line and symbol index\n7.Search\n");

		printf("8.Delete the Command\n9.Undo Command\n10.Redo Command\n11.Cut Command\n12.Copy Command\n13.Put Command\n");
		printf("14.Insert with replacement Command\n15.Implement cursor-based logic\n'c' - Clearing the console\nq/Q - Exit\n\n");
		printf("Choose the command: ");

		scanf_s(" %c", &command,1);
		

		char c;
		while ((c = getchar()) != '\n' && c != EOF);


		switch (command) {
		case '1':
			printf("You entered 1 - Append text symbols to the end  \n");
			Append(&my_buffer, &history);
			break;
		case '2':
			printf("You entered 2 - Start the new line \n");
			StartTheNewLine(&my_buffer, &history);
			break;
		case '3':
			printf("You entered 3 - Use files to save the information\n");
			Save(&my_buffer);
			break;
		case '4':
			printf("You entered 4 - Use files to load the information\n");
			Load(&my_buffer);
			break;
		case '5':
			printf("You entered 5 -  Print the current text to console\n");
			PrintText(&my_buffer);
			break;
		case '6':
			printf("You entered 6 -  Insert the text by line and symbol index\n");
			InsertTextByLine(&my_buffer, &history);
			break;
		case '7':
			printf("You entered 7 -  Search (please note that the text can be found more than once)\n");
			Search(&my_buffer);
			break;



		case '8':
			printf("You entered 8 -  Delete Command\n");
			DeleteTheCommand(&my_buffer, &history);
			break;
		case '9':
			printf("You entered 9 -  Undo Command\n");
			UndoCommand(&my_buffer, &history);
			break;
		case '10':
			printf("You entered 10 -  Redo Command\n");
			RedoCommand(&my_buffer);
			break;
		case '11':
			printf("You entered 11 -  Cut Command\n");
			CutCommand(&my_buffer);
			break;
		case '12':
			printf("You entered 12 -  Copy Command\n");
			CopyCommand(&my_buffer);
			break;
		case '13':
			printf("You entered 13 -  Put Command\n");
			PutCommand(&my_buffer);
			break;
		case '14':
			printf("You entered 14 -  Insert with replacement Command\n");
			Insert_with_replacement(&my_buffer);
			break;
		case '15':
			printf("You entered 15 -  Implement cursor-based logic\n");
			Cursor_Based_Logic(&my_buffer);
			break;


		case 'c':
			printf("You entered c -  Clear Console\n");
			ClearConsole();
			break;
		case 'q':
		case 'Q':
			a = 0;
			break;

		default:
			printf("You did not enter right number\n");
		}
	}

	if (my_buffer.data != NULL) {
		free(my_buffer.data);
	}
	return 0;
}