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

#ifndef __I6ENGINE_SAMPLES_LEVELEDITORAPPLICATION_H__
#define __I6ENGINE_SAMPLES_LEVELEDITORAPPLICATION_H__

#include "i6engine/editor/Editor.h"

namespace sample {

	class LeveleditorApplication : public i6engine::editor::Editor {
	public:
		LeveleditorApplication();

		~LeveleditorApplication();

		void AfterInitialize() override;

	private:
		std::string getBasePath() const override {
			return "../media/maps";
		}

		std::vector<std::string> getLevelFlags() const override {
			return{ "Singleplayer" };
		}
	};

} /* namespace sample */

#endif /* __I6ENGINE_SAMPLES_LEVELEDITORAPPLICATION_H__ */
