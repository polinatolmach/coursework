/*
 * Created by Polina Tolmach 
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 * A "codeeditor.h" implementation
 * 
 */

#include "codeeditor.h"

#include <QtWidgets>


QCodeEditor::QCodeEditor(QWidget *parent) :
    QPlainTextEdit(parent), codeCompleter(0)
{
    //Setting up font and tab size in our editor.
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    this->setFont(font);
    const int tabStop = 4;  // 4 characters.
    QFontMetrics metrics(font);
    this->setTabStopWidth(tabStop * metrics.width(' '));

    currentLineBackground = QColor(247, 229, 247);
    edgingBackground = QColor(227, 221, 235);
    edgingNumbersColour = QColor(122, 91, 161);

    this->setLineWrapMode(QPlainTextEdit::NoWrap);

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(changeLineNumberAreaSize(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(changeLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    changeLineNumberAreaSize(0);
    highlightCurrentLine();

  //*Creating of a completer, passing it a list of
   /*keywords for completing, adding a completer
   /* to an editor.
    */
    codeCompleter = new QCompleter(this);
    codeCompleter->setModel(modelFromTextfile(":/wordlist.txt"));
    codeCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    codeCompleter->setWrapAround(false);
    this->addCompleter(codeCompleter);
}

QCodeEditor::~QCodeEditor()
{
}

void QCodeEditor::addCompleter(QCompleter *custom_completer)
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

QAbstractItemModel *QCodeEditor::modelFromTextfile(const QString& fileName)
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

QString QCodeEditor::currentText() const
{
    QTextCursor text_cursor = textCursor();
    text_cursor.select(QTextCursor::WordUnderCursor);
    return text_cursor.selectedText();
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
       switch (completion_event->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            completion_event->ignore();
            return;
       default:
           break;
       }
    }

    QPlainTextEdit::keyPressEvent(completion_event);

    static QString endofword("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    QString completionPrefix = currentText();
    if (completionPrefix.length() < 3 || endofword.contains(completion_event->text().right(1))) {
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
    codeCompleter->complete(cursor_rect);
}

int QCodeEditor::lineNumberAreaSize()
{
    int lines_number = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++lines_number;
    }

    int edging_space = 3 + fontMetrics().width(QLatin1Char('9')) * lines_number;

    return edging_space;
}

void QCodeEditor::changeLineNumberAreaSize(int /* eachNewBlockCount */)
{
    setViewportMargins(lineNumberAreaSize(), 0, 0, 0);
}

void QCodeEditor::changeLineNumberArea(const QRect &rectangle, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rectangle.y(), lineNumberArea->width(), rectangle.height());

    if (rectangle.contains(viewport()->rect()))
        changeLineNumberAreaSize(0);
}

void QCodeEditor::resizeEvent(QResizeEvent *resize_event)
{
    QPlainTextEdit::resizeEvent(resize_event);

    QRect rectangle_area = contentsRect();
    lineNumberArea->setGeometry(QRect(rectangle_area.left(), rectangle_area.top(),
                                      lineNumberAreaSize(), rectangle_area.height()));
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
    painter.fillRect(event->rect(), edgingBackground);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(edgingNumbersColour);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

