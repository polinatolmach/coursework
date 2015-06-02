#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "syntaxlighter.h"
#include "codeeditor.h"

#include <QMessageBox>
#include <QVector>
#include <QFileInfo>
#include <QString>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setupEditor();

    this->setWindowTitle("Untitled - pEdit 1.0");
    this->setAcceptDrops(true);
    this->lightfactor = 120;

    setCentralWidget(ui->tabWidget);


    QToolButton *newTabButton = new QToolButton(this);
    ui->tabWidget->setCornerWidget(newTabButton);
    newTabButton->setAutoRaise(true);
    newTabButton->setText("+");
     //connect signals
    QObject::connect(newTabButton, SIGNAL(clicked()), this, SLOT(makeNewTab()));
    QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteTab(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupEditor()
{
tabs.clear();
tabs.resize(1);
MainWindow::setWindowTitle("Untitled - pEdit");

 MainWindow::setWindowTitle("Untitled - pEdit");
 MainWindow::setWindowIcon(QIcon(QApplication::applicationDirPath() + "/text-editor.png"));
 ui->tabWidget->setTabsClosable(false);
tabs.clear();

Tab *tab = new Tab();
tabs.push_back(tab);
tab->editor = new QCodeEditor();
tab->highlighter = new SyntaxLighter(tab->editor->document());
ui->tabWidget->addTab(tab->editor, "Untitled");
tab->number = 0;//tabs.back()->number + 1;
tab->path = "";
tab->filename = "";
ui->tabWidget->setTabText(0, "Untitled");

}



QString MainWindow::extractFilename(QString path)
{
    std::string stdPath = path.toStdString();
  //  #ifdef WIN32
  //      return QString::fromStdString(stdPath.substr(stdPath.find_last_of('\\') + 1));
  //  #else
       return QString::fromStdString(stdPath.substr(stdPath.find_last_of('/') + 1));
  //  #endif
}


void MainWindow::openFile()
{
    QString tmpPath = QFileDialog::getOpenFileName(this);
    QFile file(tmpPath);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        tabs[currentTab()]->path = tmpPath;
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
    QString tempPath = QFileDialog::getSaveFileName();
    QFile file(tempPath);
   if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        tabs[currentTab()]->path = tempPath;
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

void MainWindow::makeNewTab()
{

   // tabs.resize(tabs.size()+1);
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
     QMessageBox::about(this,tr("About"), tr("<h1>Help</h1>Here you can get some help.</br> And information."));
}



  void MainWindow::setTabName(unsigned int index, QString text){
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

void MainWindow::deleteTab(int i)
{
    tabs.erase(tabs.begin() + i);
    ui->tabWidget->removeTab(i);
    if (tabs.size() == 1) {
        ui->tabWidget->setTabsClosable(false);
    }
}

void MainWindow::on_actionSelect_All_triggered()
{
    tabs[currentTab()]->editor->selectAll();
}


