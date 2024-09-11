#ifndef FILESCONTENTS_H
#define FILESCONTENTS_H

#include <QObject>

class FilesContents
{
public:
    static QString classPri(const QString &className, bool hasForm);
    static QString classHeader(const QString &className, bool hasForm, bool hasModel);
    static QString classCpp(const QString &className, bool hasForm, bool hasModel);

    static QString interfacePri(const QString &className);
    static QString interfaceHeader(const QString &className, const QString &baseClassName, bool hasModel);

    static QString factoryPri(const QString &className);
    static QString factoryHeader(const QString &className);
    static QString factoryCpp(const QString &className);

    static QString mainProviderPri(const QString &className);

    static QString providerPri(const QString &className);
    static QString providerHeader(const QString &className);
    static QString providerCpp(const QString &className);

    static QString providerInterfacePri(const QString &className);
    static QString providerInterfaceHeader(const QString &className);

    static QString providerFactoryPri(const QString &className);
    static QString providerFactoryHeader(const QString &className);
    static QString providerFactoryCpp(const QString &className);

    static QString classUi(const QString &className, const QString &baseClassName);

    static QString resource();
};

#endif
