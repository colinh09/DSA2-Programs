#include <fstream>
#include <sstream>
#include <list>
#include <iterator>
#include "heap.h"
#include <climits>
using namespace std;

class graph 
{
public:
    //load graph and all info associated with the various edges and verticies
    void loadGraph(string infile);
	//checks if the vertex exists in the input file
	bool validVertex(string v);
    //dijsktra's algorithm to solve single source shortest path problem
    void dijkstra(string start);
    //print to an output file all of the verticies in their original order 
    //and the shortest path from starting vertex
    void outputPaths(string outfile);   

private:
    //a hash table to efficiently find items based on their ids
    hashTable vertices;
    //record all verticies that have been visited already in the original order of input graph file.
    list<string> visited;
    //size of adjancey list
    int size = 0;
    //store information associated with different graph edges (starting and ending verticies and respective costs)
    struct edge 
    {
        string startingV;
        string endingV;
        int cost;
        edge(string startVertex, string endVertex, int w) : startingV(startVertex), endingV(endVertex), cost(w) {}
    };

    //store information associated with different graph verticies
    struct vertex 
    {
        string id;
        list<edge> adj;
        bool known = false;
        //initializing dv value to be "infinity"
        int dv = INT_MAX;
        //preceeding vertex on the current shortest path
        string path = "";
        vertex(string s) : id(s) {}
    }; 
};