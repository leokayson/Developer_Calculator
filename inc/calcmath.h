#ifndef CALCMATH_H
#define CALCMATH_H

#include <QString>
#include <QDebug>
#include <QStack>
#include "calctool.h"
#include "math.h"
#include "proj_conf.h"

class CalcMath
{
public:
    CalcMath();

public slots:
    static QString baseConversion(const QString &num, int ori_base, int new_base);
    static int binary_calculate(const qlonglong num1, const qlonglong num2, QChar opt);
    static int calculate(const std::string &formula);
};

#endif // CALCMATH_H
