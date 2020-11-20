
// Program to find Dijkstra's shortest path using 
// priority_queue in STL 
#include<bits/stdc++.h> 
using namespace std; 
# define INF 0x3f3f3f3f
  
// iPair ==> Integer Pair 
typedef pair<int, int> iPair; 
  
// To add an edge 
void addEdge(vector <pair<int, int> > adj[], int u, 
                                     int v, int wt) 
{ 
    adj[u].push_back(make_pair(v, wt)); 
    adj[v].push_back(make_pair(u, wt)); 
} 
   
  
// Prints shortest paths from src to all other vertices 
void shortestPath(vector<pair<int,int> > adj[], int V, int src) 
{ 
    // Create a priority queue to store vertices that 
    // are being preprocessed. This is weird syntax in C++. 
    // Refer below link for details of this syntax 
    // http://geeksquiz.com/implement-min-heap-using-stl/ 
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq; 
  
    // Create a vector for distances and initialize all 
    // distances as infinite (INF) 
    vector<int> dist(V, INF); 
  
    // Insert source itself in priority queue and initialize 
    // its distance as 0. 
    pq.push(make_pair(0, src)); 
    dist[src] = 0; 
  
    /* Looping till priority queue becomes empty (or all 
    distances are not finalized) */
    while (!pq.empty()) 
    { 
        // The first vertex in pair is the minimum distance 
        // vertex, extract it from priority queue. 
        // vertex label is stored in second of pair (it 
        // has to be done this way to keep the vertices 
        // sorted distance (distance must be first item 
        // in pair) 
        int u = pq.top().second; 
        pq.pop(); 
  
        // Get all adjacent of u.  
        for (auto x : adj[u]) 
        { 
            // Get vertex label and weight of current adjacent 
            // of u. 
            int v = x.first; 
            int weight = x.second; 
  
            // If there is shorted path to v through u. 
            if (dist[v] > dist[u] + weight) 
            { 
                // Updating distance of v 
                dist[v] = dist[u] + weight; 
                pq.push(make_pair(dist[v], v)); 
            } 
        } 
    } 
  
    // Print shortest distances stored in dist[] 
    printf("Vertex Distance from Source\n"); 
    for (int i = 0; i < V; ++i) 
        printf("%d \t\t %d\n", i+1, dist[i]); 
} 
  
// Driver program to test methods of graph class 
int main() 
{ 
    int V = 9; 
    vector<iPair > adj[V]; 
  
    // making above shown graph 
    addEdge(adj, 0, 1, 300); 
    addEdge(adj, 0, 4, 1200); 
    addEdge(adj, 1, 2, 800); 
    addEdge(adj, 1, 6, 500); 
    addEdge(adj, 2, 5, 1500); 
    addEdge(adj, 2, 6, 2000); 
    addEdge(adj, 2, 7, 900); 
    addEdge(adj, 4, 5, 600);
    addEdge(adj, 5, 8, 200);
    addEdge(adj, 6, 3, 300);
    addEdge(adj, 7, 8, 400);

    shortestPath(adj, V, 0);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 1);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 2);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 3);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 4);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 5);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 6);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 7);
    cout << "================================" << endl << endl;
    shortestPath(adj, V, 8);
    cout << "================================" << endl << endl;
  
    return 0; 
} 