#include "menucontroller.h"

#include <QDir>
#include <QFile>

MenuController::MenuController(GridController* gc, MainWindow* w)
{
    gridController = gc;
    mainWindow = w;

    saveMazeButton = w->findChild<QPushButton*>("SaveMazeButton");
    connect(saveMazeButton, &QPushButton::released, [=]{
        CreateSave();
    });

    clearGridButton = w->findChild<QPushButton*>("ClearGridButton");
    connect(clearGridButton, &QPushButton::released, [=]{
        ClearGrid();
    });

    visualizeButton = w->findChild<QPushButton*>("VisualizeButton");
    connect(visualizeButton, &QPushButton::released, [=]{
        Visualize();
    });

    comboMaze = w->findChild<QComboBox*>("ComboMaze");
    connect(comboMaze, QOverload<int>::of(&QComboBox::activated), [=](int index){
        LoadSave(index);
    });

    saveNameInput = w->findChild<QLineEdit*>("SaveNameInput");

    LoadMazes();
}

void MenuController::LoadMazes() {

    QDir directory("Saves/");
    QStringList saves = directory.entryList(QStringList() << "*.txt", QDir::Files);

    foreach(QString filename, saves)
        comboMaze->addItem(filename.split(".txt")[0]);
}

void MenuController::CreateSave() {

    if(gridController->pathTimer != nullptr && gridController->pathTimer->isActive())
        return;

    if(gridController->dPathTimer != nullptr && gridController->dPathTimer->isActive())
        return;

    QString saveName = saveNameInput->text();

    if(saveName == "")
        return;

    QFile file("Saves/" + saveName + ".txt");


    if(file.exists()) {
        saveNameInput->setText("");
        return;
    }

    //Check if save exists

    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {

                switch(gridController->grid[x][y]->type) {

                case GNode::empty:
                        stream << "E";
                    break;

                case GNode::wall:
                        stream << "W";
                    break;

                case GNode::player:
                        stream << "P";
                    break;

                case GNode::goal:
                        stream << "G";
                    break;
                }
            }

            stream << endl;
        }   
        comboMaze->addItem(saveName);
    }

    saveNameInput->setText("");
}

void MenuController::LoadSave(int index) {

    if(gridController->pathTimer != nullptr && gridController->pathTimer->isActive())
        return;

    if(gridController->dPathTimer != nullptr && gridController->dPathTimer->isActive())
        return;

    QString filename = "Saves/" + comboMaze->itemText(index) + ".txt";

    QFile file(filename);

    if(!file.exists()) {
        return;
    }

    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        char node = ' ';

        gridController->ClearPath();

        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {

                node = ' ';

                stream >> node;

                switch (node) {

                case 'E':
                        gridController->SetNode(x, y, GNode::empty);
                    break;

                case 'W':
                        gridController->SetNode(x, y, GNode::wall);
                    break;

                case 'P':
                        gridController->SetNode(x, y, GNode::player);
                    break;

                case 'G':
                        gridController->SetNode(x, y, GNode::goal);
                    break;
                }
            }
            stream >> node;
        }
    }
}

void MenuController::ClearGrid() {

    if(gridController->pathTimer != nullptr && gridController->pathTimer->isActive())
        return;

    if(gridController->dPathTimer != nullptr && gridController->dPathTimer->isActive())
        return;

    gridController->ClearPath();
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {

            gridController->SetNode(x, y, GNode::empty);
        }
    }
}

void MenuController::ChooseAlgo(int index) {

    algo_index = index;
}

void MenuController::Visualize() {

    if(gridController->pathTimer != nullptr && gridController->pathTimer->isActive())
        return;

    if(gridController->dPathTimer != nullptr && gridController->dPathTimer->isActive())
        return;

    gridController->ClearPath();
    switch(algo_index)
    {
    case 0:
        gridController->APath();
        break;
    }
}
