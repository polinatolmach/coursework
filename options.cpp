#include "options.h"
#include "ui_options.h"

options::options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::options)
{
    ui->setupUi(this);
}

options::~options()
{
    delete ui;
}

void options::on_options_accepted()
{

}

void options::setPath(QString str)
{
    this->ui->lineEdit->setText(str);
}

void options::setExe(QString str)
{
    this->ui->lineEdit_2->setText(str);
}

QString options::getPath()
{
    return this->ui->lineEdit->text();
}


QString options::getExe()
{
    return this->ui->lineEdit_2->text();
}
