#ifndef LATTICEEXAMPLE_KDTREE_H
#define LATTICEEXAMPLE_KDTREE_H

#include "Node.h"
#include <QVector>
#include <QVector3D>

class KDTree {
public:
    static Node *buildTree(QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int maxNumberOfPointsInLeaf);
private:
    static void
    findRange(QList<int> &vertexIndices, const QVector<QVector3D> &vertices, QVector3D minVertex, QVector3D maxVertex);

    static void
    splitPoints(QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int axis, const float axisPos, QList<int> leftVertexIndices,
                QList<int> rightVertexIndices);
};


#endif //LATTICEEXAMPLE_KDTREE_H
