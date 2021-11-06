#include <vector>
#include <list>
#include "QueryTupleGraph.h"
#include "QueryTupleNode.h"

QueryTupleGraph::QueryTupleGraph(int V) {
	this->V = V;
	this->adj = new std::list<int>[V];
}

void QueryTupleGraph::DFSUtil(int v, bool visited[], std::list<int>& each_group) {
	// Mark the current node as visited and print it
	visited[v] = true;

	// Recur for all the vertices
	// adjacent to this vertex
	std::list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i) {
		if (!visited[*i]) {
			each_group.push_back(v);
			DFSUtil(*i, visited, each_group);
		}
	}
}

void QueryTupleGraph::addEdge(int v, int w) {
	adj[v].push_back(w);
	adj[w].push_back(v);
}

void QueryTupleGraph::connectedComponents(std::list<std::list<std::pair<Entity, Entity>>>& group) {
	std::list<int> each_group;

	// Mark all the vertices as not visited
	bool* visited = new bool[V];
	for (int v = 0; v < V; v++) {
		visited[v] = false;
	}

	for (int v = 0; v < V; v++) {
		if (visited[v] == false) {
			// print all reachable vertices
			// from v
			DFSUtil(v, visited, each_group);
		}
	}
	delete[] visited;

	//group.push_back(each_group);
}