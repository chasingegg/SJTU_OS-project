#include<stdio.h>
#include<windows.h>
#include<stdlib.h>

int A[30][30];
int B[30][30];
int C[30][30];

HANDLE hthread[300];

int M;
int K;
int N;

struct v
{
	int vi;
	int vj;
};

DWORD WINAPI ThreadProc(LPVOID lpParameter)//创建线程时传递给新线程一个指针参数
{
	struct v *hCur = (struct v*)lpParameter;
	int x = hCur -> vi;
	int y = hCur -> vj;


	int res = 0, i;
	for(i = 0; i < K; ++i)
		res += A[x][i] * B[i][y];
	C[x][y] = res;
	
	return 0;
}

int main()
{
    int i, j;
	printf("input M K N:\n");
	scanf("%d %d %d", &M, &K, &N);
	printf("input matrix1:\n");
	for(i = 0; i < M; ++i)
	{
		for(j = 0; j < K; ++j)
			scanf("%d", &A[i][j]);
	}

	printf("\ninput matrix2:\n");
	for(i = 0; i < K; ++i)
	{
		for(j = 0 ; j < N; ++j)
			scanf("%d", &B[i][j]);
	}

	for(i = 0; i < M; ++i)
	{
		for(j = 0; j < N; ++j)
		{
			struct v*data = (struct v*)malloc(sizeof(struct v));
			data -> vi = i;
			data -> vj = j;
			hthread[i * N + j] = CreateThread(NULL, 0, ThreadProc, data, 0, NULL);
		}
	}
	    
    WaitForMultipleObjects(M*N, hthread, TRUE, INFINITE);//等待线程结束 
    
    for(i = 0; i < M * N; ++i)
    {
    	CloseHandle(hthread[i]);
    }
	printf("\nthe result:\n");

	for(i = 0; i < M; ++i)
	{
		for(j = 0; j < N; ++j)
			printf("%d ", C[i][j]);
		printf("\n");
	}
	return 0;
}
