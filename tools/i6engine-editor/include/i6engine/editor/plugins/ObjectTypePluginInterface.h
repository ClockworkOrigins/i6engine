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

#ifndef __I6ENGINE_EDITOR_PLUGINS_OBJECTTYPEPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_OBJECTTYPEPLUGININTERFACE_H__

#include <functional>

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include <qplugin.h>
#include <QWidget>

namespace i6e {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for the widget created by a ObjectTypePlugin
	 */
	class ObjectTypeWidgetInterface : public QWidget {
	public:
		ObjectTypeWidgetInterface(QWidget * parent) : QWidget(parent), _callback() {
		}

		virtual ~ObjectTypeWidgetInterface() {
		}

		void setUpdateCallback(const std::function<void(void)> & callback) {
			_callback = callback;
		}

	protected:
		std::function<void(void)> _callback;
	};

	/**
	 * \brief interface for plugins used to offer new types of objects to be inserted in level, e.g. NPCs for RPG module
	 */
	class ObjectTypePluginInterface {
	public:
		virtual ~ObjectTypePluginInterface() {
		}

		/**
		 * \brief identifier to access this plugin
		 */
		virtual std::string getIdentifier() const = 0;

		/**
		 * \brief creates a new ObjectTypeWidget for this plugin
		 */
		virtual ObjectTypeWidgetInterface * createWidget(QWidget * parent) = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::editor::plugins::ObjectTypePluginInterface, "i6e.editor.objectType.ObjectTypePluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_OBJECTTYPEPLUGININTERFACE_H__ */
