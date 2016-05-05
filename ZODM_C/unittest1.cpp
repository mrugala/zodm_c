#include "stdafx.h"
#include "CppUnitTest.h"

namespace Microsoft {namespace VisualStudio {namespace CppUnitTestFramework	{
	template<> static std::wstring ToString<matrix_error_t>(const matrix_error_t& t) { return ToString((int)t); }
	template<> static std::wstring ToString<zodm_error_t>(const zodm_error_t& t)     { return ToString((int)t); }
}}}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZODM_C
{		
	class Matrices
	{
	protected:
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
	};

	TEST_CLASS(MatrixBasicTest), public Matrices
	{
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
				Assert::AreEqual(val, *access_matrix_cell(&mat, i / 3, i % 3));
			}
		}

		TEST_METHOD(CanModifyMatrixFields)
		{
			matrix_t mat = matrix3x3();

			Assert::AreEqual(4.0f, *access_matrix_cell(&mat, 1, 0));
			float* tmp = access_matrix_cell(&mat, 1, 0);
			*tmp = 5;
			Assert::AreEqual(5.0f, *access_matrix_cell(&mat, 1, 0));
		}

		TEST_METHOD(CantAccessCellOutsideMatrixBoundaries)
		{
			matrix_t mat = matrix3x3();

			Assert::AreEqual((float*)NULL, access_matrix_cell(&mat, 4, 0));
		}
	};

	TEST_CLASS(MatrixAddSubTest), public Matrices
	{
	public:
		TEST_METHOD(CanAddMatrices)
		{
			matrix_t mat1 = matrix3x3();
			matrix_t mat2 = matrix3x3_of_ones();
			matrix_t result = { 0 };

			Assert::AreEqual(RETURN_OK, add_matrices(&mat1, &mat2, &result));

			float val = 1;
			for (int i(0); i < 9; i++)
			{
				val++;
				Assert::AreEqual(val, *access_matrix_cell(&result, i / 3, i % 3));
			}
		}

		TEST_METHOD(CanSubtractMatrices)
		{
			matrix_t mat1 = matrix3x3();
			matrix_t mat2 = matrix3x3_of_ones();
			matrix_t result = { 0 };

			Assert::AreEqual(RETURN_OK, subtract_matrices(&mat1, &mat2, &result));

			for (int i(0); i < 9; i++)
			{
				Assert::AreEqual((float)i, *access_matrix_cell(&result, i / 3, i % 3));
			}
		}

		TEST_METHOD(CantAddMatricesWithDifferentSize)
		{
			matrix_t mat1 = matrix3x3();
			matrix_t mat2 = matrix3x4();
			matrix_t result = { 0 };

			Assert::AreEqual(DIM_B_ERROR, add_matrices(&mat1, &mat2, &result));
		}
	};

	TEST_CLASS(MatrixMultDivTest), public Matrices
	{
	public:
		TEST_METHOD(CanMultiplyAndDivideMatrixByFactor)
		{
			matrix_t mat = matrix3x3();
			matrix_t mult = { 0 }, div = { 0 };
			float factor = 3.0f;

			Assert::AreEqual(RETURN_OK, multiply_matrix(&mat, factor, &mult));
			Assert::AreEqual(RETURN_OK, divide_matrix(&mat, factor, &div));
			for (int i(0); i < 3; i++)
			{
				for (int j(0); j < 3; j++)
				{
					Assert::AreEqual(*access_matrix_cell(&mat, i, j) * factor,
						*access_matrix_cell(&mult, i, j), 0.0001f);
					Assert::AreEqual(*access_matrix_cell(&mat, i, j) / factor,
						*access_matrix_cell(&div, i, j), 0.0001f);
				}
			}
		}

		TEST_METHOD(CantMultiplyMatricesWithDifferentInnerDimmensions)
		{
			matrix_t mat1 = matrix3x4();
			matrix_t mat2 = matrix3x3();
			matrix_t result = { 0 };

			Assert::AreEqual(DIM_ERROR_MULT, multiply_matrices(&mat1, &mat2, &result));
		}

		TEST_METHOD(CanMultiplyMatricesOfDifferentSizes)
		{
			matrix_t mat1 = matrix4x3();
			matrix_t mat2 = matrix3x5();
			matrix_t result = { 0 };

			Assert::AreEqual(RETURN_OK, multiply_matrices(&mat1, &mat2, &result));

			Assert::AreEqual(4, result.a);
			Assert::AreEqual(5, result.b);

			float val_array[] = { 46, 52, 58, 64, 70,
				100, 115, 130, 145, 160,
				154, 178, 202, 226, 250,
				208, 241, 274, 307, 340 };

			for (int i(0); i < 20; i++)
			{
				Assert::AreEqual(val_array[i], *access_matrix_cell(&result, i / 5, i % 5));
			}
		}
	};

	TEST_CLASS(MatrixInversionTest), public Matrices
	{
	public:
		TEST_METHOD(CanTransposeMatrix)
		{
			matrix_t mat = matrix4x3();
			matrix_t result = { 0 };

			Assert::AreEqual(RETURN_OK, transpose_matrix(&mat, &result));

			Assert::AreEqual(3, result.a);
			Assert::AreEqual(4, result.b);

			for (int i(0); i < 3; i++)
			{
				for (int j(0); j < 4; j++)
				{
					Assert::AreEqual(*access_matrix_cell(&mat, j, i), *access_matrix_cell(&result, i, j));
				}
			}
		}

		/*
		Tests 1x1, 2x2 and 3x3 matrices only.
		*/
		TEST_METHOD(CanGetDeterminantsOfSquareMatrices)
		{
			float det;

			matrix_t mat = { 1, 1, { 7.0f } };
			Assert::AreEqual(RETURN_OK, get_determinant(&mat, &det));
			Assert::AreEqual(7.0f, det);

			mat = { 2, 2, { 7.0f, 5.0f, 
							-3.5f, 2.3f } };
			Assert::AreEqual(RETURN_OK, get_determinant(&mat, &det));
			Assert::AreEqual(7.0f*2.3f - 5.0f*-3.5f, det);

			mat = { 3, 3, { 7.0f, 5.0f, -3.5f,
							2.3f, 4.0f, -2.2f,
							4.4f, -5.1f, 3.1f } };
			Assert::AreEqual(RETURN_OK, get_determinant(&mat, &det));
			Assert::AreEqual(7.0f*(4.0f*3.1f - -2.2f*-5.1f) + 5.0f*(-2.2f*4.4f - 2.3f*3.1f)
				+ -3.5f*(2.3f*-5.1f - 4.0f*4.4f), det);
		}

		TEST_METHOD(CantGetDeterminantOfNotSquareMatrix)
		{
			matrix_t mat = { 2, 1, { 7.0f, 5.0f } };
			float det;
			Assert::AreEqual(GENERAL_DIM_ERROR, get_determinant(&mat, &det));
		}

		TEST_METHOD(CantInvertMatrixWithDetEqualTo0)
		{
			matrix_t mat = matrix3x3(), result = { 0 };
			float det;

			Assert::AreEqual(RETURN_OK, get_determinant(&mat, &det));
			Assert::AreEqual(.0f, det, .0001f);
			Assert::AreEqual(DET_ERROR, get_inverted_matrix(&mat, &result));
		}
		
		/*
		Verifies determinant, inverted matrix and if initial matrix and inverted matrix 
		give identity matrix after multiplication.
		*/
		TEST_METHOD(CanDoMatrixInversion)
		{
			matrix_t mat = { 3, 3, {3, 4, 1, 0, 3, 0, 1, 0, 1} };
			matrix_t result = { 0 }, identity = { 0 };
			float det;

			Assert::AreEqual(RETURN_OK, get_determinant(&mat, &det));
			Assert::AreEqual(6.0f, det, .0001f);
			Assert::AreEqual(RETURN_OK, get_inverted_matrix(&mat, &result));
			Assert::AreEqual(RETURN_OK, multiply_matrices(&mat, &result, &identity));

			matrix_t verify = { 3, 3, { .5f, -.6667f, -.5f, 0, .3333f, 0, -.5f, .6667f, 1.5f } };
			matrix_t ver_identity = diag(3);/*{ 3, 3, { 1, 0, 0, 0, 1, 0, 0, 0, 1 } };*/

			for (int i(0); i < 3; i++)
			{
				for (int j(0); j < 3; j++)
				{
					Assert::AreEqual(*access_matrix_cell(&verify, i, j), 
						*access_matrix_cell(&result, i, j), 0.0001f);
					Assert::AreEqual(*access_matrix_cell(&ver_identity, i, j),
						*access_matrix_cell(&identity, i, j), 0.0001f);
				}
			}
		}
	};

	TEST_CLASS(RotationTest)
	{
	public:
		TEST_METHOD(CanAssertFloatEquality)
		{
			Assert::IsTrue(assert_equal_f(5.01f, 5.001f, 0.1f));
			Assert::IsTrue(assert_equal_f(5.1f, 5.001f, 0.1f));
			Assert::IsFalse(assert_equal_f(5.11f, 5.001f, 0.1f));
			Assert::IsTrue(assert_equal_f(5.0001f, 5.00001f, 0.0001f));
		}

		TEST_METHOD(CanCalculateRotationOfBasicAgles)
		{
			matrix_t rot = { 0 };
			Assert::AreEqual(RETURN_OK, get_rotation_matrix(90, 90, 90, &rot));

			Assert::AreEqual(3, rot.a);
			Assert::AreEqual(3, rot.b);

			float epsilon = 0.0001f;

			float val_array[] = { 0, 1, 0,
								  1, 0, 0,
								  0, 0, -1 };

			for (int i(0); i < 9; i++)
			{
				Assert::AreEqual(val_array[i], *access_matrix_cell(&rot, i / 3, i % 3), epsilon);
			}
		}

		TEST_METHOD(CanCalculateRotationOfAglesUpTo180)
		{
			matrix_t rot = { 0 };
			Assert::AreEqual(ANGLE_ERROR, get_rotation_matrix(185, -160, 86, &rot));
			Assert::AreEqual(RETURN_OK, get_rotation_matrix(179, -160, 86, &rot));
			
			Assert::AreEqual(3, rot.a);
			Assert::AreEqual(3, rot.b);

			float epsilon = 0.001f;

			/*
			Values taken from calculations. Therefore this test case serves only sanity test functionality.
			*/
			float val_array[] = { -.939f, -.006f, .342f,
								  -.006f, .999f, .001f,
								  -.342f, -.001f, -.940f };

			for (int i(0); i < 9; i++)
			{
				Assert::AreEqual(val_array[i], *access_matrix_cell(&rot, i / 3, i % 3), epsilon);
			}
		}
	};

	TEST_CLASS(BasicTest)
	{
	public:
		TEST_METHOD(CanAssertFloatEquality)
		{
			Assert::IsTrue(assert_equal_f(5.01f, 5.001f, 0.1f));
			Assert::IsTrue(assert_equal_f(5.1f, 5.001f, 0.1f));
			Assert::IsFalse(assert_equal_f(5.11f, 5.001f, 0.1f));
			Assert::IsTrue(assert_equal_f(5.0001f, 5.00001f, 0.0001f));
		}

		TEST_METHOD(CanConvertEnumToString)
		{
			char str[64];
			Assert::IsTrue(!matrix_error_to_str(ROTATION_ERROR, str, 64));
			Assert::AreEqual("ROTATION_ERROR", str);
			Assert::IsTrue(!matrix_error_to_str(RETURN_OK, str, 64));
			Assert::AreEqual("RETURN_OK", str);
			Assert::IsFalse(!matrix_error_to_str(75, str, 64));
		}
	};
}