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

/**
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_MODULECONTROLLER_H__
#define __I6ENGINE_CORE_MODULECONTROLLER_H__

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/core/messaging/MessageSubscriber.h"

namespace boost {
	class thread;
} /* boost */

namespace i6e {
namespace core {

	class EngineCoreController;
	class MessagingController;
	class SubSystemController;
	class Timer;

	enum class Subsystem;

	enum class SubsystemType {
		Ticking,
		Waiting
	};

	/**
	 * \brief Part of the Subsystem that keeps the whole module running
	 */
	class ISIXE_CORE_API ModuleController : public MessageSubscriber {
		friend class SubSystemController;

	public:
		/**
		 * \brief Constructs a new ModuleController with the given subsystem type
		 */
		ModuleController(Subsystem sub);

		/**
		 * \brief Destructor
		 */
		virtual ~ModuleController();

		/**
		 * \brief Starts the thread synchronously of the subsystem and tells the SubSystemController about completion.
		 *
		 * \param lngFrameTime Current frametime.
		 */
		void startThreadTicking(const uint32_t lngFrameTime);

		/**
		 * \brief Starts the thread synchronously of the subsystem and tells the SubSystemController about completion.
		 *
		 * \param waitingFor Current frametime.
		 */
		void startThreadWaiting(const std::set<Subsystem> & waitingFor);

		/**
		 * \brief Gets the frametime of this Subsystem.
		 * \return Frametime.
		 */
		inline uint32_t getFrameTime() const {
			return _frameTime;
		}

		/**
		 * \brief registers necessary controllers for this Subsystem
		 */
		inline void setController(SubSystemController * ssc, EngineCoreController * ecc, MessagingController * mc) {
			_subsystemController = ssc;
			_ctrl = ecc;
			_messagingController = mc;
		}

		/**
		 * \brief Called just after the Controller got created
		 */
		virtual void OnThreadStart() = 0;

		/**
		 * \brief Called repeatedly according to the frametime
		 */
		virtual void Tick() = 0;

		/**
		 * \brief Called to Shutdown the Module
		 */
		virtual void ShutDown() = 0;

		Subsystem getSubsystem() const {
			return _subsystem;
		}

	protected:
		/**
		 * \brief the Subsystem
		 */
		Subsystem _subsystem;

		/**
		 * \brief the frame time of the Subsystem
		 */
		uint32_t _frameTime;

		/**
		 * \brief pointer to MessagingController
		 */
		MessagingController * _messagingController;

		/**
		 * \brief override of processMessages of MessageSubscriber for waiting subsystems
		 */
		void processMessages() override;

	private:
		/**
		 * \brief Sets subsystem timer.
		 * \param ptrTimer a Timer * argument.
		 */
		inline void setTimer(Timer * ptrTimer) {
			_ptrTimer = ptrTimer;
		}

		/**
		 * \brief Returns a pointer to the subsystem's timer.
		 * \return _ptrTimer Timer of the Subsystem of type Timer *.
		 */
		inline Timer * getTimer() const {
			return _ptrTimer;
		}

		/**
		 * \brief The main loop of a subsystem executed by it's own thread.
		 */
		void runLoopTicking();
		void runLoopWaiting();

		/**
		 * \brief pointer to the EngineCoreController
		 */
		EngineCoreController * _ctrl;

		/**
		 * \brief the Timer of the Subsystem
		 */
		Timer * _ptrTimer;

		/**
		 * \brief state of the subsystem
		 */
		bool _isRunning;

		/**
		 * \brief pointer to the SubSystemController
		 */
		SubSystemController * _subsystemController;

		/**
		 * \brief type of the Subsystem
		 */
		SubsystemType _type;

		/**
		 * \brief subsystems this subsystem is waiting for
		 */
		std::set<Subsystem> _waitingSubsystems;

		/**
		 * \brief subsystems that are notified for next tick
		 */
		std::set<Subsystem> _notifiedSubsystems;

		/**
		 * \brief list of all temporary messages waiting for notify
		 */
		std::map<Subsystem, std::vector<Message::Ptr>> _waitingMessages;

		/**
		 * \brief vector of all messages only being delivered after a parent subsystem finished
		 */
		std::vector<Message::Ptr> _messages;

		mutable std::mutex _lock;
		std::condition_variable _conditionVariable;

#ifdef ISIXE_WITH_PROFILING
		/**
		 * \brief last frame time
		 */
		uint64_t _lastTime;

		/**
		 * \brief fps
		 */
		uint16_t _fps;

		/**
		 * \brief frames being expected for this subsystem
		 */
		uint16_t _expectedFps;
#endif /* ISIXE_WITH_PROFILING */

		/**
		 * \brief activates the Subsystem
		 */
		inline void setRunning(bool b) {
			_isRunning = b;
		}

		/**
		 * \brief gets state of the Subsystem
		 */
		inline bool getRunning() const {
			return _isRunning;
		}

		void deliverMessageInternal(const ReceivedMessagePtr & msg) override;

		void stop() {
			std::unique_lock<std::mutex> ul(_lock);
			_conditionVariable.notify_one();
		}

		/**
		 * \brief forbidden
		 */
		ModuleController(const ModuleController &) = delete;

		/**
		 * \brief forbidden
		 */
		ModuleController & operator=(const ModuleController &) = delete;
	};

} /* namespace core */
} /* namespace i6e */

#endif /* __I6ENGINE_CORE_MODULECONTROLLER_H__ */

/**
 * @}
 */
