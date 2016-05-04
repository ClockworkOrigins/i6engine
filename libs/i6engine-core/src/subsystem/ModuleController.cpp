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

#include "i6engine/core/subsystem/ModuleController.h"

#include "i6engine/utils/Exceptions.h"

#ifdef ISIXE_WITH_PROFILING
	#include "i6engine/utils/Profiling.h"

	#include "i6engine/core/EngineCoreController.h"
#endif /* ISIXE_WITH_PROFILING */

#include "i6engine/core/clock/Timer.h"
#include "i6engine/core/messaging/MessageTypes.h"
#include "i6engine/core/messaging/MessagingController.h"
#include "i6engine/core/subsystem/SubSystemController.h"

#include "boost/make_shared.hpp"

namespace i6e {
namespace core {

	ModuleController::ModuleController(Subsystem sub) : MessageSubscriber(), _subsystem(sub), _frameTime(0), _messagingController(), _ctrl(nullptr), _ptrTimer(nullptr), _isRunning(false), _subsystemController(), _type(SubsystemType::Ticking), _waitingSubsystems(), _notifiedSubsystems(), _waitingMessages(), _messages(), _lock(), _conditionVariable()
#ifdef ISIXE_WITH_PROFILING
		, _lastTime(0), _fps(0), _expectedFps(0)
#endif /* ISIXE_WITH_PROFILING */
	{
	}

	ModuleController::~ModuleController() {
		delete _ptrTimer;
		_ptrTimer = nullptr;
		std::unique_lock<std::mutex> ul(_lock);
		_conditionVariable.notify_one();
	}

	void ModuleController::runLoopTicking() {
		_ptrTimer->start();
		while (true) {
			if (_subsystemController->getGlobalShutDown()) {
				ISIXE_LOG_INFO("ModuleController", "Stopping loop for " << int(_subsystem));
				_ptrTimer->pause();

				delete _ptrTimer;
				_ptrTimer = nullptr;

				setRunning(false);
				ShutDown();

				ISIXE_LOG_INFO("ModuleController", "Terminating thread execution for " << int(_subsystem));
				return;
			}

			try {
				// delegate system
				MessageSubscriber::processMessages();
				Tick();
				_messagingController->deliverMessage(boost::make_shared<Message>(messages::MessageTypes::SubsystemMessageType, messages::SubsystemMessage::SubsystemFinish, Method::Update, new MessageStruct(), _subsystem));
				_ptrTimer->update();

#ifdef ISIXE_WITH_PROFILING
				// Get current time.
				uint64_t cT = _ctrl->getCurrentTime();
				// Compute new game time (the difference between the starting time and the current time).
				uint64_t l = cT - _lastTime;
				_fps++;
				if (l >= 1000000) {
					if (_fps < _expectedFps * utils::profiling::fpsRed) {
						if (utils::profiling::fpsCrit) {
							ISIXE_LOG_DEBUG("ModuleController", "Subsystem: " << int(_subsystem) << " \x1b[31mFPS: " << _fps << "\x1b[0m");
						}
					} else {
						if (utils::profiling::fps) {
							ISIXE_LOG_DEBUG("ModuleController", "Subsystem: " << int(_subsystem) << " FPS: " << _fps);
						}
					}
					_fps = 0;
					_lastTime = cT;
				}
#endif /* ISIXE_WITH_PROFILING */
			} catch (utils::exceptions::i6exception & e) {
				e.writeLog();
				throw e;
			}
		}
	}

