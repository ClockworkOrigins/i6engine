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

#include "i6engine/api/Application.h"

namespace i6e {
namespace tools {

	class TerrainShapeCreatorApplication : public api::Application {
	public:
		TerrainShapeCreatorApplication(const std::string & goTemplate, const std::string & outFile) : api::Application(), _goTemplate(goTemplate), _outFile(outFile) {
		}

	private:
		std::string _goTemplate;
		std::string _outFile;

		void Initialize();

		void AfterInitialize();

		void Tick() {
		}

		bool ShutdownRequest() {
			return true;
		}

		void Finalize() {
		}
	};

} /* namespace tools */
} /* namespace i6e */
