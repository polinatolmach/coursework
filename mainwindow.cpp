#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "syntaxlighter.h"

#include <QMessageBox>
#include <QVector>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    this->setFixedHeight(600);
    this->setFixedWidth(800);
    this->setWindowTitle("pEdit 1.0");
    this->setAcceptDrops(true);
    this->lightfactor = 120;

    setCentralWidget(ui->tabWidget);
    tabs.resize(1);

    MainWindow::setWindowTitle("Untitled - pEdit");
    ui->plainTextEdit->setAcceptDrops(FALSE);
     MainWindow::setWindowTitle("Untitled - pEdit");
 //    MainWindow::setWindowIcon(QIcon(QApplication::applicationDirPath() + "/icon.png"));
     ui->tabWidget->setTabsClosable(false);

     Tab tab;
     tab.number = 0;
     tab.path = "";
     tab.filename = "";
     tab.plainTextEdit = ui->plainTextEdit;
     tab.highlighter = new SyntaxLighter(ui->plainTextEdit->document());
     tabs.push_back(tab);
    //sets the tab data for the first tab
     ui->tabWidget->setTabText(0, "Untitled");
     ui->plainTextEdit->setPlainText("Yo.");



     QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->plainTextEdit->setFont(font);
    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    ui->plainTextEdit->setTabStopWidth(tabStop * metrics.width(' '));

    QToolButton *newTabButton = new QToolButton(this);
    ui->tabWidget->setCornerWidget(newTabButton);
    newTabButton->setAutoRaise(true);
    newTabButton->setText("+");
     //connect signals
    QObject::connect(newTabButton, SIGNAL(clicked()), this, SLOT(makeNewTab()));
    QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteTab(int)));
    QObject::connect(this, SIGNAL(cursorPositionChanged(textCursor)), this, SLOT(highlightCurrentLine()));
    highlightCurrentLine();

}

MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::currentTab()
{
   return ui->tabWidget->currentIndex();

}


//void MainWindow::openFile()
//{
//    QString tmpPath = QFileDialog::getOpenFileName(this);
//    QFile file(tmpPath);
//    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
//        tabs[currentTab()].path = tmpPath;
//        QByteArray byteArray = file.readAll();
//        tabs[currentTab()].plainTextEdit->setPlainText(byteArray.data());

     //   MainWindow::setWindowTitle(tabs[currentTab()].path.toUtf8() + " - BenPad");
//        tabs[currentTab()].filename = extractFilename(tabs[currentTab()].path);
//        setTabName(currentTab(), tabs[currentTab()].filename);




//void MainWindow::saveFileAs()
//{
//    QString tmpPath = QFileDialog::getSaveFileName();
//    QFile file(tmpPath);
//    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
//        tabs[currentTab()].path = tmpPath;
//        file.write(ui->plainTextEdit->toPlainText().toUtf8());
//        MainWindow::setWindowTitle(tabs[currentTab()].path.toUtf8() + " - BenPad");
//      //  tabs[currentTab()].filename = extractFilename(tabs[currentTab()].path);
//        setTabName(currentTab(), tabs[currentTab()].filename);
//    }
//}

//void MainWindow::saveFile()
//{
//    QFile file(tabs[currentTab()].path);
//    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
//        file.write(ui->plainTextEdit->toPlainText().toUtf8());
//    }
//}

//void MainWindow::on_actionOpen_triggered()
//{
//  openFile();
//}

//void MainWindow::on_actionSave_triggered()
//{
//  if (tabs[currentTab()].path.isEmpty())
//  {
//      saveFileAs();
//  }else {
//      saveFile();
//  }
//}

//void MainWindow::on_actionSave_As_triggered()
//{
//    saveFileAs();
//}


void MainWindow::on_actionNew_triggered()
{
    ui->plainTextEdit->clear();
    MainWindow::setWindowTitle("Untitled - pEdit 1.0");
    setTabName(currentTab(), "Untitled");
}

void MainWindow::makeNewTab()
{

    tabs.resize(tabs.size()+1);
    Tab tab;
    tab.plainTextEdit = new QPlainTextEdit;
    tab.highlighter = new SyntaxLighter(ui->plainTextEdit->document());
    ui->tabWidget->addTab(tab.plainTextEdit, "Untitled");
    tab.number = tabs[tabs.size() - 1].number + 1;
    tab.path = "";
    tab.filename = "";
    tabs.push_back(tab);

    ui->tabWidget->setCurrentIndex(tab.number);

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

  void MainWindow::highlightCurrentLine()
 {

     QTextEdit::ExtraSelection highlight;
     highlight.cursor = ui->plainTextEdit->textCursor();
     highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
     highlight.format.setBackground(Qt::lightGray);

     QList<QTextEdit::ExtraSelection> extras;
     extras << highlight;
     ui->plainTextEdit->setExtraSelections( extras );
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
   tabs[currentTab()].plainTextEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
   tabs[currentTab()].plainTextEdit->paste();

}

void MainWindow::on_actionUndo_triggered()
{
  tabs[currentTab()].plainTextEdit->undo();

}

void MainWindow::on_actionRedo_triggered()
{
   tabs[currentTab()].plainTextEdit->redo();

}

void MainWindow::on_actionCut_triggered()
{
   tabs[currentTab()].plainTextEdit->cut();

}

void MainWindow::deleteCurrentTab()
{
    deleteTab(currentTab());
}

void MainWindow::deleteTab(int i)
{
    tabs[currentTab()].plainTextEdit->deleteLater();
    tabs.erase(tabs.begin() + i);
    ui->tabWidget->removeTab(i);
    if (tabs.size() == 1) {
        ui->tabWidget->setTabsClosable(false);
    }
}


