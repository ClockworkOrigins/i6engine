/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
