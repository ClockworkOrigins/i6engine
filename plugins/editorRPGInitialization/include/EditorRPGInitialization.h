#ifndef __I6ENGINE_PLUGINS_EDITORRPGINITIALIZATION_H__
#define __I6ENGINE_PLUGINS_EDITORRPGINITIALIZATION_H__

#include "i6engine/editor/plugins/InitializationPluginInterface.h"

#include <QObject>

namespace i6e {
namespace plugins {

	class EditorRPGInitialization : public QObject, public editor::plugins::InitializationPluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.initialization.InitializationPluginInterface")
		Q_INTERFACES(i6e::editor::plugins::InitializationPluginInterface)

	public:
		void initialize();
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORRPGINITIALIZATION_H__ */
