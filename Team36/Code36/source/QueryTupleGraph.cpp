#include <vector>
#include "QueryTupleGraph.h"
#include "QueryTupleNode.h"

QueryTupleGraph::QueryTupleGraph(int V, std::list<int>* neighbours) {
	this->V = V;
	this->adj = neighbours;
}

void QueryTupleGraph::DFSUtil(int v, bool visited[]) {
	// Mark the current node as visited and print it
	visited[v] = true;

	// Recur for all the vertices
	// adjacent to this vertex
	std::list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i) {
		if (!visited[*i]) {
			DFSUtil(*i, visited);
		}
	}
}

void QueryTupleGraph::addEdge(int v, int w) {
	adj[v].push_back(w);
	adj[w].push_back(v);
}

void QueryTupleGraph::connectedComponents() {
	// Mark all the vertices as not visited
	bool* visited = new bool[V];
	for (int v = 0; v < V; v++) {
		visited[v] = false;
	}

	for (int v = 0; v < V; v++) {
		if (visited[v] == false) {
			// print all reachable vertices
			// from v
			DFSUtil(v, visited);
		}
	}
	delete[] visited;
}