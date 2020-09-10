#include<GL/glew.h>
#include<GL/GLU.h>
#include<GLFW/glfw3.h>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl2.h"
#include"imgui_impl_opengl3.h"
#include<iostream>
#include<string>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<unordered_map>
#include"CameraObj.h"
#include"GlobalVariables.h"
#include<stdio.h>
#include<map>
#include<windows.h>
#include<shobjidl.h> 
#include<fstream>
#include"VertexData.h"
#include"GameObject.h"
#include<string>
#include<memory.h>
#include"ModelImportor.h"
#include"Texture.h"
#include"TextureRepository.h"
#include"ModelRepository.h"
#include"Vector3.h"
#include<time.h>
#include"Light.h"
#include"TextureViewQuad.h"
#include"ComputeShaderRP2.h"
#include"DebugFunctions.h"
#include"ResolutionProperties.h"
#include<chrono>

//Overriding the imGUI list box to support vector input for the UI
namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}
}

//Fucntion Prototypes
void MainUIRender();
void LoadModel(std::string path);
void UpdateTextureListUI();
void UpdateGameObjectListUI();
void CreateLight(float x, float y, float z);
void UpdateLightListUI();
void flipImage(int width, int height, float *data);
void flipImageChar(int width, int height, unsigned char *data);
void CurrentObjectRenderPass2();
void PresetScene1();
void PresetScene2();
void PresetScene3();
void PresetScene4();


// GLFW CallBacks 
void InitializeWindowSystem();
void Resize_Callback(GLFWwindow* window, int width, int height);
void Mouse_Movement_CallBack(GLFWwindow* window, double xpos, double ypos);
void Mouse_Scroll_CallBack(GLFWwindow* window, double xoffset, double yoffset);
void Mouse_Click_CallBack(GLFWwindow* window, int button, int action, int mods);
void oneTimeKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

//Window Properties
GLFWwindow* window;
int ScreenWidth = 1280;
int ScreenHeight = 720;
float lastX = ScreenWidth / 2.0f;
float lastY = ScreenHeight / 2.0f;
bool firstMouse = true;
bool moveMouse = false;

//Texture Properties 
float CurrentTextureWidth = 0.0f;
float CurrentTextureHeight = 0.0f;

//File Path Data
char textureFilePath[500] = { 'T','e','x','t','u','r','e','s','/','t','e','x','t','u','r','e','_','3','2','.','p','p','m' };
char modelFilePath[500] = { 's','p','h','e','r','e','.','o','b','j' };


//Main Camera 
CameraObj MainCamera(0.0f, -6.0f, 20.0f, 0.0f, 1.0f, 0.0f);

//Frame Rate Properties
float FPS = 60.0f;
float FPSSkip = 5.0f;
float CurrentSkipCount = 0.0f;
std::string WriteStatus;

//Timer Status
bool TimerBool = false;
//Current Frame 
float currentFrame = 0.0f;

//Constants
float Light_X = 0.0f;
float Light_Y = 0.0f;
float Light_Z = 0.0f;
int LightCount = 0;

//Current selected elements
int selectedTexture = 0;
int selectedGameObject = 0;
int selectedLight = 0;
float translationSpeed = 0.1f;

//UI List
std::vector<std::string>TextureBoxLabels;
std::vector<std::string>GameObjectsBoxLabels;
std::vector<std::string>LightBoxLabels;

//Physics Engine flags
bool collisionStatic = false;
bool enableCollision = false;
bool rotateSphere = false;

//Repository Handlers
ModelImportor modelImporter;
TextureRepository textureRepo;
ModelRepository modelRepo;
std::vector<std::shared_ptr<Light>>sceneLights;

//Debug Texture Quad
TextureViewQuad textureViewQuad;

//Compute Shader Properties
ComputeShaderRP2 c1;
bool computeRun = false;
bool debugEvaluateBool = false;
bool debugPrintbool = true;

