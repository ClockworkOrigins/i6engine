#ifndef __I6ENGINE_API_LANGUAGEMANAGER_H__
#define __I6ENGINE_API_LANGUAGEMANAGER_H__

#include <functional>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6engine {
namespace api {

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

	private:
		std::string _language;
		std::vector<std::function<void(const std::string &)>> _callbacks;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_LANGUAGEMANAGER_H__ */
