#ifndef APATHFINDER_H
#define APATHFINDER_H

#include <QList>

class GridController;
class GNode;

class ANode
{
public:
    bool walkable;
    int x,y;
    int gCost, hCost;
    ANode* parent;

    ANode(bool _walkable, int _x, int _y) {

        walkable = _walkable;
        x = _x;
        y = _y;
    }

    int fCost() {

        return gCost + hCost;
    }
};

class APathfinder
{
public:
    int GRID_SIZE;
    ANode*** grid;
    GridController* gc;
    QList<ANode*> path;
    QList<ANode*> visited;

    APathfinder(int grid_size, GridController* _gc);
    void FindPath(GNode*, GNode*);
    void CreateGrid();
    QList<ANode*> RetracePath(ANode* startNode, ANode* endNode);
    int GetDistance(ANode*, ANode*);
    QList<ANode*> GetNeighbours(ANode* node);
    ANode* NodeAt(int, int);
    QList<ANode*> ReverseList(QList<ANode*> list);
};

#endif // APATHFINDER_H
