#ifndef CALCMATH_H
#define CALCMATH_H

#include <QString>
#include <QDebug>
#include "calctool.h"
#include "math.h"
#include "proj_conf.h"

class CalcMath
{
public:
    CalcMath();

public slots:
    static QString baseConversion(const QString &num, int ori_base, int new_base);
    static QString calculate(const QString &num1, const QString &num2, QChar opt);
};

#endif // CALCMATH_H
