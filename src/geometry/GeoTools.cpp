#include "GeoTools.h"

QVector<int> GeoTools::findVertexIndiciesInRadius(QVector3D &baseVertex, QVector<QVector3D> &lookupGeometry, float radius) {
    QVector<int> result;

    for (int lookupGeometryIndex = 0; lookupGeometryIndex < lookupGeometry.length(); lookupGeometryIndex++) {
        if (lookupGeometry[lookupGeometryIndex].distanceToPoint(baseVertex) < radius) {
            result.append(lookupGeometryIndex);
        }
    }

    return result;
}
