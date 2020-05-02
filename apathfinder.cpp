#include "apathfinder.h"
#include <QDebug>
#include <GridController.h>

using namespace std;

APathfinder::APathfinder(int grid_size, GridController* _gc)
{
    gc = _gc;
    GRID_SIZE = grid_size;
    grid = new ANode**[GRID_SIZE];
}

void APathfinder::CreateGrid()
{
    grid = new ANode**[GRID_SIZE];

    for(int x = 0; x < GRID_SIZE; x++) {
        grid[x] = new ANode*[GRID_SIZE];
        for(int y = 0; y < GRID_SIZE; y++) {
            grid[x][y] = new ANode(gc->grid[x][y]->IsWalkable(), x, y);
        }
    }
}

QList<ANode*> APathfinder::ReverseList(QList<ANode*> list) {

    int size = list.size();

    for(int i = 0; i < size/2; i++)
        list.swapItemsAt(i, size - i - 1);

    return list;
}

QList<ANode*> APathfinder::RetracePath(ANode *startNode, ANode *endNode)
{
    QList<ANode*> path;
    ANode* currentNode = endNode;

    while(currentNode != startNode) {
        path.append(currentNode);
        currentNode = currentNode->parent;
    }

    path = ReverseList(path);

    return path;
}

ANode* APathfinder::NodeAt(int x, int y) {

    return grid[x][y];
}

QList<ANode*> APathfinder::GetNeighbours(ANode* node)
{
    QList<ANode*> neighbours;
    int X = node->x;
    int Y = node->y;


   /*for (int x = -1; x <= 1; x++) {
       for(int y = -1; y <= 1; y++) {

           if(x == 0 && y == 0)
               continue;

           int checkX = X + x;
           int checkY = Y + y;

           if(checkX >= 0 && checkX < GRID_SIZE) {
               if(checkY >= 0 && checkY < GRID_SIZE) {
                        neighbours.append(NodeAt(checkX, checkY));
               }
           }
       }
   }

   int checkX, checkY;

   checkX = X-1;
   checkY = Y-1;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE)) {

        if(!(checkX + 1 < GRID_SIZE && NodeAt(checkX + 1, checkY)->walkable)
            and !(checkY + 1 < GRID_SIZE && NodeAt(checkX, checkY + 1)->walkable))
                neighbours.removeOne(NodeAt(checkX, checkY));
   }

   checkX = X+1;
   checkY = Y-1;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE)) {

        if(!(checkX - 1 < GRID_SIZE && NodeAt(checkX - 1, checkY)->walkable)
            and !(checkY + 1 < GRID_SIZE && NodeAt(checkX, checkY + 1)->walkable))
                neighbours.removeOne(NodeAt(checkX, checkY));
   }

   checkX = X-1;
   checkY = Y+1;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE)) {

        if(!(checkX + 1 < GRID_SIZE && NodeAt(checkX + 1, checkY)->walkable)
            and !(checkY - 1 < GRID_SIZE && NodeAt(checkX, checkY - 1)->walkable))
                neighbours.removeOne(NodeAt(checkX, checkY));
   }

   checkX = X+1;
   checkY = Y+1;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE)) {

        if(!(checkX - 1 < GRID_SIZE && NodeAt(checkX - 1, checkY)->walkable)
            and !(checkY - 1 < GRID_SIZE && NodeAt(checkX, checkY - 1)->walkable))
                neighbours.removeOne(NodeAt(checkX, checkY));
   }*/

   int checkX = X;
   int checkY = Y-1;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE))
       neighbours.append(NodeAt(checkX, checkY));

   checkX = X;
   checkY = Y+1;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE))
       neighbours.append(NodeAt(checkX, checkY));

   checkX = X-1;
   checkY = Y;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE))
       neighbours.append(NodeAt(checkX, checkY));

   checkX = X+1;
   checkY = Y;

   if((checkX >= 0 && checkX < GRID_SIZE) and
       (checkY >= 0 && checkY < GRID_SIZE))
       neighbours.append(NodeAt(checkX, checkY));

   return neighbours;
}

int APathfinder::GetDistance(ANode * A, ANode * B) {

    int dstX = abs(A->x - B->x);
    int dstY = abs(A->y - B->y);

    if(dstX > dstY)
        return 14*dstY + 10*(dstX - dstY);

    return 14*dstX + 10 * (dstY - dstX);
}

void APathfinder::FindPath(GNode* start, GNode* target) {

    ANode* startNode = NodeAt(start->x, start->y);
    ANode* targetNode = NodeAt(target->x, target->y);

    QList<ANode*> openSet;
    QList<ANode*> closedSet;
    openSet.append(startNode);

    while (openSet.size() > 0) {
        ANode* currentNode = openSet.at(0);

        for (int i = 1; i < openSet.size(); i++) {

            if(openSet.at(i)->fCost() < currentNode->fCost() || openSet.at(i)->fCost() == currentNode->fCost()) {
                    if(openSet.at(i)->hCost < currentNode->hCost) {
                         currentNode = openSet.at(i);
                    }
            }
        }

        openSet.removeOne(currentNode);
        closedSet.append(currentNode);
        visited.append(currentNode);

        if (currentNode == targetNode) {
            path = RetracePath(startNode, targetNode);
            return;
        }

        foreach(ANode* neighbour, GetNeighbours(currentNode)) {

            if(!(neighbour->walkable) or closedSet.contains(neighbour))
            {
                continue;
            }


              int newMovementCostToNeighbour = currentNode->gCost + GetDistance(currentNode, neighbour);
              if(newMovementCostToNeighbour < neighbour->gCost || !openSet.contains(neighbour)) {

                neighbour->gCost = newMovementCostToNeighbour;
                neighbour->hCost = GetDistance(neighbour, targetNode);
                neighbour->parent = currentNode;

                if (!openSet.contains(neighbour)) { openSet.append(neighbour); }
            }

        }
    }
}
