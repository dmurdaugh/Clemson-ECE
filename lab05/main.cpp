#include <iostream>
#include <stdlib.h>

using namespace std;

#include "intset.h"

#define SIZE	1010

int main()
	{
	    btree b;
	    int tmp;
	    int s;
	    int x = 1;
	    while (x!=0)
	    {
	    cout<<" Enter Number to be inserted : ";
	                    cin>>tmp;
	                    b.insert(tmp);
	    cout<<" Enter Number to be searched : ";
	                    cin>>s;
	                    b.search(s);
	                    if (s == NULL)
	                    {
	                        cout << "not found" << endl;
	                    }
	                    else
	                    {
	                        cout << "found" << endl;
	                    }   
	                    cout << "x? ";
	                    cin >> x;
	    }
	                    system ("pause");
	                    return 0;
	}
