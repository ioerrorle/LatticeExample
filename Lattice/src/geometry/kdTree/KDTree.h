#ifndef LATTICEEXAMPLE_KDTREE_H
#define LATTICEEXAMPLE_KDTREE_H

#include "Node.h"
#include <QVector>
#include <QVector3D>

class KDTree {
public:
    static Node *buildTree(const QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int maxNumberOfPointsInLeaf, int *numOfLeaves = nullptr);

    static void
    findRange(const QList<int> &vertexIndices, const QVector<QVector3D> &vertices, QVector3D &minVertex, QVector3D &maxVertex);

    static void
    splitPoints(const QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int axis, const float axisPos, QList<int> &leftVertexIndices,
                QList<int> &rightVertexIndices);
};


#endif //LATTICEEXAMPLE_KDTREE_H
