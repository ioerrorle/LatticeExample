#include <QTextStream>
#include "ObjReader.h"
#include <QDebug>
#include <QVector2D>

QRegExp ObjReader::OBJ_ENTITY_RX = QRegExp("^\\s*(vt|vn|v|f|o|g)\\s+(.+)");
QRegExp ObjReader::SPACE_RX = QRegExp("\\s+");

bool ObjReader::readObj(QString &filename, QString *errorMessage, Geometry *geometry) {
    QFile file(filename);

    bool openFileResult = file.open(QFile::ReadOnly);
    if(!openFileResult) {
        *errorMessage = "Cannot open file " + filename;
        return false;
    }

    *geometry = parseObj(file);

    return true;
}

Geometry ObjReader::parseObj(QFile &file) {
    Geometry result;
    QTextStream stream(&file);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        int strPos = 0;
        if ((strPos = OBJ_ENTITY_RX.indexIn(line, strPos)) != -1) {
            //found entity
            QString token = OBJ_ENTITY_RX.cap(1);
            if (token == "v") { //it's a vertex
                QVector3D vertex;
                bool parseResult = parseVertex(line, &vertex);
                if (parseResult) {
                    result.vertices.append(vertex);
                } else {
                    qDebug() << "Error reading line" << line;
                }
            }
            if (token == "vt") { //it's a texture vertex
                QVector2D textureVertex;
                bool parseResult = parseTextureVertex(line, &textureVertex);
                if (parseResult) {
                    result.textureVertices.append(textureVertex);
                } else {
                    qDebug() << "Error reading line" << line;
                }
            }
            if (token == "f") { //it's a face
                QVector<int> faceVertexIndices;
                QVector<int> faceTextureVertexIndices;
                bool parseResult = parseFace(line, &faceVertexIndices, &faceTextureVertexIndices);
                if (parseResult) {
                    result.faceVertexIndices.append(faceVertexIndices);
                    result.faceTextureVertexIndices.append(faceTextureVertexIndices);
                } else {
                    qDebug() << "Error reading line" << line;
                }
            }
        }
    }

    return result;
}

bool ObjReader::parseVertex(QString &line, QVector3D *resultVertex) {
    QStringList params = line.split(SPACE_RX);
    bool ok;
    float x = params[1].toFloat(&ok);
    if (!ok) return false;
    float y = params[2].toFloat(&ok);
    if (!ok) return false;
    float z = params[3].toFloat(&ok);
    if (!ok) return false;
    *resultVertex = QVector3D(x,y,z);
    return true;
}

bool ObjReader::parseTextureVertex(QString &line, QVector2D *resultTextureVertex) {
    QStringList params = line.split(SPACE_RX);
    bool ok;
    float x = params[1].toFloat(&ok);
    if (!ok) return false;
    float y = params[2].toFloat(&ok);
    if (!ok) return false;
    *resultTextureVertex = QVector2D(x,y);
    return true;
}

bool ObjReader::parseFace(QString &line, QVector<int> *faceVertexIndices, QVector<int> *faceTextureVertexIndices) {
    QStringList params = line.split(SPACE_RX);
    bool ok;
    for (int i = 1; i < params.size(); i++) {
        QStringList s_indicesForFaceVertex = params[i].split("/");
        if (s_indicesForFaceVertex.empty()) {
            return false;
        }
        if (!s_indicesForFaceVertex.empty()) {
            int vertexIndex = s_indicesForFaceVertex[0].toInt(&ok);
            if (!ok)
                return false;
            faceVertexIndices->append(vertexIndex);
        }
    }
    return true;
}
