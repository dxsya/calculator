#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef long double real;

struct String 
{
	char* data;
	int length;
}; typedef struct String string;

void init(string* str, char* _str)
{
	str->data = _str;
	str->length = strlen(_str);
}

void remove_space(string* str)
{
	int new_length = 0;
	for (int i = 0; i < str->length; ++i)
		if (str->data[i] != ' ')
			new_length++;
	string new_str;
	new_str.length = 0;
	new_str.data = (char*)malloc(sizeof(char) * new_length);

	for (int i = 0; i < str->length; ++i)
		if (str->data[i] != ' ')
			new_str.data[new_str.length++] = str->data[i];

	str->data = new_str.data;
	str->length = new_str.length;
}

void erase(string* str, int begin, int count)
{
	string new_str;
	new_str.length = str->length - count;
	new_str.data = malloc(sizeof(char) * new_str.length);

	for (int i = 0; i < begin; ++i)
		new_str.data[i] = str->data[i];
	for (int i = begin + count; i < str->length; ++i)
		new_str.data[i - count] = str->data[i];

	str->data = new_str.data;
	str->length = new_str.length;
}

void insert(string* str, int index, string under)
{
	string new_str;
	new_str.length = str->length + under.length;
	new_str.data = malloc(sizeof(char) * new_str.length);

	for (int i = 0; i < index; ++i)
		new_str.data[i] = str->data[i];
	for (int i = 0; i < under.length; ++i)
		new_str.data[index + i] = under.data[i];
	for (int i = index; i < str->length; ++i)
		new_str.data[i + under.length] = str->data[i];

	str->data = new_str.data;
	str->length = new_str.length;
}

void push_back(string* str, char n)
{
	string new_str;
	new_str.length = str->length + 1;
	new_str.data = malloc(sizeof(char) * new_str.length);

	for (int i = 0; i < str->length; ++i)
		new_str.data[i] = str->data[i];
	new_str.data[str->length] = n;

	str->data = new_str.data;
	str->length = new_str.length;
}

void _CALC(string* notation)
{
	
}

int main() 
{
	
	return 0;
}