#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QRegExp>
#include <QRegExpValidator>

#define CONNECTTEXTCHANGE(name) connect(ui->##name##Text, &QLineEdit::textChanged, cTool, &CalcTool::on##name##TextChange);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DevCalcWindow)
    , cTool(new CalcTool(ui, this))

{
    ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    // 16 words + 1 operator + 2 delimiter spaces at most + 4 prefixes + 5 chars redundancies
    QRegExpValidator *hexValidator = new QRegExpValidator(*new QRegExp("[0-9A-Fa-fx+\\-*/><&!^~| ]{0,31}"), this);
    // 20 words + 1 operator + 6 delimiter chars at most + 0 prefixes + 5 characters redundancies
    QRegExpValidator *decValidator = new QRegExpValidator(*new QRegExp("[0-9+\\-*/><&!^~|,]{0,32}"), this);
    // 64 words + 1 operator + 14 delimiter chars at most + 4 prefixes + 5 characters redundancies
    QRegExpValidator *binValidator = new QRegExpValidator(*new QRegExp("[01b+\\-*/><&!^~| ]{0,88}"), this);
    ui->HexText->setValidator(hexValidator);
    ui->DecText->setValidator(decValidator);
    ui->BinText->setValidator(binValidator);

    connect(ui->HexText, &QLineEdit::textChanged, cTool, &CalcTool::onHexTextChange);
    connect(ui->HexText, &QLineEdit::returnPressed, cTool, &CalcTool::calculateOutput);

    connect(ui->DecText, &QLineEdit::textChanged, cTool, &CalcTool::onDecTextChange);
    connect(ui->DecText, &QLineEdit::returnPressed, cTool, &CalcTool::calculateOutput);

    connect(ui->BinText, &QLineEdit::textChanged, cTool, &CalcTool::onBinTextChange);
    connect(ui->BinText, &QLineEdit::returnPressed, cTool, &CalcTool::calculateOutput);

    connect(ui->PrefixCB, &QCheckBox::stateChanged, cTool, &CalcTool::formatOutput);
    connect(ui->DelimiterCB, &QCheckBox::stateChanged, cTool, &CalcTool::formatOutput);
    connect(ui->FixLenCB, &QCheckBox::stateChanged, cTool, &CalcTool::formatOutput);

    connect(ui->ClearBnt, &QPushButton::clicked, cTool, &CalcTool::clearOutput);
}
