#include "api/components/StaticStateComponent.h"
#include "api/components/MeshAppearanceComponent.h"
#include "api/components/CameraComponent.h"
#include "api/objects/GameObject.h"

using namespace i6engine;

class TemplateObj {
public:
	static void createGO(api::GameObject * go, const std::map<std::string, std::string> & params) {
		go->setType("Foo");

		// generic
		utils::math::i6eVector position(params, "pos");
		utils::math::i6eQuaternion rotation(params, "rot");
		utils::math::i6eVector scale(params, "scale");

		// visibility
		api::MeshAppearanceComponent * meshComp = new api::MeshAppearanceComponent(go->getID() + 1);
		meshComp->setOwnerGO(go);
		go->setGOC(meshComp);
		meshComp->Init("cube.mesh", true, position);

		// position
		api::StaticStateComponent * statComp = new api::StaticStateComponent(go->getID() + 2);
		statComp->setPosition(position);
		statComp->setRotation(rotation);
		statComp->setScale(scale);
		statComp->setOwnerGO(go);
		statComp->Init();

		api::CameraComponent * camComp = new api::CameraComponent(go->getID() + 4);
		camComp->setOwnerGO(go);
		camComp->Init(utils::math::i6eVector(0, 7, 30), utils::math::i6eVector(0, 4, -10), 1, 3.14 / 6);
		camComp->setViewportDimension(0.0f, 0.0f, 1.0f, 1.0f);
	}

private:
	TemplateObj(){}

	virtual ~TemplateObj() {}
};
