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
    keywordPatterns   << "\\balighas\\b" << "\\balignof\\b"
                      << "\\band\\b"  << "\\band_eq\\b"<<  "\\basm\\b"  << "\\bauto\\b"
                      << "\\bbitand\\b" << "\\bbitor\\b"<< "\\bbool\\b" << "\\bbreak\\b"
                      << "\\bcase\\b" << "\\bcatch\b" << "\\bchar\\b"
                      << "\\bclass\\b"  << "\\bcompl\\b" << "\\bconcept\\b"<<"\\bconst\\b"
                      << "\\bconstexpr\\b" << "\\bconst_cast\\b"
                      << "\\bcontinue\\b" << "\\bdecltype\\b" << "\\bdefault\\b"
                      << "\\bdelete\\b" << "\\bdo\\b" << "\\bdouble\\b""<<\\bdynamic_cast\\b"
                      << "\\belse\\b" << "\\benum\\b" << "\\bexplicit\\b"
                      << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b"
                      << "\\bfloat\\b" << "\\bfor\\b" << "\\bforeach\\b"
                      << "\\bfriend\\b" << "\\bfloat\\b" << "\\bgoto\\b"<< "\\bif\\b"
                      << "\\bifdef\\b" << "\\bifndef\\b"<< "\\bimplement\\b"
                      <<"\\binline\\b" << "\\bint\\b"
                      << "\\blong\\b" << "\\bmutable\b" << "\\bnamespace\\b"
                      << "\\bnew\\b" <<"\\bnoexcept\\b"<< "\\bnot\\b" << "\\bnot_eq\\b"
                      << "\\boperator\\b" << "\\bor\\b" << "\\bor_eq\\b"
                      << "\\boverride\\b" << "\\bprivate\\b" << "\\bprotected\\b""<<\\bpublic\\b"
                      << "\\bregister\\b" << "\\breinterpret_cast\\b" << "\\brequires\\b"
                      << "\\breturn\\b" << "\\bshort\\b" << "\\bsignals\\b"
                      << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bslots\\b"
                      << "\\bstatic\\b" << "\\bstatic_assert\\b" << "\\bstatic_cast\\b"
                      << "\\bstruct\\b"
                      << "\\bswitch\\b" << "\\btemplate\\b"<<"\\bthis\\b"
                      << "\\bthread_local\\b" << "\\bthrow\\b"
                      << "\\btrue\\b" << "\\btry\b" << "\\btypedef\\b"
                      << "\\btypeid\\b" <<"\\btypename\\b"<< "\\bunion\\b" << "\\bunsigned\\b"
                      << "\\busing\\b" << "\\bvirtual\\b" << "\\bvoid\\b"
                      << "\\bvolatile\\b" << "\\bwchar_t\\b" << "\\bwhile\\b""<<\\xor\\b"
                      << "\\bxor_eq\\b" ;

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

    preprocessorFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("#.*");
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("[0-9]");
    rule.format = numberFormat;
    highlightingRules.append(rule);


    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
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
