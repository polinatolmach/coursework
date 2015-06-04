/*
 * Created by Polina Tolmach 
 *
 * Distributed under the MIT License
 * http://opensource.org/licenses/MIT
 *
 * A part of pEdit project
 *
 */

#ifndef SYNTAXLIGHTER_H
#define SYNTAXLIGHTER_H

#include <QtGui>
#include <QTextDocument>
#include <QRegExp>
#include <QTextCharFormat>

class SyntaxLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
 SyntaxLighter(QTextDocument *parent = 0);
 ~SyntaxLighter();

protected:

void highlightBlock(const QString &text);


private:
    struct HighlightingRule
       {
           QRegExp pattern;
           QTextCharFormat format;
       };
       QVector<HighlightingRule> highlightingRules;

       QRegExp commentStartExpression;
       QRegExp commentEndExpression;

       QTextCharFormat keywordFormat;
       QTextCharFormat classFormat;
       QTextCharFormat singleLineCommentFormat;
       QTextCharFormat multiLineCommentFormat;
       QTextCharFormat quotationFormat;
       QTextCharFormat functionFormat;
       QTextCharFormat numberFormat;
       QTextCharFormat preprocessorFormat;
   };

#endif
