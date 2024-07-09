#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRegExp>
#include <QRegExpValidator>
#include <QFileInfo>
#include <QDir>

#define CONNECTTEXTCHANGE(name) connect(ui->##name##Text, &QLineEdit::textChanged, cTool, &CalcTool::on##name##TextChange);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DevCalcWindow)
    , cTool(new CalcTool(ui, this))

{
    // QFileInfo fileInfo(__FILE__);
    QDir projectRootDir = QDir(QFileInfo(__FILE__).absolutePath());
    projectRootDir.cdUp();

    ui->setupUi(this);
    this->setWindowTitle("Developer Calculator");
    this->setWindowIcon(QIcon(projectRootDir.path() + "/icons/calc_icon.ico"));

    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    QRegExpValidator *hexValidator = new QRegExpValidator(*new QRegExp("[(0x)0-9A-Fa-f+\\-*/><&!^~| ]+"), this);
    QRegExpValidator *decValidator = new QRegExpValidator(*new QRegExp("[0-9+\\-*/><&!^~|,]+"), this);
    QRegExpValidator *binValidator = new QRegExpValidator(*new QRegExp("[(0b)01+\\-*/><&!^~| ]+"), this);
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

    connect(ui->FixLen8CB, &QCheckBox::stateChanged, cTool, &CalcTool::fixLen8Output);
    connect(ui->FixLen16CB, &QCheckBox::stateChanged, cTool, &CalcTool::fixLen16Output);

    connect(ui->ClearBnt, &QPushButton::clicked, cTool, &CalcTool::clearOutput);
}
