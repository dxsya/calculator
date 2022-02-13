#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "String.h"
#include "Node_arr.h"

typedef long double real;

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

	real answer = _CALC(exp);
	if (answer == 962957572.248576)
		printf("Error: invalid input");
	else if (answer == 53587291.5352)
		printf("Error: division by zero");
	else
		printf("%lf\n", answer);

	return 0;
}
