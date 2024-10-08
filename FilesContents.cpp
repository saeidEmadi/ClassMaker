#include "FilesContents.h"

//========================================================================================================================

QString FilesContents::classPri(const QString &className, bool hasForm)
{
    QString content =
        QString(
        "INCLUDEPATH += $$PWD\n"
        "\n"
        "include($$PWD/Interface/Interface.pri)\n"
        "include($$PWD/Factory/Factory.pri)\n"
        "\n"
        "HEADERS += \\\n"
        "    $$PWD/%1.h\n"
        "\n"
        "SOURCES += \\\n"
        "    $$PWD/%1.cpp\n"
        )
        .arg(className);

    if (hasForm)
    {
        content +=
            QString(
            "\n"
            "FORMS += \\\n"
            "    $$PWD/%1.ui"
            )
            .arg(className);
    }

    return content;
}

//========================================================================================================================

QString FilesContents::classHeader(const QString &className, bool hasForm, bool hasModel)
{
    QString camelCaseClassName = className;
    camelCaseClassName.replace(QString("View"), QString(""));

    QString headerInclude =
        QString(
        "#include \"I%1.h\"\n"
        ).arg(camelCaseClassName);

    QString memberValue =
        QString(
        "   I%1 *m_"
        ).arg(camelCaseClassName);

    QString setModelOverride =
        QString(
        "   void setModel(I%1 *model) override;\n"
        ).arg(camelCaseClassName);

    camelCaseClassName[0] = camelCaseClassName[0].toLower();

    memberValue.append(camelCaseClassName + ";");

    QString uiNamespace =
        QString(
        "namespace Ui\n"
        "{\n"
        "   class %1;\n"
        "}\n"
        "\n"
        ).arg(className);

    QString uiVariableDefine =
        QString(
        "private:\n"
        "   Ui::%1* ui;\n"
        ).arg(className);

    QString content =
        QString(
        "#ifndef %1_H\n"
        "#define %1_H\n"
        "\n"
        "#include \"I%2.h\"\n"
        "\n"
        "%3"
        "%4"
        "class %2 final : public I%2\n"
        "{\n"
        "public:\n"
        "    %2();\n"
        "    ~%2() override;\n"
        "\n"
        "public:\n"
        "%7\n"
        "private:\n"
        "%5\n"
        "\n"
        "%6"
        "};\n"
        "\n"
        "#endif"
        )
        .arg(className.toUpper())
        .arg(className)
        .arg(hasModel ? headerInclude : "")
        .arg(hasForm ? uiNamespace : "")
        .arg(hasModel ? memberValue : "")
        .arg(hasForm ? uiVariableDefine : "").
        arg(hasModel ? setModelOverride : "")
    ;

    return content;
}

//========================================================================================================================

QString FilesContents::classCpp(const QString &className, bool hasForm, bool hasModel)
{
    QString uiHeader =
        QString(
        "#include \"ui_%1.h\"\n"
        ).arg(className);

    QString camelCaseClassName = className;
    camelCaseClassName.replace(QString("View"), QString(""));

    if (hasModel)
    {
        uiHeader.append("\n");
        uiHeader.append("#include \"");
        uiHeader.append(camelCaseClassName);
        uiHeader.append("Factory.h\"\n");
    }

    QString uiInitialize =
        QString(
        ":\n"
        "   ui(new Ui::%1)\n"
        ).arg(className);

    QString membrValue = className;
    membrValue.replace(QString("View"), QString(""));
    membrValue[0] = membrValue[0].toLower();

    QString setModelFunction =
        QString(
        "\n"
        "\n"
        "void %1::setModel(I%2 *model)\n"
        "{\n"
        "   m_%3 = model;\n"
        "}\n"
        "\n"
        "//========================================================================================================================\n"
        "\n"
        )
        .arg(className)
        .arg(camelCaseClassName)
        .arg(membrValue);

    camelCaseClassName[0] = camelCaseClassName[0].toLower();

    if (hasModel)
    {
        uiInitialize.append("  , m_" + camelCaseClassName + "(nullptr)\n");
    }

    QString uiSetup =
        "   ui->setupUi(this);\n";

    QString uiDelete =
        "   delete ui;\n";

    QString content =
        QString(
        "#include \"%1.h\"\n"
        "%2"
        "\n"
        "//========================================================================================================================\n"
        "\n"
        "%1::%1() %3"
        "{\n"
        "%4"
        "}\n"
        "\n"
        "//========================================================================================================================\n"
        "\n"
        "%1::~%1()\n"
        "{\n"
        "%5"
        "}\n"
        "\n"
        "//========================================================================================================================"
        "%6\n"
        )
        .arg(className)
        .arg(hasForm ? uiHeader : "")
        .arg(hasForm ? uiInitialize : "\n")
        .arg(hasForm ? uiSetup : "")
        .arg(hasForm ? uiDelete : "")
        .arg(hasModel ? setModelFunction : "")
    ;

    return content;
}
//========================================================================================================================

