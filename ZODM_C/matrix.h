#pragma once

typedef struct
{
	int a;
	int b;
	float data[256];
} matrix_t;

int add_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
int subtract_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
int multiply_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
float* access_matrix_cell(matrix_t* mat, int i, int j);
void print_matrix_error(int error_code);

int get_rotation_matrix(float alpha_deg, float beta_deg, float gamma_deg, matrix_t* result);
