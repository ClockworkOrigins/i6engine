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

#ifndef __I6ENGINE_SAMPLES_TOOLTIPAPPLICATION_H__
#define __I6ENGINE_SAMPLES_TOOLTIPAPPLICATION_H__

#include "i6engine/api/Application.h"

namespace sample {

	class TooltipApplication : public i6e::api::Application {
	public:
		TooltipApplication();

		~TooltipApplication();

		void Initialize();

		void AfterInitialize();

		void Tick();

		bool ShutdownRequest();

		void Finalize();
		
		void ShutDown();

	private:
		uint64_t _progressTimer;
		uint16_t _counter;
	};

} /* namespace sample */

#endif /* __I6ENGINE_SAMPLES_TOOLTIPAPPLICATION_H__ */
