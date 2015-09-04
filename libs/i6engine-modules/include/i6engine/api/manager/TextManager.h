/**
 * \addtogroup API
 * @{
 */

#ifndef __I6ENGINE_API_TEXTMANAGER_H__
#define __I6ENGINE_API_TEXTMANAGER_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6engine {
namespace api {

	class EngineController;

	/**
	 * \brief handles Texts for the Game
	 */
	class ISIXE_MODULES_API TextManager {
		friend class EngineController;

	public:
		TextManager();

		/**
		 * \brief returns the text in the current language for the given key
		 */
		std::string getText(const std::string & key) const;

	private:
		std::string _language;
		std::map<std::string, std::map<std::string, std::string>> _texts;
		
		void setLanguage(const std::string & language) {
			_language = language;
		}

		void initialize(const std::string & textDir);
		void loadFile(const std::string & file);
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_TEXTMANAGER_H__ */

/**
 * @}
 */
