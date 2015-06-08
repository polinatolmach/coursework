/*
 * Created by Polina Tolmach
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "syntaxlighter.h"
#include "codeeditor.h"

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QToolBar>
#include <QVector>
#include <QString>



namespace Ui {
class MainWindow;

}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString currentText() const;


protected:

private slots:

    void makeNewTab();
    void deleteTab(int tab_number);
    void deleteCurrentTab();

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionNew_tab_triggered();
    void on_actionSelect_All_triggered();

    void on_tabWidget_tabBarDoubleClicked();
    void switchToPrevTab();
    void switchToNextTab();

    void on_actionCompile_triggered();
    void compile();
    void on_actionCompiler_options_triggered();

private:

    void setupEditor();
    void saveFileAs();
    void saveFile();
    void openFile();
    void setTabName(unsigned int index, QString text);

    QString extractFilename(QString path);
    int currentTab();

    Ui::MainWindow *ui;
    struct Tab {
        int number;
        QString path;
        QString filename;
        QCodeEditor *editor;
        SyntaxLighter *highlighter;
        ~Tab()
        {
        delete editor;
        delete highlighter;
        }
    };
    QVector<Tab*> tabs;
};
#endif // MAINWINDOW_H
\
