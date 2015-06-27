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

/**
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_CONFIG_JOBPRIORITIES_H__
#define __I6ENGINE_CORE_CONFIG_JOBPRIORITIES_H__

namespace i6engine {
namespace core {

	enum JobPriorities {
		Prio_Subsystem,		//!< Highest priority, used only internally for ticking of subsystems, don't use this one
		Prio_High,			//!< High job priority, use this if this task should be preferred for running before other tasks
		Prio_Medium,			//!< Task can wait, but shouldn't wait to long
		Prio_Low			//!< Task can wait really long, isn't that important
	};

} /* namespace core */
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_CONFIG_JOBPRIORITIES_H__ */

/**
 * @}
 */
