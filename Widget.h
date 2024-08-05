#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class QDir;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_browse_clicked();
    void on_btn_create_clicked();

private:
    bool copyPath(const QString &sourceDir, const QString &destinationDir);
    void createFile(const QString &filePath, const QString &fileName, const QString &suffix, const QString &contents);
    void baseClassWriter(const QString &path, const QString &className, const bool hasForm);
    // void provideClassWriter(const QString &path, const QString &className);

private:
    Ui::Widget *ui;
};
#endif
