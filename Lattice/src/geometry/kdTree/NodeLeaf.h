#ifndef LATTICEEXAMPLE_NODELEAF_H
#define LATTICEEXAMPLE_NODELEAF_H

#include <QVector>
#include <QList>
#include "Node.h"

class NodeLeaf : public Node {

public:
    NodeLeaf(const QList<int> &vertexIndices, const QVector<QVector3D> &vertices);

    void
    findVerticesInRadius(const QVector3D &baseVertex, const float &radiusSquared,
                         QVector<int> &vertexIndices, QVector<QVector3D> &vertices) const override;

private:
    QVector<int> subsetVertexIndices;
    QVector<QVector3D> subsetVertices;
};


#endif //LATTICEEXAMPLE_NODELEAF_H
