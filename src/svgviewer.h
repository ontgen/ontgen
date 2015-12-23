#ifndef SVGVIEWER_H
#define SVGVIEWER_H

#include <QDialog>
#include <string>
#include <QTabWidget>
#include "svgview.h"

using namespace std;

namespace Ui {
class SVGViewer;
}

class SVGViewer : public QDialog
{
    Q_OBJECT

public:
    explicit SVGViewer(QWidget *parent = 0);
    void openSVG(string, int);
    ~SVGViewer();
    QTabWidget *tabs;

private:
    Ui::SVGViewer *ui;
};

#endif // SVGVIEWER_H
