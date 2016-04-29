#include "stdafx.h"
#include "matrix.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZODM_C
{		
	TEST_CLASS(MatrixTest1)
	{
	private:
		static matrix_t matrix3x3()
		{
			matrix_t matrix = { 3, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9 } };
			return matrix;
		}

		static matrix_t matrix3x3_of_ones()
		{
			matrix_t matrix = { 3, 3, { 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
			return matrix;
		}

		static matrix_t matrix3x4()
		{
			matrix_t matrix = { 3, 4, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
			return matrix;
		}

		static matrix_t matrix4x3()
		{
			matrix_t matrix = { 4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
			return matrix;
		}

		static matrix_t matrix3x5()
		{
			matrix_t matrix = { 3, 5, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } };
			return matrix;
		}

	public:
		TEST_METHOD(Matrix3x3IsCreatedCorrectly)
		{
			matrix_t mat = matrix3x3();
			Assert::AreEqual(3, mat.a);
			Assert::AreEqual(3, mat.b);

			float val = 0;
			for (int i(0); i < 9; i++)
			{
				val++;
				Assert::AreEqual(val, *access_matrix_field(&mat, i / 3, i % 3));
			}
		}

		TEST_METHOD(CanModifyMatrixFields)
		{
			matrix_t mat = matrix3x3();

			Assert::AreEqual((float)4.0, *access_matrix_field(&mat, 1, 0));
			float* tmp = access_matrix_field(&mat, 1, 0);
			*tmp = 5;
			Assert::AreEqual((float)5.0, *access_matrix_field(&mat, 1, 0));
		}

		TEST_METHOD(CanAddMatrices)
		{
			matrix_t mat1 = matrix3x3();
			matrix_t mat2 = matrix3x3_of_ones();
			matrix_t result = { 0 };

			Assert::AreEqual(0, add_matrices(&mat1, &mat2, &result));

			float val = 1;
			for (int i(0); i < 9; i++)
			{
				val++;
				Assert::AreEqual(val, *access_matrix_field(&result, i / 3, i % 3));
			}
		}

		TEST_METHOD(CantAddMatricesWithDifferentSize)
		{
			matrix_t mat1 = matrix3x3();
			matrix_t mat2 = matrix3x4();
			matrix_t result = { 0 };

			Assert::AreEqual((int)DIM_B_ERROR, add_matrices(&mat1, &mat2, &result));
		}

		TEST_METHOD(CantMultiplyMatricesWithDifferentInnerDimmensions)
		{
			matrix_t mat1 = matrix3x4();
			matrix_t mat2 = matrix3x3();
			matrix_t result = { 0 };

			Assert::AreEqual((int)DIM_ERROR_MULT, multiply_matrices(&mat1, &mat2, &result));
		}

		TEST_METHOD(CanMultiplyMatricesOfDifferentSizes)
		{
			matrix_t mat1 = matrix4x3();
			matrix_t mat2 = matrix3x5();
			matrix_t result = { 0 };

			Assert::AreEqual(0, multiply_matrices(&mat1, &mat2, &result));
			
			Assert::AreEqual(4, result.a);
			Assert::AreEqual(5, result.b);

			float val_array[] = { 46, 52, 58, 64, 70,
								  100, 115, 130, 145, 160,
								  154, 178, 202, 226, 250,
								  208, 241, 274, 307, 340 };
		
			for (int i(0); i < 20; i++)
			{
				Assert::AreEqual(val_array[i], *access_matrix_field(&result, i / 5, i % 5));
			}
		}

	};
}