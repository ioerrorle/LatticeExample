#include "NodeLeaf.h"
NodeLeaf::NodeLeaf(QList<int> &vertexIndices, const QVector<QVector3D> &vertices) : subsetVertexIndices(vertexIndices.toVector()) {
    const int size = subsetVertexIndices.size();
    subsetVertices.resize(size);
    for (int i = 0; i < size; i++) {
        subsetVertices[i] = vertices[subsetVertexIndices[i]];
    }
}

void NodeLeaf::findVerticesInRadius(const QVector3D &baseVertex,
                                    const float &radiusSquared, QVector<int> &vertexIndices, QVector<QVector3D> &vertices) const {
    const int subsetVerticesSize = subsetVertices.size();
    for (int vertexIndex = 0; vertexIndex < subsetVerticesSize; vertexIndex++) {
        const float currentRadiusSquared = (subsetVertices[vertexIndex] - baseVertex).lengthSquared();
        if (currentRadiusSquared <= radiusSquared) {
            vertexIndices.append(subsetVertexIndices[vertexIndex]);
            vertices.append(subsetVertices[vertexIndex]);
        }
    }
}
