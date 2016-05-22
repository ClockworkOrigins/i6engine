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

#ifndef __I6ENGINE_EDITOR_PLUGINS_TYPEPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_TYPEPLUGININTERFACE_H__

#include <qplugin.h>
#include <QWidget>

namespace i6e {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for the widget created by a TypePlugin
	 */
	class TypeWidgetInterface : public QWidget {
	public:
		TypeWidgetInterface(QWidget * parent) : QWidget(parent) {
		}

		virtual ~TypeWidgetInterface() {
		}

		/**
		 * \brief sets values to this widget
		 */
		virtual void setValues(const std::string & values) = 0;

		/**
		 * \brief gets values of this widget
		 */
		virtual std::string getValues() const = 0;

		/**
		 * \brief sets this widget to be read only or not
		 */
		virtual void setReadOnly(bool readOnly) = 0;
	};

	/**
	 * \brief interface for plugins used to offer types for Components attributes
	 */
	class TypePluginInterface {
	public:
		virtual ~TypePluginInterface() {
		}

		/**
		 * \brief identifier to access this plugin
		 */
		virtual std::string getIdentifier() const = 0;

		/**
		 * \brief creates a new TypeWidget for this plugin
		 */
		virtual TypeWidgetInterface * createWidget(QWidget * parent) = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::editor::plugins::TypePluginInterface, "i6e.editor.type.TypePluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_TYPEPLUGININTERFACE_H__ */
