#include <QBasicTimer>
#include <qgridlayout.h>
#include <QtCore>
#include <QFrame>

#include "GridController.h"

const int pX = 3;
const int pY = 8;
const int gX = 12;
const int gY = 8;

GNode::GNode(int X, int Y)
{
    x = X;
    y = Y;
    btn = new QPushButton();
    //btn->resize(btnSize, btnSize);
    //btn->setFixedSize(btnSize, 2*btnSize);
    type = empty;
}

void GNode::UpdateButton() {

    QPalette pal = btn->palette();

    QColor wallC(3, 62, 107);
    QColor emptyC(102, 163, 210);
    QColor playerC(0, 104, 90);
    QColor goalC(255, 146, 0);
    QColor pathC(52, 197, 28);
    QColor visitedC(207, 190, 177);

    if(vp == path)
    {
        pal.setColor(QPalette::Button, pathC);
    }
    else if(vp == visited)
    {
        pal.setColor(QPalette::Button, visitedC);
    }
    else
    {
        switch(type)
        {
        case GNode::empty:
            pal.setColor(QPalette::Button, emptyC);
            break;

        case GNode::wall:
            pal.setColor(QPalette::Button, wallC);
            break;

        case GNode::player:
            pal.setColor(QPalette::Button, playerC);
            break;

        case GNode::goal:
            pal.setColor(QPalette::Button, goalC);
              break;
        }
    }

    btn->setAutoFillBackground(true);
    btn->setPalette(pal);
    btn->setFlat(true);
    btn->update();
}

GridController::GridController(int grid_size, MainWindow* window) {

    GRID_SIZE = grid_size;

    w = window;
    grid = new GNode**[GRID_SIZE];
    grid2 = new GNode**[GRID_SIZE];

    CreateGrid();
}

void GridController::CreateGrid()
{

    //1st grid
    for(int x = 0; x < GRID_SIZE; x++)
    {
        grid[x] = new GNode*[GRID_SIZE];
        grid2[x] = new GNode*[GRID_SIZE];
        for(int y = 0; y < GRID_SIZE; y++)
        {
             grid[x][y] = new GNode(x, y);
             grid2[x][y] = new GNode(x, y);
        }
    }

    grid[pX][pY]->type = GNode::player;
    grid[gX][gY]->type = GNode::goal;

    grid2[pX][pY]->type = GNode::player;
    grid2[gX][gY]->type = GNode::goal;
}

void GridController::ClearPath() {

    if(pathTimer != nullptr && pathTimer->isActive())
        return;

    for(int x = 0; x < GRID_SIZE; x++) {
        for(int y = 0; y < GRID_SIZE; y++) {

            grid[x][y]->vp = GNode::none;
            grid2[x][y]->vp = GNode::none;
        }
   }
}

void GridController::OnPress(int x, int y) {

    if(pathTimer != nullptr and pathTimer->isActive())
        return;

    if(dPathTimer != nullptr && dPathTimer->isActive())
        return;

    GNode * elem = grid[x][y];
    GNode * elem2 = grid2[x][y];

    ClearPath();

    if(elem->type == GNode::empty)
    {
        elem->type = GNode::wall;
        elem2->type = GNode::wall;
    }
    else if(elem->type == GNode::wall)
    {
         elem->type = GNode::empty;
         elem2->type = GNode::empty;
    }

    UpdateGrid();
}

void GridController::SetNode(int x, int y, GNode::Type new_type) {

    if(pathTimer != nullptr && pathTimer->isActive())
        return;

    if(dPathTimer != nullptr && dPathTimer->isActive())
        return;

    GNode* elem = grid[x][y];
     GNode* elem2 = grid2[x][y];

    if(elem->type == GNode::player or elem->type == GNode::goal)
        return;

    elem->type = new_type;
    elem->UpdateButton();

    elem2->type = new_type;
    elem2->UpdateButton();
}

