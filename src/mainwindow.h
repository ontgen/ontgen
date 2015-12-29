/**
  This is Network Topologies Generator, a tool with the objective to assist network architects in creating new survivor topologies.
    Copyright (C) 2015  Silvana Trindade

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
