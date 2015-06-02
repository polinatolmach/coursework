#include <QtWidgets>

#include "codeeditor.h"

QCodeEditor::QCodeEditor(QWidget *parent) :
    QPlainTextEdit(parent), codeCompleter(0)
{

    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    this->setFont(font);
    const int tabStop = 4;  // 4 characters
    QFontMetrics metrics(font);
    this->setTabStopWidth(tabStop * metrics.width(' '));
    currentLineBackground = QColor(247, 229, 247);
    marginBackground = QColor(227, 221, 235);
    marginForeground = QColor(122, 91, 161);

    this->setLineWrapMode(QPlainTextEdit::NoWrap);

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();


    codeCompleter = new QCompleter(this);
    codeCompleter->setModel(modelFromFile(":/wordlist.txt"));
    codeCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    codeCompleter->setWrapAround(false);
    this->setCompleter(codeCompleter);
}

QCodeEditor::~QCodeEditor()
{
}

void QCodeEditor::setCompleter(QCompleter *custom_completer)
{
    if (codeCompleter)
        QObject::disconnect(codeCompleter, 0, this, 0);

    codeCompleter = custom_completer;

    if (!codeCompleter)
        return;

    codeCompleter->setWidget(this);
    codeCompleter->setCompletionMode(QCompleter::PopupCompletion);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(codeCompleter, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

//QCompleter *QCodeEditor::completer() const
//{
//    return codeCompleter;
//}

QAbstractItemModel *QCodeEditor::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(codeCompleter);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, codeCompleter);
}

QString QCodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void QCodeEditor::insertCompletion(const QString& completion)
{
    if (codeCompleter->widget() != this)
        return;
    QTextCursor text_cursor = textCursor();
    int extra = completion.length() - codeCompleter->completionPrefix().length();
    text_cursor.movePosition(QTextCursor::Left);
    text_cursor.movePosition(QTextCursor::EndOfWord);
    text_cursor.insertText(completion.right(extra));
    setTextCursor(text_cursor);
}

void QCodeEditor::focusInEvent(QFocusEvent *completion_event)
{
    if (codeCompleter)
        codeCompleter->setWidget(this);
    QPlainTextEdit::focusInEvent(completion_event);
}

void QCodeEditor::keyPressEvent(QKeyEvent *completion_event)
{
    if (codeCompleter && codeCompleter->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (completion_event->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            completion_event->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((completion_event->modifiers() & Qt::ControlModifier) && completion_event->key() == Qt::Key_E); // CTRL+E
    if (!codeCompleter || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(completion_event);

    const bool ctrlOrShift = completion_event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!codeCompleter || (ctrlOrShift && completion_event->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (completion_event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || completion_event->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(completion_event->text().right(1)))) {
        codeCompleter->popup()->hide();
        return;
    }

    if (completionPrefix != codeCompleter->completionPrefix()) {
        codeCompleter->setCompletionPrefix(completionPrefix);
        codeCompleter->popup()->setCurrentIndex(codeCompleter->completionModel()->index(0, 0));
    }
    QRect cursor_rect = cursorRect();
    cursor_rect.setWidth(codeCompleter->popup()->sizeHintForColumn(0)
                + codeCompleter->popup()->verticalScrollBar()->sizeHint().width());
    codeCompleter->complete(cursor_rect); // popup it up!
}

//void QCodeEditor::setTabSpaces(const int tabStop) {
//    QFontMetrics metrics(this->font());
//    this->setTabStopWidth(tabStop * metrics.width(' '));
//}

//void QCodeEditor::setFont(const QFont& font)
//{
//    this->setTabSpaces(4);
//    this->setFont(font);
//}

int QCodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void QCodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QCodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void QCodeEditor::resizeEvent(QResizeEvent *resize_event)
{
    QPlainTextEdit::resizeEvent(resize_event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void QCodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(currentLineBackground);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void QCodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), marginBackground);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(marginForeground);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

