#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "ObjWriter.h"

bool ObjWriter::writeGeometry(QString &path, Geometry &geometry, QString *errorMessage) {
    QFile file(path);

    bool openFileResult = file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    if(!openFileResult) {
        *errorMessage = "Couldn't write file";
        return false;
    }
    QTextStream stream(&file);

    for (QVector3D vec : geometry.vertices) {
        stream << "v " << vec.x() << " " << vec.y() << " " << vec.z() << "\n";
    }
    for (QVector2D vecT : geometry.textureVertices) {
        stream << "vt " << vecT.x() << " " << vecT.y() << "\n";
    }
    for (int faceIndex = 0; faceIndex < geometry.faceVertexIndices.size(); faceIndex++) {
        stream << "f";
        QVector<int> faceVertexIndices = geometry.faceVertexIndices[faceIndex];
        QVector<int> faceTextureVertexIndices = geometry.faceTextureVertexIndices[faceIndex];
        for (int vertexIndex = 0; vertexIndex < faceVertexIndices.size(); vertexIndex++) {
            stream << " " << faceVertexIndices[vertexIndex] << "/" << faceTextureVertexIndices[vertexIndex];
        }
        stream << "\n";
    }
    file.close();
}