int main(void)
{
	//Set the repo reference
	modelImporter.modelRepository = &modelRepo;

	//Initialize the Main window
	InitializeWindowSystem();

	//Create the light box
	CreateLight(1.2f, 1.0f, 2.0f);

	//Setup Matrices
	glm::mat4 projection(1.0f);
	glm::mat4  view(1.0f);

	//Debuging texture quad
	textureViewQuad.Create();

	//Main glfw Loop
	while (!glfwWindowShouldClose(window))
	{
		//Enable Depth Test
		glEnable(GL_DEPTH_TEST);

		//Clear Buffers
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calculation of delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//Process user Inputs
		processInput(window);
		//Get the view from the camera
		view = MainCamera.GetEulerViewMatix();
		//Set projection matrix
		projection = glm::perspective(glm::radians(MainCamera.CameraSetting_Zoom), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100000000000000000.0f);

		//Render The models
		for (int i = 0; i < modelRepo.gameObjects.size(); i++)
		{
			//RenderPass 1 
			glViewport(0, 0, textureRepo.Textures[0].textureWidth, textureRepo.Textures[0].textureHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, modelRepo.gameObjects[i]->RP1_TextureMap);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			modelRepo.gameObjects[i]->RenderPass_1(view, projection, MainCamera.Camera_Postion, LightBoxLabels.size(), sceneLights);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//Render Pass 2
			glViewport(0, 0, ScreenWidth, ScreenHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (modelRepo.gameObjects[i]->textureAttached == true)
			{
				if (CurrentSkipCount == 0)
				{
					CurrentObjectRenderPass2();
					debugEvaluateBool = true;
				}
				CurrentSkipCount++;
				int skipValue = (int)FPSSkip;
				if (CurrentSkipCount == skipValue)
				{
					CurrentSkipCount = 0;
				}
			}

			//Render Pass 3 and Forward Rendering
			glViewport(0, 0, ScreenWidth, ScreenHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			modelRepo.gameObjects[i]->RenderPass_3(view, projection, MainCamera.Camera_Postion, LightBoxLabels.size(), sceneLights);
		}

		//Render the Lights
		for (int i = 0; i < sceneLights.size(); i++)
		{
			sceneLights[i]->Render(view, projection, MainCamera.Camera_Postion);
		}
		//Setup the view port
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//Render UI
		MainUIRender();
		//Maintain FPS
		while (glfwGetTime() < lastFrame + 1.0 / FPS) {
			// sleep
		}
		lastFrame += 1.0 / FPS;

		currentFrame++;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//GLFW Call back for one time key press
void oneTimeKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

//Process input and move camera
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Forward, deltaTime);

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Backward, deltaTime);

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Left, deltaTime);

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Right, deltaTime);

	}
}

//Initailize the main glfw window and opengl 
void InitializeWindowSystem()
{
	//Initialize GLFW and OPENGL Pointers
	if (!glfwInit())
		std::cout << "failed to load up glfw" << std::endl;
	window = glfwCreateWindow(ScreenWidth, ScreenHeight, "AnimationSystem", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "failed to load up glfw" << std::endl;
		std::cout << "terminating" << std::endl;
	}
	//Setup Call backs
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Resize_Callback);
	glfwSetCursorPosCallback(window, Mouse_Movement_CallBack);
	glfwSetMouseButtonCallback(window, Mouse_Click_CallBack);
	glfwSetScrollCallback(window, Mouse_Scroll_CallBack);
	glfwSetKeyCallback(window, oneTimeKeyCallBack);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//Setup GLEW 
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "error occured in glew" << std::endl;
	}
	//Setup ImGUI
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

