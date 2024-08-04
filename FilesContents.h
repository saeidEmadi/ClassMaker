#ifndef FILESCONTENTS_H
#define FILESCONTENTS_H

#include <QObject>

class FilesContents
{
public:
    static QString classPri(const QString& className, bool hasForm);
    static QString classHeader(const QString& className, bool hasForm);
    static QString classCpp(const QString& className, bool hasForm);

    static QString interfacePri(const QString& className);
    static QString interfaceHeader(const QString& className, const QString& baseClassName);

    static QString factoryPri(const QString& className);
    static QString factoryHeader(const QString& className);
    static QString factoryCpp(const QString& className);

    static QString classUi(const QString& className, const QString& baseClassName);
};

#endif
