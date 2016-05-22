/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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