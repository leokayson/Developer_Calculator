#ifndef CALCTOOL_H
#define CALCTOOL_H

#include <QObject>
#include <QLineEdit>
#include <QDebug>
#include <QVector>
#include <QHash>
#include "ui_mainwindow.h"
#include "calcmath.h"
#include "array"
#include "proj_conf.h"

class CalcTool : public QObject
{
    Q_OBJECT
public:
    CalcTool(Ui::DevCalcWindow *ui, QObject *parent = nullptr);
    ~CalcTool();
public slots:
    void onHexTextChange(const QString &hexText);
    void onDecTextChange(const QString &decText);
    void onBinTextChange(const QString &binText);
    void calculateOutput();
    void fixLen8Output();
    void fixLen16Output();
    void formatOutput(bool outputHistory = false);
    void clearOutput();

private:
    void handleTextChange(const QString &text, const uint8_t ori_base);
    void declimateArray(QString baseStr, std::array<QString, 3> &temArr);
    void declimateText(QString &text, const uint8_t decLen, const QChar decChar);
    void prefixArray(QString baseStr, std::array<QString, 3> &temArr);
    void fixLenArray(QString baseStr, std::array<QString, 3> &temArr);
    void errorOccur();
    void spiltText(QString& newText, std::array<QString, 3> &arr);
    bool updating = false;

    Ui::DevCalcWindow *ui = nullptr;
    std::array<QString, 3> hexOriTextArr;
    std::array<QString, 3> decOriTextArr;
    std::array<QString, 3> binOriTextArr;
    QHash<QString, QHash<QString, QString>> ctConfigs;
    QVector<QChar> validOp;

};

#endif // CALCTOOL_H
