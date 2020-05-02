#ifndef DIJKSTRAPATHFINDER_H
#define DIJKSTRAPATHFINDER_H
#include <QList>

class GridController;
class GNode;

struct Node {

    enum Type { unvisited, visited };
    int x;
    int y;
    int value = INT_MAX;
    Node* prev = nullptr;
    Type type;

    Node(int X, int Y, Type T) {

        x = X;
        y = Y;
        type = T;
    }
};

class DijkstraPathfinder
{
public:
    //int** grid;
    //int** tempGrid;
    Node*** grid;
    QList<Node*> unvisited;
    QList<Node*> path;
    QList<Node*> visited;
    int GRID_SIZE;
    GridController* gridController;
    //QList<std::vector<int>> path;
    //QList<std::vector<int>> visited;

    DijkstraPathfinder(int,GridController*);

    void CopyArr(int**, int**);
    void SetUpGrid();
    bool AreArrEqual(int**, int**);
    int MinNeighbourCost(int, int, int&, int&);
    void FindPath(GNode* start, GNode* target);
    void UpdateNeighbours(Node*);
    int CheckNeighbours(int x, int y, int& mx, int&my);
    bool IsInfinitePath();
    Node* GetSmallestNode();
};

#endif // DIJKSTRAPATHFINDER_H
