#include "Widget.h"
#include "ui_Widget.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "FilesContents.h"

//========================================================================================================================

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->comboBox_baseClass->addItems({ "QObject", "QWidget", "QMainWindow", "QDialog" });
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
    m_className = ui->lineEdit_className->text();

    bool hasForm = ui->comboBox_baseClass->currentIndex() > 0;

    QString path = ui->lineEdit_path->text();

    QDir destinationDirectory(path);
    destinationDirectory.mkdir(m_className);
    destinationDirectory.cd(m_className);
    destinationDirectory.mkdir("Interface");
    destinationDirectory.mkdir("Factory");

    QString classPath = path + "/" + m_className;

    createFile(classPath, m_className, "pri", FilesContents::classPri(m_className, hasForm));
    createFile(classPath, m_className, "h", FilesContents::classHeader(m_className, hasForm));
    createFile(classPath, m_className, "cpp", FilesContents::classCpp(m_className, hasForm));

    QString interfacePath = classPath + "/Interface";
    QString baseClassName = ui->comboBox_baseClass->currentText();

    createFile(interfacePath, "Interface", "pri", FilesContents::interfacePri(m_className));
    createFile(interfacePath, "I" + m_className, "h", FilesContents::interfaceHeader(m_className, baseClassName));

    QString factoryPath = classPath + "/Factory";

    createFile(factoryPath, "Factory", "pri", FilesContents::factoryPri(m_className));
    createFile(factoryPath, m_className + "Factory", "h", FilesContents::factoryHeader(m_className));
    createFile(factoryPath, m_className + "Factory", "cpp", FilesContents::factoryCpp(m_className));

    ui->lineEdit_result->setText(QString("include($$PWD/%1/%1.pri)").arg(m_className));

    if (hasForm)
    {
        createFile(classPath, m_className, "ui", FilesContents::classUi(m_className, baseClassName));
    }
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
