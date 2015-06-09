/*
 * Created by Polina Tolmach
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 * A "syntaxlighter.h" implementation
 *
 */

#include<QtGui>
#include <QRegExp>
#include <QTextDocument>

#include "syntaxlighter.h"

SyntaxLighter::SyntaxLighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkMagenta);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    QFile file(":/wordlist.txt");
       if (file.open(QFile::ReadOnly)) {
           while (!file.atEnd()) {
               QByteArray line = file.readLine();
               if (!line.isEmpty()) {
                   keywordPatterns << tr("%1%2%3").arg("\\b", line.trimmed(), "\\b");
                   keywordPatterns << tr("%1%2%3").arg("\\b", line.trimmed().toUpper(), "\\b");
               }
           }
       }


    foreach (QString pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkCyan);
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    preprocessorFormat.setForeground(Qt::darkYellow);
    rule.pattern = QRegExp("#.*");
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("[0-9]");
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

SyntaxLighter::~SyntaxLighter()
{
}

void SyntaxLighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
    setCurrentBlockState(0);

   /* Multiline comment highlighting.
    * If there's no state, the returned value is -1.
    * If the previous block state was "in comment" state is 1
    * and we start the search for an end expression at the beginning of the text block.
    * If the previousBlockState() returns 0,
    * we start the search at the location of the first occurrence of a start expression.
    * When an end expression is found, we count the length of the comment and apply the format.
    * Then we search for the next occurrence of the start expression and repeat the process.
    * If no end expression can be found in the current block,
    * we set the current block state to 1, "in comment".
    */
    int startIndex = 0;
    int commentLength;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        if (endIndex == -1) {
             setCurrentBlockState(1);
             commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression,
                                             startIndex + commentLength);
    }
}
