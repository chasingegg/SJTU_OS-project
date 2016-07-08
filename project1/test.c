#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>

int main()
{
    int temp;
    temp = syscall(314, 1);
    if(temp == 1)
		printf("success!\n");
    else
        printf("failed!\n");
    
    return 0;
}
