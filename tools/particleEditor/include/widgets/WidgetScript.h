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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETSCRIPT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETSCRIPT_H__

#include "ui_widgetScript.h"

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace utils {
	class SyntaxHighlighter;
} /* namespace utils */
namespace widgets {

	class WidgetScript : public QWidget, public Ui::scriptWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		WidgetScript(QWidget * par);

		/**
		 * \brief destructor
		 */
		~WidgetScript();

		QString getScript() const;

		void parseScript();

	signals:
		void notifyChanged();

	public slots:
		void loadScript(ParticleUniverse::ParticleSystem * system);

	private slots:
		void changedText();

	private:
		bool _changeable;
		utils::SyntaxHighlighter * _syntaxHighlighter;
		QString _text;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETSCRIPT_H__ */
