#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/*
	Error _CALC
	962957572.248576: translate_to_notation error [wrong sequence, example {3, -, ...}]
	53587291.5352: division on the zero
	-1532523.6367: _CALC error [wrong result, example {3, 4} or {}]
	-53518506.2452: _CALC error [wrong result, example {*}]
*/

typedef long double real;

typedef struct String
{
	char* data;
	int length;
} string;

typedef struct Node
{
	int flag;
	real number;
	char operation;
} node;

typedef struct Node_arr
{
	node* arr;
	int length;
	int capacity;
} node_arr;

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

void push_back(string* str, char n)
{
	string new_str;
	new_str.length = str->length + 1;
	new_str.data = (char*)malloc(sizeof(char) * new_str.length);

	for (int i = 0; i < str->length; ++i)
		new_str.data[i] = str->data[i];
	new_str.data[str->length] = n;


	char* temp = str->data;
	str->data = new_str.data;
	str->length = new_str.length;

	free(temp);
}

void add_node(node_arr* nodes, int flag, real number, char operation) 
{	
	if (nodes->capacity == 0)
		nodes->arr = (node*)calloc(++nodes->capacity, sizeof(node));

	if (nodes->length >= nodes->capacity)
		nodes->arr = (node*)realloc(nodes->arr, ++nodes->capacity * sizeof(node));

	nodes->arr[nodes->length].flag = flag;
	nodes->arr[nodes->length].operation = operation;
	nodes->arr[nodes->length++].number = number;
}

int compare_op(char main, char a, char b, char c, char d)
{
	return (main == a || main == b || main == c || main == d);
}

int get_priority(char main, char operation) 
{
	int flag = 0;
	if (main == '+')
		return (compare_op(operation, '-', '*', '/', '^'));
	else if (main == '-')
		return (compare_op(operation, '+', '*', '/', '^'));
	else if (main == '*')
		return (compare_op(operation, '/', '^', '#', '#'));
	else if (main == '/')
		return (compare_op(operation, '*', '^', '#', '#'));
	else if (main == '^')
		return 0;
}

node_arr translate_to_notation(string* expression)
{
	int operations_capacity = 1;

	node_arr nodes;
	nodes.length = 0;
	nodes.capacity = 0;

	string operations;
	operations.length = 0;
	operations.data = (char*)calloc(operations_capacity, sizeof(char));

	string num;
	num.length = 0;
	num.data = (char*)calloc(operations_capacity, sizeof(char));

	string var;
	var.length = 0;
	var.data = (char*)calloc(operations_capacity, sizeof(char));

	real var_value = -1;
	real number = -1;

	for (int i = 0; i < expression->length; ++i)
	{
		if ((expression->data[i] >= '0' && expression->data[i] <= '9') ||
			(expression->data[i] >= 'A' && expression->data[i] <= 'Z') ||
			(expression->data[i] >= 'a' && expression->data[i] <= 'z') ||
			(expression->data[i] == '.'))
		{
			if (expression->data[i] >= '0' && expression->data[i] <= '9' || expression->data[i] == '.')
			{
				if (number == -1)
					number = 0;
				push_back(&num, expression->data[i]);
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
			number = atof(num.data);
			if (var_value != -1) 
			{
				for(int letter = 0; letter < var.length; ++letter)
					printf("%c", var.data[letter]);
				printf(" = ");
				scanf("%lf", &var_value);

				free(var.data);
				var.length = 0;
				number = var_value;
			}
			add_node(&nodes, 0, number, ' ');
			
			free(num.data);
			num.length = 0;
			num.data = (char*)calloc(operations_capacity, sizeof(char));
		}
		number = -1;
		var_value = -1;

		int op_len = operations.length;
		if (expression->data[i] == '(')
			push_back(&operations, '(');
		else if (expression->data[i] == ')')
		{
			for (int j = operations.length - 1; j > -1; --j) 
			{
				if (operations.data[j] == '(') 
				{
					operations.length -= (operations.length - j);
					break;
				}
				add_node(&nodes, 1, 0, operations.data[j]);
			}
		}
		else if (compare_op(expression->data[i], '+', '-', '*', '/') || expression->data[i] == '^') 
		{
			while (op_len > 0 && get_priority(expression->data[i], operations.data[op_len - 1]))
				add_node(&nodes, 1, 0, operations.data[op_len-- - 1]);
			operations.length = op_len;
			push_back(&operations, expression->data[i]);
		}
	}

	if (number != -1 || var_value != -1)
	{
		number = atof(num.data);
		if (var_value != -1)
		{
			for (int letter = 0; letter < var.length; ++letter)
				printf("%c", var.data[letter]);
			printf(" = ");
			scanf("%lf", &var_value);

			free(var.data);
			var.length = 0;
			number = var_value;
		}
		add_node(&nodes, 0, number, ' ');
		
		free(num.data);
		num.data = (char*)calloc(operations_capacity, sizeof(char));
	}

	if (operations.length != 0)
		for (int j = operations.length - 1; j > -1; --j)
			add_node(&nodes, 1, 0, operations.data[j]);

	return nodes;
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

char* get_string(int* len) 
{
	*len = 0;
	int capacity = 1;
	char* str = (char*)malloc(sizeof(char));

	char c = getchar();
	while (c != '\n') 
	{
		str[(*len)++] = c;

		if (*len >= capacity) 
		{
			capacity *= 2;
			str = (char*)realloc(str, capacity * sizeof(char));
		}
		c = getchar();
	}
	str[*len] = '\0';

	return str;
}

int main()
{
	int len;
	char* s = get_string(&len);

	string exp;
	exp.length = len;
	exp.data = s;

	printf("%lf\n", _CALC(exp));

	return 0;
}
