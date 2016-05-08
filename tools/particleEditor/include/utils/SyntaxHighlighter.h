#ifndef __I6ENGINE_PARTICLEEDITOR_UTILS_SYNTAXHIGHLIGHTER_H__
#define __I6ENGINE_PARTICLEEDITOR_UTILS_SYNTAXHIGHLIGHTER_H__

#include <QSyntaxHighlighter>

namespace i6e {
namespace particleEditor {
namespace utils {
	
	class SyntaxHighlighter : public QSyntaxHighlighter {
		Q_OBJECT

	public:
		SyntaxHighlighter(QTextDocument * parent = nullptr);

	protected:
		void highlightBlock(const QString & text) Q_DECL_OVERRIDE;

	private:
		struct HighlightingRule {
			QRegExp pattern;
			QTextCharFormat format;
		};
		QVector<HighlightingRule> highlightingRules;

		QRegExp commentStartExpression;
		QRegExp commentEndExpression;

		QTextCharFormat keywordFormat;
		QTextCharFormat classFormat;
	};

} /* namespace utils */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_UTILS_SYNTAXHIGHLIGHTER_H__ */
