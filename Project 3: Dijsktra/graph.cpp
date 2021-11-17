#include "graph.h"
using namespace std;

//load input file and all info associated with the verticies
void graph::loadGraph(string infile) 
{
	string txtLine, startingV, endingV;
	int cost;
	vertex *pv;
	ifstream input(infile);
	//store info about each graph edge given in the input txt file.
	while (getline(input, txtLine)) 
	{
		//get starting vertex, ending vertex and the associated cost
		stringstream ss(txtLine);
		ss >> startingV >> endingV >> cost;
		//store info into edge
		edge e = edge(startingV, endingV, cost);
		//if the starting vertex hadn't been visited yet...
		if (!vertices.contains(startingV)) 
		{
			pv = (vertex *)(new vertex(startingV));
			vertices.insert(startingV, pv);
			visited.push_back(startingV);
			size++;
		}
		//get pointer to starting vertex
		else 
		{ 
			pv = ((vertex *)vertices.getPointer(startingV)); 
		}
		//add edge to adjancey list
		pv->adj.push_back(e);
		//check if the ending vertex exists
		//if not, add to adjancey list
		if (!vertices.contains(endingV)) 
		{
			pv = (vertex *)(new vertex(endingV));
			vertices.insert(endingV, pv);
			visited.push_back(endingV);
			size++;
		}
	}
}

//check if the vertex exists using hash's contain function
bool graph::validVertex(string v) 
{
	return vertices.contains(v);
}

//dijskta's algorithm to solve the singe source shortest path problem
void graph::dijkstra(string start) 
{
	vertex *pv, *pstart, *pend;
	//get starting vertex's pointer
	pv = (vertex *) vertices.getPointer(start);
	pv->dv = 0;
	pv->known = true;
	pv->path = start;
	//each edge leaving the starting vertex is added to heap
	heap graphHeap = heap(size);
	list<edge>::iterator it;
	for (it = pv->adj.begin(); it != pv->adj.end(); it++)
	{
		//insert vertex into the heap if possible
		if (graphHeap.insert(it->endingV, it->cost) == 0) 
		{
			pend = (vertex *) vertices.getPointer(it->endingV);
			pend->dv = it->cost;
			pend->path = (pv->path) + ", " + it->endingV;
		}
		//if inserting the vertex into the heap is not possible, it must already exist with dv value and path
		//compare dv value and path, if the cost is better (lower), update values
		else 
		{
			pend = (vertex *) vertices.getPointer(it->endingV);
			if (it->cost < pend->dv) 
			{
				pend->dv = it->cost;
				pend->path = (pv->path) + ", " + it->endingV;
			}
			graphHeap.setKey(it->endingV, pend->dv);
		}        
	}
	string id; //id of the vertex with the smallest dv value
	int dv; //shortest distance so far from starting vertex to current vertex
	int newVertex; //distances for vertices branching from current vertex
	
	while (!graphHeap.deleteMin(&id, &dv)) 
	{
		pv = (vertex *) vertices.getPointer(id);
		//we know that the current dv value MUST be the shortest path length from the starting vertex
		pv->known = true;
		//add edges branching out from current vertex to the heap
		for (it = pv->adj.begin(); it != pv->adj.end(); it++) 
		{		
			//check if we already know the shortest path length
			pstart = (vertex *) vertices.getPointer(it->startingV);
			pend = (vertex *) vertices.getPointer(it->endingV);
			if (pend->known == true) 
			{
				continue;
			}
			//verties branching from current vertex path cost = pervious vertex cost + edge cost
			newVertex = dv + it->cost;		
		 	//insert vertex into the heap if possible
			if ( graphHeap.insert(it->endingV, newVertex) == 0 ) 
			{
				pend = (vertex *) vertices.getPointer(it->endingV);
				pend->dv = newVertex;
				pend->path = (pstart->path) + ", " + it->endingV;
			}
			//if inserting the vertex into the heap is not possible, it must already exist with dv value and path
			//compare dv value and path, if the cost is better (lower), update values
			else 
			{
				pend = (vertex *) vertices.getPointer(it->endingV);
				if (newVertex < pend->dv) 
				{
					pend->dv = newVertex;
					pend->path = (pstart->path) + ", " + it->endingV;
				}
				graphHeap.setKey(it->endingV, pend->dv);
			}
		}
	}
}

//print shortest path from starting vertex in original order
void graph::outputPaths(string outputFile) 
{	
	vertex *pv;
	list<string>::iterator it;
	ofstream output(outputFile);	
	//print to an output file specified by user
	for (it = visited.begin(); it != visited.end(); it++) 
	{
		pv = (vertex *) vertices.getPointer(*it);
		//original cost was set to intmax, so if it doesn't change, then no path was found
		if (pv->dv == INT_MAX) 
		{
			output << pv->id << ": NO PATH" << endl;
		}
		//if not intmax, a path was obviously found. Print out in appropriate format
		else 
		{
			output << pv->id << ": " << pv->dv << " [" << pv->path << "]" << endl;
		}
	}
}