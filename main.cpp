#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <GridController.h>
#include <menucontroller.h>

const int GRID_SIZE = 16;

GridController* gridController;
MenuController* menuController;

void SaveMaze();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Grid memory allocation;
    gridController = new GridController(GRID_SIZE, &w);
    gridController->ShowGrid();
    //gridController->RandomizeGrid();
    gridController->UpdateGrid();

    menuController = new MenuController(gridController, &w);

    w.show();
    return a.exec();
}



