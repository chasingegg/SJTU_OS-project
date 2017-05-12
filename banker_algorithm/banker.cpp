#include <iostream>
using namespace std;

#define MAXPROCESS 50           
#define MAXRESOURCE 100                       
int AVAILABLE[MAXRESOURCE];             
int MAX[MAXPROCESS][MAXRESOURCE];            
int ALLOCATION[MAXPROCESS][MAXRESOURCE];    
int NEED[MAXPROCESS][MAXRESOURCE];            
int REQUEST[MAXPROCESS][MAXRESOURCE];       
bool FINISH[MAXPROCESS];                     
int p[MAXPROCESS];                            
int m, n;                                    
void Init();
bool Safe();
void Bank();
void showdata(int, int);
int main()
{
	Init();
	Safe();
	Bank();
}
void Init()         
{
	int i, j;
	cout << "please input the number of process:";
	cin >> m;
	cout << "Please input the number of resource:";
	cin >> n;
	cout << "please input max[i][j],follow the form" << m << "x" << n << endl;
	for (i = 0; i<m; i++)
		for (j = 0; j<n; j++)
			cin >> MAX[i][j];
	cout << "please input allocation[i][j],follow the form" << m << "x" << n <<  endl;
	for (i = 0; i<m; i++)
	{
		for (j = 0; j<n; j++)
		{
			cin >> ALLOCATION[i][j];
			NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];
			if (NEED[i][j]<0)
			{
				cout << "allocation[" << i + 1 << "][" << j + 1 << "] is wrong, input again:" << endl;
				j--;
				continue;
			}
		}
	}
	cout << "please input available[i]:" << endl;
	for (i = 0; i<n; i++)
	{
		cin >> AVAILABLE[i];
	}
}
void Bank()                
{
	int i, cusneed, flag = 0;
	char again;
	while (1)
	{
		showdata(n, m);  
		cout << endl;
	input:
		cout << "please input the process number need to request" << endl;
		cin >> cusneed;
		if (cusneed > m)
		{
			cout << "no such process, again" << endl;
			goto input;
		}
		cout << "please input request[i]" << endl;
		for (i = 0; i<n; i++)
		{
			cin >> REQUEST[cusneed][i];
		}
		for (i = 0; i<n; i++)
		{
			if (REQUEST[cusneed][i]>NEED[cusneed][i])
			{
				cout << "please do not bigger than need, again!" << endl;
				goto input;
			}
			if (REQUEST[cusneed][i]>AVAILABLE[i])
			{
				cout << "please do not bigger than available, again!" << endl;
				goto input;
			}
		}
		for (i = 0; i<n; i++) 
		{
			AVAILABLE[i] -= REQUEST[cusneed][i];
			ALLOCATION[cusneed][i] += REQUEST[cusneed][i];
			NEED[cusneed][i] -= REQUEST[cusneed][i];
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
				AVAILABLE[i] += REQUEST[cusneed][i];
				ALLOCATION[cusneed][i] -= REQUEST[cusneed][i];
				NEED[cusneed][i] += REQUEST[cusneed][i];
			}
		}
		for (i = 0; i<n; i++)
		{
			if (NEED[cusneed][i] <= 0)
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
		for (i = 0; i<m; i++)
		{
			FINISH[i] = false;
		}
		cout << "request again?yes press y/Y,otherwise press others" << endl;
		cin >> again;
		if (again == 'y' || again == 'Y')
		{
			continue;
		}
		break;
	}
}
bool Safe()                                    
{
	int i, j, k, l = 0;
	int Work[MAXRESOURCE];                    
	for (i = 0; i<n; i++)
		Work[i] = AVAILABLE[i];
	for (i = 0; i<m; i++)
	{
		FINISH[i] = false;  
	}
	for (i = 0; i<m; i++)
	{
		for (j = 0; j<n; j++)
		{
			if (NEED[i][j]>Work[j])
			{
				break;
			}
		}
		if (j == n && FINISH[i]==false)
		{
			FINISH[i] = true;
			for (k = 0; k<n; k++)
			{
				Work[k] += ALLOCATION[i][k];
			}
			p[l++] = i;
			i = -1;
		}
		else 
		{
			continue;
		}
		if (l == m)  
		{
			cout << "safe" << endl;
			cout << "quene:" << endl;
			for (i = 0; i<l; i++)  
			{
				cout << p[i];
				if (i != l - 1)
				{
					cout << "-->";
				}
			}
			cout << "" << endl;
			return true;
		}
	} 
	cout << "unsafe" << endl;
	return false;
}

void showdata(int n, int m)  
{
	int i, j;
	cout << endl;
	cout << "-------------------------------------------------------------" << endl;
	cout << "available:    ";
	for (j = 0; j<n; j++)
		cout << "    " << AVAILABLE[j];
	cout << endl;
	cout << "need:" << endl;
	for (i = 0; i<m; i++)
	{
		cout << "    process" << i << ":";

		for (j = 0; j<n; j++)
			cout << "     " << NEED[i][j];
		cout << endl;
	}

	cout << endl;
	cout << "allocation:    " << endl << endl;

	for (i = 0; i<m; i++)
	{
		cout << "    process" << i << ":";

		for (j = 0; j<n; j++)
			cout << "     " << ALLOCATION[i][j];
		cout << endl;
	}
	cout << endl;
}   