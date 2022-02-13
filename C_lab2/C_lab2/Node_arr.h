#pragma once

#include "Node.h"

typedef struct Node_arr
{
	node* arr;
	int length;
	int capacity;
} node_arr;

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

void add_node(node_arr* nodes, int flag, long double number, char operation)
{
	if (nodes->capacity == 0)
		nodes->arr = (node*)calloc(++nodes->capacity, sizeof(node));

	if (nodes->length >= nodes->capacity)
		nodes->arr = (node*)realloc(nodes->arr, ++nodes->capacity * sizeof(node));

	nodes->arr[nodes->length].flag = flag;
	nodes->arr[nodes->length].operation = operation;
	nodes->arr[nodes->length++].number = number;
}
