#include <iostream>
#include <chrono>
#include "graph.h"

using namespace std;

int main() 
{
    //read from input file, get starting vertex, output answer to single source shortest path problem
    string inputFile, startVertex, outputFile;
    //empty graph
    graph graph;
    //retrieve input file from user
	cout << "What is the name of the graph file?" << endl;
    cout << "Graph File: ";
    cin >> inputFile;
    cout << endl;
    //use information in input file to create a corresponding graph
    graph.loadGraph(inputFile);
	//retrieve starting vertex from user
	do 
    {
		cout << "What vertex would you like to start from? Enter its ID. " << endl;
        cout << "Starting Vertex: ";
		cin >> startVertex;
        cout << endl;
	} while (graph.validVertex(startVertex) == false);
    //cpu time for how many seconds it takes for dijkstra solve the problem

    //start the timer
    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    // Find shortest path from specified start point to each other vertex
    graph.dijkstra(startVertex);

    //stop the timer and calculate duration of running dijksta's algorithm
    chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
	chrono::duration<double> timeDiff = chrono::duration_cast<chrono::duration<double>>(endTime - startTime);
    cout << "Total time spent (in seconds) executing Dijkstra's algorithm: " << timeDiff.count() << endl;
    //retrieve output file that contains all the shortest paths from the starting vertex
    cout << "What is the name of the output file?" << endl;
    cout << "Output File: ";
    cin >> outputFile;
    cout << endl;
    //in order, put shortest paths into file
    graph.outputPaths(outputFile);
}