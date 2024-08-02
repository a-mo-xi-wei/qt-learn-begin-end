#include "FSCapture.h"
#include "./ui_FSCapture.h"
#include<QPushButton>
FSCapture::FSCapture(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FSCapture)
{
    ui->setupUi(this);
    connect(ui->editBtn,&QPushButton::clicked,[]{qDebug()<<__FUNCTION__;});
    connect(ui->optionBtn,&QPushButton::clicked,[]{qDebug()<<__FUNCTION__;});
}

FSCapture::~FSCapture()
{
    delete ui;
}

void FSCapture::on_recordBtn_clicked()
{
    qDebug()<<__FUNCTION__;
}

