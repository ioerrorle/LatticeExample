#ifndef LATTICEEXAMPLE_LATTICEDEFORM_H
#define LATTICEEXAMPLE_LATTICEDEFORM_H

#include "src/geometry/Geometry.h"
#include <QVector>
#include <QVector3D>

class LatticeDeform {

public:
    static Geometry deform(Geometry restBody, Geometry deformedBody, Geometry bodyToDeform, float radius);
    static bool deformVertices(const QVector<QVector3D> &restBodyVertices, Node *tree, const QVector<QVector3D> &deformedBodyVertices, const QVector<QVector3D> &bodyToDeformVertices, QVector<QVector3D> &deformedGeometryVertices, float radiusSquared);

protected:
    static QVector<float> calculateTransformationWeights(const QVector3D &baseVertex, const QVector<QVector3D> &verticesToTransform, float metaballRadius);
};


#endif //LATTICEEXAMPLE_LATTICEDEFORM_H
