#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

class Graph {
private:
    int vertices;
    vector<vector<int>> adj_matrix;

public:
    void newGraph() {
        int kolpoints = 0;
        cout << "Количество точек графа: ";
        cin >> kolpoints;
        vertices = kolpoints;
        adj_matrix = vector<vector<int>>(vertices, vector<int>(vertices, 0));

        for (int i = 0; i < kolpoints; i++) {
            int k;
            cout << "Количество путей из точки " << i << ": ";
            cin >> k;
            for (int j = 0; j < k; j++) {
                int n;
                cout << "Точка, к которой идет путь: ";
                cin >> n;
                adj_matrix[i][n] = 1;
                adj_matrix[n][i] = 1;  // Удалить эту строку для ориентированного графа
            }
        }
    }

    vector<int> dfsIterative(int start) {
        vector<bool> visited(vertices, false);
        stack<int> stack;
        vector<int> result;

        stack.push(start);

        while (!stack.empty()) {
            int vertex = stack.top();
            stack.pop();

            if (!visited[vertex]) {
                result.push_back(vertex);
                visited[vertex] = true;

                for (int neighbor = vertices - 1; neighbor >= 0; --neighbor) {
                    if (adj_matrix[vertex][neighbor] == 1 && !visited[neighbor]) {
                        stack.push(neighbor);
                    }
                }
            }
        }

        return result;
    }

    void dfsRecursiveUtil(int vertex, vector<bool>& visited, vector<int>& result) {
        visited[vertex] = true;
        result.push_back(vertex);

        for (int neighbor = 0; neighbor < vertices; ++neighbor) {
            if (adj_matrix[vertex][neighbor] == 1 && !visited[neighbor]) {
                dfsRecursiveUtil(neighbor, visited, result);
            }
        }
    }

    vector<int> dfsRecursive(int start) {
        vector<bool> visited(vertices, false);
        vector<int> result;
        dfsRecursiveUtil(start, visited, result);
        return result;
    }

    bool isBipartiteUtil(int v, vector<int>& colors, int color) {
        colors[v] = color;
        for (int neighbor = 0; neighbor < vertices; ++neighbor) {
            if (adj_matrix[v][neighbor] == 1) {
                if (colors[neighbor] == -1) {
                    if (!isBipartiteUtil(neighbor, colors, 1 - color)) {
                        return false;
                    }
                }
                else if (colors[neighbor] == color) {
                    return false;
                }
            }
        }
        return true;
    }

    bool dvudolnost() {
        vector<int> colors(vertices, -1);
        for (int v = 0; v < vertices; ++v) {
            if (colors[v] == -1) {
                if (!isBipartiteUtil(v, colors, 0)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool findCycle() {
        vector<bool> visited(vertices, false);
        vector<bool> recStack(vertices, false);

        auto cycleHelper = [this, &visited, &recStack](int v, auto& cycleHelperRef) -> bool {
            visited[v] = true;
            recStack[v] = true;

            for (int neighbor = 0; neighbor < vertices; ++neighbor) {
                if (adj_matrix[v][neighbor] == 1) {
                    if (!visited[neighbor]) {
                        if (cycleHelperRef(neighbor, cycleHelperRef)) {
                            return true;
                        }
                    }
                    else if (recStack[neighbor]) {
                        return true;
                    }
                }
            }

            recStack[v] = false;
            return false;
            };

        for (int node = 0; node < vertices; ++node) {
            if (!visited[node]) {
                if (cycleHelper(node, cycleHelper)) {
                    return true;
                }
            }
        }

        return false;
    }
};

int main() {

    setlocale(LC_ALL, "ru");
    Graph g;
    g.newGraph();

    vector<int> dfsIter = g.dfsIterative(0);
    cout << "Stack обход в глубину: ";
    for (int v : dfsIter) {
        cout << " " << v;
    }
    cout << endl;

    vector<int> dfsRec = g.dfsRecursive(0);
    cout << "Рекурсивный обход в глубину: ";
    for (int v : dfsRec) {
        cout << " " << v;
    }
    cout << endl;

    cout << "Проверка на двудольность: " << (g.dvudolnost() ? "Да" : "Нет") << endl;

    cout << "Поиск цикла: " << (g.findCycle() ? "Есть" : "Нет") << endl;

    return 0;
}