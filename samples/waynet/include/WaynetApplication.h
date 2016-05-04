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

#ifndef __I6ENGINE_SAMPLES_WAYNETAPPLICATION_H__
#define __I6ENGINE_SAMPLES_WAYNETAPPLICATION_H__

#include "../../common/include/CommonApplication.h"

#include "clockUtils/iniParser/iniParser.h"

namespace sample {

	class WaynetApplication : public CommonApplication {
	public:
		WaynetApplication();

		~WaynetApplication();

		void Initialize();

		void AfterInitialize();

		void Tick() {
			CommonApplication::Tick();
		}

		void Finalize() {
			CommonApplication::Finalize();
		}

		void ShutDown() {
			CommonApplication::ShutDown();
		}

		bool ShutdownRequest() {
			return true;
		}

	private:
		clockUtils::iniParser::IniParser _iniParser;

		void News(const i6e::api::GameMessage::Ptr & msg);
	};

} /* namespace sample */

#endif /* __I6ENGINE_SAMPLES_WAYNETAPPLICATION_H__ */
