/*
 * Created by Polina Tolmach
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "syntaxlighter.h"
#include "codeeditor.h"

#include <QMessageBox>
#include <QVector>
#include <QFileInfo>
#include <QString>
#include <QToolButton>
#include <QFileDialog>
#include <QProcess>
#include <QByteArray>
#include <QShortcut>


QString path = "D:\\MinGW64\\bin\\";
QString compiler = "g++.exe";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setupEditor();
    //Set window settings.
    this->setWindowIcon(QIcon(":/texticon.png"));
    this->setWindowTitle("Untitled - pEdit 1.0");
    this->setAcceptDrops(true);
    setCentralWidget(ui->tabWidget);

    QToolButton *newTabButton = new QToolButton(this);
    ui->tabWidget->setCornerWidget(newTabButton);
    newTabButton->setAutoRaise(true);
    newTabButton->setText("+");
    //Connecting signals for making and deleting tabs.
    QObject::connect(newTabButton, SIGNAL(clicked()), this, SLOT(makeNewTab()));
    QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteTab(int)));

    //Creating keyboard shortcuts.
    QShortcut *shortcut = new QShortcut(QKeySequence("ctrl+t"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(makeNewTab()));

    shortcut = new QShortcut(QKeySequence("ctrl+s"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_actionSave_triggered()));

    shortcut = new QShortcut(QKeySequence("ctrl+shift+s"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_actionSave_As_triggered()));

    shortcut = new QShortcut(QKeySequence("ctrl+o"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_actionOpen_triggered()));

    shortcut = new QShortcut(QKeySequence("ctrl+tab"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(switchToNextTab()));

    shortcut = new QShortcut(QKeySequence("ctrl+shift+tab"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(switchToPrevTab()));

    shortcut = new QShortcut(QKeySequence("ctrl+d"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(deleteCurrentTab()));

    shortcut = new QShortcut(QKeySequence("ctrl+q"), ui->tabWidget);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_actionExit_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupEditor()
{
    //Creating the first tab.
    tabs.resize(1);
    tabs.clear();
    ui->tabWidget->setTabsClosable(false);
    Tab *tab = new Tab();
    tabs.push_back(tab);
    tab->editor = new QCodeEditor();
    tab->highlighter = new SyntaxLighter(tab->editor->document());
    ui->tabWidget->addTab(tab->editor, "Untitled");
    tab->number = 0;
    tab->path = "";
    tab->filename = "";
    ui->tabWidget->setTabText(0, "Untitled");
}


QString MainWindow::extractFilename(QString path)
{
    std::string stdPath = path.toStdString();
    return QString::fromStdString(stdPath.substr(stdPath.find_last_of('/') + 1));
}

void MainWindow::openFile()
{
    QString currentPath = QFileDialog::getOpenFileName(this, "Open text file");
    QFile file(currentPath);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        tabs[currentTab()]->path = currentPath;
        QByteArray byteArray = file.readAll();
        tabs[currentTab()]->editor->setPlainText(byteArray.data());

        MainWindow::setWindowTitle(tabs[currentTab()]->path.toUtf8() + " - pEdit");
        tabs[currentTab()]->filename = extractFilename(tabs[currentTab()]->path);
        setTabName(currentTab(), tabs[currentTab()]->filename);
    }
}

int MainWindow::currentTab()
{
   return ui->tabWidget->currentIndex();

}

void MainWindow::saveFileAs()
{
    QString currentPath = QFileDialog::getSaveFileName(0,"Save file",QDir::currentPath(),
                                                    "Text file (*.txt);;Header (*.h);;Source file(*.cpp)",
                                                        new QString("Text files (*.txt)"));
    QFile file(currentPath);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        tabs[currentTab()]->path = currentPath;
        file.write(tabs[currentTab()]->editor->toPlainText().toUtf8());
        MainWindow::setWindowTitle(tabs[currentTab()]->path.toUtf8() + " - pEdit");
        tabs[currentTab()]->filename = extractFilename(tabs[currentTab()]->path);
        setTabName(currentTab(), tabs[currentTab()]->filename);
    }
}

void MainWindow::switchToNextTab()
{
    if (currentTab() != (tabs.size() - 1)) {
        ui->tabWidget->setCurrentIndex(currentTab() + 1);
    }else {
        ui->tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::switchToPrevTab()
{
    if (currentTab() != 0) {
        ui->tabWidget->setCurrentIndex(currentTab() - 1);
    }
    else {
        ui->tabWidget->setCurrentIndex(tabs.size() - 1);
    }
}

void MainWindow::saveFile()
{
    QFile file(tabs[currentTab()]->path);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        file.write(tabs[currentTab()]->editor->toPlainText().toUtf8());
    }
}

void MainWindow::on_actionOpen_triggered()
{
  openFile();
}

void MainWindow::on_actionSave_triggered()
{
  if (tabs[currentTab()]->path.isEmpty())
  {
      saveFileAs();
  }else {
      saveFile();
  }
}

void MainWindow::on_actionSave_As_triggered()
{
    saveFileAs();
}


void MainWindow::on_actionNew_triggered()
{
    tabs[currentTab()]->editor->clear();
    MainWindow::setWindowTitle("Untitled - pEdit 1.0");
    setTabName(currentTab(), "Untitled");
}

void MainWindow::on_tabWidget_tabBarDoubleClicked()
{
    makeNewTab();
}

void MainWindow::makeNewTab()
{
    Tab *tab = new Tab();
    tab->editor = new QCodeEditor();
    tab->highlighter = new SyntaxLighter(tab->editor->document());
    ui->tabWidget->addTab(tab->editor, "Untitled");
    tab->number = tabs.back()->number + 1;
    tab->path = "";
    tab->filename = "";
    tabs.push_back(tab);

    ui->tabWidget->setCurrentIndex(tab->number);
    if (tabs.size() > 1) {
        ui->tabWidget->setTabsClosable(true);
    }
}


void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}


void MainWindow::on_actionAbout_triggered()
{
     QMessageBox::about(this,tr("About"), tr("<h1>pEdit v1.0</h1>A text editor supporting C++ syntax highlighting, "
                                             "keywords completing and compiling. <br> Polina Tolmach, 2015 </br>"));

}

void MainWindow::setTabName(unsigned int index, QString text)
{
      ui->tabWidget->setTabText(index, text);
}


void MainWindow::on_actionNew_tab_triggered()
{
    makeNewTab();
}


void MainWindow::on_actionCopy_triggered()
{
   tabs[currentTab()]->editor->copy();
}

void MainWindow::on_actionPaste_triggered()
{
   tabs[currentTab()]->editor->paste();
}

void MainWindow::on_actionUndo_triggered()
{
  tabs[currentTab()]->editor->undo();

}

void MainWindow::on_actionRedo_triggered()
{
   tabs[currentTab()]->editor->redo();

}

void MainWindow::on_actionCut_triggered()
{
   tabs[currentTab()]->editor->cut();
}

void MainWindow::deleteCurrentTab()
{
    deleteTab(currentTab());
}

void MainWindow::deleteTab(int tab_number)
{
    tabs.erase(tabs.begin() + tab_number);
    ui->tabWidget->removeTab(tab_number);
    if (tabs.size() == 1) {
        ui->tabWidget->setTabsClosable(false);
    }
}

void MainWindow::on_actionSelect_All_triggered()
{
    tabs[currentTab()]->editor->selectAll();
}

void MainWindow::on_actionCompile_triggered()
{
    process = new QProcess();

    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(compile()));
    process->start(QString("cmd /c "+path[0]+": & cd "+path+ " & ")
                            + path + compiler + " " + this->tabs[currentTab()]->path + " -o temp.exe");
    process->waitForFinished(-1);
    delete process;

    system(("\""+path+"temp.exe"+"\" & pause").toStdString().c_str());
}

void MainWindow::compile()
{
    qDebug() << "Process error code:" << process->error();
    QByteArray outData = process->readAllStandardOutput();
    qDebug()<<QString(outData);
    QMessageBox::about(this,tr("About"), QString(outData));
}

void MainWindow::on_actionCompiler_options_triggered()
{
   options compiler_options;
   compiler_options.setExe(compiler);
   compiler_options.setPath(path);
   compiler_options.exec();
   path = compiler_options.getPath();
   compiler = compiler_options.getExe();
}

