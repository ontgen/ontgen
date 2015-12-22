#include "svgviewer.h"
#include "ui_svgviewer.h"

SVGViewer::SVGViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SVGViewer)
{
    ui->setupUi(this);
}

SVGViewer::~SVGViewer()
{
    delete ui;
}
