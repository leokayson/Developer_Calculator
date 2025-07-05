#include "calcmath.h"
#include <qglobal.h>
#include <cctype>
#include <stack>
#include <stdexcept>
#include <string>

CalcMath::CalcMath() {}

QString CalcMath::baseConversion( const QString &num, int ori_base, int new_base ) {
    if ( num == "" ) {
        return (QString)( num );
    }
    bool ok = true;
    qlonglong ori_num = num.toLongLong( &ok, ori_base );

    if ( !ok ) {
        return ( "ERROR" );
    }
    return QString::number( ori_num, new_base );
}

QString CalcMath::decEvaluate( const QString &expression ) {
    QStack<quint64> values;
    QStack<QChar> ops;

    // 定义运算符优先级
    auto precedence = []( QChar op ) {
        switch ( op.toLatin1() ) {
            case '!':
                return 3;  
            case '&':      
            case '>':      
            case '<':      
            case '|':      
            case '^':
                return 2;  
            case '*':
            case '/':
            case '%':
                return 1;  
            case '+':
            case '-':
                return 0;  
            default:
                return -1; 
        }
    };

    // 应用运算符计算
    auto apply_op = [ &values, &ops ]() {
        QChar op = ops.top();
        ops.pop();

        if ( op == '!' ) {
            quint64 a = values.top();
            values.pop();
            values.push( ~a );  // 按位取反
            return;
        }

        quint64 b = values.top();
        values.pop();
        quint64 a = values.top();
        values.pop();

        switch ( op.toLatin1() ) {
            case '+':
                values.push( a + b );
                break;
            case '-':
                values.push( a - b );
                break;
            case '*':
                values.push( a * b );
                break;
            case '/':
                if ( b == 0 )
                    throw std::runtime_error( "Division by zero" );
                values.push( a / b );
                break;
            case '%':
                values.push( a % b );
                break;
            case '^':
                values.push( a ^ b );
                break;
            case '>':
                values.push( a >> b );
                break;
            case '<':
                values.push( a << b );
                break;
            case '&':
                values.push( a & b );
                break;
            case '|':
                values.push( a | b );
                break;
        }
    };

    for ( uint i = 0; i < expression.length(); i++ ) {
        if ( expression[ i ] == ' ' )
            continue;

        if ( is_dec_char( expression[ i ] ) ) {
            QString num_str;
            while ( i < expression.length() && is_dec_char( expression[ i ] ) )
                num_str += expression[ i++ ];
            i--;
            values.push( num_str.toULongLong() );
        } else if ( expression[ i ] == '(' ) {
            ops.push( expression[ i ] );
        } else if ( expression[ i ] == ')' ) {
            while ( !ops.empty() && ops.top() != '(' )
                apply_op();
            ops.pop();  // pop '('
        } else if ( expression[ i ] == '+' || expression[ i ] == '-' || expression[ i ] == '*' ||
                    expression[ i ] == '/' || expression[ i ] == '^' || expression[ i ] == '!' ||
                    expression[ i ] == '&' || expression[ i ] == '|' || expression[ i ] == '>' ||
                    expression[ i ] == '<' || expression[ i ] == '%' ) {
            if ( ( expression[ i ] == '-' && i == 0 ) || ( expression[ i ] == '-' && expression[ i - 1 ] == '(' ) ||
                 expression[ i ] == '!' ) {
                ops.push( expression[ i ] );
                continue;
            }

            while ( !ops.empty() && ops.top() != '(' && precedence( ops.top() ) >= precedence( expression[ i ] ) )
                apply_op();
            ops.push( expression[ i ] );
        } else {
            throw std::runtime_error( "Invalid character in expression" );
        }
    }

    while ( !ops.empty() )
        apply_op();

    return QString::number( values.top() );
}

