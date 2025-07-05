#ifndef CALCTOOL_H
#define CALCTOOL_H

#include <QDebug>
#include <QHash>
#include <QLineEdit>
#include <QObject>
#include <QVector>
#include "ui_mainwindow.h"

class CalcTool : public QObject {
public:
    CalcTool( Ui::DevCalcWindow *ui, QObject *parent = nullptr );
    ~CalcTool();
public slots:
    void onHexTextChange( const QString &hexText );
    void onDecTextChange( const QString &decText );
    void onBinTextChange( const QString &binText );
    void calculateOutput();
    void fixLen8Output();
    void fixLen16Output();
    void formatOutput( bool outputHistory = false );
    void clearOutput();

private:
    void handleTextChange( const QString &text, int base );
    QString formatFormula( QString &formula, int base, bool b_fix_len, bool b_declimate, bool b_prefix );

    void errorOccur();
    bool updating;

    Ui::DevCalcWindow *ui = nullptr;
    QString hex_formula;
    QString dec_formula;
    QString bin_formula;
    QHash<int, QHash<QString, QString>> ctConfigs;
    QVector<QChar> validOp;
};

#endif  // CALCTOOL_H
