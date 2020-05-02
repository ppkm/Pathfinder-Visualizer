#ifndef GRIDCONTROLLER_H
#define GRIDCONTROLLER_H

#include "mainwindow.h"
#include <QPushButton>
#include <QDebug>

#include <apathfinder.h>
#include <dijkstrapathfinder.h>
#include <QTimer>

class GNode
{

public:
    enum Type { empty, player, goal, wall };
    enum VisualPath { none, path, visited };

    int x, y;
    VisualPath vp = none;
    QPushButton* btn;
    Type type = empty;
    GNode(int X, int Y);

    void UpdateButton();
    bool IsWalkable() { return type != Type::wall; }
};

class GridController : public QObject
{

    Q_OBJECT

public:

    int GRID_SIZE;
    GNode*** grid;
    GNode*** grid2;
    MainWindow * w;

    APathfinder* aPath;
    DijkstraPathfinder* dPath;
    QTimer* pathTimer = nullptr;
    QTimer* dPathTimer = nullptr;


public:
    GridController(int, MainWindow*);

    void OnPress(int x, int y);


    void SetNode(int x, int y, GNode::Type new_type);
    void ClearPath();
    void CreateGrid();
    void ShowGrid();
    void RandomizeGrid();
    void UpdateGrid();
    void SaveMaze();
    void LoadMaze();

    void APath();

private slots:
    void DrawPath();
    void DrawPath2();
};

#endif // GRIDCONTROLLER_H