void GridController::ShowGrid() {

    QFrame* GridFrame = w->findChild<QFrame*>("GridFrame");

    const int btn_size = 25;

    for(int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            //QString name = QString::number(x) + ";" + QString::number(y);
            grid[x][y]->btn = new QPushButton(GridFrame);
            grid[x][y]->btn->setGeometry(x*btn_size, y*btn_size, btn_size, btn_size);

            connect(grid[x][y]->btn, &QPushButton::pressed, [=]{
                OnPress(x, y);
            });

            grid2[x][y]->btn = new QPushButton(GridFrame);
            grid2[x][y]->btn->setGeometry(x*btn_size + 416, y*btn_size, btn_size, btn_size);

            connect(grid2[x][y]->btn, &QPushButton::pressed, [=]{
                OnPress(x, y);
            });
        }
    }
}

void GridController::UpdateGrid()
{
    for(int x = 0; x < GRID_SIZE; x++)
    {
        for(int y = 0; y < GRID_SIZE; y++)
        {
            grid[x][y]->UpdateButton();
            grid2[x][y]->UpdateButton();
        }
    }
}

void GridController::APath() {

    aPath = new APathfinder(GRID_SIZE, this);
    aPath->CreateGrid();

    GNode* player = grid[pX][pY];
    GNode* goal = grid[gX][gY];

    aPath->FindPath(player, goal);

    pathTimer = new QTimer(this);
    connect(pathTimer, SIGNAL(timeout()), this, SLOT(DrawPath()));
    pathTimer->start(25);

    ////////

    //return;

    player = grid2[pX][pY];
    goal = grid2[gX][gY];

    dPath = new DijkstraPathfinder(GRID_SIZE, this);
    dPath->FindPath(player, goal);

    dPathTimer = new QTimer(this);
    connect(dPathTimer, SIGNAL(timeout()), this, SLOT(DrawPath2()));
    dPathTimer->start(25);
}

void GridController::DrawPath() {

   GNode* player = grid[pX][pY];
   GNode* goal = grid[gX][gY];

   bool setVisited = true;

   foreach(ANode* vNode, aPath->visited) {
       if((vNode->x == player->x && vNode->y == player->y)
               or (vNode->x == goal->x && vNode->y == goal->y))
               continue;

       if(grid[vNode->x][vNode->y]->vp != GNode::visited) {

           grid[vNode->x][vNode->y]->vp = GNode::visited;
           setVisited = false;
           break;
       }
   }

   if(setVisited) {
       aPath->visited.clear();
       bool setPath = false;

        foreach(ANode* node, aPath->path) {
            if(node->x == goal->x && node->y == goal->y)
                continue;
            if(grid[node->x][node->y]->vp != GNode::path) {
                grid[node->x][node->y]->vp = GNode::path;
                setPath = true;
                break;
            }
        }

        if(setPath == false) {
            pathTimer->stop();

            delete(aPath);
            delete(pathTimer);
            pathTimer = nullptr;
        }
   }

    UpdateGrid();
}

void GridController::DrawPath2() {

    GNode* player = grid2[pX][pY];
    GNode* goal = grid2[gX][gY];

    bool setVisited = true;

    foreach(Node* n, dPath->visited) {

        GNode* vNode = grid2[n->x][n->y];

        if((vNode->x == player->x && vNode->y == player->y)
                or (vNode->x == goal->x && vNode->y == goal->y))
                continue;

        if(grid2[vNode->x][vNode->y]->vp != GNode::visited) {

            grid2[vNode->x][vNode->y]->vp = GNode::visited;
            setVisited = false;
            break;
        }
    }

    if(setVisited) {
        dPath->visited.clear();


        bool setPath = false;

         foreach(Node* n, dPath->path) {

             GNode* node = grid2[n->x][n->y];

             if(node->x == goal->x && node->y == goal->y)
                 continue;

             if(grid2[node->x][node->y]->vp != GNode::path) {
                 grid2[node->x][node->y]->vp = GNode::path;
                 setPath = true;
                 break;
             }
         }

         if(setPath == false) {
             dPathTimer->stop();

             delete(dPath);
             delete(dPathTimer);
             dPathTimer = nullptr;
         }
    }

     UpdateGrid();
}
