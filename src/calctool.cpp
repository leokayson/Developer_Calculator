#include "CalcTool.h"

#define SET_CTCONFIGS(top_key, sub_key, value) ctConfigs[top_key][sub_key] = value

CalcTool::CalcTool(Ui::DevCalcWindow *ui, QObject *parent)
    : QObject(parent), ui(ui) {
    this->hexOriTextArr = {"0", "", ""};
    this->decOriTextArr = {"0", "", ""};
    this->binOriTextArr = {"0", "", ""};
    this->validOp = {'+', '-', '*', '/', '^', '&', '|', '!', '>', '<', '~', '%'};
    this->ctConfigs.insert("hex", {{ { "base", "16" }, { "delimiterLen", "4" }, { "delimiterChar", " " }, { "prefix", "0x" } , { "fixLen", "8" } }});
    this->ctConfigs.insert("dec", {{ { "base", "10" }, { "delimiterLen", "3" }, { "delimiterChar", "," }, { "prefix", "" }, { "fixLen", "0" }  }});
    this->ctConfigs.insert("bin", {{ { "base", "2" }, { "delimiterLen", "4" }, { "delimiterChar", " " }, { "prefix", "0b" }, { "fixLen", "32" }  }});
}

CalcTool::~CalcTool() { delete ui; };

void CalcTool::onHexTextChange(const QString &text) {
    if (updating) {
        return;
    }
    updating = true;
    // Make cursorPoint count from the right
    int cp = ui->HexText->displayText().length() - ui->HexText->cursorPosition();
    handleTextChange(text, 16);
    ui->HexText->setCursorPosition(ui->HexText->displayText().length() - cp);
    updating = false;
}

void CalcTool::onDecTextChange(const QString &text) {
    if (updating) {
        return;
    }
    updating = true;
    // Make cursorPoint count from the right
    int cp = ui->DecText->displayText().length() - ui->DecText->cursorPosition();
    handleTextChange(text, 10);
    ui->DecText->setCursorPosition(ui->DecText->displayText().length() - cp);
    updating = false;
}

void CalcTool::onBinTextChange(const QString &text) {
    if (updating) {
        return;
    }
    updating = true;
    // Make cursorPoint count from the right
    int cp = ui->BinText->displayText().length() - ui->BinText->cursorPosition();
    handleTextChange(text, 2);
    ui->BinText->setCursorPosition(ui->BinText->displayText().length() - cp);
    updating = false;
}

void CalcTool::handleTextChange(const QString &text, const uint8_t ori_base) {
    QString newText = "0";
    std::array<QString, 3> temArr = {"", "", ""};

    switch (ori_base) {
    case (2): {
        if (text != "") {
            newText = text;
            newText = newText.replace(*new QRegExp("0b| "), "");
        }
        break;
    }
    case (10): {
        if (text != "") {
            newText = text;
            newText = newText.replace(*new QRegExp(",| "), "");
        }
        break;
    }

    case (16): {
        if (text != "") {
            newText = text;
            newText = newText.replace(*new QRegExp("0x| "), "");
        }
    }

    default: {
        break;
    }
    }

    spiltText(newText, temArr);
    for (int i = 0; i < 3; i += 2) {
        hexOriTextArr[i] = CalcMath::baseConversion(temArr[i], ori_base, 16);
        decOriTextArr[i] = CalcMath::baseConversion(temArr[i], ori_base, 10);
        binOriTextArr[i] = CalcMath::baseConversion(temArr[i], ori_base, 2);
    }
    hexOriTextArr[1] = decOriTextArr[1] = binOriTextArr[1] = temArr[1];
    formatOutput();
}

void CalcTool::spiltText(QString &newText, std::array<QString, 3> &arr) {
    int loc = -1;
    QChar op;
    foreach (op, validOp) {
        if ((loc = newText.lastIndexOf(op)) != -1) {
            break;
        }
    }
    if (loc == -1) {
        arr[0] = newText;
        arr[1] = "";
        arr[2] = "";
    } else {
        arr[0] = newText.left(loc);
        arr[1] = op;
        arr[2] = newText.right(newText.size() - loc - 1);
    }
#ifdef DEBUG_MODE
    qDebug() << newText << " " << loc << " : " << arr[0] << " " << arr[1] << " "
             << arr[2];
#endif
}

void CalcTool::calculateOutput() {
#ifdef DEBUG_MDOE
    qDebug() << "Calculate output triggered!";
#endif
    updating = true;
    QString formula = decOriTextArr[0] + decOriTextArr[1] + decOriTextArr[2];
    QString res = QString::number(CalcMath::calculate(formula.toStdString()), 10);

    hexOriTextArr = {CalcMath::baseConversion(res, 10, 16), "", ""};
    decOriTextArr = {res, "", ""};
    binOriTextArr = {CalcMath::baseConversion(res, 10, 2), "", ""};

    formatOutput(true);
    updating = false;
}

