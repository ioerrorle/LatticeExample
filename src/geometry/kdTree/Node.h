#ifndef LATTICEEXAMPLE_NODE_H
#define LATTICEEXAMPLE_NODE_H


#include <QtGui/QVector3D>

class Node {

public:
    virtual void findVerticesInRadius(const QVector3D &baseVertex, const float &radiusSquared, QVector<int> &vertexIndices, QVector<QVector3D> &vertices) const = 0;
};


#endif //LATTICEEXAMPLE_NODE_H
