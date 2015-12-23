#include "svgviewer.h"
#include "ui_svgviewer.h"
#include <QtWidgets>
#include <QWidget>
#include <QHBoxLayout>

SVGViewer::SVGViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SVGViewer),
    tabs(new QTabWidget)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(tabs);
}

SVGViewer::~SVGViewer()
{
    delete ui;
}

void SVGViewer::openSVG(string file, int index) {
    QFile f(QString::fromStdString(file));
    QWidget *tab = new QWidget();
    SvgView *svg = new SvgView();
    svg->openFile(f);
    QHBoxLayout *l = new QHBoxLayout;
    tab->setLayout(l);
    l->addWidget(svg);
    string t = "topology "+to_string(index);

    tabs->addTab(tab,QString::fromStdString(t));
}
