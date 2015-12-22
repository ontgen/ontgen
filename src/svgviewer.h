#ifndef SVGVIEWER_H
#define SVGVIEWER_H

#include <QDialog>

namespace Ui {
class SVGViewer;
}

class SVGViewer : public QDialog
{
    Q_OBJECT

public:
    explicit SVGViewer(QWidget *parent = 0);
    ~SVGViewer();

private:
    Ui::SVGViewer *ui;
};

#endif // SVGVIEWER_H
