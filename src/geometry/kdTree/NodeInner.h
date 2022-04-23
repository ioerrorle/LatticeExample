#ifndef LATTICEEXAMPLE_NODEINNER_H
#define LATTICEEXAMPLE_NODEINNER_H

#include "Node.h"

class NodeInner : public Node {
public:
    NodeInner(const Node *leftChild, const Node *rightChild, int splitAxis, const float splitPos);

    void
    findVerticesInRadius(const QVector3D &baseVertex, const float &radiusSquared,
                         QVector<int> &vertexIndices, QVector<QVector3D> &vertices) const override;

protected:
    const Node *childLeft;
    const Node *childRight;

    int splitAxis;
    float splitPos;
};


#endif //LATTICEEXAMPLE_NODEINNER_H
