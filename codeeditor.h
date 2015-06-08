/*
 * Created by Polina Tolmach
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 */

#ifndef CODEEDITOR
#define CODEEDITOR

#include <QPlainTextEdit>
#include <QObject>
#include <QAbstractItemModel>
#include <QCompleter>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QCompleter;
class LineNumberArea;

class QCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    QCodeEditor(QWidget *parent = 0);
    ~QCodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaSize();
    void addCompleter(QCompleter *custom_completer);
    QCompleter *completer() const;
    QString currentText() const;
    QAbstractItemModel *modelFromTextfile(const QString& fileName);

protected:
    void resizeEvent(QResizeEvent *resize_event);
    void focusInEvent(QFocusEvent *completion_event);
    void keyPressEvent(QKeyEvent *completion_event);

private slots:
    void changeLineNumberAreaSize(int newBlockCount);
    void highlightCurrentLine();
    void changeLineNumberArea(const QRect &rectangle, int);
    void insertCompletion(const QString &completion);

private:
    QWidget *lineNumberArea;
    QColor edgingNumbersColour;
    QColor edgingBackground;
    QColor currentLineBackground;
    QCompleter *codeCompleter;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(QCodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaSize(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    QCodeEditor *codeEditor;
};

#endif // CODEEDITOR