	void ModuleController::runLoopWaiting() {
		while (true) {
			if (_subsystemController->getGlobalShutDown()) {
				ISIXE_LOG_INFO("ModuleController", "Stopping loop for " << int(_subsystem));
				setRunning(false);
				ShutDown();
				_messagingController->unregisterMessageType(messages::MessageTypes::SubsystemMessageType, this);
				ISIXE_LOG_INFO("ModuleController", "Terminating thread execution for " << int(_subsystem));
				return;
			}

			try {
				// delegate system
				{
					std::unique_lock<std::mutex> ul(_lock);
					_conditionVariable.wait(ul);
				}
				processMessages();
				bool triggered = true;

				for (Subsystem subsystem : _waitingSubsystems) {
					if (_notifiedSubsystems.find(subsystem) == _notifiedSubsystems.end()) {
						triggered = false;
						break;
					}
				}
				if (triggered) {
					for (auto m : _messages) {
						_ptrMessageMethod[m->getMessageType()](m);
					}
					_messages.clear();
					_notifiedSubsystems.clear();
					Tick();
					_messagingController->deliverMessage(boost::make_shared<Message>(messages::MessageTypes::SubsystemMessageType, messages::SubsystemMessage::SubsystemFinish, Method::Update, new MessageStruct(), _subsystem));

#ifdef ISIXE_WITH_PROFILING
					// Get current time.
					uint64_t cT = _ctrl->getCurrentTime();
					// Compute new game time (the difference between the starting time and the current time).
					uint64_t l = cT - _lastTime;
					_fps++;
					if (l >= 1000000) {
						if (utils::profiling::fps) {
							ISIXE_LOG_DEBUG("ModuleController", "Subsystem: " << int(_subsystem) << " FPS: " << _fps);
						}
						_fps = 0;
						_lastTime = cT;
					}
#endif /* ISIXE_WITH_PROFILING */
				}
			} catch (utils::exceptions::i6exception & e) {
				e.writeLog();
				throw e;
			}
		}
	}

	void ModuleController::startThreadTicking(const uint32_t lngFrameTime) {
		_type = SubsystemType::Ticking;
		_frameTime = lngFrameTime;
		OnThreadStart();
		setRunning(true);
		setTimer(new Timer(_ctrl, lngFrameTime));
		_subsystemController->SubSystemInitialized();

#ifdef ISIXE_WITH_PROFILING
		_expectedFps = 1000000 / getFrameTime();
		_lastTime = _ctrl->getCurrentTime();
#endif /* ISIXE_WITH_PROFILING */

		runLoopTicking();
	}

	void ModuleController::startThreadWaiting(const std::set<Subsystem> & waitingFor) {
		_type = SubsystemType::Waiting;
		_waitingSubsystems = waitingFor;
		_messagingController->registerMessageType(messages::MessageTypes::SubsystemMessageType, this);
		setRunning(true);
		OnThreadStart();
		_subsystemController->SubSystemInitialized();

#ifdef ISIXE_WITH_PROFILING
		_lastTime = _ctrl->getCurrentTime();
#endif /* ISIXE_WITH_PROFILING */

		runLoopWaiting();
	}

	void ModuleController::processMessages() {
		while (updateBuffer());
		swapMessageBuffer();

		for (ReceivedMessagePtr & rm : *_objInActiveMessageVector) {
			Message::Ptr & m = rm->message;

			if (m->getSubtype() == 0) {
				reset();
				return;
			}
#ifdef ISIXE_WITH_PROFILING
			m->getContent()->insertTimestamp("ModuleController processMessages");
#endif /* ISIXE_WITH_PROFILING */
			if (m->getMessageType() == messages::SubsystemMessageType) {
				_notifiedSubsystems.insert(m->getSender());
				_messages.insert(_messages.end(), _waitingMessages[m->getSender()].begin(), _waitingMessages[m->getSender()].end());
				_waitingMessages.erase(m->getSender());
			} else {
				assert(_ptrMessageMethod.find(m->getMessageType()) != _ptrMessageMethod.end());
				if (_waitingSubsystems.find(m->getSender()) == _waitingSubsystems.end()) {
					_ptrMessageMethod[m->getMessageType()](m);
				} else {
					_waitingMessages[m->getSender()].push_back(m);
				}
			}
		}
		_objInActiveMessageVector->clear();
	}

	void ModuleController::deliverMessageInternal(const ReceivedMessagePtr & msg) {
		MessageSubscriber::deliverMessageInternal(msg);
		if (msg->message->getMessageType() == messages::SubsystemMessageType) {
			std::unique_lock<std::mutex> ul(_lock);
			_conditionVariable.notify_one();
		}
	}

} /* namespace core */
} /* namespace i6e */