QString CalcMath::dec2Hex( quint64 num ) {
    if ( num == 0 )
        return "0";  // handle 0

    QString hex;
    while ( num > 0 ) {
        hex = "0123456789ABCDEF"[ num % 16 ] + hex;
        num /= 16;
    }

    return hex;
}

QString CalcMath::dec2Bin( quint64 num ) {
    if ( num == 0 )
        return "0";  // handle 0

    QString binary;
    while ( num > 0 ) {
        binary = ( num % 2 == 0 ? "0" : "1" ) + binary;
        num /= 2;
    }

    return binary;
}

bool CalcMath::is_hex_char( QChar c ) {
    return ( c >= '0' && c <= '9' ) || ( c >= 'A' && c <= 'F' ) || ( c >= 'a' && c <= 'f' );
}

bool CalcMath::is_dec_char( QChar c ) {
    return c >= '0' && c <= '9';
}

bool CalcMath::is_bin_char( QChar c ) {
    return c == '0' || c == '1';
}

QString CalcMath::DecExpr2HexExpr( const QString &dec_expr ) {
    QString hex_expr;
    QString num_str;

    for ( uint i = 0; i < dec_expr.length(); i++ ) {
        if ( dec_expr[ i ] == ' ' )
            continue;

        if ( is_hex_char( dec_expr[ i ] ) ) {
            num_str.clear();

            while ( i < dec_expr.length() && is_hex_char( dec_expr[ i ] ) )
                num_str += dec_expr[ i++ ];
            i--;

            hex_expr += dec2Hex( num_str.toULongLong() );
        } else {
            hex_expr += dec_expr[ i ];
        }
    }
    return hex_expr;
}

QString CalcMath::DecExpr2BinExpr( const QString &dec_expr ) {
    QString bin_expr;
    QString num_str;

    for ( uint i = 0; i < dec_expr.length(); i++ ) {
        if ( dec_expr[ i ] == ' ' )
            continue;

        if ( is_hex_char( dec_expr[ i ] ) ) {
            num_str.clear();

            while ( i < dec_expr.length() && is_hex_char( dec_expr[ i ] ) )
                num_str += dec_expr[ i++ ];
            i--;

            bin_expr += dec2Bin( num_str.toULongLong() );
        } else {
            bin_expr += dec_expr[ i ];
        }
    }
    return bin_expr;
}

QString CalcMath::HexExpr2DecExpr( const QString &hex_expr ) {
    QString dec_expr;
    QString num_str;

    for ( uint i = 0; i < hex_expr.length(); i++ ) {
        if ( is_hex_char( hex_expr[ i ] ) ) {
            num_str.clear();
            if ( ( ( hex_expr[ i ] == '0' ) && ( i + 1 < hex_expr.length() ) && hex_expr[ i + 1 ] == 'x' ) ) {
                i += 2;  // skip 0x
            }
            while ( i < hex_expr.length() && ( is_hex_char( hex_expr[ i ] ) ) )
                num_str += hex_expr[ i++ ];
            i--;

            dec_expr += QString::number( num_str.toULongLong( nullptr, 16 ) );
        } else {
            dec_expr += hex_expr[ i ];
        }
    }
    return dec_expr;
}

QString CalcMath::BinExpr2DecExpr( const QString &bin_expr ) {
    QString dec_expr;
    QString num_str;

    for ( uint i = 0; i < bin_expr.length(); i++ ) {
        if ( is_bin_char( bin_expr[ i ] ) ) {
            num_str.clear();
            if ( ( ( bin_expr[ i ] == '0' ) && ( i + 1 < bin_expr.length() ) && bin_expr[ i + 1 ] == 'b' ) ) {
                i += 2;  // skip 0b
            }
            while ( i < bin_expr.length() && is_bin_char( bin_expr[ i ] ) ) {
                num_str += bin_expr[ i++ ];
            }
            i--;

            dec_expr += QString::number( num_str.toULongLong( nullptr, 2 ) );
        } else {
            dec_expr += bin_expr[ i ];
        }
    }
    return dec_expr;
}
