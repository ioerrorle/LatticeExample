#include <src/geometry/kdTree/KDTree.h>
#include "Geometry.h"
#include <QList>
#include <QDebug>
#include <QElapsedTimer>

Geometry::Geometry(QVector<QVector3D> &vertices, QVector<QVector2D> &textureVertices,
                   QVector<QVector<int>> &faceVertexIndices,
                   QVector<QVector<int>> &faceTextureVertexIndices) : vertices(vertices),
                                                                            textureVertices(textureVertices),
                                                                            faceVertexIndices(faceVertexIndices),
                                                                            faceTextureVertexIndices(
                                                                                    faceTextureVertexIndices) {}

Geometry::Geometry() {

}

Node *Geometry::getKdTree() {
    if (kdTree == nullptr) {
        QList<int> indices;
        for (int i = 0; i < vertices.size(); i++) {
            indices << i;
        }
        QElapsedTimer timer;
        timer.start();
        qDebug() << "Building of the tree started";
        kdTree = KDTree::buildTree(indices, vertices, 10);
        qDebug() << "Building of the tree ended, took" << timer.elapsed()<<"ms";
    }
    return kdTree;
}
