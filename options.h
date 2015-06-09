/*
 * Created by Polina Tolmach
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

namespace Ui {
class options;
}

class options : public QDialog
{
    Q_OBJECT

public:
    explicit options(QWidget *parent = 0);
    ~options();
    void setPath(QString str);
    void setExe(QString str);
    QString getPath();
    QString getExe();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::options *ui;
};

#endif // OPTIONS_H
