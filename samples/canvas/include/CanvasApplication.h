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

#ifndef __I6ENGINE_SAMPLES_CANVASAPPLICATION_H__
#define __I6ENGINE_SAMPLES_CANVASAPPLICATION_H__

#include "i6engine/api/Application.h"

namespace sample {

	class CanvasApplication : public i6engine::api::Application {
	public:
		CanvasApplication();

		~CanvasApplication();

		void Initialize();

		void AfterInitialize();

		void Tick();

		bool ShutdownRequest();

		void Finalize();
		
		void ShutDown();

	private:
		double _percent;
	};

} /* namespace sample */

#endif /* __I6ENGINE_SAMPLES_CANVASAPPLICATION_H__ */
