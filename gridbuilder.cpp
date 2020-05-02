#include "gridbuilder.h"

GridBuilder::GridBuilder(GNode*** new_grid)
{
    grid = new_grid;
}



void GridBuilder::BuildWallAt(int x, int y)
{
    grid[x][y]->type = GNode::wall;
}

void GridBuilder::RemoveWallAt(int x, int y)
{
    grid[x][y]->type = GNode::empty;
}
