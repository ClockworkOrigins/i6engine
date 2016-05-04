/**
 * \addtogroup API
 * @{
 */

#ifndef __I6ENGINE_API_LANGUAGEMANAGER_H__
#define __I6ENGINE_API_LANGUAGEMANAGER_H__

#include <functional>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace api {

	/**
	 * \brief handles the current language of the Game
	 * a change of the language notifies all registered callbacks
	 */
	class ISIXE_MODULES_API LanguageManager {
	public:
		LanguageManager();

		/**
		 * \brief adds a callback to be called on language change
		 */
		void addCallback(const std::function<void(const std::string &)> & callback) {
			_callbacks.push_back(callback);
		}

		/**
		 * \brief changes language
		 */
		void setLanguage(const std::string & language);

		/**
		 * \brief updates all subscribers
		 * might be necessary if your system depends on setting the value through language manager but is initialized afterwards as most things will be
		 */
		void updateSubscribers();

	private:
		std::string _language;
		std::vector<std::function<void(const std::string &)>> _callbacks;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_LANGUAGEMANAGER_H__ */

/**
 * @}
 */
