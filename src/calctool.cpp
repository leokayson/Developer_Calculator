#include "CalcTool.h"
#include <qchar.h>
#include <string>
#include "calcmath.h"
#include "proj_conf.h"

#define SET_CTCONFIGS( top_key, sub_key, value ) ctConfigs[ top_key ][ sub_key ] = value

CalcTool::CalcTool( Ui::DevCalcWindow *ui, QObject *parent ) : QObject( parent ), ui( ui ) {
    this->hex_formula = "0";
    this->dec_formula = "0";
    this->bin_formula = "0";
    this->validOp = { '+', '-', '*', '/', '^', '&', '|', '!', '>', '<', '~', '%' };
    this->updating = 0;
    this->ctConfigs.insert(
        16, { { { "delimiterLen", "4" }, { "delimiterChar", " " }, { "prefix", "x" }, { "fixLen", "8" } } } );
    this->ctConfigs.insert(
        10, { { { "delimiterLen", "3" }, { "delimiterChar", "," }, { "prefix", "d" }, { "fixLen", "0" } } } );
    this->ctConfigs.insert(
        2, { { { "delimiterLen", "4" }, { "delimiterChar", " " }, { "prefix", "b" }, { "fixLen", "32" } } } );
}

CalcTool::~CalcTool() {
    delete ui;
};

void CalcTool::onHexTextChange( const QString &text ) {
    if ( updating ) {
        return;
    }
    // Make cursorPoint count from the right
    int cp = ui->HexText->displayText().length() - ui->HexText->cursorPosition();
    handleTextChange( text, 16 );
    ui->HexText->setCursorPosition( ui->HexText->displayText().length() - cp );
}

void CalcTool::onDecTextChange( const QString &text ) {
    if ( updating ) {
        return;
    }
    // Make cursorPoint count from the right
    int cp = ui->DecText->displayText().length() - ui->DecText->cursorPosition();
    handleTextChange( text, 10 );
    ui->DecText->setCursorPosition( ui->DecText->displayText().length() - cp );
}

void CalcTool::onBinTextChange( const QString &text ) {
    if ( updating ) {
        return;
    }
    // Make cursorPoint count from the right
    int cp = ui->BinText->displayText().length() - ui->BinText->cursorPosition();
    handleTextChange( text, 2 );
    ui->BinText->setCursorPosition( ui->BinText->displayText().length() - cp );
}

void CalcTool::handleTextChange( const QString &text, int base ) {
    QString newText = text;
    QChar prefix = ctConfigs[ base ][ "prefix" ][ 0 ];
    QChar delimiterChar = ctConfigs[ base ][ "delimiterChar" ][ 0 ];
    if ( base != 10 ) {
        newText = newText.replace( '0' + prefix, "" );  // remove all spaces
    }
    newText = newText.replace( delimiterChar, "" );

    switch ( base ) {
        case ( 2 ): {
            bin_formula = newText;
            dec_formula = CalcMath::BinExpr2DecExpr( bin_formula );
            hex_formula = CalcMath::DecExpr2HexExpr( dec_formula );
            break;
        }
        case ( 10 ): {
            dec_formula = newText;
            bin_formula = CalcMath::DecExpr2BinExpr( dec_formula );
            hex_formula = CalcMath::DecExpr2HexExpr( dec_formula );
            break;
        }

        case ( 16 ): {
            hex_formula = newText;
            dec_formula = CalcMath::HexExpr2DecExpr( hex_formula );
            bin_formula = CalcMath::DecExpr2BinExpr( dec_formula );
            break;
        }

        default: {
            break;
        }
    }

    formatOutput();
}

void CalcTool::calculateOutput() {
    DEBUG_PRINT( "Calculate output triggered!" );

    dec_formula = CalcMath::decEvaluate( dec_formula );
    hex_formula = CalcMath::DecExpr2HexExpr( dec_formula );
    bin_formula = CalcMath::DecExpr2BinExpr( dec_formula );

    formatOutput( true );
}

void CalcTool::fixLen8Output() {
    if ( updating ) {
        return;
    }
    ui->FixLen16CB->setChecked( false );
    SET_CTCONFIGS( 16, "fixLen", "8" );
    SET_CTCONFIGS( 2, "fixLen", "32" );
    formatOutput( false );
}

