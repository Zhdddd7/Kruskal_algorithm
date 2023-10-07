#include <iostream>
#include <vector>
#include <algorithm>
#include<time.h>
#include <ctime>
#include<fstream>
#include<windows.h>
using namespace std;
struct Edge {
    int src, dest, weight;
};

class UnionFind {
private:
    std::vector<int> parent, rank;

public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression
        }
        return parent[x];
    }

    void unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};

bool edgeCompare(Edge a, Edge b) {
    return a.weight < b.weight;
}

std::vector<Edge> kruskal(std::vector<Edge>& edges, int V) {
    sort(edges.begin(), edges.end(), edgeCompare);

    UnionFind uf(V);
    std::vector<Edge> result;

    for (const auto& edge : edges) {
        int x = uf.find(edge.src);
        int y = uf.find(edge.dest);

        if (x != y) {
            result.push_back(edge);
            uf.unionSets(x, y);
        }
    }
    return result;
}

std::vector<Edge> initializeGraph(int nodes, int edges) {
    std::vector<Edge> graphEdges;

    // Seed the random number generator
    std::srand(std::time(0));

    for (int i = 0; i < edges; ++i) {
        Edge newEdge;

        newEdge.src = std::rand() % nodes;   // random source node
        newEdge.dest = std::rand() % nodes;  // random destination node
        while(newEdge.src == newEdge.dest) { // ensuring source and dest are different
            newEdge.dest = std::rand() % nodes;
        }
        newEdge.weight = (std::rand() % 16) + 5;  // random weight between 5 and 20

        graphEdges.push_back(newEdge);
    }

    return graphEdges;
}

double Time_calculate(int E){

    int V = E-1;  // Number of vertices
    std::vector<Edge> edges(E);
    edges=initializeGraph(V,E);
    //initialize the graph
    LARGE_INTEGER t1,t2,tc;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

    std::vector<Edge> mst = kruskal(edges, V);
//time measure of Kurskal algorithm
    QueryPerformanceCounter(&t2);
    double time=(double)(t2.QuadPart-t1.QuadPart)/(double)tc.QuadPart;
    return time;
}

void Write_txt(double *time_recorder){
    ofstream file;
    string file_name="time_recorder.txt";
    file.open(file_name.c_str());
    double *p=time_recorder;
    for(int i=0;i<200;i++){
        file<<*p<<" ";
        p++;
    }
    file.close();
}

int main() {
    double time_recorder[220]={0};
    double *p=time_recorder;
    for(int e=50;e<10000;e+=50){
        *p=Time_calculate(e);
        p++;
    }
    Write_txt(time_recorder);
    return 0;
   
}
