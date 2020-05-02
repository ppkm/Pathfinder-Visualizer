#include "dijkstrapathfinder.h"
#include <GridController.h>
#include <algorithm>

const int pX = 3;
const int pY = 8;
const int gX = 12;
const int gY = 8;

DijkstraPathfinder::DijkstraPathfinder(int grid_size, GridController* gc)
{
    gridController = gc;
    GRID_SIZE = grid_size;

    //SetUpGrid();
}

void DijkstraPathfinder::UpdateNeighbours(Node* current) {

    int checkX = current->x;
    int checkY = current->y;

    checkX = current->x - 1;
    if((checkX >= 0 and checkX < GRID_SIZE) and (gridController->grid[checkX][checkY]->IsWalkable())){
        if(grid[checkX][checkY]->value > current->value + 1) {
            grid[checkX][checkY]->value = current->value + 1;
            grid[checkX][checkY]->prev = current;
        }
    }


    checkX = current->x + 1;
    if((checkX >= 0 and checkX < GRID_SIZE) and (gridController->grid[checkX][checkY]->IsWalkable())) {
        if(grid[checkX][checkY]->value > current->value + 1) {
            grid[checkX][checkY]->value = current->value + 1;
            grid[checkX][checkY]->prev = current;
        }
    }



    checkX = current->x;

    checkY = current->y - 1;
    if((checkY >= 0 and checkY < GRID_SIZE) and (gridController->grid[checkX][checkY]->IsWalkable())){
        if(grid[checkX][checkY]->value > current->value + 1) {
            grid[checkX][checkY]->value = current->value + 1;
            grid[checkX][checkY]->prev = current;
        }
    }

    checkY = current->y + 1;
    if((checkY >= 0 and checkY < GRID_SIZE) and (gridController->grid[checkX][checkY]->IsWalkable())) {
        if(grid[checkX][checkY]->value > current->value + 1) {
            grid[checkX][checkY]->value = current->value + 1;
            grid[checkX][checkY]->prev = current;
        }
    }

    current->type = Node::visited;
    unvisited.removeOne(current);
    visited.append(current);
}

bool DijkstraPathfinder::IsInfinitePath() {

    foreach(Node* n, unvisited)
        if(n->value < INT_MAX)
            return false;

    return true;
}

Node* DijkstraPathfinder::GetSmallestNode() {

    Node* smallest = unvisited.first();
    int min = smallest->value;

    foreach(Node* n, unvisited) {

        if(n->value < min) {
            min = n->value;
            smallest = n;
        }
    }

    return smallest;
}

void DijkstraPathfinder::FindPath(GNode* start, GNode* target) {

    grid = new Node**[GRID_SIZE];

    for(int x = 0; x < GRID_SIZE; x++) {

        grid[x] = new Node*[GRID_SIZE];

        for(int y = 0; y < GRID_SIZE; y++) {

            if(!gridController->grid[x][y]->IsWalkable())
                continue;

            grid[x][y] = new Node(x, y, Node::unvisited);
            unvisited.append(grid[x][y]);
        }
    }

    Node* current = grid[start->x][start->y];
    Node* targetNode = grid[target->x][target->y];
    current->value = 0;

    while(targetNode->type != Node::visited && !IsInfinitePath()) {

        current = GetSmallestNode();
        UpdateNeighbours(current);
    }

    Node* buffer = targetNode->prev;

    if(IsInfinitePath() && buffer == nullptr)
        return;

    while(buffer != grid[start->x][start->y]) {

        path.insert(0, buffer);
        buffer = buffer->prev;
    }
}



















