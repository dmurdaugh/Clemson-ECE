#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include "intpq.h"

#define M 10
#define N 10
#define INF 10000000

class state_info {			// This class contains a state of the graph. x and y are positions in the maze. 
  private:				    //  We insert an instance of this class along with the key into the priority     	
  long key;				    //  queue. When remove_min is called, we will know which state we are in.
  short x;
  short y;

  public:
  state_info() : key(-1), x(-1), y(-1) {}
  state_info(long k, short l, short m) : key(k), x(l), y(m) {}

  short getX() { return x; }
  short getY() { return y; }
};

unsigned int m, n;			// The dimensions of the maze. m rows and n columns.
short **maze;				// This contains the maze (got from input).
bool **visited;				// This tells us if a particular state has been visited during a Dijkstra computation.
long **indices;				// This contains a handle (index) into the binary heap for each state. This is 
					    //  required for performing decrease_key.
long **cost;				// This contains the cost labels for each state in the maze.

void update(void *st, long index);	// Forward reference to the function update.

IntPQ s(M * N, &update);		    // Creates an instance of integer priority queue. Apart from the size of the PQ, we also
					    // need to provide a user defined update function. This function is called whenever the
					    // PQ changes its internal state.

void update(void *st, long index) {	// This function is called whenever the PQ changes its state. Helps us keep a handle (index)
  state_info* sinfo = (state_info*) st;	    // on the PQ for each state. 
  indices[sinfo->getX()][sinfo->getY()] = index;
}

void dijkstra() {			// Perform the Dijkstra's shortest path computation.
state_info* info= NULL;
unsigned int cs, x, y, i, j, nx, ny, newc;
while( !s.is_empty()){
	s.print();
	info= (state_info*) s.remove_min();
	x=info -> getX();
	y=info -> getY();
	cs=cost[x][y];
	
	nx = x+1;
	ny= y;
	if (nx < m && ny < n){
		if( !visited[nx][ny]){
			if(indices[nx][ny] != -1){
				//cs=cs + cost[nx][ny];
				newc=cs + maze[nx][ny];
					if(newc < cost[nx][ny]){
						s.decrease_key(indices[nx][ny], newc);
						cost[nx][ny]=newc;
					}
			}
			else{
				cost[nx][ny]=cs + maze[nx][ny];
				
				indices[nx][ny]=s.insert(cost[nx][ny], new state_info (cost[nx][ny], nx, ny));
				}
		}
	}
		
	nx = x;
	ny= y+1;
	if (nx < m && ny < n){
		if( !visited[nx][ny]){
			if(indices[nx][ny] != -1){
				//cs=cs + cost[nx][ny];
				newc=cs + maze[nx][ny];
					if(newc < cost[nx][ny]){
						s.decrease_key(indices[nx][ny], newc);
						cost[nx][ny]=newc;
					}
			}
			else{
				cost[nx][ny]=cs + maze[nx][ny];
				indices[nx][ny]=s.insert(cost[nx][ny], new state_info (cost[nx][ny], nx, ny));
				}
		}
	}
	visited[x][y]=true;
}
}

int main() {
  // Read number of rows and columns.
  cin >> m >> n;
  if (m >= M || n >= N) {
    cout << "Enter input size < 1000" << endl;
    return 0;
  }
  // Initialize the maze and visited structure.
  maze = new short* [m];
  visited = new bool* [m];
  indices = new long* [m];
  cost = new long* [m];
  for (unsigned int i = 0; i < m; ++i) {
    maze[i] = new short[n];
    visited[i] = new bool[n];
    indices[i] = new long[n];
    cost[i] = new long[n];
  }

  // Read in the maze and initialize data structures.
  for (unsigned int i = 0; i < m; ++i)
    for (unsigned int j = 0; j < n; ++j) {
      cin >> maze[i][j];
      visited[i][j] = false;
      indices[i][j] = -1;
      cost[i][j] = INF;
    }

  // Insert start state into a priority queue and initialize initial parameters.
  cost[0][0] = maze[0][0];
  long pos = s.insert(cost[0][0], new state_info (cost[0][0], 0, 0));
  indices[0][0] = pos;
  dijkstra();

  cout << "The shortest distance is : " << cost[m-1][n-1] << endl;

  return 0;
}
