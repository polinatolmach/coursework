/*
 * Created by Polina Tolmach
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 */

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

void options::on_pushButton_2_clicked()
{
    this->close();
}

void options::on_pushButton_clicked()
{
    this->accept();
}
