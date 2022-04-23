#ifndef LATTICEEXAMPLE_OBJWRITER_H
#define LATTICEEXAMPLE_OBJWRITER_H

#include <QString>
#include <src/geometry/Geometry.h>

class ObjWriter {

public:
    static bool writeGeometry(QString &path, Geometry &geometry, QString *errorMessage);
};


#endif //LATTICEEXAMPLE_OBJWRITER_H
