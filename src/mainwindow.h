#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "about.h"
#include "svgview.h"
#include "GraphEditor.hpp"
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    std::string appPath;
    bool addingNode;
    GraphEditor *graphEditor;

    void addNode(int x, int y);

private slots:
    void on_pushButton_clicked();

    void on_fixedRegions_clicked();

    void on_fexibleRegions_clicked();

    void on_measures_clicked();

    void on_help_clicked();

    void openNewWindow();

    void on_pushButton_2_clicked();

    void on_addnode_clicked();

    void on_new_topology_clicked();

    void on_m_simulation_clicked();

private:
    Ui::MainWindow *ui;
    About *aboutWindow;
};

#endif // MAINWINDOW_H
