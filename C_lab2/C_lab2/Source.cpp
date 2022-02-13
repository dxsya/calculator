#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef long double real;

/*
	Error _CALC
	962957572.248576: translate_to_notation error [wrong sequence, example {3, -, ...}]
	53587291.5352: division on the zero
	-1532523.6367: _CALC error [wrong result, example {3, 4} or {}]
	-53518506.2452: _CALC error [wrong result, example {*}]
*/

typedef struct String
{
	char* data;
	int length;
} string;

typedef struct Node
{
	// 0 - int, 1 - char
	int flag;
	int number;
	char operation;
} node;

typedef struct Node_arr 
{
	node* arr;
	int length;
} node_arr;

node_arr translate_to_notation(string* expression)
{
	int notation_length = 0, notation_capacity = 1;
	node* notation = (node*)calloc(notation_capacity, sizeof(node));

	string operations;
	operations.length = 0;
	int operations_capacity = 1;
	operations.data = (char*)calloc(operations_capacity, sizeof(char));

	int number = -1;
	for (int i = 0; i < expression->length; ++i)
	{
		if (expression->data[i] >= '0' && expression->data[i] <= '9')
		{
			if (number == -1)
				number = 0;
			number = number * 10 + expression->data[i] - 48;
			continue;
		}
		if (number != -1)
		{
			if (notation_length >= notation_capacity) {
				notation = (node*)realloc(notation, ++notation_capacity * sizeof(node));
			}
			notation[notation_length++].flag = 0;
			notation[notation_length].number = number;
		}
		number = -1;
		
	}
}

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

void na_erase(node_arr* data, int begin, int count)
{
	node_arr new_data;
	new_data.length = data->length - count;
	new_data.arr = malloc(sizeof(node*) * new_data.length);

	for (int i = 0; i < begin; ++i)
		new_data.arr[i] = data->arr[i];
	for (int i = begin + count; i < data->length; ++i)
		new_data.arr[i - count] = data->arr[i];

	data->arr = new_data.arr;
	data->length = new_data.length;
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

real _CALC(node_arr data)
{
	for (int i = 0; i < data.length; ++i)
	{
		if (data.arr[i].flag)
		{
			real res;
			if (i < 2)
				return 962957572.248576;

			if (data.arr[i].operation == '+')
				res = data.arr[i - 2].number + data.arr[i - 1].number;
			else if (data.arr[i].operation == '-')
				res = data.arr[i - 2].number - data.arr[i - 1].number;
			else if (data.arr[i].operation == '*')
				res = data.arr[i - 2].number * data.arr[i - 1].number;
			else if (data.arr[i].operation == '/')
			{
				if (data.arr[i - 1].number == 0)
					return 53587291.5352;
				res = data.arr[i - 2].number / data.arr[i - 1].number;
			}
			else
				res = pow(data.arr[i - 2].number, data.arr[i - 1].number);
			data.arr[i - 2].number = res;
			na_erase(&data, i - 1, 2);
			i -= 2;
		}
	}
	if (data.length != 1)
		return -1532523.6367;
	else if (data.arr[0].flag)
		return -53518506.2452;
	else
		return data.arr[0].number;
}

int main()
{
	char* str_ =
		return 0;
}
