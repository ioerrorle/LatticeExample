#ifndef LATTICEEXAMPLE_LATTICEDEFORM_H
#define LATTICEEXAMPLE_LATTICEDEFORM_H

#include "src/geometry/Geometry.h"
#include <QVector>
#include <QVector3D>

class LatticeDeform {

public:
    static Geometry deform(Geometry restBody, Geometry deformedBody, Geometry bodyToDeform, float radius);

protected:
    static QVector<float> calculateTransformationWeights(QVector3D &baseVertex, float metaballRadius, QVector<QVector3D> &verticesToTransform);
};


#endif //LATTICEEXAMPLE_LATTICEDEFORM_H