QString FilesContents::interfacePri(const QString &className)
{
    QString content =
        QString(
        "INCLUDEPATH += $$PWD\n"
        "\n"
        "HEADERS += \\\n"
        "    $$PWD/I%1.h"
        )
        .arg(className);

    return content;
}

//========================================================================================================================

QString FilesContents::interfaceHeader(const QString &className, const QString &baseClassName, bool hasModel)
{
    QString camelCaseClassName = className;
    camelCaseClassName.replace(QString("View"), QString(""));

    QString setModel =
        QString(
        "    virtual void setModel(I%1 *model) = 0;\n"
        ).arg(camelCaseClassName);

    QString defineModelInterface =
        QString("class I%1;\n").arg(camelCaseClassName);

    QString content =
        QString(
        "#ifndef I%1_H\n"
        "#define I%1_H\n"
        "\n"
        "#include <%3>\n"
        "\n"
        "%5\n"
        "\n"
        "class I%2 : public %3\n"
        "{\n"
        "    Q_OBJECT\n"
        "public:\n"
        "    virtual ~I%2() = default;\n"
        "\n"
        "public:\n"
        "%4\n"
        "\n"
        "};\n"
        "\n"
        "#endif"
        )
        .arg(className.toUpper())
        .arg(className)
        .arg(baseClassName)
        .arg(hasModel ? setModel : "")
        .arg(hasModel ? defineModelInterface : "");

    return content;
}
//========================================================================================================================

QString FilesContents::factoryPri(const QString &className)
{
    QString content =
        QString(
        "INCLUDEPATH += $$PWD\n"
        "\n"
        "HEADERS += \\\n"
        "    $$PWD/%1Factory.h\n"
        "\n"
        "SOURCES += \\\n"
        "    $$PWD/%1Factory.cpp"
        )
        .arg(className);

    return content;
}

//========================================================================================================================

QString FilesContents::factoryHeader(const QString &className)
{
    QString content =
        QString(
        "#ifndef %1FACTORY_H\n"
        "#define %1FACTORY_H\n"
        "\n"
        "#include \"I%2.h\"\n"
        "\n"
        "class %2Factory\n"
        "{"
        "\n"
        "public:\n"
        "    static I%2 *create();\n"
        "};\n"
        "\n"
        "#endif"
        )
        .arg(className.toUpper())
        .arg(className);

    return content;
}

//========================================================================================================================

QString FilesContents::factoryCpp(const QString &className)
{
    QString content =
        QString(
        "#include \"%1Factory.h\"\n"
        "\n"
        "#include \"%1.h\"\n"
        "\n"
        "//========================================================================================================================\n"
        "\n"
        "I%1 *%1Factory::create()\n"
        "{\n"
        "    return new %1;\n"
        "}\n"
        "\n"
        "//========================================================================================================================"
        )
        .arg(className);

    return content;
}
//========================================================================================================================

QString FilesContents::mainProviderPri(const QString &className)
{
    QString content = QString(
        "include($$PWD/Provider/Provider.pri)\n"
        "include($$PWD/%1View/%1View.pri)\n"
        "include($$PWD/%1/%1.pri)\n"
        ).arg(className);

    return content;
}
//========================================================================================================================

QString FilesContents::providerPri(const QString &className)
{
    QString content = QString(
        "INCLUDEPATH += $$PWD\n"
        "\n"
        "include($$PWD/Interface/Interface.pri)\n"
        "include($$PWD/Factory/Factory.pri)\n"
        "HEADERS += \\\n"
        "    $$PWD/%1Provider.h\n"
        "\n"
        "SOURCES += \\\n"
        "    $$PWD/%1Provider.cpp\n"
        ).arg(className);

    return content;
}
//========================================================================================================================
QString FilesContents::providerHeader(const QString &className)
{
    QString camelCaseClassName = className;
    camelCaseClassName[0] = camelCaseClassName[0].toLower();

    QString content =
        QString(
        "#ifndef %1PROVIDER_H\n"
        "#define %1PROVIDER_H\n"
        "\n"
        "#include \"I%2Provider.h\"\n"
        "\n"
        "class %2Provider final : public I%2Provider\n"
        "{\n"
        "public:\n"
        "    %2Provider();\n"
        "    ~%2Provider() override;\n"
        "\n"
        "private:\n"
        "    I%2* %3() const override;\n"
        "    I%2View* %3View() const override;\n"
        "\n"
        "private:\n"
        "    I%2* m_%3;\n"
        "    I%2View* m_%3View;\n"
        "\n"
        "};\n"
        "\n"
        "#endif"
        )
        .arg(className.toUpper())
        .arg(className)
        .arg(camelCaseClassName)
    ;

    return content;
}
//========================================================================================================================
QString FilesContents::providerCpp(const QString &className)
{
    QString camelCaseClassName = className;
    camelCaseClassName[0] = camelCaseClassName[0].toLower();

    QString content = QString(
        "#include \"%1Provider.h\"\n"
        "\n"
        "#include \"%1Factory.h\"\n"
        "#include \"%1ViewFactory.h\"\n"
        "\n"
        "//========================================================================================================================\n"
        "\n"
        "%1Provider::%1Provider()\n"
        "    : m_%2(%1Factory::create())\n"
        "    , m_%2View(%1ViewFactory::create())\n"
        "{\n"
        "    m_%2View->setModel(m_%2);\n"
        "}\n"
        "\n"
        "//========================================================================================================================"
        "\n"
        "%1Provider::~%1Provider()\n"
        "{\n"
        "    delete m_%2;\n"
        "    delete m_%2View;\n"
        "}\n"
        "\n"
        "//========================================================================================================================"
        "\n"
        "I%1View* %1Provider::%2View() const \n"
        "{\n"
        "    return m_%2View;\n"
        "}\n"
        "\n"
        "//========================================================================================================================"
        "\n"
        "I%1* %1Provider::%2() const \n"
        "{\n"
        "    return m_%2;\n"
        "}\n"
        "\n"
        "//========================================================================================================================"
        ).arg(className)
        .arg(camelCaseClassName);

    return content;
}
//========================================================================================================================

