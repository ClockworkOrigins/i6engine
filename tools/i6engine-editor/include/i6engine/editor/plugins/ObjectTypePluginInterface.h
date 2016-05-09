/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
