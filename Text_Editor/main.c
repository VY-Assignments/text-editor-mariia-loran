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

struct Clipboard {
	char* c_data;
	int64_t c_length;
};


void Push(struct History* h, char* value)
{
	char* text_save = (value == NULL) ? "" : value;
	if (h->top < Max_Undo - 1) {
		h->top++;
	}
	else {
		free(h->h_addres[0]);
		for (int i = 1; i < Max_Undo; i++) {
			h->h_addres[i - 1] = h->h_addres[i];
			h->h_length[i - 1] = h->h_length[i];
		}
		h->top = Max_Undo - 1;
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

void ClearRedoHistory(struct History* redo_history) {
	while (redo_history->top >= 0) {
		free(redo_history->h_addres[redo_history->top]);
		redo_history->top--;
	}
}



void UndoCommand(struct DynamicBuffer* buffer, struct History* history, struct History* redo_history)
{
	if (history->top == - 1) {
		printf("Nothing to undo\n");
		return; 
	}
	Push(redo_history, buffer->data);	
	char* prev_text = history->h_addres[history->top];
	buffer->data = malloc(strlen(prev_text) +1);
	if (buffer->data != NULL) {
	  strcpy_s(buffer->data,strlen(prev_text) +1,prev_text);
	}
	buffer->length = history->h_length[history->top];
	free( history->h_addres[history->top]);
	history->top--;
	printf("Success\n");
}

void RedoCommand(struct DynamicBuffer* buffer, struct History* history, struct History* redo_history)
{
	if (redo_history->top == -1) {
		printf("Nothing to redo\n");
		return;
	}
	Push(history, buffer->data);
	free(buffer->data);
	char* next_text = redo_history->h_addres[redo_history->top];
	buffer->data = malloc(strlen(next_text) + 1);
	if (buffer->data != NULL) {
		strcpy_s(buffer->data, strlen(next_text) + 1, next_text);
	}
	buffer->length = redo_history->h_length[redo_history->top];
	free(redo_history->h_addres[redo_history->top]);
	redo_history->top--;
	printf("Redo Success\n");
}






void CutCommand(struct DynamicBuffer* buffer , struct History* history,struct Clipboard* clipboard)
{
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	int target_line = 0;
	int target_symbol = 0;
	int number_of_symbols = 0;
	Cordinates_for_delete(&target_line, &target_symbol, &number_of_symbols);
	if (number_of_symbols <= 0) {
		printf("Nothing to cut\n");
		return;
	}
	int64_t cut_position = FindInserPosition(buffer, target_line, target_symbol);
	if (cut_position == -1) return;
	int64_t cut_symbols = 0;
	while (cut_symbols < number_of_symbols && (cut_position + cut_symbols) < buffer->length)
	{
		cut_symbols++;
	}
	if (cut_symbols == 0) {
		printf("No chaacters to cut");
		return;
	}

	if (clipboard->c_data != NULL) {
		free(clipboard->c_data);
		clipboard->c_data = NULL;
		clipboard->c_length = 0;
	}
	clipboard->c_data = malloc((size_t)cut_symbols + 1);
	if (clipboard->c_data == NULL) {
		printf("Memory allocation error");
		return;
	}
	memcpy(clipboard->c_data, &buffer->data[cut_position], (size_t)cut_symbols);
	clipboard->c_length = cut_symbols;
	clipboard->c_data[clipboard->c_length] = '\0';

	Push(history, buffer->data);
	int64_t old_length = buffer->length;
	int64_t bytes_to_move = old_length - (cut_position + cut_symbols);
	if (bytes_to_move > 0) {
		memmove(&buffer->data[cut_position], &buffer->data[cut_position + cut_symbols], (size_t)bytes_to_move);
	}
	int64_t new_length = old_length - cut_symbols;;
	ResizeBuffer(buffer, new_length);
	buffer->length = new_length;
	buffer->data[buffer->length] = '\0';
	printf("Text has been cutted\n");
}


void CopyCommand(struct DynamicBuffer* buffer, struct History* history, struct Clipboard* clipboard)
{
	if (buffer->data == NULL || buffer->length == 0) {
		printf("Buffer is empty");
		return;
	}
	int target_line = 0, target_symbol = 0, number_of_symbols = 0;
	Cordinates_for_delete(&target_line, &target_symbol, &number_of_symbols);
	if (number_of_symbols <= 0) {
		printf("Nothing to copy\n");
		return;
	}
	int64_t copy_position = FindInserPosition(buffer, target_line, target_symbol);
	if (copy_position == -1) return;
	
	int64_t copy_symbols = 0;
	while (copy_symbols < number_of_symbols && (copy_position + copy_symbols) < buffer->length)
	{
		copy_symbols++;
	}
	if (copy_symbols == 0) {
		printf("No chaacters to copy");
		return;
	}
	if (clipboard->c_data != NULL) free(clipboard->c_data);
	clipboard->c_data = malloc((size_t)copy_symbols + 1);
	if (clipboard->c_data == NULL) return;
	
	memcpy(clipboard->c_data, &buffer->data[copy_position], (size_t)copy_symbols);
	clipboard->c_length = copy_symbols;
	clipboard->c_data[clipboard->c_length] = '\0';
	printf("Text has been copied\n");

}


void PutCommand(struct DynamicBuffer* buffer, struct History* history, struct Clipboard* clipboard)
{
	if (clipboard->c_data == NULL || clipboard->c_length == 0) {
		printf("Buffer is empty\n");
		return;
	}
	int target_line = 0,target_symbol = 0;
	printf("Choose line and index: ");
	if (scanf_s("%d %d", &target_line, &target_symbol) != 2) {
		printf("Bad format \n");
		int ch;while ((ch = getchar()) != '\n' && ch != EOF);
		return;
	}
	int ch;while ((ch = getchar()) != '\n' && ch != EOF);
		
	int64_t insert_position = 0;
	if (buffer->length > 0) {
		insert_position = FindInserPosition(buffer, target_line, target_symbol);
		if (insert_position == -1) return;
	}
	else {
		if (target_line != 0 || target_symbol != 0) {
			printf("Error: Bufer is empty");
			return;
		}
	}
	Push(history, buffer->data);
	InserText(buffer, insert_position, clipboard->c_data);
	printf("Text has been putted\n");
}









void Insert_with_replacement(struct DynamicBuffer* buffer, struct History* history)
{
	int target_line = 0, target_symbol = 0;
	printf("Choose line and index: ");
	if (scanf_s("%d %d", &target_line, &target_symbol) != 2) {
		printf("Bad format \n");
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
		return;
	}
	int ch;while ((ch = getchar()) != '\n' && ch != EOF);

	char new_text[128];
	printf("Write text : ");
	if (fgets(new_text, sizeof(new_text), stdin) == NULL) {
		return;
	}
	TrimNewLine(new_text);

	int64_t input_len = (int64_t)strlen(new_text);
	if (input_len == 0) return;

	int64_t start_position = 0;
	if (buffer->length > 0) {
		start_position = FindInserPosition(buffer, target_line, target_symbol);
		if (start_position == -1) return;
	}
	else {
		if (target_line != 0 || target_symbol != 0) {
			printf("Error: Bufer is empty");
			return;
		}
	}

	int64_t required_length = start_position + input_len;
	int64_t current_old_lenght = buffer->length;
	if (required_length > current_old_lenght) {
		if (!ResizeBuffer(buffer, required_length)) {
			return;
		}
		buffer->length = required_length;
		buffer->data[buffer->length] = '\0';
	}
	memcpy(&buffer->data[start_position], new_text, (size_t)input_len);
	printf("Text has been replaced \n");

}



void Cursor_Based_Logic(struct DynamicBuffer* buffer, struct History* history, struct History* redo_history, struct Clipboard* clipboard)
{
	static int cursor_line = 0;
	static int cursor_symbol = 0;
	printf("--------CURSOR MENU--------\n");
	printf("1.Move curor\n8.Delete text from cursor\n");
	printf("11.Cut text from cursor\n12.Past text from cursor\n");
	printf("13.Copy text from cursor\n14.Insert with replacement at cursor\n");
	printf("Choose the command: ");

	char sub_input[10];
	if (fgets(sub_input, sizeof(sub_input), stdin) == NULL) return;
	TrimNewLine(sub_input);
	int action;
	if (sscanf_s(sub_input, "%d", &action) != 1) {
		printf("Invalid input");
		action = -1;
	}

	int  t_line = 0, t_sym = 0;
	int num_of_symbols = 0;
	char text[120];
	int64_t pos = 0;
	int64_t old_length = 0;
	int64_t bytes_to_move = 0;
	int64_t actual_symbols = 0;
	int64_t required_lenght = 0;
	switch (action) {
	case 1:
		printf("Enter new cursor position(line and symbols): \n");
		if (scanf_s("%d %d", &t_line, &t_sym) != 2) {
			int ch;while ((ch = getchar()) != '\n' && ch != EOF);
			return;
		}
		{
			int ch;while ((ch = getchar()) != '\n' && ch != EOF);
		}
		if (buffer->length > 0) {
			if (FindInserPosition(buffer, t_line, t_sym) == -1) return;
		}
		else if (t_line != 0 || t_sym != 0) {
			printf("Buffer is empty. Cursor 0 0\n");
			return;
		}
		cursor_line = t_line;
		cursor_symbol = t_sym;
		printf("Cursor successfully moved to %d %d\n", cursor_line, cursor_symbol);
		break;
	case 8:
		printf("You entered 8 - Delete text from cursor\n");
		if (buffer->data == NULL || buffer->length == 0) {
			printf("Buffer is empty");
			return;
		}
		printf("Choose line and index: ");
		if (scanf_s("%d", &num_of_symbols) != 1) {
			printf("Bad format \n");
			int ch; while ((ch = getchar()) != '\n' && ch != EOF);
			return;
		}
		{
			int ch; while ((ch = getchar()) != '\n' && ch != EOF);
		}if (num_of_symbols <= 0) {
			printf("Nothing to delete\n");
			return;
		}

		pos = FindInserPosition(buffer, cursor_line, cursor_symbol);
		if (pos == -1)	return;

		actual_symbols = 0;
		while (actual_symbols < num_of_symbols && (pos + actual_symbols) < buffer->length)
		{
			actual_symbols++;
		}
		if (actual_symbols == 0) {
			printf("No characters ");
			return;
		}
		Push(history, buffer->data);
		ClearRedoHistory(redo_history);
		old_length = buffer->length;
		bytes_to_move = old_length - (pos + actual_symbols);
		if (bytes_to_move > 0) {
			memmove(&buffer->data[pos], &buffer->data[pos + actual_symbols], (size_t)bytes_to_move);
		}
		buffer->length = old_length - actual_symbols;;
		ResizeBuffer(buffer, buffer->length);
		buffer->data[buffer->length] = '\0';
		printf("Text has been deleted woth cursor\n");
		break;
	case 11:
		printf("You entered 11 -  Cut text from cursor\n");
		if (buffer->data == NULL || buffer->length == 0) {
			printf("Buffer is empty");
			return;
		}
		printf("Choose line and index: ");
		if (scanf_s("%d", &num_of_symbols) != 1) {
			printf("Bad format \n");
			int ch; while ((ch = getchar()) != '\n' && ch != EOF);
			return;
		}
		{
			int ch; while ((ch = getchar()) != '\n' && ch != EOF);
		}if (num_of_symbols <= 0) {
			printf("Nothing to delete\n");
			return;
		}
		pos = FindInserPosition(buffer, cursor_line, cursor_symbol);
		if (pos == -1)	return;
		actual_symbols = 0;
		while (actual_symbols < num_of_symbols && (pos + actual_symbols) < buffer->length)
		{
			actual_symbols++;
		}
		if (actual_symbols == 0) {
			printf("No characters ");
			return;
		}
		if (clipboard->c_data != NULL) {
			free(clipboard->c_data);
			clipboard->c_data = malloc((size_t)actual_symbols + 1);
			if (clipboard->c_data == NULL) {
				printf("Memory allocation error");
				return;
			}
		}
		memcpy(clipboard->c_data, &buffer->data[pos], (size_t)actual_symbols);
		clipboard->c_length = actual_symbols;
		clipboard->c_data[clipboard->c_length] = '\0';
		Push(history, buffer->data);
		ClearRedoHistory(redo_history);
		old_length = buffer->length;
		bytes_to_move = old_length - (pos + actual_symbols);
		if (bytes_to_move > 0) {
			memmove(&buffer->data[pos], &buffer->data[pos + actual_symbols], (size_t)bytes_to_move);
		}
		buffer->length = old_length - actual_symbols;;
		ResizeBuffer(buffer, buffer->length);
		buffer->data[buffer->length] = '\0';
		printf("Text has been deleted woth cursor\n");
		break;
	case 12:
		printf("You entered 12 - Paste text from cursor\n");
		if (clipboard->c_data == NULL || clipboard->c_length == 0) {
			printf("Buffer is empty\n");
			return;
		}
		if (buffer->length > 0) {
			pos = FindInserPosition(buffer, cursor_line, cursor_symbol);
			if (pos == -1) return;
		}
		Push(history, buffer->data);
		ClearRedoHistory(redo_history);
		InserText(buffer, pos, clipboard->c_data);
		cursor_symbol += (int)clipboard->c_length;
		break;
	case 13:
		printf("You entered 12 - Copy text from cursor\n");
		if (buffer->data == NULL || buffer->length == 0) {
			printf("Buffer is empty");
			return;
		}
		printf("Enter num of symbols to copy: ");
		if (scanf_s("%d", &num_of_symbols) != 1) {
			printf("Bad format \n");
			int ch; while ((ch = getchar()) != '\n' && ch != EOF);
			return;
		}
		{
			int ch; while ((ch = getchar()) != '\n' && ch != EOF);
		}
		if (num_of_symbols <= 0) {
			printf("Nothing to delete\n");
			return;
		}

		pos = FindInserPosition(buffer, cursor_line, cursor_symbol);
		if (pos == -1)	return;

		actual_symbols = 0;
		while (actual_symbols < num_of_symbols && (pos + actual_symbols) < buffer->length)
		{
			actual_symbols++;
		}
		if (actual_symbols == 0) {
			printf("No characters ");
			return;
		}
		if (clipboard->c_data != NULL) {
			free(clipboard->c_data);
		}
		clipboard->c_data = malloc((size_t)actual_symbols + 1);

		if (clipboard->c_data == NULL) {
			printf("Buffer is empty\n");
			return;
		}

		memcpy(clipboard->c_data, &buffer->data[pos], (size_t)actual_symbols);
		clipboard->c_length = actual_symbols;
		clipboard->c_data[clipboard->c_length] = '\0';

		printf("Text copied to clipboard%s\n", clipboard->c_data);
		break;


	case 14:
		printf("You entered 14 - Insert with replacement at cursor\n\n");
		printf("Write Text");
		if (fgets(text, sizeof(text), stdin) == NULL) return;
		TrimNewLine(text);

		actual_symbols = (int64_t)strlen(text);
		if (actual_symbols == 0) {
			return;
		}
		if (buffer->length > 0) {
			pos = FindInserPosition(buffer, cursor_line, cursor_symbol);
			if (pos == -1) return;
		}
		Push(history, buffer->data);
		ClearRedoHistory(redo_history);

		required_lenght = pos + actual_symbols;
		if (required_lenght > buffer->length) {
			if (!ResizeBuffer(buffer, required_lenght)) return;
			buffer->length = required_lenght;
			buffer->data[buffer->length] = '\0';
		}
		memcpy(&buffer->data[pos], text, (size_t)actual_symbols);
		printf("Text replaced successfully\n");
		break;

	default:
		printf("You did not enter right number\n");
		break;

	}
}


int main() {
	
	struct History history = { {NULL}, {0}, -1 };
	//struct History history = { 0 };
	//struct History redo_history = {0};
	struct History redo_history = { {NULL}, {0}, -1 };
	struct DynamicBuffer my_buffer = { NULL, 0 };
	struct Clipboard clipboard = { NULL, 0 };
	int command;
	int a = 1;
	while (a) {
		printf("--------MENU--------\n");
		printf("1.Append text symbols to the end\n2.Start the new line\n3.Use files to save the information\n4.Use files to load the information\n5.Print the current text to console\n");
		printf("6.Insert the text by line and symbol index\n7.Search\n");

		printf("8.Delete the Command\n9.Undo Command\n10.Redo Command\n11.Cut Command\n12.Paste Command\n13.Copy Command\n");
		printf("14.Insert with replacement Command\n15.Implement cursor-based logic\n16.Clearing the console\n17.Exit\n\n");
		printf("Choose the command: ");

		scanf_s("%d", &command);
		

		char c;
		while ((c = getchar()) != '\n' && c != EOF);


		switch (command) {
		case 1:
			printf("You entered 1 - Append text symbols to the end  \n");
			Append(&my_buffer, &history);
			break;
		case 2:
			printf("You entered 2 - Start the new line \n");
			StartTheNewLine(&my_buffer, &history);
			break;
		case 3:
			printf("You entered 3 - Use files to save the information\n");
			Save(&my_buffer);
			break;
		case 4:
			printf("You entered 4 - Use files to load the information\n");
			Load(&my_buffer);
			break;
		case 5:
			printf("You entered 5 -  Print the current text to console\n");
			PrintText(&my_buffer);
			break;
		case 6:
			printf("You entered 6 -  Insert the text by line and symbol index\n");
			InsertTextByLine(&my_buffer, &history);
			break;
		case 7:
			printf("You entered 7 -  Search (please note that the text can be found more than once)\n");
			Search(&my_buffer);
			break;



		case 8:
			printf("You entered 8 -  Delete Command\n");
			DeleteTheCommand(&my_buffer, &history);
			break;
		case 9:
			printf("You entered 9 -  Undo Command\n");
			UndoCommand(&my_buffer, &history, &redo_history);
			break;
		case 10:
			printf("You entered 10 -  Redo Command\n");
			RedoCommand(&my_buffer, &history, &redo_history);
			break;
		case 11:
			printf("You entered 11 -  Cut Command\n");
			ClearRedoHistory(&redo_history);
			CutCommand(&my_buffer, &history, &clipboard);
			break;
		case 12:
			printf("You entered 12 -  Paste Command\n");
			ClearRedoHistory(&redo_history);
			PutCommand(&my_buffer, &history, &clipboard);
			break;
		case 13:
			printf("You entered 13 -  Copy Command\n");
			CopyCommand(&my_buffer, &history, &clipboard);
			break;
		case 14:
			printf("You entered 14 -  Insert with replacement Command\n");
			ClearRedoHistory(&redo_history);
			Insert_with_replacement(&my_buffer, &history);
			break;
		case 15:
			printf("You entered 15 -  Implement cursor-based logic\n");
			Cursor_Based_Logic(&my_buffer, &history, &redo_history, &clipboard);
			break;


		case 16:
			printf("You entered 16 -  Clear Console\n");
			ClearConsole();
			break;
		case 17:
			a = 0;
			break;

		default:
			printf("You did not enter right number\n");
		}
	}
	for (int i = 0; i <= history.top; i++) free(history.h_addres[i]);
	for (int i = 0; i <= redo_history.top; i++) free(redo_history.h_addres[i]);
	
	if (my_buffer.data != NULL) {
		free(my_buffer.data);
	}
	if (clipboard.c_data != NULL) {
		free(clipboard.c_data);
	}
	return 0;
}