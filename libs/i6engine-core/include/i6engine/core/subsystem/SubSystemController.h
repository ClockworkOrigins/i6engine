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

#ifndef __I6ENGINE_CORE_SUBSYSTEMCONTROLLER_H__
#define __I6ENGINE_CORE_SUBSYSTEMCONTROLLER_H__

#include <cstdint>
#include <set>

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "boost/thread.hpp"

/**
 * \page page_subsystems SubSystems
 *
 * \section sec_core_subsystemController SubsystemController
 *
 * The SubSystemController manages all SubSystems and notifies them of any possible operations like Initialize, Start or Shutdown.
 * To create a SubSystem just derive it from this class. As the SubSystemController is a subclass of the MessagingController, all SubSystems automatically are able to send Messages.
 * The functions OnThreadStart(), Tick() and Shutdown() <b>have to be</b> implemented by all SubSystems.
 * The RunLoop() is executed until the SubSystemController notified a shutdown. Otherwise, within the loop, all incoming Messages need to be processed and afterwards the System can do its own work. Finally the Thread sleeps until it gets another Timer-Interupt.
 *
 * \section sec_core_subsystem Subsystem
 * For the future, all SubSystems should have this structure:
 * \li ~Factory: Uses the factory pattern; creates all objects and handles (3rd party objects as well)
 * \li ~Controller: coordinates the Subsystem; Implements the Tick() function
 * \li ~Facade: implements the facade pattern
 * Currently available SubSystems are:
 * \li \ref page_graphic
 * \li \ref page_gui
 * \li \ref page_object
 * \li \ref page_network
 * \li \ref page_input
 * \li \ref page_physics
 */

namespace i6engine {
namespace core {

	class EngineControllerInterface;
	class EngineCoreController;
	class ModuleController;

	enum class SubsystemType;

	typedef struct {
		ModuleController * module;
		SubsystemType type;
		uint32_t framerate;
		std::set<Subsystem> waitingFor;
	} QueuedModule;

	/**
	 * \class SubSystemController
	 * \brief This class is used as an abstraction layer.
	 *
	 * All subsystem have to register at the SubSystemController.
	 * Each registered subsystem will be notified by the SubSystemController of any possible operation.
	 *
	 * SubSystemController.h defines the macro:
	 * \li \c LNG_MESSAGING_FRAME_TIME 	This macro sets the frametime the MessagingController subsystem works with.
	 *
	 * See \ref macrodoc for a list of all globally defined macros.
	 */
	class ISIXE_CORE_API SubSystemController {
		/* Only the EngineCoreController should run methods of this class */
		friend class EngineCoreController;

	public:
		/**
		 * \brief Default constructor.
		 */
		SubSystemController();

		/**
		 * \brief This method will get the flag which indicates if the subsystems have to be shut down.
		 * \return Boolean Flag indicating if the subsystems have to be shut down.
		 */
		inline bool getGlobalShutDown() const { return _bolGlobalShutDown; }

		/**
		 * \brief This method will set the flag which will indicate that the subsystems have to be shut down.
		 */
		void ShutDown();

		/**
		 * \brief Waits for all controlled subsystem-threads to terminate.
		 */
		inline void JoinAllSubsystems() { _objThreadGrp.join_all(); }

		/**
		 * \brief This methods tells the SubSystemController that the synchronously started SubSystem has been completed its startup sequence.
		 */
		inline void SubSystemInitialized() { _bolWaitForInit = false; }

		/**
		 * \brief This method queues a user defined Subsystem to start.
		 *
		 * \param objSubsystem Subsystem to be started.
		 * \param lngFrameTime Framerate of the subsystem.
		 */
		void QueueSubSystemStart(ModuleController * objSubsystem, const uint32_t lngFrameTime);

		/**
		 * \brief This method queues a user defined Subsystem to start.
		 *
		 * \param objSubsystem Subsystem to be started.
		 * \param waitingFor subsystems, this subsystem is waiting for
		 */
		void QueueSubSystemStart(ModuleController * objSubsystem, const std::set<Subsystem> & waitingFor);

		/**
		 * \brief This method returns the amount of subsystems.
		 * \return Count of subsystems.
		 */
		inline int GetSubSystemAmount() const { return _objQueuedSubSystems.size(); }

		/**
		 * \brief returns true, if all registered subsystems shut down or false, if not
		 */
		bool isShutdownComplete();

		/**
		 * \brief registers an EngineCoreController for the SubSystemController
		 */
		void registerController(EngineCoreController * ecc) { _coreController = ecc; }

	private:
		bool _bolGlobalShutDown;
		bool _bolWaitForInit;
		boost::thread_group _objThreadGrp;
		std::vector<QueuedModule> _objQueuedSubSystems;
		EngineCoreController * _coreController;

		/**
		 * \brief This method will be called when starting all subsystems.
		 */
		void Start();

		/**
		 * \brief This method will start a subsystem synchronously.
		 */
		void startSubSystemTicking(ModuleController * objSubSystem, const uint32_t lngFrameTime);

		/**
		 * \brief This method will start a subsystem synchronously.
		 */
		void startSubSystemWaiting(ModuleController * objSubSystem, const std::set<Subsystem> & waitingFor);

		/**
		 * \brief forbidden
		 */
		SubSystemController(const SubSystemController &) = delete;

		/**
		 * \brief forbidden
		 */
		SubSystemController & operator=(const SubSystemController &) = delete;
	};

} /* namespace core */
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_SUBSYSTEMCONTROLLER_H__ */

/**
 * @}
 */
