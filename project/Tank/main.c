#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <learnopengl/camera.h>
#include <learnopengl/Controler.h>
#include <learnopengl/MyCube.h>
#include <learnopengl/Bomb.h>

#include <iostream>

#define numCubes 4
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void initGLFWwindow();
void renderLoop(Shader & ourShader, Shader & sceneShader, Model & tankModel, Model & sceneModel, MyCube  myCubes[numCubes]);
void configOpenglState();
void renderTank(Shader & ourShader, Model & tankModel);
void renderScene(Shader & ourShader, Shader & sceneShader, Model & sceneModel, MyCube  myCubes[numCubes]);
void setLight(Shader & ourShader);
// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float myTime = 0.0f;
int flag = 0;

//window
GLFWwindow* window = NULL;

//tankControler
Controler tankControler = Controler(glm::vec3(0.0f, 0.0f, 0.0f));

//Bomb
Bomb * myBombPtr;

int main()
{
	//init the window;
	initGLFWwindow();

    // configure global opengl state
	configOpenglState();
    
    // build and compile shaders
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
	Shader sceneShader("sceneShader.vs", "sceneShader.fs");

    // load models
    Model tankModel("resources/Tank/IS_2/is 2.FBX");
	Model sceneModel("resources/scene/sceneobj/竞技场.obj");
	MyCube myCubes[numCubes] = { MyCube(glm::vec3(0.0f, 0.0f, -15.0f)), MyCube(glm::vec3(0.0f, 0.0f, 15.0f)), MyCube(glm::vec3(15.0f, 0.0f, 0.0f)), MyCube(glm::vec3(-15.0f, 0.0f, 0.0f)) };
	myBombPtr = &Bomb();

    // render loop
	renderLoop(ourShader, sceneShader, tankModel, sceneModel, myCubes);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//camera.ProcessKeyboard(FORWARD, deltaTime);
		tankControler.move(FRONT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//camera.ProcessKeyboard(BACKWARD, deltaTime);
		tankControler.move(BACK, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//camera.ProcessKeyboard(LEFT, deltaTime);
		tankControler.move(FRONT_AND_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//camera.ProcessKeyboard(LEFT, deltaTime);
		tankControler.move(BACK_AND_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//camera.ProcessKeyboard(RIGHT, deltaTime);
		tankControler.move(FRONT_AND_RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//camera.ProcessKeyboard(RIGHT, deltaTime);
		tankControler.move(BACK_AND_RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (!myBombPtr->cube.flag) {
			myBombPtr->cube.flag = true;
			myBombPtr->front = camera.Front;
			myBombPtr->front.y = 0.0f;
			myBombPtr->cube.position = tankControler.getObjectPositon();
		}
	}
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    tankControler.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

//init GLFW window
//-----------------------------------
void initGLFWwindow() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
}

//configure opengl global state
//-------------------------------
void configOpenglState() {
	glEnable(GL_DEPTH_TEST);
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

//set parameter of light
//-------------------------------------
void setLight(Shader & ourShader) {
	//set parameter of pointLight
	ourShader.setVec3("pointLights.position", glm::vec3(0.0f, 5.0f, -10.0f));
	ourShader.setVec3("pointLights.ambient", 0.05f, 0.05f, 0.05f);
	ourShader.setVec3("pointLights.diffuse", 0.8f, 0.8f, 0.8f);
	ourShader.setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);
	ourShader.setFloat("pointLights.constant", 1.0f);
	ourShader.setFloat("pointLights.linear", 0.07f);
	ourShader.setFloat("pointLights.quadratic", 0.017f);

	//set parameter of DirLight
	ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	//set Flag
	if (myTime > 5.0f) {
		myTime = 0.0f;
		flag = flag ^ 0x1;
		ourShader.setInt("flag", flag);
	}

	ourShader.setVec3("viewPos", camera.Position);
	ourShader.setFloat("material.shininess", 32.0f);
}

//the function of render loop
//------------------------------------------------------------------
void renderLoop(Shader & ourShader, Shader & sceneShader, Model & tankModel, Model & sceneModel, MyCube  myCubes[numCubes]) {
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		myTime += deltaTime;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		//set light source
		setLight(ourShader);

		renderTank(ourShader, tankModel);
		renderScene(ourShader, sceneShader, sceneModel, myCubes);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//render the object of tank
//---------------------------------
void renderTank(Shader & ourShader, Model & tankModel) {
	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
	camera.setParameter(tankControler.getObjectPositon(), tankControler.getRotateAngleTop());
	glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);

	// render the loaded model
	glm::mat4 modelTop;
	modelTop = glm::translate(modelTop, tankControler.getObjectPositon()); // translate it down so it's at the center of the scene
	modelTop = glm::scale(modelTop, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
	modelTop = glm::translate(modelTop, glm::vec3(0.0f, 0.0f, 200.0f));
	modelTop = glm::rotate(modelTop, glm::radians(-(tankControler.getRotateAngleTop() + 90.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
	modelTop = glm::translate(modelTop, glm::vec3(0.0f, 0.0f, -200.0f));
	modelTop = glm::rotate(modelTop, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelTop = glm::rotate(modelTop, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ourShader.setMat4("model", modelTop);
	tankModel.Draw(ourShader, 0);

	glm::mat4 modelBottom;
	modelBottom = glm::translate(modelBottom, tankControler.getObjectPositon()); // translate it down so it's at the center of the scene
	modelBottom = glm::scale(modelBottom, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
	modelBottom = glm::translate(modelBottom, glm::vec3(0.0f, 0.0f, 200.0f));
	modelBottom = glm::rotate(modelBottom, glm::radians(-(tankControler.getRotateAngleBottom() + 90.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
	modelBottom = glm::translate(modelBottom, glm::vec3(0.0f, 0.0f, -200.0f));
	modelBottom = glm::rotate(modelBottom, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelBottom = glm::rotate(modelBottom, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ourShader.setMat4("model", modelBottom);
	tankModel.Draw(ourShader, 1);
}

//render the object of tank
//---------------------------------
void renderScene(Shader & ourShader, Shader & sceneShader, Model & sceneModel, MyCube  myCubes[numCubes]) {
	
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
	glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);
	
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::translate(model, glm::vec3(0.0f, -16.0f, 0.0f));
	ourShader.setMat4("model", model);
	sceneModel.Draw(ourShader);

	sceneShader.use();
	for (int i = 0; i < numCubes; ++i) {
		if (myCubes[i].flag) {
			if (glm::distance(myBombPtr->cube.position, myCubes[i].position) < 0.6f) {
				myBombPtr->cube.flag = false;
				myCubes[i].flag = false;
				continue;
			}
			sceneShader.setMat4("projection", projection);
			sceneShader.setMat4("view", view);
			glm::mat4 model;
			model = glm::translate(model, myCubes[i].position);
			sceneShader.setMat4("model", model);
			myCubes[i].Draw(sceneShader);
		}
	}
	if (myBombPtr->cube.flag) {
		sceneShader.setMat4("projection", projection);
		sceneShader.setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, myBombPtr->cube.position);
		sceneShader.setMat4("model", model);
		myBombPtr->Draw(sceneShader);
		myBombPtr->updatePosition(deltaTime);
		if (myBombPtr->cube.position.x > 20.f || myBombPtr->cube.position.z > 20.f) {
			myBombPtr->cube.flag = false;
		}
	}
}