QString FilesContents::providerInterfacePri(const QString &className)
{
    QString content = QString(
        "INCLUDEPATH += $$PWD\n"
        "\n"
        "HEADERS += \\\n"
        "    $$PWD/I%1Provider.h\n"
        ).arg(className);
    return content;
}
//========================================================================================================================

QString FilesContents::providerInterfaceHeader(const QString &className)
{
    QString camelCaseClassName = className;
    camelCaseClassName[0] = camelCaseClassName[0].toLower();

    QString content =
        QString(
        "#ifndef I%1PROVIDER_H\n"
        "#define I%1PROVIDER_H\n"
        "\n"
        "#include <QObject>\n"
        "\n"
        "#include \"I%2.h\"\n"
        "#include \"I%2View.h\"\n"
        "\n"
        "class I%2Provider : public QObject\n"
        "{\n"
        "public:\n"
        "    virtual ~I%2Provider() = default;\n"
        "\n"
        "    virtual I%2* %3() const = 0;\n"
        "    virtual I%2View* %3View() const = 0;\n"
        "};\n"
        "\n"
        "#endif"
        )
        .arg(className.toUpper())
        .arg(className)
        .arg(camelCaseClassName);

    return content;
}
//========================================================================================================================

QString FilesContents::providerFactoryPri(const QString &className)
{
    QString content = QString(
        "INCLUDEPATH += $$PWD\n"
        "\n"
        "HEADERS += \\\n"
        "    $$PWD/%1ProviderFactory.h\n"
        "\n"
        "SOURCES += \\\n"
        "    $$PWD/%1ProviderFactory.cpp\n"
        ).arg(className);
    return content;
}
//========================================================================================================================

QString FilesContents::providerFactoryHeader(const QString &className)
{
    QString content =
        QString(
        "#ifndef %1FACTORY_H\n"
        "#define %1FACTORY_H\n"
        "\n"
        "#include \"I%2Provider.h\"\n"
        "\n"
        "class %2ProviderFactory\n"
        "{\n"
        "public:\n"
        "    static I%2Provider* create();\n"
        "\n"
        "};\n"
        "\n"
        "#endif"
        )
        .arg(className.toUpper())
        .arg(className);
    return content;
}
//========================================================================================================================

QString FilesContents::providerFactoryCpp(const QString &className)
{
    QString content = QString(
        "#include \"%1ProviderFactory.h\"\n"
        "\n"
        "#include \"%1Provider.h\"\n"
        "\n"
        "//========================================================================================================================\n"
        "I%1Provider *%1ProviderFactory::create()\n"
        "{\n"
        "    return new %1Provider;\n"
        "}\n"
        "\n"
        "//========================================================================================================================\n"
        ).arg(className);
    return content;
}

//========================================================================================================================

QString FilesContents::classUi(const QString &className, const QString &baseClassName)
{
    QString content =
        QString(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<ui version=\"4.0\">\n"
        " <class>%1</class>\n"
        " <widget class=\"%2\" name=\"%1\">\n"
        "  <property name=\"geometry\">\n"
        "   <rect>\n"
        "    <x>0</x>\n"
        "    <y>0</y>\n"
        "    <width>400</width>\n"
        "    <height>300</height>\n"
        "   </rect>\n"
        "</property>\n"
        "<property name=\"windowTitle\">\n"
        " <string>%1</string>\n"
        "</property>\n"
        " </widget>\n"
        " <resources/>\n"
        " <connections/>\n"
        "</ui>\n"
        )
        .arg(className)
        .arg(baseClassName);

    if (baseClassName == "QMainWindow")
    {
        content.replace(" </widget>\n", "<widget class=\"QWidget\" name=\"centralwidget\"/>\n </widget>\n");
    }

    return content;
}
//========================================================================================================================

QString FilesContents::resource()
{
    QString content = QString(
        "<RCC>"
        "<qresource prefix=\"/\">"
        "</qresource>"
        "</RCC>"
        );

    return content;
}

//========================================================================================================================
