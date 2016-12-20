//Denzel Murdaugh
//Scheduling_SRTN.c
//

#include <stdio.h>
#include <stdlib.h>


int main(){
	int Queue[26][2];
	int i=0;
	int count=0;
	int k;
	char c1='A';
	int cpu_flag=0;
	int cpu_job[2]={0,0};
	int none=0;
	int flag=0;
	int next[2]={-1,100000};
	while(scanf("%d %d",&Queue[i][0],&Queue[i][1])==2){
		count++;
		i++;
	}
	//i=0;j=1;
/*	for(k=0;k<count;k++){*/
/*		printf("%d %d\n",Queue[i],Queue[j]);*/
/*		i+=2;*/
/*		j+=2;*/
/*	}*/
	int info[count][3];
	int time=0;
	int ready[count][1];
	for(i=0;i<count;i++){
		ready[i][0]=-1;
		info[i][1]=0;
	}
	printf("time\tcpu\tready queue (pid/rst)\n");
	printf("----\t---\t---------------------\n");
	while(flag==0){
		for(i=0;i<count;i++){
			if(Queue[i][0]==time){
				if(cpu_flag==0){//run job
					cpu_flag=1;
					cpu_job[0]=i;
					cpu_job[1]=Queue[i][1];
					info[i][2]=time;
				}
				else{//put job on ready Queue
					if(cpu_job[1]>Queue[i][1]){//Check for SRT
						ready[cpu_job[0]][0]=cpu_job[1];
						cpu_flag=1;
						info[cpu_job[0]][1]=Queue[cpu_job[0]][1]-cpu_job[1];
						cpu_job[0]=i;
						cpu_job[1]=Queue[i][1];
						info[i][2]=time;
						
					}
					else
					ready[i][0]=Queue[i][1];
				}
			}
		}
		printf("  %d\t",time);
		if(cpu_flag==1){
			printf("%c%d\t",c1+cpu_job[0],cpu_job[1]);
			cpu_job[1]--;
		}
		none=1;
		for(k=0;k<count;k++){
			if(ready[k][0] > 0){
				printf("%c%d, ",c1+k,ready[k][0]);
				none=0;
			}
		}
		if(none==1){
			printf("--");
		}
		printf("\n");
		if(cpu_job[1]==0){//When job is done look to place another on CPU
			cpu_flag=0;
			next[1]=1000000;
			info[cpu_job[0]][0]=time+1;
			if(none==0){
				for(k=0;k<count;k++){
					if(ready[k][0]>0 && ready[k][0]<next[1]){
						next[0]=k;
						next[1]=ready[k][0];
					}
				}
				info[next[0]][2]=time+1;
				cpu_flag=1;
				cpu_job[0]=next[0];
				cpu_job[1]=next[1];
				ready[next[0]][0]=-1;
			}
		}
		if(none!=0 && cpu_flag==0){
			flag=1;
			i=0;
			while(i<count){
				if(Queue[i][0]>time){
					flag=0;
					break;
				}
				i++;
			}
		}
		time++;
	}
	printf("     arrival service departure turnaround wait\n");
	printf("pid  time    time     time      time     time\n");
	printf("---  ------- ------- --------- ---------- ----\n");
	for(i=0;i<count;i++){
		printf(" %c      %2d       %2d      %2d      %2d       %2d\n",c1+i,Queue[i][0],Queue[i][1],info[i][0],info[i][0]-Queue[i][0],info[i][2]-Queue[i][0]-info[i][1]);
	}
	printf("service wait\n");
	printf(" time   time\n");
	printf("------- -----\n");
	flag=0;
	while(flag<count){
		next[1]=100000;
	for(i=0;i<count;i++){			
		if(Queue[i][1] > 0 && Queue[i][1] < next[1]){
			next[0]=i;
			next[1]=Queue[i][1];
		}
	}
	flag++;
	printf("  %d     %d\n",next[1],info[next[0]][2]-Queue[next[0]][0]-info[next[0]][1]);
	Queue[next[0]][1]=-1;
	}
	
 return 0;
}


