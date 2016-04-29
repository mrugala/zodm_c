#pragma once

typedef struct
{
	int a;
	int b;
	float data[256];
} matrix_t;

#define ERROR_CODE(x) \
		x(RETURN_OK) \
		x(DIM_A_ERROR) \
		x(DIM_B_ERROR) \
		x(DIM_ERROR_MULT) \
		x(NULL_MATRIX_ERROR) \
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

int add_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
int multiply_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
float* access_matrix_field(matrix_t* mat, int i, int j);
void print_matrix_error(int error_code);