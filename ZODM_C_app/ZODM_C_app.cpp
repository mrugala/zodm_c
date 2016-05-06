// ZODM_C_app.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ZODM_C_app.h"
#include "../ZODM_C/matrix.h"
#include "../ZODM_C/zodm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			matrix_t mat1 = diag(3);
			matrix_t mat2 = { 3, 4, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
			matrix_t res = { 0 };
			print_matrix_error(multiply_matrices(&mat1, &mat2, &res));

			// TODO: code your application's behavior here.
			getchar();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
