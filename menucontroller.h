#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <QPushButton>
#include <GridController.h>
#include <mainwindow.h>
#include <QLineEdit>
#include <QComboBox>

class MenuController : QObject
{

    Q_OBJECT

public:
    int algo_index = 0;

    MenuController(GridController*, MainWindow*);

    void LoadMazes();
    void CreateSave();
    void LoadSave(int);
    void ChooseAlgo(int);
    void ClearGrid();

    void Visualize();

private:
    MainWindow* mainWindow;
    GridController* gridController;

    QPushButton* saveMazeButton;
    QPushButton* clearGridButton;
    QPushButton* visualizeButton;

    QComboBox* comboMaze;

    QLineEdit* saveNameInput;
};

#endif // MENUCONTROLLER_H
