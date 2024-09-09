#include "Widget.h"
#include "ui_Widget.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QClipboard>
#include <QTimer>

#include "FilesContents.h"

//========================================================================================================================

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->comboBox_baseClass->addItems({ "QObject", "QWidget", "QMainWindow", "QDialog", "ModelView" });
}

//========================================================================================================================

Widget::~Widget()
{
    delete ui;
}

//========================================================================================================================

void Widget::on_btn_browse_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory();

    if (filePath != "")
    {
        ui->lineEdit_path->setText(filePath);
    }
}
//========================================================================================================================

void Widget::on_btn_create_clicked()
{
    ui->lineEdit_className->setText(ui->lineEdit_className->text().trimmed());

    if (ui->lineEdit_path->text().isEmpty() ||
        ui->lineEdit_className->text().isEmpty())
    {
        QMessageBox::warning(this, "Invalid Input Value", "Class name and path cannot be empty.", QMessageBox::Button::Ok, QMessageBox::Icon::Critical);
        return;
    }

    if (ui->lineEdit_className->text().at(0) < QChar('A'))
    {
        QMessageBox::warning(this, "Invalid Input Value", "Class name cant Start with number.", QMessageBox::Button::Ok, QMessageBox::Icon::Critical);
        return;
    }

    QDir dir(ui->lineEdit_path->text());

    if (!dir.exists())
    {
        QMessageBox::warning(this, "Invalid Input Value", "This Path not exists.", QMessageBox::Button::Ok, QMessageBox::Icon::Critical);
        return;
    }

    for (const QFileInfo &file : dir.entryInfoList(QDir::Dirs))
    {
        if (file.fileName() == ui->lineEdit_className->text())
        {
            QMessageBox::warning(this, "Invalid Input Value", "This Class exists.", QMessageBox::Button::Ok, QMessageBox::Icon::Critical);
            return;
        }
    }

    QString path = ui->lineEdit_path->text();
    QString baseClassName = ui->lineEdit_className->text();

    if (ui->comboBox_baseClass->currentText() != "ModelView")
    {
        bool hasForm = ui->comboBox_baseClass->currentIndex() > 0;

        baseClassWriter(path, baseClassName, ui->comboBox_baseClass->currentText(), hasForm);
    }
    else
    {
        provideClassWriter(path, baseClassName);

        if (ui->checkBox_Resource->isChecked())
        {
            resourceWriter(path, baseClassName);
        }
    }

    if (ui->checkBox_Resource->isChecked())
    {
        resourceWriter(path, baseClassName);

        if (ui->comboBox_baseClass->currentText() != "ModelView")
        {
            bool hasForm = ui->comboBox_baseClass->currentIndex() > 0;
            createFile(path + "/" + baseClassName, baseClassName,
                "pri", FilesContents::classPri(baseClassName, hasForm).append("\n\ninclude($$PWD/Resources/Resources.pri)\n"));
        }
        else
        {
            createFile(path + "/" + baseClassName, baseClassName,
                "pri", FilesContents::mainProviderPri(baseClassName).append("\n\ninclude($$PWD/Resources/Resources.pri)\n"));
        }
    }

    ui->lineEdit_result->setText(QString("include($$PWD/%1/%1.pri)").arg(baseClassName));
}

//========================================================================================================================

void Widget::createFile(const QString &filePath, const QString &fileName, const QString &suffix, const QString &contents)
{
    QFile file(filePath + "/" + fileName + "." + suffix);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    file.write(contents.toLatin1());

    file.close();
}
//========================================================================================================================

void Widget::baseClassWriter(const QString &path, const QString &className, const QString &baseClassName, const bool hasForm)
{
    QDir destinationDirectory(path);

    destinationDirectory.mkdir(className);
    destinationDirectory.cd(className);

    QString classPath = path + "/" + className;

    destinationDirectory.mkdir("Interface");
    destinationDirectory.mkdir("Factory");

    createFile(classPath, className, "pri", FilesContents::classPri(className, hasForm));
    createFile(classPath, className, "h", FilesContents::classHeader(className, hasForm));
    createFile(classPath, className, "cpp", FilesContents::classCpp(className, hasForm));

    QString interfacePath = classPath + "/Interface";

    createFile(interfacePath, "Interface", "pri", FilesContents::interfacePri(className));
    createFile(interfacePath, "I" + className, "h", FilesContents::interfaceHeader(className, baseClassName));

    QString factoryPath = classPath + "/Factory";

    createFile(factoryPath, "Factory", "pri", FilesContents::factoryPri(className));
    createFile(factoryPath, className + "Factory", "h", FilesContents::factoryHeader(className));
    createFile(factoryPath, className + "Factory", "cpp", FilesContents::factoryCpp(className));

    if (hasForm)
    {
        createFile(classPath, className, "ui", FilesContents::classUi(className, baseClassName));
    }
}

//========================================================================================================================

void Widget::resourceWriter(const QString &path, const QString &className)
{
    QDir destinationDirectory(path);
    destinationDirectory.cd(className);
    destinationDirectory.mkdir("Resources");

    createFile(path + "/" + className + "/Resources", "Resources", "pri", QString("RESOURCES += $$PWD/%1Resources.qrc").arg(className));
    createFile(path + "/" + className + "/Resources", className + "Resources", "qrc", FilesContents::resource());
}
//========================================================================================================================

void Widget::provideClassWriter(const QString &path, const QString &className)
{
    QDir destinationDirectory(path);

    destinationDirectory.mkdir(className);
    destinationDirectory.cd(className);

    destinationDirectory.mkdir(QString("Provider"));

    QString _path = path + "/" + className;

    createFile(_path, className, "pri", FilesContents::mainProviderPri(className));

    createFile(_path + "/Provider", "Provider", "pri", FilesContents::providerPri(className));

    destinationDirectory.cd("Provider");

    destinationDirectory.mkdir("Interface");
    destinationDirectory.mkdir("Factory");

    QString providerInterfacePath = _path + "/Provider/Interface";

    createFile(providerInterfacePath, "Interface", "pri", FilesContents::providerInterfacePri(className));
    createFile(providerInterfacePath, "I" + className + "Provider", "h", FilesContents::providerInterfaceHeader(className));

    QString providerFactoryPath = _path + "/Provider/Factory";
    createFile(providerFactoryPath, "Factory", "pri", FilesContents::providerFactoryPri(className));
    createFile(providerFactoryPath, className + "ProviderFactory", "h", FilesContents::providerFactoryHeader(className));
    createFile(providerFactoryPath, className + "ProviderFactory", "cpp", FilesContents::providerFactoryCpp(className));

    createFile(_path + "/Provider", className + "Provider", "h", FilesContents::providerHeader(className));
    createFile(_path + "/Provider", className + "Provider", "cpp", FilesContents::providerCpp(className));

    baseClassWriter(_path, className, "QObject", false);
    baseClassWriter(_path, className + "View", "QWidget", true);
}

//========================================================================================================================

void Widget::on_btn_copy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->lineEdit_result->text());

    ui->btn_copy->setStyleSheet("background-color:green;color:white;");

    QTimer::singleShot(500, this, &Widget::onTimeout);
}
//========================================================================================================================

void Widget::onTimeout()
{
    ui->btn_copy->setStyleSheet("");
}

//========================================================================================================================
