#include <src/geometry/GeoTools.h>
#include "LatticeDeform.h"
#include <QDebug>
#include <cmath>

Geometry LatticeDeform::deform(Geometry restBody, Geometry deformedBody, Geometry bodyToDeform, float radius) {
    QVector<QVector3D> deformedGeometryVertices(bodyToDeform.vertices);
    deformedGeometryVertices.detach();
    QVector<QVector2D> deformedGeometryTextureVertices(bodyToDeform.textureVertices);
    QVector<QVector<int>> deformedGeometryFaceVertexIndices(bodyToDeform.faceVertexIndices);
    QVector<QVector<int>> deformedGeometryFaceTextureVertexIndices(bodyToDeform.faceTextureVertexIndices);
    QVector<int> movedIndices;

    for (int currentVertexIndexToDeform = 0;
        currentVertexIndexToDeform < bodyToDeform.vertices.size(); currentVertexIndexToDeform++) {
        QVector3D currentVertexToDeform = bodyToDeform.vertices[currentVertexIndexToDeform];

        QVector<int> forceVertexIndices;
        QVector<QVector3D> forceVertices;

        restBody.getKdTree()->findVerticesInRadius(currentVertexToDeform, pow(radius, 2), forceVertexIndices, forceVertices);

        QVector<float> weightsForForceVertices = calculateTransformationWeights(currentVertexToDeform, radius, forceVertices);
        for (int i = 0; i < forceVertexIndices.size(); i++) {
            int forceVertexIndex = forceVertexIndices[i];
            QVector3D forceVertex = restBody.vertices[forceVertexIndex];
            QVector3D deformedForceVertex = deformedBody.vertices[forceVertexIndex];
            float transformWeight = weightsForForceVertices[i];
            QVector3D transformationVector = (forceVertex - deformedForceVertex) * transformWeight;
            deformedGeometryVertices[currentVertexIndexToDeform] -= transformationVector;
        }
    }

    return Geometry(deformedGeometryVertices, deformedGeometryTextureVertices, deformedGeometryFaceVertexIndices, deformedGeometryFaceTextureVertexIndices);
}

QVector<float> LatticeDeform::calculateTransformationWeights(QVector3D &baseVertex, float metaballRadius,
                                                             QVector<QVector3D> &verticesToTransform) {
    QVector<float> result;
    float sumDistance = 0;
    for (QVector3D vertexToTransform : verticesToTransform) {
        float distance = vertexToTransform.distanceToPoint(baseVertex);
        sumDistance += distance;
    }
    for (QVector3D vertexToTransform : verticesToTransform) {
        float distance = vertexToTransform.distanceToPoint(baseVertex);
        float weight =  distance / sumDistance;
        result.append(weight);
    }
    return result;
}
