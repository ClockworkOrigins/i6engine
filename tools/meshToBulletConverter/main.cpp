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

#include <thread>

#include "MeshStrider.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/modules/graphics/ResourceManager.h"

#include "i6engine/modules/physics/collisionShapes/MeshStriderCollisionShape.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"

#include "clockUtils/argParser/ArgumentParser.h"
#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/Ogre.h"
#include "OGRE/OgreDefaultHardwareBufferManager.h"

int main(int argc, char ** argv) {
	REGISTER_VARIABLE(std::string, mesh, "", "The mesh file without path to be converted to a collision shape");
	REGISTER_VARIABLE(std::string, shape, "", "The collision shape file where the export shalls go to including an optional path");

	if (PARSE_COMMANDLINE() != clockUtils::ClockError::SUCCESS) {
		std::cerr << GETLASTPARSERERROR() << std::endl;
		return 1;
	} else if (HELPSET()) {
		std::cout << GETHELPTEXT() << std::endl;
	} else if (!mesh.isSet()) {
		std::cerr << "Parameter 'mesh' not set!" << std::endl;
		return 1;
	} else if (!shape.isSet()) {
		std::cerr << "Parameter 'shape' not set!" << std::endl;
		return 1;
	} else {
		std::cout << "Initializing Ogre" << std::endl;
		std::string ogrePath;
		// GRAPHIC is correct, because we want the same variable as for ogre
		clockUtils::iniParser::IniParser iniParser;
		iniParser.load("i6engine.ini");
		if (clockUtils::ClockError::SUCCESS != iniParser.getValue("GRAPHIC", "ogreConfigsPath", ogrePath)) {
			std::cerr << "An exception has occurred: value ogreConfigsPath in section GRAPHIC not found!" << std::endl;
			return 1;
		}
		Ogre::LogManager * lm = new Ogre::LogManager();
		lm->createLog("ogre.log", true, false, false);
		Ogre::Root * root = new Ogre::Root("", ogrePath + "/ogre.cfg", "");
		Ogre::DefaultHardwareBufferManager * dhbm = new Ogre::DefaultHardwareBufferManager();
		i6e::modules::ResourceManager * resourceManager = new i6e::modules::ResourceManager(ogrePath);

		std::cout << "Loading mesh '" << mesh << "'" << std::endl;
		Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().load(mesh, "i6engine");
		Ogre::Mesh * mp = meshPtr.get();

		std::cout << "Creating collision shape" << std::endl;
		btBvhTriangleMeshShape * fallShape = new btBvhTriangleMeshShape(new i6e::tools::MeshStrider(mp), true);
		btConvexTriangleMeshShape * ctms = new btConvexTriangleMeshShape(fallShape->getMeshInterface());
		ctms->initializePolyhedralFeatures();
		btShapeHull * sh = new btShapeHull(ctms);
		sh->buildHull(0.0);
		btConvexHullShape * chs = new btConvexHullShape();
		for (int i = 0; i < sh->numVertices(); i++) {
			chs->addPoint(sh->getVertexPointer()[i]);
		}

		btDefaultSerializer * serializer = new btDefaultSerializer();
		serializer->startSerialization();
		chs->serializeSingleShape(serializer);
		serializer->finishSerialization();

		std::vector<char> data(serializer->getCurrentBufferSize());

		for (size_t i = 0; i < data.size(); i++) {
			data[i] = serializer->getBufferPointer()[i];
		}

		i6e::modules::MeshStriderCollisionShapeData * mscsd = new i6e::modules::MeshStriderCollisionShapeData(data);

		std::string serialized = mscsd->Serialize();

		delete mscsd;

		std::ofstream fs(shape, std::ios_base::binary);
		fs << serialized;
		fs.close();
		std::cout << "Exported '" << shape << "'" << std::endl;
		
		delete serializer;
		delete fallShape;
		delete resourceManager;
		delete dhbm;
		delete root;
		delete lm;
	}

	return 0;
}

