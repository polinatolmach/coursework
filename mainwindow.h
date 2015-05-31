#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "syntaxlighter.h"

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QVector>
#include <QShortcut>
#include <QString>

#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QIODevice>
#include <QMessageBox>
#include <QRegExp>
#include <QShortcut>
#include <QTextEdit>
#include <QTextStream>
#include <QToolButton>
#include <QWidget>
#include <string>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QTextCursor>
#include <QDialog>
 class QAbstractItemModel;
 class QComboBox;
 class QCompleter;
 class QLabel;
 class QLineEdit;
 class QProgressBar;

 class TextEdit;





namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:


private slots:

 //   QString extractFilename(QString path);
//    void switchToPrevTab();
//    void switchToNextTab();
    void makeNewTab();
    void deleteTab(int);
    void deleteCurrentTab();

    void on_actionNew_triggered();
//    void on_actionOpen_triggered();
//    void on_actionSave_triggered();
//    void on_actionSave_As_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void highlightCurrentLine();
    void on_actionNew_tab_triggered();

private:
//    void saveFileAs();
//    void saveFile();
//    void openFile();

    QString textUnderCursor() const;
   // QString extractFilename(QString path);
    int currentTab();
    int lightfactor;
    void setTabName(unsigned int index, QString text);

    Ui::MainWindow *ui;
  //  QString curFilename;

    QShortcut *shortcut;

    struct Tab {
        int number;
        QString path;
        QString filename;
        QPlainTextEdit *plainTextEdit;
        SyntaxLighter *highlighter;
    };
    QVector<Tab> tabs;



};


#endif // MAINWINDOW_H
\
