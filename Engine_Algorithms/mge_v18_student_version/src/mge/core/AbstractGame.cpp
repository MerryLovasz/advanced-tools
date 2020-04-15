#include <iostream>
#include "glm.hpp"
#include "AbstractGame.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "mge/Octree/Level/Levels/Octree.hpp"
#include "mge/Octree/Level/Components/Collider.hpp"
#include <chrono>
#include <thread>


AbstractGame::AbstractGame():_window(NULL),_renderer(NULL),_world(NULL), _fps(0)
{
    //ctor
}

AbstractGame::~AbstractGame()
{
    //dtor
    delete _window;
	delete _octree;
    delete _renderer;
    delete _world;
}

void AbstractGame::initialize() {
   // std::cout << "Initializing engine..." << '\n' << '\n';
    _initializeWindow();
    _printVersionInfo();
    _initializeGlew();
	//_initializeOctree(1);
    _initializeRenderer();
    _initializeWorld();
    _initializeScene();
   // std::cout << '\n' << "Engine initialized." << '\n' << '\n';
}

///SETUP

void AbstractGame::_initializeWindow() {
	//std::cout << "Initializing window..." << '\n';
	_window = new sf::RenderWindow( sf::VideoMode(800,600), "My Game!", sf::Style::Default, sf::ContextSettings(24,8,0,3,3));
	//_window->setVerticalSyncEnabled(true);
   // std::cout << "Window initialized." << '\n' << '\n';
}

void AbstractGame::_printVersionInfo() {
//	std::cout << "Context info:" << '\n';
  //  std::cout << "----------------------------------" << '\n';
    //print some debug stats for whoever cares
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    //nice consistency here in the way OpenGl retrieves values
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

/*    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    std::cout << "----------------------------------" << '\n' << '\n';*/
}

void AbstractGame::_initializeGlew() {
//	std::cout << "Initializing GLEW..." << '\n';
    //initialize the opengl extension wrangler
    GLint glewStatus = glewInit();
//	std::cout << "Initialized GLEW, status (1 == OK, 0 == FAILED):" << (glewStatus == GLEW_OK) << '\n' << '\n';
}

void AbstractGame::_initializeRenderer() {
    //setup our own renderer
//	std::cout << "Initializing renderer..." << '\n';
	_renderer = new Renderer();
    _renderer->setClearColor(0,0,0);
//    std::cout << "Renderer done." << '\n' << '\n';
}

void AbstractGame::_initializeWorld() {
    //setup the world
//	std::cout << "Initializing world..." << '\n';
	_world = new World();
//    std::cout << "World initialized." << '\n' << '\n';
}

void AbstractGame::_initializeOctree(int pObjectCount)
{
	//setup the Octree
//	std::cout << "Initializing octree..." << '\n';
//	nodes = pObjectCount;
//	level = floor(log(7 * nodes + 1) / log(8));
	level = 3;
	std::cout << "Octree Level: " << level << std::endl;
	_octree = new Octree(glm::vec3(0, 0, 0), glm::vec3(25, 25, 25), (int)level);
//	std::cout << "Octree initialized." << '\n' << '\n';
}

///MAIN GAME LOOP

void AbstractGame::run()
{
    //setting to calculate fps
	sf::Clock renderClock;
    int frameCount = 0;
    float timeSinceLastFPSCalculation = 0;

	//settings to make sure the update loop runs at 60 fps
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Clock updateClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window->isOpen()) {
		timeSinceLastUpdate += updateClock.restart();

		if (timeSinceLastUpdate > timePerFrame)
		{
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		    while (timeSinceLastUpdate > timePerFrame) {
                timeSinceLastUpdate -= timePerFrame;
                _update(timePerFrame.asSeconds());
		    }

            _render();
            _window->display();

            //fps count is updated once every 1 second
            frameCount++;
            timeSinceLastFPSCalculation += renderClock.restart().asSeconds();
            if (timeSinceLastFPSCalculation > 0.1) {
               _fps = frameCount/timeSinceLastFPSCalculation;

			   tbf = 1.f / (int)_fps; 
			   std::cout << "Time Between Frames: " << tbf * 1000 << '\n'; //denote tbf in milliseconds
			   totalFrames += (int)_fps;
			   std::cout << "FPS: " << (int)_fps << '\n';
			   secondsPassed++;
               timeSinceLastFPSCalculation -= 1;
			   frameCount = 0;

            }

		}

		//empty the event queue
		_processEvents();
    }
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
	_octree->Update(pStep);
	Collider::count = 0;\

	std::vector<Pair> * pairs =
		_octree->Collisions(nullptr);
	delete pairs;
}

void AbstractGame::_render () {
    _renderer->render(_world);
}

void AbstractGame::_processEvents()
{
	sf::Event event;
	bool exit = false;


	//we must empty the event queue
	while( _window->pollEvent( event ) ) {
        //give all system event listeners a chance to handle events
        //optionally to be implemented by you for example you could implement a
        //SystemEventDispatcher / SystemEventListener pair which allows Listeners to
        //register with the dispatcher and then do something like:
        //SystemEventDispatcher::dispatchEvent(event);
		
        switch (event.type) {
            case sf::Event::Closed:
                exit = true;
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    exit = true;
                }
                break;
            case sf::Event::Resized:
                //would be better to move this to the renderer
                //this version implements nonconstrained match viewport scaling
                glViewport(0, 0, event.size.width, event.size.height);
                break;

            default:
                break;
        }
			
	}


	if (exit || secondsPassed >= 5.f /*|| totalFrames >= 1000*/) {
        _window->close();
	}
}



