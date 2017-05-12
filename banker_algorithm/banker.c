#include<stdio.h>

#define MAXPROCESS 50  //最大进程数
#define MAXRESOURCE 100 //最大资源数

int available[MAXRESOURCE]; //可用资源数组
int max[MAXPROCESS][MAXRESOURCE]; //最大需求矩阵
int allocation[MAXPROCESS][MAXRESOURCE]; //分配矩阵
int need[MAXPROCESS]; //需求矩阵
int request[MAXPROCESS][MAXRESOURCE]; //进程需要资源数
bool finish[MAXPROCESS]; //是否有足够的资源分配
int p[MAXPROCESS]; //记录序列

int m, n; //m个进程 n个资源

void Init()
{
    int i, j;
    printf("input the number of process: ");
    scanf("%d", &m);
    printf("input the number of resource: ");
    scanf("%d", &n);
    printf("input max[i][j]:\n");
    for(i = 0; i < m; ++i)
        for(j = 0; j < n; ++j)
            scanf("%d", &max[i][j]);
    printf("input allocation[i][j]:\n");
    for(i = 0; i < m; ++i)
        for(j = 0; j < n; ++j)
        {
            scanf("%d", &allocation[i][j]);
            need[i][j] = max[i][j] - allocation[i][j];
        }
    
    prinf("input available[i]:\n");
    for(i = 0; i < n; ++i)
        scanf("%d", available[i]);
}

void showstate(int m, int n)
{
    int i, j;
	
	printf("\n-------------------------------------------------------------\n";
	cout << "available:    ";
	for (j = 0; j < n; ++j)
		cout << "    " << available[j];
    printf("\n  need:\n");
	for (i = 0; i < m; ++i)
	{
		printf("    process %d:", i);
		for (j = 0; j < n; ++j)
		    printf("    %d", need[i][j]);
		printf("\n");
	}

	printf("\nallocation:    \n"); 
	for (i = 0; i < m; ++i)
	{
		printf("    process %d:", i);
		for (j = 0; j < n; ++j)
		    printf("    %d", allocation[i][j]);
		printf("\n");
	}
	printf("\n");
}
int main()
{
    Init();
    int cusneed; //发出请求 
    char again;
    while(1)
    {
        showdate(m, n);

        printf("input the process number need to request:\n");
		scanf("%d", &cusneed);
		
		printf("input request[i]:\n");
		for (i = 0; i < n; ++i)
		{
			cin >> request[cusneed][i];
		}
		
		for (i = 0; i < n; ++i) 
		{
			available[i] -= request[cusneed][i]; 
			allocation[cusneed][i] += request[cusneed][i];
			need[cusneed][i] -= request[cusneed][i]; 
		}
		if (Safe())
		{
			cout << "accepted" << endl;
		}
		else
		{
			cout << "refuesd!" << endl;
			for (i = 0; i<n; i++)
			{
				available[i] += request[cusneed][i];
				allocation[cusneed][i] -= request[cusneed][i];
				need[cusneed][i] += request[cusneed][i];
			}
		}
		for (i = 0; i < n; ++i)
		{
			if (need[cusneed][i] <= 0)
			{
				flag++;
			}
		}
		if (flag == n) 
		{
			for (i = 0; i<n; i++)
			{
				AVAILABLE[i] += ALLOCATION[cusneed][i];
				ALLOCATION[cusneed][i] = 0;
				NEED[cusneed][i] = 0;
			}
			cout << "process" << cusneed << " released" << endl;
			flag = 0;
		}
		for (i = 0; i < m; ++i)
		{
			finish[i] = false;
		}
		cout << "request again?yes press y/Y,otherwise press others" << endl;
		cin >> again;
		if (again == 'y' || again == 'Y')
		{
			continue;
		}
		break;
	}
    

    return 0;
}
