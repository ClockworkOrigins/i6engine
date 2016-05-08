#include "utils/SyntaxHighlighter.h"

namespace i6e {
namespace particleEditor {
namespace utils {

	SyntaxHighlighter::SyntaxHighlighter(QTextDocument * parent) : QSyntaxHighlighter(parent) {
		HighlightingRule rule;

		keywordFormat.setForeground(Qt::blue);
		keywordFormat.setFontWeight(QFont::Bold);
		QStringList keywordPatterns;
		keywordPatterns << "\\bsystem\\b" << "\\btechnique\\b" << "\\baffector\\b"
			<< "\\bobserver\\b" << "\\bemitter\\b" << "\\brenderer\\b";

		foreach(const QString & pattern, keywordPatterns) {
			rule.pattern = QRegExp(pattern);
			rule.format = keywordFormat;
			highlightingRules.append(rule);
		}
	}

	void SyntaxHighlighter::highlightBlock(const QString & text) {
		foreach(const HighlightingRule &rule, highlightingRules) {
			QRegExp expression(rule.pattern);
			int index = expression.indexIn(text);
			while (index >= 0) {
				int length = expression.matchedLength();
				setFormat(index, length, rule.format);
				index = expression.indexIn(text, index + length);
			}
		}
		setCurrentBlockState(0);
	}

} /* namespace utils */
} /* namespace particleEditor */
} /* namespace i6e */