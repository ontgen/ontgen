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
#include "pngviewer.h"
#include "ui_pngviewer.h"
#include <QtWidgets>
#include <QWidget>
#include <QHBoxLayout>

PNGViewer::PNGViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PNGViewer),
    tabs(new QTabWidget)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(tabs);
    this->setWindowFlags(Qt::Window);//habilita botão de fechar
}

PNGViewer::~PNGViewer()
{
    delete ui;
}

void PNGViewer::openPNG(string file, int index)
{
    QFile f(QString::fromStdString(file));
    QWidget *tab = new QWidget();
    PngView *png = new PngView();

    png->openFile(f);

    QHBoxLayout *l = new QHBoxLayout;
    tab->setLayout(l);
    l->addWidget(png);
    string t = "topology "+to_string(index);

    tabs->addTab(tab,QString::fromStdString(t));
}
