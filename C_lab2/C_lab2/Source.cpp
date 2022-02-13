#define _CRT_SECURE_NO_WARNINGS

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
	double number;
	char operation;
} node;

typedef struct Node_arr 
{
	node* arr;
	int length;
} node_arr;

void push_back(string* str, char n)
{
	string new_str;
	new_str.length = str->length + 1;
	new_str.data = (char*)malloc(sizeof(char) * new_str.length);

	for (int i = 0; i < str->length; ++i)
		new_str.data[i] = str->data[i];
	new_str.data[str->length] = n;

	str->data = new_str.data;
	str->length = new_str.length;
	free(new_str.data);
}

node_arr translate_to_notation(string* expression)
{
	int notation_capacity = 1;
	int operations_capacity = 1;
	
	node_arr nodes;
	nodes.arr = (node*)calloc(notation_capacity, sizeof(node));
	nodes.length = 0;

	string operations;
	operations.length = 0;
	operations.data = (char*)calloc(operations_capacity, sizeof(char));

	string var;
	real var_value = -1;
	var.length = 0;
	var.data = (char*)calloc(operations_capacity, sizeof(char));


	int number = -1;
	for (int i = 0; i < expression->length; ++i)
	{
		if ((expression->data[i] >= '0' && expression->data[i] <= '9') || 
			(expression->data[i] >= 'a' && expression->data[i] <= 'Z'))
		{
			if (expression->data[i] >= '0' && expression->data[i] <= '9')
			{
				if (number == -1)
					number = 0;
				number = number * 10 + expression->data[i] - 48;
				continue;
			}
			else
			{
				if (var_value == -1)
					var_value = 0;
				push_back(&var, expression->data[i]);
				continue;
			}
		}

		if (number != -1 || var_value != -1)
		{
			if (var_value != -1) {
				printf("%s = ", var.data);
				scanf("%lf", &var_value);
				
				free(var.data);
				var.length = 0;
				var.data = (char*)calloc(operations_capacity, sizeof(char));
				number = var_value;
			}

			if (nodes.length >= notation_capacity) 
			{
				nodes.arr = (node*)realloc(nodes.arr, ++notation_capacity * sizeof(node));
			}
			nodes.arr[nodes.length].flag = 0;
			nodes.arr[nodes.length++].number = number;
		}
		number = -1;
		var_value = -1;
		
		switch (expression->data[i]) {
		case '(':
			push_back(&operations, ')');
			break;
		case '+':
			push_back(&operations, '+');
			break;
		case '-':
			push_back(&operations, '-');
			break;
		case '*':
			push_back(&operations, '*');
			break;
		case '/':
			push_back(&operations, '/');
			break;
		case '^':
			push_back(&operations, '^');
			break;
		case ')':
			for (int j = operations.length - 1; j > -1; --j) {
				if (operations.data[j] == '(') {
					operations.length -= (operations.length - j);
					break;
				}

				if (nodes.length >= notation_capacity) {
					nodes.arr = (node*)realloc(nodes.arr, ++notation_capacity * sizeof(node));
				}
				nodes.arr[nodes.length].flag = 1;
				nodes.arr[nodes.length++].operation = operations.data[j];
			}
		}
	}

	if (number != -1)
	{
		if (nodes.length >= notation_capacity)
		{
			nodes.arr = (node*)realloc(nodes.arr, ++notation_capacity * sizeof(node));
		}
		nodes.arr[nodes.length].flag = 0;
		nodes.arr[nodes.length++].number = number;
	}

	if (operations.length != 0) {
		for (int j = operations.length - 1; j > -1; --j) {

			if (nodes.length >= notation_capacity) {
				nodes.arr = (node*)realloc(nodes.arr, ++notation_capacity * sizeof(node));
			}
			nodes.arr[nodes.length].flag = 1;
			nodes.arr[nodes.length++].operation = operations.data[j];
		}
	}
	return nodes;
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
	new_str.data = (char*)malloc(sizeof(char) * new_str.length);

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
	new_data.arr = (node*)malloc(sizeof(node) * new_data.length);

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
	new_str.data = (char*)malloc(sizeof(char) * new_str.length);

	for (int i = 0; i < index; ++i)
		new_str.data[i] = str->data[i];
	for (int i = 0; i < under.length; ++i)
		new_str.data[index + i] = under.data[i];
	for (int i = index; i < str->length; ++i)
		new_str.data[i + under.length] = str->data[i];

	str->data = new_str.data;
	str->length = new_str.length;
}

real _CALC(string task)
{
	
	remove_space(&task);
	node_arr data = translate_to_notation(&task);
	
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

char* get_string(int* len) {
	*len = 0; // изначально строка пуста
	int capacity = 1; // ёмкость контейнера динамической строки (1, так как точно будет '\0')
	char* s = (char*)malloc(sizeof(char)); // динамическая пустая строка

	char c = getchar(); // символ для чтения данных

	// читаем символы, пока не получим символ переноса строки (\n)
	while (c != '\n') {
		s[(*len)++] = c; // заносим в строку новый символ

		// если реальный размер больше размера контейнера, то увеличим его размер
		if (*len >= capacity) {
			capacity *= 2; // увеличиваем ёмкость строки в два раза
			s = (char*)realloc(s, capacity * sizeof(char)); // создаём новую строку с увеличенной ёмкостью  
		}

		c = getchar(); // считываем следующий символ          
	}

	s[*len] = '\0'; // завершаем строку символом конца строки

	return s; // возвращаем указатель на считанную строку
}

int main() 
{
	int len; // длина строки
	char* s = get_string(&len); // считываем динамическую строку

	string exp;
	exp.length = len;
	exp.data = s;

	printf("%lf\n", _CALC(exp));

	return 0;
}
