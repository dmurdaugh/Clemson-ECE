#include <stdio.h>
#include <stdlib.h>

int main(){
	int i,j,n;
	time_t t;

	//int instructions[100];
	srand((unsigned) time(&t));

	for(i=0;i<30;i++){
		printf("INSERT\n");
		printf("%d\n",rand() % 4+100);
		printf("rand%d\n",rand()%300);
		printf("%d\n",rand() % 4);
		printf("rand%d\n",rand()%300);
		j=rand() % 5;
		printf("%d\n",j);
		for(n=0;n<j;n++){
			printf("%d\n",rand() % 6);
		}
		printf("%d\n",rand() % 400);
		printf("%d\n",rand() % 400);
		printf("%d\n",rand() % 3);
		printf("%.2f\n",2.22);
		printf("%d\n",rand() % 8);
	}
return 0;
}

