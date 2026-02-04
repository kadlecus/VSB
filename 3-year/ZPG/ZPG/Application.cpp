#define GLM_ENABLE_EXPERIMENTAL
#include "Application.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Models/sphere.h"
#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/cube.h"
#include "Controller.h"
#include "Models/plain2.h"
#include "Models/plain.h"
#include "Callback_function.h"
#include "ISubject.h"
#include <cstdlib>
#include <ctime>  
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <filesystem>
#include <iostream>
#include "AppContext.h"
#include "ArcadeScene.h"

#include "SkyboxObject.h"
#include "CubemapTexture.h"
#include "BezierSpline.h"
#include "BezierSplineMove.h"


Application::Application() : window(nullptr) 
{
    this->appcontext = nullptr;
    this->camera = nullptr;
    this->controller = nullptr;
}

Application::~Application() {
    delete camera;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initialization() {
    if (!glfwInit()) {
        throw std::runtime_error("ERROR: could not start GLFW3");
    }

    window = glfwCreateWindow(800, 600, "ZPG", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("ERROR: could not open window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    controller = new Controller();
    camera = new Camera();  
    std::vector<LightManager*> managers;

    scenes.push_back(new Scene());
    scenes.push_back(new Scene());
    scenes.push_back(new Scene());
    scenes.push_back(new Scene());
    scenes.push_back(new ArcadeScene());
    scenes.push_back(new Scene());

    scenes[0]->setLightManager(new LightManager());
    scenes[1]->setLightManager(new LightManager());
    scenes[2]->setLightManager(new LightManager());
    scenes[3]->setLightManager(new LightManager());
    scenes[4]->setLightManager(new LightManager());
    scenes[5]->setLightManager(new LightManager());

    for (auto* scene : scenes) 
    {    
        managers.push_back(scene->getLightManager());
    }
     this->appcontext = new AppContext();
     this->appcontext->camera = camera;
     this->appcontext->scenes = scenes;
     this->appcontext->lightManagers = managers;
  
    glfwSetWindowUserPointer(window, appcontext); 
    glfwSetCursorPosCallback(window, Controller::mouseCallback);
    glfwSetMouseButtonCallback(window, Controller::mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    camera->setResolution(width, height);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->setProjection(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    
    camera->updateDirection();

    glEnable(GL_DEPTH_TEST);
    glewExperimental = GL_TRUE;
    glewInit();
}

void Application::createShaders() {
    
    Shader* vertexshaderTriangle = new Shader();
    vertexshaderTriangle->createShaderFromFile(GL_VERTEX_SHADER, "vertexShaders.txt");

    Shader* vertexNoTex = new Shader();
    vertexNoTex->createShaderFromFile(GL_VERTEX_SHADER, "vertexNoTex.txt");

    Shader* vertexCube = new Shader();
    vertexCube->createShaderFromFile(GL_VERTEX_SHADER, "cubeboxVer.txt");


    Shader* fragCube= new Shader();
    fragCube->createShaderFromFile(GL_FRAGMENT_SHADER, "cubebox.txt");
    ShaderProgram* Cube = new ShaderProgram(vertexCube, fragCube);

 
    Shader* lambert = new Shader();
    lambert->createShaderFromFile(GL_FRAGMENT_SHADER, "lambertAll.txt"); 
    ShaderProgram* Lambert= new ShaderProgram(vertexshaderTriangle, lambert);

    Shader* lambert1 = new Shader();
    lambert1->createShaderFromFile(GL_FRAGMENT_SHADER, "lambertAll.txt"); 
    ShaderProgram* Lambert1 = new ShaderProgram(vertexshaderTriangle, lambert1);

    Shader* constant = new Shader();
    constant->createShaderFromFile(GL_FRAGMENT_SHADER, "constantAll.txt"); 
    ShaderProgram* Constant= new ShaderProgram(vertexshaderTriangle, constant);

    Shader* constant1 = new Shader();
    constant->createShaderFromFile(GL_FRAGMENT_SHADER, "constantAll.txt");
    ShaderProgram* Constant1 = new ShaderProgram(vertexshaderTriangle, constant1);

    Shader* phong = new Shader();
    phong->createShaderFromFile(GL_FRAGMENT_SHADER, "phongAll.txt"); 
    ShaderProgram* Phong = new ShaderProgram(vertexshaderTriangle, phong);

    Shader* space = new Shader();
    space->createShaderFromFile(GL_FRAGMENT_SHADER, "space.txt");
    ShaderProgram* Space = new ShaderProgram(vertexshaderTriangle, space);

    Shader* spaceconst = new Shader();
    spaceconst->createShaderFromFile(GL_FRAGMENT_SHADER, "const_texture.txt");
    ShaderProgram* SpaceC = new ShaderProgram(vertexshaderTriangle, spaceconst);


    Shader* phongNoTex = new Shader();
    phongNoTex->createShaderFromFile(GL_FRAGMENT_SHADER, "phonNoTex.txt");
    ShaderProgram* PhongNoTex = new ShaderProgram(vertexNoTex, phongNoTex);

    Shader* lamText = new Shader();
    lamText->createShaderFromFile(GL_FRAGMENT_SHADER, "lambertAllTex.txt");
    ShaderProgram* LamText = new ShaderProgram(vertexshaderTriangle, lamText);



    Shader* blinn = new Shader();
    blinn->createShaderFromFile(GL_FRAGMENT_SHADER, "blinnAll.txt"); 
    ShaderProgram* Blinn = new ShaderProgram(vertexshaderTriangle, blinn); 



    camera->attach(Constant);
    camera->attach(Lambert);
    camera->attach(Phong);
    camera->attach(PhongNoTex);
    camera->attach(Blinn);
    camera->attach(Lambert1);
    camera->attach(Constant1);
    camera->attach(Space);
    camera->attach(SpaceC);
    camera->attach(LamText);
    camera->attach(Cube);
    camera->notify();


    scenes[0]->addShaderProgram(Constant);
    scenes[0]->addShaderProgram(Phong);
    scenes[0]->addShaderProgram(PhongNoTex);
    scenes[0]->addShaderProgram(Blinn);
    scenes[0]->addShaderProgram(Cube);

   
    scenes[1]->addShaderProgram(Phong);
    scenes[1]->addShaderProgram(Constant);

    scenes[2]->addShaderProgram(Constant);
    scenes[2]->addShaderProgram(Lambert1);
  
    scenes[3]->addShaderProgram(Space);
    scenes[3]->addShaderProgram(Lambert);
    scenes[3]->addShaderProgram(Constant);
    scenes[3]->addShaderProgram(SpaceC);
    scenes[3]->addShaderProgram(LamText);

    scenes[4]->addShaderProgram(PhongNoTex);
    scenes[4]->addShaderProgram(Constant);

    scenes[5]->addShaderProgram(PhongNoTex);
    scenes[5]->addShaderProgram(Phong);
    scenes[5]->addShaderProgram(Constant);
    scenes[5]->addShaderProgram(SpaceC);

    Lambert1->checkError();
    Constant->checkError();
    Lambert->checkError();
    Phong->checkError();
    Blinn->checkError();
    Constant1->checkError();
    PhongNoTex->checkError();
    Space->checkError();
    LamText->checkError();
    Cube->checkError();
}

void Application::createModels() {

 

    if (scene_switch == 0)
    {
        std::vector<std::string> faces
        {
            "Textures/sky/cubemap/negx.jpg",    // -X
            "Textures/sky/cubemap/negy.jpg",  // -Y
            "Textures/sky/cubemap/negz.jpg",    // -Z
            "Textures/sky/cubemap/posx.jpg",   // +X
            "Textures/sky/cubemap/posy.jpg",     // +Y
            "Textures/sky/cubemap/posz.jpg",   // +Z
        };

        Texture* grassTexture = new Texture("Textures/multipletexture/grass.png", GL_TEXTURE0);
        Model* box = new Model("skybox.obj", grassTexture);
        CubemapTexture* skyboxCubemap = new CubemapTexture(faces);

        
        SkyboxObject* skybox = new SkyboxObject(
            box,               
            skyboxCubemap,     
            scenes[0]->getShaderProgram(4),            
            this->camera
            );

       
        scenes[scene_switch]->addObject(std::shared_ptr<DrawableObject>(skybox));

        Texture* fionaTexture = new Texture("Models/assets/shrek/fiona.png", GL_TEXTURE0);
        Model* fiona = new Model("shrek/fiona.obj", fionaTexture);

        Texture* shrekTexture = new Texture("Models/assets/shrek/shrek.png", GL_TEXTURE0);
        Model* shrek = new Model("shrek/shrek.obj", shrekTexture);

        Texture* toiledTexture = new Texture("Models/assets/shrek/toiled.jpg", GL_TEXTURE0);
        Model* toiled = new Model("shrek/toiled.obj", toiledTexture);

        std::shared_ptr<DrawableObject> Shrek = std::make_shared<DrawableObject>(shrek, scenes[scene_switch]->getShaderProgram(1));
        std::shared_ptr<DrawableObject> Fiona = std::make_shared<DrawableObject>(fiona, scenes[scene_switch]->getShaderProgram(1));
        std::shared_ptr<DrawableObject> Toiled = std::make_shared<DrawableObject>(toiled, scenes[scene_switch]->getShaderProgram(1));

        Shrek->setTransformation(std::make_shared<Transformation>());
        Shrek->getTransformation()->addComponent(std::make_shared<Translate>(-1.0f, -1.6f, 0.0f));
        Shrek->getTransformation()->addComponent(std::make_shared<Scale>(2.0f, 2.0f, 2.0f)); 
        Shrek->setMaterial(new Material(
            0.3f,
            1.0f,
            0.0f,
            32.0f             // shininess
        ));
        
        Fiona->setTransformation(std::make_shared<Transformation>());
        Fiona->getTransformation()->addComponent(std::make_shared<Translate>(1.0f, -1.6f, 0.0f)); 
        Fiona->getTransformation()->addComponent(std::make_shared<Scale>(0.9f, 0.9f, 0.9f));
        Material* fionaMaterial = new Material(
            0.3f,
            1.0f,
            0.0f,
            32.0f
        );
        Fiona->setMaterial(fionaMaterial);
        Toiled->setTransformation(std::make_shared<Transformation>());
        Toiled->getTransformation()->addComponent(std::make_shared<Translate>(0.0f, -1.6f, -2.0f));
        Material* toiletMaterial = new Material(
            0.3f,
            1.0f,
            1.0f,              
            32.0f
        );
        Toiled->setMaterial(toiletMaterial);

        Model* pl = new Model("teren.obj", grassTexture);
        std::shared_ptr<DrawableObject> Plain = std::make_shared<DrawableObject>(pl, scenes[scene_switch]->getShaderProgram(1));
        Plain->getTransformation()->addComponent(std::make_shared<Translate>(0.0f, -1.6f, 0.0f));
        Plain->getTransformation()->addComponent(std::make_shared<Scale>(1.0f, 1.0f, 1.0f));

        Material* plainMaterial = new Material(
            1.0f,
            1.0f,
            0.0f,
            32.0f
        );

        Plain->setMaterial(plainMaterial);
        scenes[scene_switch]->addObject(Plain);
        scenes[scene_switch]->addObject(Shrek);
        scenes[scene_switch]->addObject(Fiona);
        scenes[scene_switch]->addObject(Toiled);

        Material* material = new Material(
            0.1f,
            0.2f,
            0.0f,
            8.0f
        );

    for (int i = 0; i <= 50; i++) {
        float xRandom = (rand() % 400 - 200) / 10.0f;   
        float zRandom = -(rand() % 400) / 10.0f - 5.0f; 
        float scaleRandom = (rand() % 5 + 5) / 10.0f;
        scenes[scene_switch]->addObjects(tree, sizeof(tree), nullptr, 6,
            glm::vec3(xRandom +  (float)i, -1.5f, zRandom),
            glm::vec3(1.1f),
            glm::vec3(0.5f, 0.7f, 0.0f), material, 2);
        
        
    }

    for (int i = 0; i <= 50; i++) {
        float xRandom = (rand() % 400 - 200) / 10.0f;   
        float zRandom = -(rand() % 400) / 10.0f - 5.0f; 
        float scaleRandom = (rand() % 5 + 5) / 10.0f;
        scenes[scene_switch]->addObjects(bushes, sizeof(bushes), nullptr,6,
            glm::vec3((float)i + xRandom, -1.5f,  zRandom),
            glm::vec3(scaleRandom),
            glm::vec3(0.5f, 0.5f, 0.0f), material, 2); 
    }

    auto shader = scenes[0]->getShaderProgram(0);
    Model* lightModel = new Model(sphere, sizeof(sphere), nullptr, 6);
        
    PointLight* movingLight = new PointLight(
        glm::vec3(0.0f, 3.0f, -5.0f),        
        glm::vec3(0.6f, 0.6f, 1.0f),        
        1.5f,                                
        glm::vec3(1.0f, 0.7f, 0.02f),        
        true,                                 
        lightModel,                         
        shader                                
    );

    PointLight* movingLight2 = new PointLight(
        glm::vec3(0.0f, 3.0f, -15.0f),
        glm::vec3(0.6f, 0.6f, 1.0f),
        0.4f,
        glm::vec3(1.0f, 0.09f, 0.032f),
        true,
        lightModel,
        shader
    );

    PointLight* movingLight3 = new PointLight(
        glm::vec3(7.0f, 3.0f, -15.0f),
        glm::vec3(0.6f, 0.6f, 1.0f),
        0.4f,
        glm::vec3(1.0f, 0.09f, 0.032f),
        true,
        lightModel,
        shader
    );

    SpotLight* spotlight = new SpotLight(
        glm::vec3(0.0f, 5.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, -1.0f),
        2.0f,
        glm::vec3(0.2f, 0.2f, 0.2f),
        1.0f,
        glm::vec3(0.2f, 0.09f, 0.0f),
        nullptr,
        scenes[0]->getShaderProgram(0)
    );


        scenes[0]->addLight(spotlight);
        

        scenes[0]->addLight(movingLight);
        scenes[0]->addLight(movingLight2);
        scenes[0]->addLight(movingLight3);
    
    }
    

    if(scene_switch == 1)
    {

        Texture* wood = new Texture("Textures/multipletexture/wooden_fence.png", GL_TEXTURE0);
        Texture* grasss = new Texture("Textures/multipletexture/grass.png", GL_TEXTURE0);
        Model* box1 = new Model(triangle3, sizeof(triangle3), wood, 8);
        Model* box2 = new Model(triangle3, sizeof(triangle3), grasss, 8);
        Model* lightModel = new Model(sphere, sizeof(sphere));

        std::shared_ptr<DrawableObject> obj1 = std::make_shared<DrawableObject>(box1, scenes[scene_switch]->getShaderProgram(0));
        std::shared_ptr<DrawableObject> obj2 = std::make_shared<DrawableObject>(box2, scenes[scene_switch]->getShaderProgram(0));

        obj1->setTransformation(std::make_shared<Transformation>());
        obj1->getTransformation()->addComponent(std::make_shared<Translate>(2.0f, 0.0f, -3.0f));

   
        obj1->getTransformation()->addComponent(std::make_shared<CustomTransformation>());

        obj2->setTransformation(std::make_shared<Transformation>());
        obj2->getTransformation()->addComponent(std::make_shared<Translate>(0.0f, 0.0f, -3.0f));

        scenes[scene_switch]->addObject(obj1);
        scenes[scene_switch]->addObject(obj2);

        PointLight* l = new PointLight(
            glm::vec3(0.0f, 3.0f, 0.0f),
            glm::vec3(0.6f, 0.6f, 1.0f),
            2.0f,
            glm::vec3(1.0f, 0.09f, 0.032f),
            lightModel,
            scenes[scene_switch]->getShaderProgram(1)
        );
        scenes[scene_switch]->addLight(l);

    
    }

    if(scene_switch == 2)
    {

        Model* m = new Model("login.obj", nullptr);
        std::shared_ptr<DrawableObject> dw = std::make_shared<DrawableObject>(m,scenes[scene_switch]->getShaderProgram(1));

        scenes[scene_switch]->addObject(dw);
      
        PointLight* l = new PointLight(
            glm::vec3(0.0f, 0.0f, 2.0f),
            glm::vec3(0.6f, 0.6f, 1.0f),
            1.0f,
            glm::vec3(1.0f, 0.09f, 0.032f),
            nullptr,
            nullptr
        );

        scenes[scene_switch]->addLight(l);

    }
    if (scene_switch == 3)
    {
        Texture* earth_texture = new Texture("Textures/2k_earth_daymap.jpg", GL_TEXTURE0);
        Texture* moon_texture = new Texture("Textures/2k_moon.jpg", GL_TEXTURE0);
        Texture* sun_texture = new Texture("Textures/2k_sun.jpg", GL_TEXTURE0);
        Texture* merkur_texture = new Texture("Textures/2k_mercury.jpg", GL_TEXTURE0);
        Texture* venus_texture = new Texture("Textures/2k_venus_surface.jpg", GL_TEXTURE0);
        Texture* jupiter_texture = new Texture("Textures/2k_jupiter.jpg", GL_TEXTURE0);
        Texture* saturn_texture = new Texture("Textures/2k_saturn.jpg", GL_TEXTURE0);
        Texture* uran_texture = new Texture("Textures/2k_uranus.jpg", GL_TEXTURE0);
        Texture* neptun_texture = new Texture("Textures/2k_neptune.jpg", GL_TEXTURE0);
        Texture* mars_texture = new Texture("Textures/2k_mars.jpg", GL_TEXTURE0);

      
        Model* sunModel = new Model("sphere.obj", sun_texture);
        glm::vec3 sunPos = glm::vec3(0.0f, 0.0f, 0.0f);
        auto sun = std::make_shared<PointLight>(sunPos, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f), sunModel, scenes[scene_switch]->getShaderProgram(3));
        auto sunTrans = sun->getTransformation();
        sunTrans->clear();
        sunTrans->addComponent(std::make_shared<Translate>(sunPos.x, sunPos.y, sunPos.z));
        sun->getSelfTransformation()->addComponent(std::make_shared<Scale>(5.0f, 5.0f, 5.0f));
        sun->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.05f));
        scenes[scene_switch]->addLight(sun.get());
       
        Material* earthmaterial = new Material(
            0.8f,
            0.5f,
            0.0f,
            8.0f
        );
        Material* moonMat = new Material(
            0.8f,
            0.9f,
            0.0f,
            4.0f
        );
        Material* merkurMat = new Material(
            0.8f,
            0.8f,
            0.05f,
            8.0f
        );
        Material* marsMat = new Material(
            0.8f,
            0.8f,
            0.05f,
            8.0f
        );
   
        Material* jupiterMat = new Material(
            0.8f,
            0.9f,
            0.1f,
            12.0f
        );
        Material* uranMat = new Material(
            0.8f,
            0.9f,
            0.1f,
            12.0f
        );

        float sunRadius = 5.0f;

        float merkurOrbitRadius = 2.0f;
        float venusOrbitRadius = 4.0f;
        float earthOrbitRadius = 6.0f;
        float marsOrbitRadius = 9.0f;
        float jupiterOrbitRadius = 12.0f;
        float saturnOrbitRadius = 15.0f;
        float uranOrbitRadius = 17.0f;
        float neptuneOrbitRadius = 19.0f;
    
        auto merkurOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        
        merkurOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 1.0f));
        merkurOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + merkurOrbitRadius, 0.0f, 0.0f));
        
        auto merkur = std::make_shared<DrawableObject>(new Model("sphere.obj", merkur_texture), scenes[scene_switch]->getShaderProgram(4));
       
        merkur->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 2.6f));
        merkur->getSelfTransformation()->addComponent(std::make_shared<Scale>(0.3f, 0.3f, 0.3f));
        merkur->setMaterial(merkurMat);

        auto venusOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        venusOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.4f));
        venusOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + venusOrbitRadius, 0.0f, 0.0f));

        auto venus = std::make_shared<DrawableObject>(new Model("sphere.obj", venus_texture), scenes[scene_switch]->getShaderProgram(4));
        venus->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 2.6f));
        venus->getSelfTransformation()->addComponent(std::make_shared<Scale>(0.6f, 0.6f, 0.6f));
        venus->setMaterial(marsMat);
       
        auto earthOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        earthOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.09f));
        earthOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + earthOrbitRadius, 0.0f, 0.0f));


        auto earth = std::make_shared<DrawableObject>(new Model("sphere.obj", earth_texture), scenes[scene_switch]->getShaderProgram(0));
        earth->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.5f)); 
        earth->getSelfTransformation()->addComponent(std::make_shared<Scale>(0.5f, 0.5f, 0.5f));
        earth->setMaterial(earthmaterial);
      
        auto moonOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        moonOrbit->getInheritTransformation()->addComponent(
            std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.5f)  
        );
        moonOrbit->getInheritTransformation()->addComponent(
            std::make_shared<Translate>(2.0f, 0.0f, 0.0f) 
        );
        auto moon = std::make_shared<DrawableObject>(new Model("sphere.obj", moon_texture), scenes[scene_switch]->getShaderProgram(4));
   
        moon->getSelfTransformation()->addComponent(std::make_shared<Scale>(0.2f, 0.2f, 0.2f)); 
        moon->getSelfTransformation()->addComponent(
            std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.3f) 
        );
        moon->setMaterial(moonMat);

        auto marsOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        marsOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.18f));
        marsOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + marsOrbitRadius, 0.0f, 0.0f));

        auto mars = std::make_shared<DrawableObject>(new Model("sphere.obj", mars_texture), scenes[scene_switch]->getShaderProgram(4));
        mars->getSelfTransformation()->addComponent(std::make_shared<Scale>(0.45f, 0.45f, 0.45f));
        mars->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.7f));
        mars->setMaterial(marsMat);

   
        auto jupiterOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        jupiterOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), -0.05f));
        jupiterOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + jupiterOrbitRadius, 0.0f, 0.0f));

        auto jupiter = std::make_shared<DrawableObject>(new Model("sphere.obj", jupiter_texture), scenes[scene_switch]->getShaderProgram(4));
        jupiter->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 1.0f));
        jupiter->getSelfTransformation()->addComponent(std::make_shared<Scale>(1.5f, 1.5f, 1.5f));
        jupiter->setMaterial(jupiterMat);

        auto saturnOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        saturnOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.03f));
        saturnOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + saturnOrbitRadius, 0.0f, 0.0f));

        auto saturn = std::make_shared<DrawableObject>(new Model("sphere.obj", saturn_texture), scenes[scene_switch]->getShaderProgram(4));
        saturn->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.5f));
        saturn->getSelfTransformation()->addComponent(std::make_shared<Scale>(1.4f, 1.4f, 1.4f));
        saturn->setMaterial(marsMat);


        
        auto uranOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        uranOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.02f));
        uranOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + uranOrbitRadius, 0.0f, 0.0f));

        auto uran = std::make_shared<DrawableObject>(new Model("sphere.obj", uran_texture), scenes[scene_switch]->getShaderProgram(4));
        uran->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.5f));
        uran->getSelfTransformation()->addComponent(std::make_shared<Scale>(1.0f, 1.0f, 1.0f));
        uran->setMaterial(uranMat);


     
        auto neptuneOrbit = std::make_shared<DrawableObject>(nullptr, scenes[scene_switch]->getShaderProgram(0));
        neptuneOrbit->getInheritTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.015f));
        neptuneOrbit->getInheritTransformation()->addComponent(std::make_shared<Translate>(sunRadius + neptuneOrbitRadius, 0.0f, 0.0f));

        auto neptune = std::make_shared<DrawableObject>(new Model("sphere.obj", neptun_texture), scenes[scene_switch]->getShaderProgram(4));
        neptune->getSelfTransformation()->addComponent(std::make_shared<Rotate>(1.0f, glm::vec3(0, 1, 0), 0.5f));
        neptune->getSelfTransformation()->addComponent(std::make_shared<Scale>(1.0f, 1.0f, 1.0f));
        neptune->setMaterial(marsMat);
        
        
       
        jupiterOrbit->addChild(jupiter);
        saturnOrbit->addChild(saturn);
        merkurOrbit->addChild(merkur);
        marsOrbit->addChild(mars);
        earth->addChild(moonOrbit);
        moonOrbit->addChild(moon);
        venusOrbit->addChild(venus);
        earthOrbit->addChild(earth);
        neptuneOrbit->addChild(neptune);
        uranOrbit->addChild(uran);

        sun->addChild(earthOrbit);
        sun->addChild(merkurOrbit);
        sun->addChild(venusOrbit);
        sun->addChild(marsOrbit);
        sun->addChild(saturnOrbit);
        sun->addChild(jupiterOrbit);
        sun->addChild(uranOrbit);
        sun->addChild(neptuneOrbit);

        scenes[scene_switch]->addObject(sun);
    }


    if(scene_switch == 4)
    {
        Model* lightModel = new Model(sphere, sizeof(sphere));

        PointLight* l = new PointLight(
            glm::vec3(0.0f, 2.0f, -2.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            glm::vec3(1.0f, 0.09f, 0.032f),
            lightModel,
            scenes[scene_switch]->getShaderProgram(1)
        );

        scenes[scene_switch]->addLight(l);
    
    }
    if (scene_switch == 5) 
    {
        std::vector<std::string> faces
        {
            "Textures/sky/cubemap/negx.jpg",    // -X
            "Textures/sky/cubemap/negy.jpg",  // -Y
            "Textures/sky/cubemap/negz.jpg",    // -Z
            "Textures/sky/cubemap/posx.jpg",   // +X
            "Textures/sky/cubemap/posy.jpg",     // +Y
            "Textures/sky/cubemap/posz.jpg",   // +Z
        };

        Texture* grassTexture = new Texture("Textures/multipletexture/grass.png", GL_TEXTURE0);
        Model* box = new Model("skybox.obj", grassTexture);
        CubemapTexture* skyboxCubemap = new CubemapTexture(faces);


        SkyboxObject* skybox = new SkyboxObject(
            box,
            skyboxCubemap,
            scenes[0]->getShaderProgram(4),
            this->camera
        );


        scenes[scene_switch]->addObject(std::shared_ptr<DrawableObject>(skybox));

        Model* lightModel = new Model(sphere, sizeof(sphere));

        PointLight* l = new PointLight(
            glm::vec3(0.0f, 5.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            glm::vec3(1.0f, 0.09f, 0.032f),
            lightModel,
            scenes[scene_switch]->getShaderProgram(2)
        );
        scenes[scene_switch]->addLight(l);

        Material* formulaMat = new Material(
            0.25f,
            0.9f,
            0.1f,
            12.0f
        );

        Model* formula = new Model("formula1.obj", nullptr);
        std::shared_ptr<DrawableObject> Formula = std::make_shared<DrawableObject>(formula,scenes[scene_switch]->getShaderProgram(0));

        Texture* grass = new Texture("Textures/multipletexture/grass.png", GL_TEXTURE0);

        Model* plain = new Model(plain2, sizeof(plain2), grass, 8);
        std::shared_ptr<DrawableObject> Plain = std::make_shared<DrawableObject>(plain, scenes[scene_switch]->getShaderProgram(3));

        std::vector<glm::vec3> body = {
             {  0, -1,  10 },
             { 10, -1,  10 },
             { 10, -1, -10 },
             {  0, -1, -10 },
             { -10, -1, -10 },
             { -10, -1,  10 },
             {  0, -1,  10 }  
        };

        BezierSpline spline(body);
       

        Formula->setColor(glm::vec3(0.67f, 0.55f, 0.91f));
        Formula->setMaterial(formulaMat);
        Formula->getTransformation()->addComponent(std::make_shared<BezierSplineMove>(spline, 0.3f));
      
        Formula->getSelfTransformation()->addComponent(std::make_shared<Translate>(0.0f, -1.0f, 0.0f));
        Formula->getSelfTransformation()->addComponent(std::make_shared<Scale>(0.1f, 0.1f, 0.1f));
        Formula->getSelfTransformation()->addComponent(std::make_shared<Rotate>(90.0f, glm::vec3(0, 1, 0)));

        Plain->getSelfTransformation()->addComponent(std::make_shared<Translate>(0.0f, -1.9f, 0.0f));
        Plain->getSelfTransformation()->addComponent(std::make_shared<Scale>(100.0f, 1.0f, 100.0f));
        
        scenes[scene_switch]->addObject(Formula);
        scenes[scene_switch]->addObject(Plain); 
    }
}

void Application::run() {
 
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    double lastTime = 0;
  
   

    while (!glfwWindowShouldClose(window)) {
        
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        int prev_scene = scene_switch;
        
        glfwPollEvents();

        controller->processInput(window, camera, deltaTime, this->scene_switch);
        this->appcontext->scene_index = this->scene_switch;
        
        if (prev_scene != scene_switch) {
            if (scenes[scene_switch]->getObjectsCount() == 0) {
                createModels();
            }
            
            camera->notify();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scenes[scene_switch]->update(deltaTime, this->camera); // or 0.016f
        scenes[scene_switch]->draw();

        glfwSwapBuffers(window);
    }
}
