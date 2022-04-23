#include <cmath>
#include "NodeInner.h"

void NodeInner::findVerticesInRadius(const QVector3D &baseVertex, const float &radiusSquared, QVector<int> &vertexIndices, QVector<QVector3D> &vertices) const {
    const Node *childToSearch;
    const Node *otherChild;
    if (baseVertex[splitAxis] < splitPos) {
        childToSearch = childLeft;
        otherChild = childRight;
    } else {
        childToSearch = childRight;
        otherChild = childLeft;
    }
    childToSearch->findVerticesInRadius(baseVertex, radiusSquared, vertexIndices, vertices);
    const float distanceToPlaneSquared = float(pow(splitPos - baseVertex[splitAxis], 2));
    if (distanceToPlaneSquared > radiusSquared)
        return;
    otherChild->findVerticesInRadius(baseVertex, radiusSquared, vertexIndices, vertices);
}

NodeInner::NodeInner(const Node *leftChild, const Node *rightChild, int splitAxis, const float splitPos) {
    this->childLeft = leftChild;
    this->childRight = rightChild;
    this->splitAxis = splitAxis;
    this->splitPos = splitPos;
}
