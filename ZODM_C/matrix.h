#ifndef __MATRIX_H__
#define __MATRIX_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int a;
	int b;
	float data[256];
} matrix_t;

#define MATRIX_ERROR_CODE(x) \
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
	MATRIX_ERROR_CODE(C)
} matrix_error_t;

#undef C

matrix_error_t add_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
matrix_error_t subtract_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
matrix_error_t multiply_matrices(matrix_t* mat1, matrix_t* mat2, matrix_t* result);
float* access_matrix_cell(matrix_t* mat, int i, int j);
void print_matrix_error(int error_code);

matrix_error_t get_rotation_matrix(float alpha_deg, float beta_deg, float gamma_deg, matrix_t* result);

#ifdef __cplusplus
}
#endif

#endif /*__MATRIX_H__*/