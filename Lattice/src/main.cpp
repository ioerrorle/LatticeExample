#include <iostream>
#include <QDebug>
#include <QVector3D>
#include <QVector>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <src/geometry/Geometry.h>
#include <src/geometry/GeoTools.h>
#include <src/lattice/LatticeDeform.h>
#include <src/objReader/ObjReader.h>
#include <src/objWriter/ObjWriter.h>

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

struct CommandLineArgs {
    QString restGeometryFilePath;
    QString deformedGeometryFilePath;
    QString inputGeometryFilePath;
    QString outputGeometryFilePath;
    float metaballRadius;
};

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage, CommandLineArgs *cmdLineArgs)
{
    parser.setApplicationDescription("Utility to deform geometry");
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    QCommandLineOption helpOption = parser.addHelpOption();
    QCommandLineOption versionOption = parser.addVersionOption();

    QCommandLineOption restGeometryOption = QCommandLineOption(QStringList() << "rg" << "rest-geometry",
                                          QCoreApplication::translate("main", "Path to rest geometry .obj (required)"),
                                          QCoreApplication::translate("main", "rest-geometry"));
    parser.addOption(restGeometryOption);

    QCommandLineOption deformedGeometryOption = QCommandLineOption(QStringList() << "dg" << "deformed-geometry",
                                              QCoreApplication::translate("main", "Path to deformed geometry .obj (required)"),
                                              QCoreApplication::translate("main", "deformed-geometry"));
    parser.addOption(deformedGeometryOption);

    QCommandLineOption inputGeometryOption = QCommandLineOption(QStringList() << "i" << "input",
                                           QCoreApplication::translate("main", "Path to geometry .obj to deform (required)"),
                                           QCoreApplication::translate("main", "input"));
    parser.addOption(inputGeometryOption);

    QCommandLineOption outputGeometryOption = QCommandLineOption(QStringList() << "o" << "output",
                                            QCoreApplication::translate("main", "Path to output geometry .obj (required)"),
                                            QCoreApplication::translate("main", "output"));
    parser.addOption(outputGeometryOption);

    QCommandLineOption metaballRadiusOption(QStringList() << "r" << "radius",
                                            QCoreApplication::translate("main", "Metaball radius, decimal (required)"),
                                            QCoreApplication::translate("main", "radius"));
    parser.addOption(metaballRadiusOption);

    if (!parser.parse(QCoreApplication::arguments())) {
        *errorMessage = parser.errorText();
        return CommandLineError;
    }

    if (parser.isSet(versionOption)) {
        return CommandLineVersionRequested;
    }

    if (parser.isSet(helpOption)) {
        return CommandLineHelpRequested;
    }

    bool restGeometrySet = parser.isSet(restGeometryOption);
    bool deformedGeometrySet = parser.isSet(deformedGeometryOption);
    bool inputGeometrySet = parser.isSet(inputGeometryOption);
    bool outputGeometrySet = parser.isSet(outputGeometryOption);
    bool radiusSet = parser.isSet(metaballRadiusOption);

    bool requiredParametersSet = restGeometrySet && deformedGeometrySet && inputGeometrySet && outputGeometrySet && radiusSet;

    if (!requiredParametersSet) {
        *errorMessage = "Required parameters are not set";
        return CommandLineError;
    }

    bool metaballRadiusOk;

    float metaballRadius = parser.value(metaballRadiusOption).toFloat(&metaballRadiusOk);
    if (!metaballRadiusOk) {
        *errorMessage = "Invalid metaball radius (must be float)";
        return CommandLineError;
    }

    cmdLineArgs->restGeometryFilePath = parser.value(restGeometryOption);
    cmdLineArgs->deformedGeometryFilePath = parser.value(deformedGeometryOption);
    cmdLineArgs->inputGeometryFilePath = parser.value(inputGeometryOption);
    cmdLineArgs->outputGeometryFilePath = parser.value(outputGeometryOption);

    cmdLineArgs->metaballRadius = metaballRadius;

    return CommandLineOk;
}

void printError(QString errorMessage) {
    fputs(qPrintable(errorMessage), stderr);
    fputs("\n\n", stderr);
}

int main(int argCount, char *argv[]) {
    QCoreApplication app(argCount, argv);
    QCoreApplication::setApplicationName("Lattice example");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    QString errorMessage;
    CommandLineArgs args;
    CommandLineParseResult commandLineParseResult = parseCommandLine(parser, &errorMessage, &args);

    switch (commandLineParseResult) {
        case CommandLineOk:
            break;
        case CommandLineError:
            printError(errorMessage);
            fputs(qPrintable(parser.helpText()), stderr);
            return 1;
        case CommandLineVersionRequested:
            printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
                   qPrintable(QCoreApplication::applicationVersion()));
            return 0;
        case CommandLineHelpRequested:
            parser.showHelp();
            Q_UNREACHABLE();
    }

    Geometry restGeometry;
    QString parseErrorMessage;
    bool parseResult = ObjReader::readObj(args.restGeometryFilePath, &parseErrorMessage, &restGeometry);
    if (!parseResult) {
        printError(parseErrorMessage);
        return 1;
    }

    Geometry deformedGeometry;
    parseResult = ObjReader::readObj(args.deformedGeometryFilePath, &parseErrorMessage, &deformedGeometry);
    if (!parseResult) {
        printError(parseErrorMessage);
        return 1;
    }

    bool sameVerticesCount = restGeometry.vertices.size() == deformedGeometry.vertices.size();
    bool sameTextureVerticesCount = restGeometry.textureVertices.size() == deformedGeometry.textureVertices.size();

    if (!sameVerticesCount || !sameTextureVerticesCount) {
        errorMessage = "Vertices count of rest and deformed models do not match.";
        printError(errorMessage);
        return 1;
    }

    Geometry inputGeometry;
    parseResult = ObjReader::readObj(args.inputGeometryFilePath, &parseErrorMessage, &inputGeometry);
    if (!parseResult) {
        printError(parseErrorMessage);
        return 1;
    }

    Geometry outputGeometry = LatticeDeform::deform(restGeometry, deformedGeometry, inputGeometry, args.metaballRadius);
    bool writeResult = ObjWriter::writeGeometry(args.outputGeometryFilePath, outputGeometry, &parseErrorMessage);
    if (!writeResult) {
        printError(parseErrorMessage);
        return 1;
    }

    return 0;
}