void CalcTool::fixLen16Output() {
    if ( updating ) {
        return;
    }
    ui->FixLen8CB->setChecked( false );
    SET_CTCONFIGS( 16, "fixLen", "16" );
    SET_CTCONFIGS( 2, "fixLen", "64" );
    formatOutput( false );
}

QString CalcTool::formatFormula( QString &formula, int base, bool b_fix_len, bool b_declimate, bool b_prefix ) {
    uint fixLen = ctConfigs[ base ][ "fixLen" ].toUInt();
    uint delimiterLen = ctConfigs[ base ][ "delimiterLen" ].toUInt();
    QChar prefix = ctConfigs[ base ][ "prefix" ][ 0 ];
    QChar delimiterChar = ctConfigs[ base ][ "delimiterChar" ][ 0 ];

    QString new_formula = "", hanled_expr;
    bool ( *judgeFunc )( QChar );

    if ( base == 2 ) {
        judgeFunc = CalcMath::is_bin_char;
        prefix = 'b';
    } else if ( base == 10 ) {
        judgeFunc = CalcMath::is_dec_char;
        prefix = 'd';
    } else if ( base == 16 ) {
        judgeFunc = CalcMath::is_hex_char;
        prefix = 'x';
    }
    DEBUG_PRINT( "fixLen: " << fixLen );

    for ( uint i = 0; i < formula.length(); i++ ) {
        if ( judgeFunc( formula[ i ] ) ) {
            hanled_expr.clear();

            if ( ( ( formula[ i ] == '0' ) && ( i + 1 < formula.length() ) && formula[ i + 1 ] == prefix ) ) {
                i += 2;  // skip 0x or 0b
            }

            while ( i < formula.length() && ( judgeFunc( formula[ i ] ) ) )
                hanled_expr += formula[ i++ ];
            i--;

            if ( b_fix_len ) {
                while ( hanled_expr.length() < fixLen ) {
                    hanled_expr = "0" + hanled_expr;  // pad with 0 for binary
                }
            }

            if ( b_declimate ) {
                for ( int j = hanled_expr.length() - delimiterLen; j > 0; j -= delimiterLen ) {
                    hanled_expr.insert( j, delimiterChar );
                }
            }

            if ( b_prefix ) {
                hanled_expr = '0' + prefix + hanled_expr;
            }

            new_formula += hanled_expr;
        } else {
            new_formula += formula[ i ];
        }
    }

    return new_formula;
}
void CalcTool::clearOutput() {
    dec_formula.clear();
    bin_formula.clear();
    hex_formula.clear();

    formatOutput( false );
}

void CalcTool::formatOutput( bool outputHistory ) {
    if ( updating ) {
        return;
    }

    bool b_fix_len = ( ui->FixLen8CB->isChecked() || ui->FixLen16CB->isChecked() ),
         b_declimate = ui->DelimiterCB->isChecked(), b_prefix = ui->PrefixCB->isChecked();
    QString q_hex_formula, q_dec_formula, q_bin_formula, history = "";

    updating = true;

    DEBUG_PRINT( "formatOutput:" );
    DEBUG_PRINT( "hexFormula:" << hex_formula );
    DEBUG_PRINT( "decFormula:" << dec_formula );
    DEBUG_PRINT( "binFormula:" << bin_formula );
    DEBUG_PRINT( "outputHistory: " << outputHistory );

    q_hex_formula = formatFormula( hex_formula, 16, b_fix_len, b_declimate, b_prefix );
    q_dec_formula = formatFormula( dec_formula, 10, false, b_declimate, b_prefix );
    q_bin_formula = formatFormula( bin_formula, 2, b_fix_len, b_declimate, b_prefix );

    if ( outputHistory ) {
        history += ui->HexText->displayText() + " = " + q_hex_formula + "\r\n";
        history += ui->DecText->displayText() + " = " + q_dec_formula + "\r\n";
        history += ui->BinText->displayText() + " = " + q_bin_formula + "\r\n";
        history += "================";
        ui->HistoryText->appendPlainText( history );
    }

    ui->HexText->setText( q_hex_formula );
    ui->DecText->setText( q_dec_formula );
    ui->BinText->setText( q_bin_formula );

    DEBUG_PRINT( "================" );

    updating = false;
}
