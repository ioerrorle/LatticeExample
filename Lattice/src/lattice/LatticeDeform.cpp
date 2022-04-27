#include <src/geometry/GeoTools.h>
#include "LatticeDeform.h"
#include <QDebug>
#include <cmath>
#include <QElapsedTimer>

Geometry LatticeDeform::deform(Geometry restBody, Geometry deformedBody, Geometry bodyToDeform, float radius) {
    QVector<QVector3D> deformedGeometryVertices(bodyToDeform.vertices);
    deformedGeometryVertices.detach();
    QVector<QVector2D> deformedGeometryTextureVertices(bodyToDeform.textureVertices);
    QVector<QVector<int>> deformedGeometryFaceVertexIndices(bodyToDeform.faceVertexIndices);
    QVector<QVector<int>> deformedGeometryFaceTextureVertexIndices(bodyToDeform.faceTextureVertexIndices);

    Node *tree = restBody.getKdTree();

    QElapsedTimer timer;
    timer.start();

    bool result = deformVertices(restBody.vertices, tree, deformedBody.vertices, bodyToDeform.vertices, deformedGeometryVertices, radius*radius);

    qDebug() << "Lattice took" << timer.elapsed() << "milliseconds";

    if (result)
        return Geometry(deformedGeometryVertices, deformedGeometryTextureVertices, deformedGeometryFaceVertexIndices, deformedGeometryFaceTextureVertexIndices);
    else
        return Geometry();
}

bool LatticeDeform::deformVertices(const QVector<QVector3D> &restBodyVertices, Node *tree, const QVector<QVector3D> &deformedBodyVertices, const QVector<QVector3D> &bodyToDeformVertices, QVector<QVector3D> &deformedGeometryVertices, float radiusSquared) {
    if (restBodyVertices.size() != deformedBodyVertices.size())
        return false;

    if (bodyToDeformVertices.size() != deformedGeometryVertices.size())
        return false;

    for (int currentVertexIndexToDeform = 0;
        currentVertexIndexToDeform < bodyToDeformVertices.size(); currentVertexIndexToDeform++) {
        QVector3D currentVertexToDeform = bodyToDeformVertices[currentVertexIndexToDeform];

        QVector<int> forceVertexIndices;
        QVector<QVector3D> forceVertices;

        tree->findVerticesInRadius(currentVertexToDeform, radiusSquared, forceVertexIndices, forceVertices);

        QVector<float> weightsForForceVertices = calculateTransformationWeights(currentVertexToDeform, forceVertices, radiusSquared);
        for (int i = 0; i < forceVertexIndices.size(); i++) {
            int forceVertexIndex = forceVertexIndices[i];
            QVector3D forceVertex = restBodyVertices[forceVertexIndex];
            QVector3D deformedForceVertex = deformedBodyVertices[forceVertexIndex];
            float transformWeight = weightsForForceVertices[i];
            QVector3D transformationVector = (forceVertex - deformedForceVertex) * transformWeight;
            deformedGeometryVertices[currentVertexIndexToDeform] -= transformationVector;
        }
    }
    return true;
}

QVector<float> LatticeDeform::calculateTransformationWeights(const QVector3D &baseVertex, const QVector<QVector3D> &verticesToTransform, float metaballRadiusSquared) {
    QVector<float> result;
    if (metaballRadiusSquared == 0)
        return QVector<float>(verticesToTransform.size(), 0);
    float sumWeight = 0;
    for (QVector3D vertexToTransform : verticesToTransform) {
        float distanceSquaredNormalized = (baseVertex - vertexToTransform).lengthSquared()/metaballRadiusSquared;
        float weight = exp(-9 * distanceSquaredNormalized);

        result.append(weight);
        sumWeight += weight;
    }
    if (sumWeight == 0)
        return QVector<float>(verticesToTransform.size(), 0);

    for (int i = 0; i < result.size(); i++) {
        result[i] = result[i] / sumWeight;
    }
    return result;
}

//blinn
//QVector<float> LatticeDeform::calculateTransformationWeights(const QVector3D &baseVertex, const QVector<QVector3D> &verticesToTransform, float metaballRadiusSquared) {
//    QVector<float> result;
//    float sumWeight = 0;
//    for (QVector3D vertexToTransform : verticesToTransform) {
//        float distanceSquaredNormalized = (baseVertex - vertexToTransform).lengthSquared()/metaballRadiusSquared;
//        float weight = exp(-9 * distanceSquaredNormalized);

//        result.append(weight);
//        sumWeight += weight;
//    }

//    for (int i = 0; i < result.size(); i++) {
//        result[i] = result[i] / sumWeight;
//    }
//    return result;
//}

//renderman
//QVector<float> LatticeDeform::calculateTransformationWeights(const QVector3D &baseVertex, const QVector<QVector3D> &verticesToTransform, float metaballRadiusSquared) {
//    QVector<float> result;
//    float sumWeight = 0;
//    for (QVector3D vertexToTransform : verticesToTransform) {
//        float distanceSquaredNormalized = (baseVertex - vertexToTransform).lengthSquared() / metaballRadiusSquared;
//        float weight = 1 - 3*distanceSquaredNormalized + 3*pow(distanceSquaredNormalized, 2) - pow(distanceSquaredNormalized, 3);

//        result.append(weight);
//        sumWeight += weight;
//    }

//    for (int i = 0; i < result.size(); i++) {
//        result[i] = result[i] / sumWeight;
//    }
//    return result;
//}


//LINKS
//QVector<float> LatticeDeform::calculateTransformationWeights(const QVector3D &baseVertex, const QVector<QVector3D> &verticesToTransform, float metaballRadius) {
//    QVector<float> result;
//    float radDiv9 = metaballRadius / 9;
//    float sumWeight = 0;
//    for (QVector3D vertexToTransform : verticesToTransform) {
//        float distanceSquared = (baseVertex - vertexToTransform).lengthSquared();
//        float weight;
//        if (distanceSquared < radDiv9) {
//            weight = 1-3*distanceSquared/metaballRadius;
//        } else {
//            weight = 1.5f*pow((1-sqrt(distanceSquared/metaballRadius)), 2);
//        }

//        result.append(weight);
//        sumWeight += weight;
//    }

//    for (int i = 0; i < result.size(); i++) {
//        result[i] = result[i] / sumWeight;
//    }
//    return result;
//}
