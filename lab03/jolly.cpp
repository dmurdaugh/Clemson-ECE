// jolly.cpp
// Denzel Murdaugh
// CPSC 2120 Lab03
// 2/6/2014
// 
#include <iostream>
#include <cmath>
#include <cstdio>
 
using namespace std;
 
int array[3000];
 
int diff[2999];
 
int main()
{
	int n;
	while( !(cin >> n).eof() )
	{
		int i;
		for( i = 0; i < n ; i++ )
		{
			cin >> array[i];	
		}
		for( i = 1; i < n; i++ )
			diff[i] = 0;
		for( i = 0; i < n-1; i++ )
		{
		int d = abs( array[i]-array[i+1] );
		if( d < 1 || d > n-1 || diff[d] == 1 )
		{
			cout << "Not jolly" << endl;
			break;
		}
		diff[d] = 1;
	}
	if( i == n-1 )
	{
		cout << "Jolly" <<endl;
	}
	}
return 0;
}
