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

#ifndef __I6ENGINE_SAMPLES_COMMONAPPLICATION_H__
#define __I6ENGINE_SAMPLES_COMMONAPPLICATION_H__

#include "i6engine/api/Application.h"
#include "i6engine/api/configs/ObjectConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	class CommonApplication : virtual public i6e::api::Application {
	public:
		CommonApplication(bool move, bool shootBall);

		virtual ~CommonApplication();

		virtual void Initialize();

		virtual void AfterInitialize();

		virtual void Tick();

		virtual bool ShutdownRequest();

		virtual void Finalize();
		
		virtual void ShutDown();

	protected:
		bool _showFPS;
		i6e::api::WeakGOPtr _camera;

		/**
		 * \brief In this map all registered buttons and their respective actions are stored.
		 * The boolean indicates if the button is still pressed.
		 */
		std::map<std::string, std::pair<boost::function<void(void)>, bool>> _eventMap;

		bool _move;
		bool _shootBall;

		/**
		 * \brief processes input messages
		 */
		void InputMailbox(const i6e::api::GameMessage::Ptr & msg);

		void Forward();
		void Backward();
		void Left();
		void Right();
		void Down();
		void Up();
		void RotateLeft();
		void RotateRight();
		void RotateUp();
		void RotateDown();
		void LeanLeft();
		void LeanRight();
	};

} /* namespace sample */

#endif /* __I6ENGINE_SAMPLES_COMMONAPPLICATION_H__ */
