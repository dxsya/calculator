#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "String.h"
#include "Node_arr.h"

#define INVALID_INPUT 962957572.248576
#define ZERO_DIV 53587291.5352

typedef long double real;

real hash_table[32500];

void init_hash() 
{
	for (int i = 0; i < 32500; ++i)
		hash_table[i] = -34456.52474;
}

int hash(string str)
{
	int res = 0;
	for (int i = 0; i < str.length; ++i)
		res += str.data[i] % 23;
	return res;
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
		return (compare_op(operation, '*', '/', '^', '#'));
	else if (main == '/')
		return (compare_op(operation, '/', '*', '^', '#'));
	else if (main == '^')
		return 0;
}

void add_number_to_node(real number, real var_value, string* num, string* var, node_arr* nodes) 
{
	number = atof(num->data);
	if (var_value != -1)
	{
		real hash_num = hash_table[hash(*var)];
		if (hash_num == -34456.52474)
		{
			for (int letter = 0; letter < var->length; ++letter)
				printf("%c", var->data[letter]);
			printf(" = ");
			scanf("%lf", &var_value);
			hash_table[hash(*var)] = var_value;

			number = var_value;
		}
		else
			number = hash_num;
		free(var->data);
		var->length = 0;
	}
	add_node(nodes, 0, number, ' ');

	free(num->data);
	init(num);
}


node_arr translate_to_notation(string* expression)
{
	node_arr nodes;
	nodes.length = 0;
	nodes.capacity = 0;

	string operations, num, var;
	init(&operations);
	init(&num);
	init(&var);

	real var_value = -1;
	real number = -1;

	for (int i = 0; i < expression->length; ++i)
	{
		if (expression->data[i] >= '0' && expression->data[i] <= '9' || 
			expression->data[i] == '.')
		{
			if (number == -1)
				number = 0;
			push_back(&num, expression->data[i]);
			continue;
		}
		else if ((expression->data[i] >= 'A' && expression->data[i] <= 'Z') ||
				 (expression->data[i] >= 'a' && expression->data[i] <= 'z'))
		{
			if (var_value == -1)
				var_value = 0;
			push_back(&var, expression->data[i]);
			continue;
		}

		if (number != -1 || var_value != -1)
			add_number_to_node(number, var_value, &num, &var, &nodes);

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
		add_number_to_node(number, var_value, &num, &var, &nodes);


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
				return INVALID_INPUT;

			if (data.arr[i].operation == '+')
				res = data.arr[i - 2].number + data.arr[i - 1].number;
			else if (data.arr[i].operation == '-')
				res = data.arr[i - 2].number - data.arr[i - 1].number;
			else if (data.arr[i].operation == '*')
				res = data.arr[i - 2].number * data.arr[i - 1].number;
			else if (data.arr[i].operation == '/')
			{
				if (data.arr[i - 1].number == 0)
					return ZERO_DIV;
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
		return INVALID_INPUT;
	else if (data.arr[0].flag)
		return INVALID_INPUT;
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
	init_hash();

	int len;
	char* s = get_string(&len);

	string exp;
	exp.length = len;
	exp.data = s;

	real answer = _CALC(exp);
	if (answer == INVALID_INPUT)
		printf("Error: invalid input");
	else if (answer == ZERO_DIV)
		printf("Error: division by zero");
	else
		printf("%lf\n", answer);

	return 0;
}
