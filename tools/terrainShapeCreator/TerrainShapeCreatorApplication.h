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
