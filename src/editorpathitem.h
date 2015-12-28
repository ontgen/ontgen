#ifndef EDITORPATHITEM_H
#define EDITORPATHITEM_H

#include <QGraphicsPathItem>

class EditorPathItem : public QGraphicsPathItem
{
public:
    int edgeIndex;
    double distance;
};

#endif // EDITORPATHITEM_H
