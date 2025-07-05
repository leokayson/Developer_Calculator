#ifndef CALCMATH_H
#define CALCMATH_H

#include <QDebug>
#include <QStack>
#include <QString>
#include "math.h"

typedef unsigned long long uint64_t;

class CalcMath {
private:
    static QString dec2Bin( quint64 num );
    static QString dec2Hex( quint64 num );

public:
    CalcMath();

public slots:
    static bool is_hex_char( QChar c );
    static bool is_dec_char( QChar c );
    static bool is_bin_char( QChar c );
    static QString baseConversion( const QString &num, int ori_base, int new_base );
    static QString decEvaluate(        const QString &expression );
    static QString DecExpr2HexExpr( const QString &dec_expr );
    static QString DecExpr2BinExpr( const QString &dec_expr );
    static QString HexExpr2DecExpr( const QString &hex_expr );
    static QString BinExpr2DecExpr( const QString &bin_expr );
};

#endif  // CALCMATH_H
