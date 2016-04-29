#include "stdafx.h"
#include "matrix.h"
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define ERROR_CODE(x) \
		x(RETURN_OK) \
		x(DIM_A_ERROR) \
		x(DIM_B_ERROR) \
		x(DIM_ERROR_MULT) \
		x(NULL_MATRIX_ERROR) \
		x(ROTATION_ERROR) \
		x(TOP) \

#define C(x) x,

typedef enum
{
	ERROR_CODE(C)
} matrix_error_t;

#undef C
#define C(x) #x,

static const char* matrix_error_str[] = {
	ERROR_CODE(C)
};

#undef C

#define MATRIX_CELL(rows, i, j) ((rows)*((i))+((j)))

float* access_matrix_cell(matrix_t* mat, int i, int j)
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

	int i, j;
	for (i = 0; i < mat1->a; ++i)
	{
		for (j = 0; j < mat2->b; ++j)
		{
			float* tmp = access_matrix_cell(result, i, j);
			*tmp = oper(mat1, mat2, i, j);
		}
	}

	return RETURN_OK;
}

static float subtract_cells(matrix_t* mat1, matrix_t* mat2, int i, int j)
{
	return (*access_matrix_cell(mat1, i, j)) - (*access_matrix_cell(mat2, i, j));
}

static float add_cells(matrix_t* mat1, matrix_t* mat2, int i, int j)
{
	return (*access_matrix_cell(mat1, i, j)) + (*access_matrix_cell(mat2, i, j));
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

	int i, j, k;
	for (i = 0; i < mat1->a; ++i)
	{
		for (j = 0; j < mat2->b; ++j)
		{
			for (k = 0; k < mat1->b; k++)
			{
				float* tmp = access_matrix_cell(result, i, j);
				*tmp += (*access_matrix_cell(mat1, i, k)) * (*access_matrix_cell(mat2, k, j));
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

int get_rotation_matrix(float alpha_deg, float beta_deg, float gamma_deg, matrix_t* result)
{
	if (!result)
		return NULL_MATRIX_ERROR;

	float alpha = (float)M_PI / 180 * alpha_deg;
	float beta = (float)M_PI / 180 * beta_deg;
	float gamma = (float)M_PI / 180 * gamma_deg;

	matrix_t r = { 0 };
	matrix_t rot_x = { 3, 3, { 1, 0, 0, 0, cosf(alpha), -sinf(alpha), 0, sinf(alpha), cosf(alpha) } };
	matrix_t rot_y = { 3, 3, { cosf(beta), 0, sinf(beta), 0, 1, 0, -sinf(beta), 0, cosf(beta) } };
	matrix_t rot_z = { 3, 3, { cosf(gamma), -sinf(gamma), 0, sinf(gamma), cosf(gamma), 0, 0, 0, 1 } };

	if (multiply_matrices(&rot_x, &rot_y, &r) || multiply_matrices(&r, &rot_x, result))
		return ROTATION_ERROR;

	return RETURN_OK;
}