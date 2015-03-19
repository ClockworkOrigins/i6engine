#ifndef __I6ENGINE_API_TEXTMANAGER_H__
#define __I6ENGINE_API_TEXTMANAGER_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6engine {
namespace api {

	class EngineController;

	class ISIXE_MODULES_API TextManager {
		friend class EngineController;

	public:
		/**
		 * \brief returns the text in the actual language for the given key
		 */
		std::string getText(const std::string & key) const;

	private:
		std::string _language;
		std::map<std::string, std::map<std::string, std::string>> _texts;

		TextManager();

		void setLanguage(const std::string & language) {
			_language = language;
		}

		void initialize(const std::string & textDir);
		void loadFile(const std::string & file);
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_TEXTMANAGER_H__ */
