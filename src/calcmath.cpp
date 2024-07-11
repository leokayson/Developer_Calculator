#include "calcmath.h"

CalcMath::CalcMath() {}

QString CalcMath::baseConversion(const QString &num, int ori_base,
                                 int new_base) {
    if (num == "") {
        return (QString)(num);
    }
    bool ok = true;
    qlonglong ori_num = num.toLongLong(&ok, ori_base);

    if (!ok) {
        return ("ERROR");
    }
    return QString::number(ori_num, new_base);
}

QString CalcMath::calculate(const QString &num1, const QString &num2,
                            QChar opt) {
    bool ok = true;
    qlonglong numL1 = 0, numL2 = 0, res = 0;

    numL1 = num1.toLongLong(&ok, 10);
    if (!ok) {
        return "ERROR";
    }
    numL2 = num2.toLongLong(&ok, 10);
    if (!ok) {
        return "ERROR";
    }

    switch(opt.unicode()) {
    case((ushort)'+'): {
        res = numL1 + numL2;
        break;
    }
    case((ushort)'-'): {
        res = numL1 - numL2;
        break;
    }
    case((ushort)'*'): {
        res = numL1 * numL2;
        break;
    }
    case((ushort)'/'): {
        res = numL1 / numL2;
        break;
    }
    case((ushort)'&'): {
        res = numL1 & numL2;
        break;
    }
    case((ushort)'|'): {
        res = numL1 | numL2;
        break;
    }
    // '!' just to avoid confusion, the role in the calculator is the same as '~'
    case((ushort)'!'):
    case((ushort)'~'): {
        int bitWidth = numL2 == 0 ? 1 : static_cast<int>(std::log2(numL2) + 1);
        int bitMask = (1 << bitWidth) - 1;
        res = (~numL2) & bitMask;
        break;
    }
    case((ushort)'>'): {
        res = numL1 >> numL2;
        break;
    }
    case((ushort)'<'): {
        res = numL1 << numL2;
        break;
    }
    case((ushort)'^'): {
        res = std::pow(numL1, numL2);
        break;
    }
    case((ushort)'%'): {
        res = numL1 % numL2;
        break;
    }
    defualt: {
        return("ERROR");
    }
    }
    return QString::number(res, 10);
}
