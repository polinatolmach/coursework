#include<QtGui>
#include <QRegExp>
#include <QTextDocument>

#include "syntaxlighter.h"

SyntaxLighter::SyntaxLighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                 << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                 << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                 << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                 << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                 << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                 << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                 << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                 << "\\bunion\\b" << "\\bunsigned\\b" << "\\bfunction\\b"<< "\\bvirtual\\b"
                 << "\\bvoid\\b" << "\\bvolatile\\b";
    foreach (QString pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkCyan);

    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);


    numberFormat.setForeground(Qt::green);
    rule.pattern = QRegExp("[0-9]");
    rule.format = numberFormat;
    highlightingRules.append(rule);


    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
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

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
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
