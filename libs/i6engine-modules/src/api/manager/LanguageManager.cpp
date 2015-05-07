#include "i6engine/api/manager/LanguageManager.h"

namespace i6engine {
namespace api {

	LanguageManager::LanguageManager() : _language("Deutsch"), _callbacks() {
	}

	void LanguageManager::setLanguage(const std::string & language) {
		if (language != _language) {
			_language = language;

			for (auto & cb : _callbacks) {
				cb(_language);
			}
		}
	}

} /* namespace api */
} /* namespace i6engine */
