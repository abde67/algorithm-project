




#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

class Graph {
private:
    map<int, vector<pair<int, int>>> adjList;

public:
    void initializeFromFile(const string& filename);
    void addNode(int node);
    void deleteNode(int node);
    void addEdge(int node1, int node2, int weight);
    void deleteEdge(int node1, int node2);
    void updateWeight(int node1, int node2, int newWeight);
    void bfs(int start);
    void dfs(int start);
    void shortestPath(int source, int destination);
    void displayGraph();
};

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Graph::initializeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << RED << "Error opening file!" << RESET << endl;
        return;
    }
    
    string line, type;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Ignore empty lines and comments
        
        stringstream ss(line);
        ss >> type;
        
        if (type == "NODES") {
            int node;
            while (ss >> node) {
                addNode(node);
            }
        } else if (type == "EDGES") {
            while (getline(file, line) && !line.empty()) {
                stringstream edgeStream(line);
                int node1, node2, weight;
                edgeStream >> node1 >> node2 >> weight;
                addEdge(node1, node2, weight);
            }
        }
    }
    
    file.close();
    cout << GREEN << "Graph initialized from file!" << RESET << endl;
}

void Graph::addNode(int node) {
    if (adjList.find(node) == adjList.end()) {
        adjList[node] = {};
        cout << GREEN << "Node " << node << " added successfully!" << RESET << endl;
    } else {
        cout << RED << "Node " << node << " already exists!" << RESET << endl;
    }
}

void Graph::deleteNode(int node) {
    if (adjList.find(node) != adjList.end()) {
        adjList.erase(node);
        for (auto &pair : adjList) {
            pair.second.erase(remove_if(pair.second.begin(), pair.second.end(),
                                        [node](const std::pair<int, int> &edge) { return edge.first == node; }),
                              pair.second.end());
        }
        cout << YELLOW << "Node " << node << " deleted." << RESET << endl;
    } else {
        cout << RED << "Node does not exist!" << RESET << endl;
    }
}



void Graph::addEdge(int node1, int node2, int weight) {
    if (adjList.find(node1) == adjList.end() || adjList.find(node2) == adjList.end()) {
        cout << RED << "One or both nodes do not exist!" << RESET << endl;
        return;
    }
    adjList[node1].push_back({node2, weight});
    adjList[node2].push_back({node1, weight});
    cout << GREEN << "Edge added between " << node1 << " and " << node2 << " with weight " << weight << RESET << endl;
}

void Graph::deleteEdge(int node1, int node2) {
    auto &edges1 = adjList[node1];
    auto &edges2 = adjList[node2];
    edges1.erase(remove_if(edges1.begin(), edges1.end(), [node2](pair<int, int> edge) { return edge.first == node2; }), edges1.end());
    edges2.erase(remove_if(edges2.begin(), edges2.end(), [node1](pair<int, int> edge) { return edge.first == node1; }), edges2.end());
    cout << YELLOW << "Edge between " << node1 << " and " << node2 << " deleted." << RESET << endl;
}

void Graph::updateWeight(int node1, int node2, int newWeight) {
    for (auto &edge : adjList[node1]) {
        if (edge.first == node2) edge.second = newWeight;
    }
    for (auto &edge : adjList[node2]) {
        if (edge.first == node1) edge.second = newWeight;
    }
    cout << BLUE << "Weight updated for edge " << node1 << " - " << node2 << " to " << newWeight << RESET << endl;
}

void Graph::bfs(int start) {
    map<int, bool> visited;
    queue<int> q;
    q.push(start);
    visited[start] = true;
    cout << "BFS Traversal: ";
    while (!q.empty()) {
        int node = q.front(); q.pop();
        cout << node << " ";
        for (auto neighbor : adjList[node]) {
            if (!visited[neighbor.first]) {
                visited[neighbor.first] = true;
                q.push(neighbor.first);
            }
        }
    }
    cout << endl;
}

void Graph::dfs(int start) {
    map<int, bool> visited;
    stack<int> s;
    s.push(start);
    cout << "DFS Traversal: ";
    while (!s.empty()) {
        int node = s.top(); s.pop();
        if (!visited[node]) {
            cout << node << " ";
            visited[node] = true;
            for (auto neighbor : adjList[node]) {
                if (!visited[neighbor.first]) {
                    s.push(neighbor.first);
                }
            }
        }
    }
    cout << endl;
}

void Graph::displayGraph() {
    cout << "Graph Representation:\n";
    for (auto node : adjList) {
        cout << "Node " << node.first << " -> ";
        for (auto edge : node.second) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }
}

int main() {
    Graph g;
    g.initializeFromFile("graph_data.txt");
    g.displayGraph();
    int choice, node1, node2, weight;
    while (true) {
        cout << "\n1. Add Node\n2. Delete Node\n3. Add Edge\n4. Delete Edge\n5. Update Weight\n6. BFS Traversal\n7. DFS Traversal\n8. Display Graph\n9. Exit\nEnter choice: ";
        cin >> choice;
        clearScreen();
        switch (choice) {
            case 1:
                cout << "Enter node ID: "; cin >> node1; g.addNode(node1); break;
            case 2:
                cout << "Enter node ID to delete: "; cin >> node1; g.deleteNode(node1); break;
            case 3:
                cout << "Enter two nodes and weight: "; cin >> node1 >> node2 >> weight; g.addEdge(node1, node2, weight); break;
            case 4:
                cout << "Enter two nodes: "; cin >> node1 >> node2; g.deleteEdge(node1, node2); break;
            case 5:
                cout << "Enter two nodes and new weight: "; cin >> node1 >> node2 >> weight; g.updateWeight(node1, node2, weight); break;
            case 6:
                cout << "Enter starting node: "; cin >> node1; g.bfs(node1); break;
            case 7:
                cout << "Enter starting node: "; cin >> node1; g.dfs(node1); break;
            case 8:
                g.displayGraph(); break;
            case 9:
                return 0;
        }
    }
}
