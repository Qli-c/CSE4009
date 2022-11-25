/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int arr1, arr2, arr3, arr4, arr5, arr6, arr7, arr8;
	
	if(M == 32)
	{
		for(int k = 0; k < N; k += 8)
		{
			for(int j = 0; j < M; j += 8)
			{
				for(int i = 0; i < 8; i++)
				{
					arr1 = A[k+i][j];
					arr2 = A[k+i][j+1];
					arr3 = A[k+i][j+2];
					arr4 = A[k+i][j+3];
					arr5 = A[k+i][j+4];
					arr6 = A[k+i][j+5];
					arr7 = A[k+i][j+6];
					arr8 = A[k+i][j+7];
					B[j][k+i] = arr1;					
					B[j+1][k+i] = arr2;					
					B[j+2][k+i] = arr3;					
					B[j+3][k+i] = arr4;					
					B[j+4][k+i] = arr5;					
					B[j+5][k+i] = arr6;					
					B[j+6][k+i] = arr7;					
					B[j+7][k+i] = arr8;			
				}
			}
		}
	}

	else
	{
		for(int k = 0; k < 64; k += 8)
		{
			for(int j = 0; j < 56; j += 8)
			{
				for(int i = 0; i < 8; i++)
				{
					arr1 = A[k+i][j];
					arr2 = A[k+i][j+1];
					arr3 = A[k+i][j+2];
					arr4 = A[k+i][j+3];
					arr5 = A[k+i][j+4];
					arr6 = A[k+i][j+5];
					arr7 = A[k+i][j+6];
					arr8 = A[k+i][j+7];
					B[j][k+i] = arr1;					
					B[j+1][k+i] = arr2;					
					B[j+2][k+i] = arr3;					
					B[j+3][k+i] = arr4;					
					B[j+4][k+i] = arr5;					
					B[j+5][k+i] = arr6;					
					B[j+6][k+i] = arr7;					
					B[j+7][k+i] = arr8;
				}
			}
		}
		for(int k = 0; k < 67; k++)
		{
			arr1 = A[k][56];
			arr2 = A[k][57];
			arr3 = A[k][58];
			arr4 = A[k][59];
			arr5 = A[k][60];
			B[56][k] = arr1;
			B[57][k] = arr2;
			B[58][k] = arr3;
			B[59][k] = arr4;
			B[60][k] = arr5;
		}
		for(int k = 0; k < 56; k += 8)
		{
			for(int j = 0; j < 3; j++)
			{
				arr1 = A[64+j][k];
				arr2 = A[64+j][k+1];
				arr3 = A[64+j][k+2];
				arr4 = A[64+j][k+3];
				arr5 = A[64+j][k+4];
				arr6 = A[64+j][k+5];
				arr7 = A[64+j][k+6];
				arr8 = A[64+j][k+7];
				B[k][64+j] = arr1;
				B[k+1][64+j] = arr2;
				B[k+2][64+j] = arr3;
				B[k+3][64+j] = arr4;
				B[k+4][64+j] = arr5;
				B[k+5][64+j] = arr6;
				B[k+6][64+j] = arr7;
				B[k+7][64+j] = arr8;
			}
		}
	}
}
					

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

