#ifndef __I6ENGINE_PLUGINS_EDITORSAVEOBJECTRPG_H__
#define __I6ENGINE_PLUGINS_EDITORSAVEOBJECTRPG_H__

#include "i6engine/editor/plugins/SaveObjectPluginInterface.h"

#include <fstream>

namespace i6engine {
namespace plugins {

	class EditorSaveObjectRPG : public QObject, public editor::plugins::SaveObjectPluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.saveObject.SaveObjectPluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::SaveObjectPluginInterface)

	public:
		EditorSaveObjectRPG();

		/**
		 * \brief initializes save process using given level file
		 */
		void startLevel(const QString & level);

		/**
		 * \brief tries to save GameObject during saving in editor
		 * \param[in] go the GOPtr to the GameObject to be saved
		 * \param[in] element the XMLElement this go should be linked to if necessary
		 * \returns true if object was handled by this plugin, otherwise false
		 */
		bool saveObject(const i6engine::api::GOPtr & go, tinyxml2::XMLElement * element);

		/**
		 * \brief finishes save process
		 */
		void finishLevel();

	private:
		std::ofstream _file;
		QString _scriptsPath;
		QString _fileExtension;
		QString _level;

		void insertMethodDeclaration();
		void saveNPC(const std::string & identifier, const Vec3 & pos, const Quaternion & rot);
		void saveItem(const std::string & identifier, const Vec3 & pos, const Quaternion & rot);
		void insertMethodEnd();
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORSAVEOBJECTRPG_H__ */
