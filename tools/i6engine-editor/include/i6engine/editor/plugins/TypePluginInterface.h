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

#ifndef __I6ENGINE_EDITOR_PLUGINS_TYPEPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_TYPEPLUGININTERFACE_H__

#include <qplugin.h>
#include <QWidget>

namespace i6engine {
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
} /* namespace i6engine */

Q_DECLARE_INTERFACE(i6engine::editor::plugins::TypePluginInterface, "i6engine.editor.type.TypePluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_TYPEPLUGININTERFACE_H__ */
