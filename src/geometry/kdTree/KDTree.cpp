#include <cfloat>
#include "KDTree.h"
#include "NodeLeaf.h"
#include "NodeInner.h"
#include <QDebug>

Node *KDTree::buildTree(QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int maxNumberOfPointsInLeaf) {
    if (vertexIndices.size() <= maxNumberOfPointsInLeaf) {
        return new NodeLeaf(vertexIndices, vertices);
    }

    QVector3D minVertex;
    QVector3D maxVertex;
    findRange(vertexIndices, vertices, minVertex, maxVertex);
    QVector3D sizes = minVertex - maxVertex;
    int splitAxis;
    if (sizes.x() > std::max(sizes.y(), sizes.z())) {
        splitAxis = 0;
    } else if (sizes.y() > std::max(sizes.x(), sizes.z())) {
        splitAxis = 1;
    } else {
        splitAxis = 2;
    }
    const float splitPos = minVertex[splitAxis] + sizes[splitAxis] / 2.0f;

    QList<int> leftVertexIndices;
    QList<int> rightVertexIndices;
    splitPoints(vertexIndices, vertices, splitAxis, splitPos, leftVertexIndices, rightVertexIndices);
    if (leftVertexIndices.isEmpty()) {
        return new NodeLeaf(vertexIndices, vertices);
    }

    const Node *childLeft = buildTree(leftVertexIndices, vertices, maxNumberOfPointsInLeaf);
    const Node *childRight = buildTree(rightVertexIndices, vertices, maxNumberOfPointsInLeaf);
    return new NodeInner(childLeft, childRight, splitAxis, splitPos);
}

void
KDTree::findRange(QList<int> &vertexIndices, const QVector<QVector3D> &vertices, QVector3D minVertex, QVector3D maxVertex) {
    QVector3D minPoint = QVector3D(FLT_MIN, FLT_MIN, FLT_MIN);
    QVector3D maxPoint = QVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
    for (int &vertexIndex : vertexIndices)
        for (int axis = 0; axis < 3; axis++) {
            const float value = vertices[vertexIndex][axis];
            if (value < minPoint[axis])
                minPoint[axis] = value;
            if (value > maxPoint[axis])
                maxPoint[axis] = value;
        }
    minVertex = QVector3D(minPoint[0], minPoint[1], minPoint[2]);
    maxVertex = QVector3D(maxPoint[0], maxPoint[1], maxPoint[2]);
}

void
KDTree::splitPoints(QList<int> &vertexIndices, const QVector<QVector3D> &vertices, int axis, const float axisPos, QList<int> leftVertexIndices,
                    QList<int> rightVertexIndices) {
    leftVertexIndices.clear();
    rightVertexIndices.clear();

    for (int vertexIndex : vertexIndices) {
        if (vertices[vertexIndex][axis] < axisPos) {
            leftVertexIndices.append(vertexIndex);
        } else {
            rightVertexIndices.append(vertexIndex);
        }
    }
}
