#ifndef CODEEDITOR
#define CODEEDITOR

#include <QPlainTextEdit>
#include <QObject>
#include <QAbstractItemModel>
#include <QCompleter>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class QCompleter;
class LineNumberArea;

class QCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    QCodeEditor(QWidget *parent = 0);
    ~QCodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    //void setTabSpaces(const int tabStop);
    //void setFont(const QFont& font);
    void setCompleter(QCompleter *custom_completer);
    QCompleter *completer() const;
    QString textUnderCursor() const;
    QAbstractItemModel *modelFromFile(const QString& fileName);

protected:
    void resizeEvent(QResizeEvent *resize_event);
    void focusInEvent(QFocusEvent *completion_event);// Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *completion_event);// Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void insertCompletion(const QString &completion);

private:
    QWidget *lineNumberArea;
    QColor marginForeground;
    QColor marginBackground;
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
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    QCodeEditor *codeEditor;
};

#endif // CODEEDITOR
