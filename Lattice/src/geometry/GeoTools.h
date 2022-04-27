#ifndef LATTICEEXAMPLE_GEOTOOLS_H
#define LATTICEEXAMPLE_GEOTOOLS_H

#include <QVector3D>
#include <QVector>
#include <src/geometry/Geometry.h>

class GeoTools {
public:
    static int findClosestVertexIndex(QVector3D &baseVertex, QVector<QVector3D> &lookupGeometry, float *distance);
    static QVector<int> findVertexIndiciesInRadius(QVector3D &baseVertex, QVector<QVector3D> &lookupGeometry, float radius);

};


#endif //LATTICEEXAMPLE_GEOTOOLS_H
