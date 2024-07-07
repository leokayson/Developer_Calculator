#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QCheckBox>
#include <QPushButton>
#include "calctool.h"
#include "proj_conf.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DevCalcWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initUI();

private:
    Ui::DevCalcWindow *ui;
    CalcTool *cTool;
};
#endif // MAINWINDOW_H
