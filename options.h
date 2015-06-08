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
    void on_options_accepted();

private:
    Ui::options *ui;
};

#endif // OPTIONS_H
