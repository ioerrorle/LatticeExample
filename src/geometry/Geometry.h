#ifndef LATTICEEXAMPLE_GEOMETRY_H
#define LATTICEEXAMPLE_GEOMETRY_H

#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <src/geometry/kdTree/Node.h>

class Geometry {

public:

    Geometry();

    Geometry(QVector<QVector3D> &vertices, QVector<QVector2D> &textureVertices,
             QVector<QVector<int>> &faceVertexIndices, QVector<QVector<int>> &faceTextureVertexIndices);
    Node *getKdTree();

    QVector<QVector3D> vertices;
    QVector<QVector2D> textureVertices;
    QVector<QVector<int>> faceVertexIndices;
    QVector<QVector<int>> faceTextureVertexIndices;
private:
    Node *kdTree = nullptr;

};


#endif //LATTICEEXAMPLE_GEOMETRY_H
