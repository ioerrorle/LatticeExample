#ifndef LATTICEEXAMPLE_OBJREADER_H
#define LATTICEEXAMPLE_OBJREADER_H

#include <QVector>
#include <QVector3D>
#include <src/geometry/Geometry.h>
#include <QFile>

class ObjReader {

public:
    static bool readObj(QString &filename, QString *errorMessage, Geometry *geometry);
private:
    static QRegExp OBJ_ENTITY_RX;
    static QRegExp SPACE_RX;
    static Geometry parseObj(QFile &file);

    static bool parseVertex(QString &line, QVector3D *resultVertex);

    static bool parseTextureVertex(QString &line, QVector2D *resultTextureVertex);

    static bool parseFace(QString &line, QVector<int> *faceVertexIndices, QVector<int> *faceTextureVertexIndices);
};


#endif //LATTICEEXAMPLE_OBJREADER_H
