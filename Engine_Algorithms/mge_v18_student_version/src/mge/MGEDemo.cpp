
#include <string>
#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/Octree/Level/Levels/Octree.hpp"
#include "mge/Octree/Level/AABB.hpp"
#include "mge/Octree/Level/OBB.hpp"
#include "mge/Octree/Level/Components/Collider.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();
    //setup the custom part so we can display some text
	//std::cout << "Initializing HUD" << '\n';
	_hud = new DebugHud(_window);
	//std::cout << "HUD initialized." << '\n' << '\n';
}

//build the game _world
void MGEDemo::_initializeScene()
{

    //MESHES
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_unity.obj");

	//MATERIALS
	AbstractMaterial* cubeDynamicMaterial = new ColorMaterial(glm::vec3(0, 1, 0));
	AbstractMaterial* cubeStaticMaterial = new ColorMaterial(glm::vec3(0, 0, 1));
	AbstractMaterial* cubeHitMaterial = new ColorMaterial(glm::vec3(1, 0, 0));

   //SCENE SETUP

   //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(8,16,6));
    camera->rotate(glm::radians(-90.0f), glm::vec3(1,0,0));
	camera->setBehaviour(new KeysBehaviour());
    _world->add(camera);
    _world->setMainCamera(camera);

	int DynamicObjCount = 10;

	int width = 4;
	int height = 4;
	int staticObjectCount = width * height;
	int objectCount = staticObjectCount + DynamicObjCount;

	printf("Total amount of objects: %i\n", objectCount);
	
	_initializeOctree(objectCount);

	//dynamic objects
	for (int i = 0; i < DynamicObjCount; i++) {
	GameObject * cubeD = new GameObject("dynamic cube", glm::vec3(i * 0.4f, i * 0.1f, 0));
	cubeD->setMesh(cubeMeshF);
	cubeD->setMaterial(cubeDynamicMaterial);
	cubeD->SetHitMaterial(cubeHitMaterial);
	cubeD->SetCollider(new OBB(cubeD, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)));
	cubeD->setBehaviour(new RotatingBehaviour(glm::vec3(0.1f, 0.1f, 0.1f)));
	_world->add(cubeD);
	_octree->Add(cubeD);
	}


	//static objects in grid shape
	for (int i = 0; i < width; i++) { //width
		for (int j = 0; j < height; j++) { //height
			glm::vec3 pos = glm::vec3(i * 2.f, 0, j * 2.f); 
			GameObject * cubeS = new GameObject("static cube", pos);
			cubeS->setMesh(cubeMeshF);
			cubeS->setMaterial(cubeStaticMaterial);
			cubeS->SetHitMaterial(cubeHitMaterial);
			cubeS->SetCollider(new AABB(cubeS, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)));
			_world->add(cubeS);
			_octree->Add(cubeS);
		}
	}
	
}

void MGEDemo::_render() {
    AbstractGame::_render();

    _updateHud();
}

void MGEDemo::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

MGEDemo::~MGEDemo()
{
	//dtor
}
