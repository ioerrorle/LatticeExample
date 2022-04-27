#include <cfloat>
#include "KDTree.h"
#include "NodeLeaf.h"
#include "NodeInner.h"
#include <QDebug>

Node *KDTree::buildTree(const QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int maxNumberOfPointsInLeaf, int *numOfLeaves) {
    if (vertexIndices.size() <= maxNumberOfPointsInLeaf) {
        if (numOfLeaves != nullptr)
            ++*numOfLeaves;
        return new NodeLeaf(vertexIndices, vertices);
    }

    QVector3D minVertex;
    QVector3D maxVertex;
    findRange(vertexIndices, vertices, minVertex, maxVertex);
    QVector3D sizes = maxVertex - minVertex;
    int splitAxis;
    if (sizes.x() >= std::max(sizes.y(), sizes.z())) {
        splitAxis = 0;
    } else if (sizes.y() >= std::max(sizes.x(), sizes.z())) {
        splitAxis = 1;
    } else {
        splitAxis = 2;
    }
    const float splitPos = minVertex[splitAxis] + sizes[splitAxis] / 2.0f;

    QList<int> leftVertexIndices;
    QList<int> rightVertexIndices;
    splitPoints(vertexIndices, vertices, splitAxis, splitPos, leftVertexIndices, rightVertexIndices);
    if (leftVertexIndices.isEmpty() || rightVertexIndices.isEmpty()) {
        if (numOfLeaves != nullptr)
            ++*numOfLeaves;
        return new NodeLeaf(vertexIndices, vertices);
    }

    const Node *childLeft = buildTree(leftVertexIndices, vertices, maxNumberOfPointsInLeaf, numOfLeaves);
    const Node *childRight = buildTree(rightVertexIndices, vertices, maxNumberOfPointsInLeaf, numOfLeaves);
    return new NodeInner(childLeft, childRight, splitAxis, splitPos);
}

void
KDTree::findRange(const QList<int> &vertexIndices, const QVector<QVector3D> &vertices, QVector3D &minVertex, QVector3D &maxVertex) {
    QVector3D min = QVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
    QVector3D max = QVector3D(FLT_MIN, FLT_MIN, FLT_MIN);
    for (int const &vertexIndex : vertexIndices)
        for (int axis = 0; axis < 3; axis++) {
            const float value = vertices[vertexIndex][axis];
            if (value < min[axis]) {
                min[axis] = value;
            }
            if (value > max[axis]) {
                max[axis] = value;
            }
        }
    minVertex = QVector3D(min[0], min[1], min[2]);
    maxVertex = QVector3D(max[0], max[1], max[2]);
}

void
KDTree::splitPoints(const QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int axis, const float axisPos, QList<int> &leftVertexIndices,
                    QList<int> &rightVertexIndices) {
    leftVertexIndices.clear();
    rightVertexIndices.clear();

    for (const int &vertexIndex : vertexIndices) {
        if (vertices[vertexIndex][axis] < axisPos) {
            leftVertexIndices << vertexIndex;
        } else {
            rightVertexIndices << vertexIndex;
        }
    }
}
