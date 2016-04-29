#include "stdafx.h"
#include "matrix.h"
#include <stdio.h>

#define MATRIX_CELL(rows, i, j) ((rows)*((i))+((j)))

float* access_matrix_field(matrix_t* mat, int i, int j)
{
	if (i >= mat->a || j >= mat->b) return NULL;
	return &mat->data[MATRIX_CELL(mat->b, i, j)];
}

static int sub_add_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result, 
	float (*oper)(matrix_t*, matrix_t*, int, int))
{
	if (!mat1 || !mat2 || !result)
		return NULL_MATRIX_ERROR;

	if (mat1->a != mat2->a)
		return DIM_A_ERROR;

	if (mat1->b != mat2->b)
		return DIM_B_ERROR;

	result->a = mat1->a;
	result->b = mat1->b;

	for (int i = 0; i < mat1->a; ++i)
	{
		for (int j = 0; j < mat2->b; ++j)
		{
			float* tmp = access_matrix_field(result, i, j);
			*tmp = oper(mat1, mat2, i, j);
		}
	}

	return RETURN_OK;
}

static float subtract_cells(matrix_t* mat1, matrix_t* mat2, int i, int j)
{
	return (*access_matrix_field(mat1, i, j)) - (*access_matrix_field(mat2, i, j));
}

static float add_cells(matrix_t* mat1, matrix_t* mat2, int i, int j)
{
	return (*access_matrix_field(mat1, i, j)) + (*access_matrix_field(mat2, i, j));
}

int add_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result)
{
	float(*oper)(matrix_t*, matrix_t*, int, int) = add_cells;

	return sub_add_matrices(mat1, mat2, result, oper);
}

int subtract_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result)
{
	float(*oper)(matrix_t*, matrix_t*, int, int) = subtract_cells;

	return sub_add_matrices(mat1, mat2, result, oper);
}

int multiply_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result)
{
	if (!mat1 || !mat2 || !result)
		return NULL_MATRIX_ERROR;

	if (mat1->b != mat2->a)
		return DIM_ERROR_MULT;

	result->a = mat1->a;
	result->b = mat2->b;

	for (int i = 0; i < mat1->a; ++i)
	{
		for (int j = 0; j < mat2->b; ++j)
		{
			for (int k = 0; k < mat1->b; k++)
			{
				float* tmp = access_matrix_field(result, i, j);
				*tmp += (*access_matrix_field(mat1, i, k)) * (*access_matrix_field(mat2, k, j));
			}
		}
	}

	return RETURN_OK;
}

void print_matrix_error(int error_code)
{
	if (error_code < TOP)
	{
		printf("Matrix error: %s", matrix_error_str[error_code]);
	}
	else
	{
		printf("Invalid error code for matrix error");
	}
	
}