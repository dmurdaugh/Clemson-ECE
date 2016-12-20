// Denzel Murdaugh
// main.cpp
// 03/27/14


#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
using std::cin;
using std::cout;
using std::endl;
using std::left;
bool visited[1000][1000];
int last_a[1000][1000];
int last_b[1000][1000];
int A;
int B;
int C;
int action;
void visit(int a,int b,int la,int lb);
void print(int i,int j);

int main(){
	int graph[1000][1000];
	
	cout<< "Enter A:";
	cin>> A;
	cout<< "Enter B:";
	cin>> B;
	cout<< "Enter C:";
	cin>> C;
	
	if(A > 1000 || B> 1000){
		cout<< "Invalid input impossible"<< endl;
		return 0;
	}
	visit(0,0,0,0);
		for(int i=0;i<=A;i++){
			for(int j=0;j<=B;j++){
				if(i+j==C && visited[i][j]){
						print(i,j);
						return 0;
				}
			}
		}
		cout << "Impossible" <<endl;
return 0;
}

void visit(int a,int b,int la,int lb){
int found;	
	if ( a+b==C || visited[a][b]==true){
		if (a+b==C && found!=1){
		//print(a,b);
		found=1;
		visited[a][b]=true;
		}
		return;
	}
//	cout<< a <<" "<< b<< endl;
	
	visited[a][b]=true;
	last_a[a][b]=la;
	last_b[a][b]=lb;
	
	
	visit(A,b,a,b); //Fill A
	action=1;

	visit(a,B,a,b); //Fill B
	action=2;
	visit(0,b,a,b); //Empty A
	action=3;
	visit(a,0,a,b); //Empty B
	action=4;
		
	if(a + b > B){
		visit(a-(B-b),B,a,b);//pour a to b
		action=5;
	}
	else{
		visit(0,a+b,a,b);// else empty a into b
		action=5;
	}
	if(a + b > A){
		visit(A,b-(A-a),a,b);//pour b to a
		action=6;
	}
	else{
		visit(a+b,0,a,b);//else empty b into a
		action=6;
	}
//	switch (action){
//	case 1: 
//		cout<<"Filling A: "<< "[a="<< a << " b=" << b << "]" <<endl;
//		break;
//	case 2:
//	cout<<"Filling B: "<< "[a="<< a << " b=" << b << "]" <<endl;
//	break;
//	case 3:
//	cout<<"Emptying A: "<< "[a="<< a << " b=" << b << "]" <<endl;
//	break;
//	case 4:
//	cout<<"Emptying B: "<< "[a="<< a << " b=" << b << "]" <<endl;
//	break;
//	case 5:
//	cout<<"pouring a to b: "<< "[a="<< a << " b=" << b << "]" <<endl;
//	break;
//	case 6:
//	cout<<"pouring b to a: "<< "[a="<< a << " b=" << b << "]" <<endl;
//	}
	return;
}

void print(int a,int b){
if(a==0 && b==0)
return;

print(last_a[a][b],last_b[a][b]);
				cout << left << "[a="<< a << " b=" << b << "]" <<endl;
				return;
}
				
		
