#pragma once

#include "matrix.h"
#include "stdbool.h"

typedef struct
{
	float x_mm;
	float y_mm;
	float z_mm;
	float alpha_deg;
	float beta_deg;
	float gamma_deg;
} input_data_t;

int calculate_zodm_5(input_data_t* input, float q_vect[/*static*/ 5]);

bool assert_equal_f(float a, float b, float epsilon);