void CalcTool::fixLen8Output() {
    if (updating) {
        return;
    }
    updating = true;
    ui->FixLen16CB->setChecked(false);
    SET_CTCONFIGS("hex", "fixLen", "8");
    SET_CTCONFIGS("bin", "fixLen", "32");
    formatOutput(false);
    updating = false;
}
void CalcTool::fixLen16Output() {
    if (updating) {
        return;
    }
    updating = true;
    ui->FixLen8CB->setChecked(false);
    ctConfigs["hex"]["fixLen"] = "16";
    ctConfigs["bin"]["fixLen"] = "64";
    formatOutput(false);
    updating = false;
}

void CalcTool::fixLenArray(QString baseStr, std::array<QString, 3> &temArr) {
    uint fixLen = ctConfigs[baseStr]["fixLen"].toUInt();
    if (temArr[0].length() < fixLen) {
        temArr[0] = temArr[0].rightJustified(fixLen, '0');;
    }
    if (temArr[2] != "" && temArr[2].length() < fixLen) {
        temArr[2] = temArr[2].rightJustified(fixLen, '0');;
    }
}

void CalcTool::declimateArray(QString baseStr, std::array<QString, 3> &temArr) {
    uint8_t decLen = ctConfigs[baseStr]["delimiterLen"].toShort();
    QChar decChar = ctConfigs[baseStr]["delimiterChar"].at(0);
#ifdef DEBUG_MODE
    qDebug() << "decLen: " << decLen << "decChar: " << decChar;
#endif
    declimateText(temArr[0], decLen, decChar);
    declimateText(temArr[2], decLen, decChar);
}

void CalcTool::declimateText(QString &text, const uint8_t decLen, const QChar decChar) {
    for (int i = text.size() - decLen; i > 0; i -= decLen) {
        text = text.insert(i, decChar);
    }
#ifdef DEBUG_MODE
    qDebug() << "after declimateText, text: " << text;
#endif
}

void CalcTool::prefixArray(QString baseStr, std::array<QString, 3> &temArr) {
    QString prefix = ctConfigs[baseStr]["prefix"];
    if (temArr[0] != "" && temArr[0] != "0") {
        temArr[0] = prefix + temArr[0];
    }
    if (temArr[2] != "" && temArr[2] != "0") {
        temArr[2] = prefix + temArr[2];
    }
}

void CalcTool::clearOutput() {
    updating = true;
    hexOriTextArr = {"0", "", ""};
    decOriTextArr = {"0", "", ""};
    binOriTextArr = {"0", "", ""};
    formatOutput();
    updating = false;
}

void CalcTool::formatOutput(bool outputHistory) {
#ifdef DEBUG_MODE
    qDebug() << "formatOutput:";
    qDebug() << "hexOriTextArr:" << hexOriTextArr[0] << hexOriTextArr[1]
             << hexOriTextArr[2];
    qDebug() << "decOriTextArr:" << decOriTextArr[0] << decOriTextArr[1]
             << decOriTextArr[2];
    qDebug() << "binOriTextArr:" << binOriTextArr[0] << binOriTextArr[1]
             << binOriTextArr[2];
    qDebug() << "outputHistory: " << outputHistory;
#endif
    std::array<QString, 3> hexTempArr = hexOriTextArr;
    std::array<QString, 3> decTempArr = decOriTextArr;
    std::array<QString, 3> binTempArr = binOriTextArr;

    if (ui->FixLen8CB->isChecked() || ui->FixLen16CB->isChecked()) {
#ifdef DEBUG_MODE
        qDebug() << "FixLenCB isChecked";
#endif
        fixLenArray("hex", hexTempArr);
        fixLenArray("dec", decTempArr);
        fixLenArray("bin", binTempArr);
    }
    if (ui->DelimiterCB->isChecked()) {
#ifdef DEBUG_MODE
        qDebug() << "DelimiterCB isChecked";
#endif
        declimateArray("hex", hexTempArr);
        declimateArray("dec", decTempArr);
        declimateArray("bin", binTempArr);
    }
    if (ui->PrefixCB->isChecked()) {
#ifdef DEBUG_MODE
        qDebug() << "PrefixCB isChecked";
#endif
        prefixArray("hex", hexTempArr);
        prefixArray("dec", decTempArr);
        prefixArray("bin", binTempArr);
    }
    QString hexStr = hexTempArr[0] + hexTempArr[1] + hexTempArr[2];
    QString decStr = decTempArr[0] + decTempArr[1] + decTempArr[2];
    QString binStr = binTempArr[0] + binTempArr[1] + binTempArr[2];

    if(outputHistory) {
        QString history = "";
        history += ui->HexText->displayText() + " = " + hexStr + "\r\n";
        history += ui->DecText->displayText() + " = " + decStr + "\r\n";
        history += ui->BinText->displayText() + " = " + binStr + "\r\n";
        history += "================";
        ui->HistoryText->appendPlainText(history);
    }

    ui->HexText->setText(hexStr.toUpper().replace("X","x"));
    ui->DecText->setText(decStr);
    ui->BinText->setText(binStr);
#ifdef DEBUG_MODE
    qDebug() << "================";
#endif
}
