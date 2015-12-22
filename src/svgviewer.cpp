#include "svgviewer.h"
#include "ui_svgviewer.h"
#include <QtWidgets>

SVGViewer::SVGViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SVGViewer),
    svg(new SvgView)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(svg);
}

SVGViewer::~SVGViewer()
{
    delete ui;
}

void SVGViewer::openSVG(string file) {
    QFile f(QString::fromStdString(file));
    svg->openFile(f);
}