//Main UI Render Function
void MainUIRender()
{
	//Setup the ImGUI Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Main GUI");

	//Set the model loading elements
	ImGui::InputText("Model", modelFilePath, sizeof(modelFilePath));
	if (ImGui::Button("Load Model"))
	{
		int size = sizeof(modelFilePath) / sizeof(char);
		std::string path = "";
		for (int i = 0; i < size; i++) {
			path = path + modelFilePath[i];
		}
		LoadModel(path);
	}

	//Texture loading UI Elments
	ImGui::InputText("File", textureFilePath, sizeof(textureFilePath));
	if (ImGui::Button("Load Texture"))
	{
		int size = sizeof(textureFilePath) / sizeof(char);
		std::string path = "";
		for (int i = 0; i < size; i++) {
			path = path + textureFilePath[i];
		}
		textureRepo.LoadTexture(path);
		UpdateTextureListUI();
	}

	//FPS Elements
	ImGui::Text("Current FPS: %f", (float)FPS);
	ImGui::Text("Current Frame: %f", (float)currentFrame);
	ImGui::Text("FPS Limiter");
	ImGui::SliderFloat("FPS", &FPS, 1.0f, 120.0f);
	ImGui::Text("FPS Skip: %f", (float)FPSSkip);
	ImGui::Text("FPS Count Skip");
	ImGui::SliderFloat("FPSSkip", &FPSSkip, 0.0f, 50.0f);

	//Texture Repo Elements
	ImGui::PushItemWidth(350);
	ImGui::Text("Texture Repository");
	ImGui::ListBox("Textures", &selectedTexture, TextureBoxLabels);
	ImGui::Text("Selected ID : %f", (float)selectedTexture);

	//Model Repo Elements
	ImGui::Text("Model Repository");
	ImGui::ListBox("Model", &selectedGameObject, GameObjectsBoxLabels);
	ImGui::Text("Selected ID : %f", (float)selectedGameObject);

	//Attach Texture 
	if (ImGui::Button("Attach Texture"))
	{
		CurrentTextureWidth = textureRepo.Textures[selectedTexture].textureWidth;
		CurrentTextureHeight = textureRepo.Textures[selectedTexture].textureHeight;
		modelRepo.gameObjects[selectedGameObject]->ComputeShaderRP2.ComputeShaderTextureDataSetup(textureRepo.Textures[selectedTexture].textureData, modelRepo.gameObjects[0]->computePipelineVertexData, textureRepo.Textures[selectedTexture].textureWidth, textureRepo.Textures[selectedTexture].textureHeight);
		modelRepo.gameObjects[selectedGameObject]->AttachTexture(textureRepo.Textures[selectedTexture]);
	}

	//Model Translation elements
	ImGui::Text("Model Translation");
	if (ImGui::Button("Model X+"))
	{
		modelRepo.gameObjects[selectedGameObject]->worldPosition.x += translationSpeed;
	}
	ImGui::SameLine();
	if (ImGui::Button("Model X-"))
	{
		modelRepo.gameObjects[selectedGameObject]->worldPosition.x -= translationSpeed;
	}
	if (ImGui::Button("Model Y+"))
	{
		modelRepo.gameObjects[selectedGameObject]->worldPosition.y += translationSpeed;
	}
	ImGui::SameLine();
	if (ImGui::Button("Model Y-"))
	{
		modelRepo.gameObjects[selectedGameObject]->worldPosition.y -= translationSpeed;
	}
	if (ImGui::Button("Model Z+"))
	{
		modelRepo.gameObjects[selectedGameObject]->worldPosition.z += translationSpeed;
	}
	ImGui::SameLine();
	if (ImGui::Button("Model Z-"))
	{
		modelRepo.gameObjects[selectedGameObject]->worldPosition.z -= translationSpeed;
	}

	//Preset Scene Loading buttons
	if (ImGui::Button("Preset Scene 1"))
	{
		PresetScene1();
	}
	if (ImGui::Button("Preset Scene 2"))
	{
		PresetScene2();
	}
	if (ImGui::Button("Preset Scene 3"))
	{
		PresetScene3();
	}
	if (ImGui::Button("Preset Scene 4"))
	{
		PresetScene4();
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

//Load Preset 1
void PresetScene1()
{
	std::string path = "Textures/texture_256.ppm";
	textureRepo.LoadTexture(path);
	UpdateTextureListUI();
	LoadModel("Models/sphere_100.obj");
}

//Load Preset 2
void PresetScene2()
{
	std::string path = "Textures/texture_256.ppm";
	textureRepo.LoadTexture(path);
	UpdateTextureListUI();
	LoadModel("Models/monkey_100.obj");
}

//Load Preset 3
void PresetScene3()
{
	std::string path = "Textures/texture_256.ppm";
	textureRepo.LoadTexture(path);
	UpdateTextureListUI();
	LoadModel("Models/bunny_100.obj");
}

//Load Preset 4
void PresetScene4()
{
	std::string path = "Textures/texture_256.ppm";
	textureRepo.LoadTexture(path);
	UpdateTextureListUI();
	LoadModel("Models/bird_100.obj");
}

//Run the Second render pass of the currently seleted object
void CurrentObjectRenderPass2()
{
	//Create a Buffer
	float *Buff = new float[CurrentTextureWidth * CurrentTextureHeight * 3];
	int size = CurrentTextureWidth * CurrentTextureHeight * 3;

	//Store the Texutre Map
	glBindFramebuffer(GL_FRAMEBUFFER, modelRepo.gameObjects[0]->RP1_TextureMap);
	glNamedFramebufferReadBuffer(modelRepo.gameObjects[0]->RP1_TextureMap, GL_RGB);
	glReadPixels(0, 0, CurrentTextureWidth, CurrentTextureHeight, GL_RGB, GL_FLOAT, Buff);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Run the compute shader
	modelRepo.gameObjects[0]->ComputeShaderRP2.LaunchComputeShader(Buff, modelRepo.gameObjects[0]->computePipelineVertexData, textureRepo.Textures[0].textureData, MainCamera.Camera_Postion, modelRepo.gameObjects[0]->model, CurrentTextureWidth, CurrentTextureHeight);

}


//Function to create light
void CreateLight(float x, float y, float z)
{
	if (sceneLights.size() == 10)
	{
		return;
	}
	sceneLights.push_back(std::make_shared<Light>());
	sceneLights.back()->CreateLight(Vector3(x, y, z));
	UpdateLightListUI();

}

//Update Light List
void UpdateLightListUI() {

	std::string name = "Light" + std::to_string(LightCount);
	LightBoxLabels.push_back(name);
	LightCount++;
}

//Load The models
void LoadModel(std::string path)
{
	if (textureRepo.Textures.size() != 0)
	{
		std::shared_ptr<GameObject> gameObject;
		gameObject = std::make_shared<GameObject>();
		modelImporter.LoadObjFile(path, gameObject);
		gameObject->CreateObject(glm::vec3(0.0f, -6.0f, 0.0f), 1.0f, GL_TRIANGLES, path, "shell.ppm");
		gameObject->CreateFrameBuffer(ScreenWidth, ScreenHeight, textureRepo.Textures[0].textureWidth, textureRepo.Textures[0].textureHeight);
		std::cout << gameObject->uniqueVertices.back().triangleIndex;
		UpdateGameObjectListUI();
	}
}

//Function to Update Texture List
void UpdateTextureListUI()
{
	TextureBoxLabels.push_back(textureRepo.Textures.back().name);
}

//Function to Update Model List
void UpdateGameObjectListUI()
{
	GameObjectsBoxLabels.push_back(modelRepo.gameObjects.back()->name);
}

//GLFW Resize
void Resize_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
//GLFW Mouse Click Callback
void Mouse_Click_CallBack(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		moveMouse = true;
		std::cout << moveMouse << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

		moveMouse = false;
		std::cout << moveMouse << std::endl;
	}

}
//GLFW Mouse movement
void Mouse_Movement_CallBack(GLFWwindow* window, double xpos, double ypos)
{

	if (moveMouse)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;
		//std::cout << xoffset << "    " << yoffset << std::endl;

		if (xoffset != 0)
		{
			if (xoffset < 0)
			{
				xoffset = xoffset / xoffset;
				xoffset = (-1)* xoffset;
			}
			else
			{
				xoffset = xoffset / xoffset;
			}
		}
		if (yoffset != 0)
		{
			if (yoffset < 0)
			{
				yoffset = yoffset / yoffset;
				yoffset = (-1)* yoffset;
			}
			else
			{
				yoffset = yoffset / yoffset;
			}
		}

		MainCamera.ProcessMouseInput(xoffset, yoffset);
	}
}
//GLFW Mouse scroll
void Mouse_Scroll_CallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	MainCamera.ProcessMouseScrollInput(yoffset);
}
void flipImage(int width, int height, float *data)
{
	char rgb[3];

	for (int y = 0; y < height / 2; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int top = (x + y * width) * 3;
			int bottom = (x + (height - y - 1) * width) * 3;

			memcpy(rgb, data + top, sizeof(rgb));
			memcpy(data + top, data + bottom, sizeof(rgb));
			memcpy(data + bottom, rgb, sizeof(rgb));
		}
	}
}
void flipImageChar(int width, int height, unsigned char *data)
{
	char rgb[3];

	for (int y = 0; y < height / 2; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int top = (x + y * width) * 3;
			int bottom = (x + (height - y - 1) * width) * 3;

			memcpy(rgb, data + top, sizeof(rgb));
			memcpy(data + top, data + bottom, sizeof(rgb));
			memcpy(data + bottom, rgb, sizeof(rgb));
		}
	}
}