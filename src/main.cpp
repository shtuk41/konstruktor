// GlfwLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define GLM_FORCE_RADIANS

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include <axes3d.h>
#include <camera.h>
#include <exceptions.h>
#include <shaders.h>
#include <freetype/freetype.h>
#include <window.h>
#include <optionsreader.h>

float rotateY = 0.025f;
float rotateX = -0.145f;

int window_width = options_reader::OptionsReader::GetApplicationWindowOptions()->GetWidth();
int window_height = options_reader::OptionsReader::GetApplicationWindowOptions()->GetHeight();

#define ENABLE_DEBUG_WINDOW_OUTPUT

static void glfw_error_callback(int error, const char* description)
{
	std::cout << "Glfw Error " << error << " : " << description << "\n";
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS && action != GLFW_REPEAT)

		return;

	switch (key)

	{

	case GLFW_KEY_ESCAPE:

		glfwSetWindowShouldClose(window, GL_TRUE);

		break;

	case GLFW_KEY_SPACE:

		rotateX = 0;

		rotateY = 0;

		break;

	case GLFW_KEY_Z:

		//rotateX += 0.01f;

		break;

	case GLFW_KEY_X:

		//rotateX -= 0.01f;

		break;

	case GLFW_KEY_A:

		//rotateY += 0.01f;

		break;

	case GLFW_KEY_S:

		//rotateY -= 0.01f;

		break;

	default:

		break;

	}
}

double previous_xpos;
double previous_ypos;
double xposition;
double yposition;
bool rotateEnable = false;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	xposition = xpos;
	yposition = ypos;

	if (rotateEnable)
	{
		double delta_x = xpos - previous_xpos;

		rotateY += (float)(0.005f * delta_x);

		double delta_y = ypos - previous_ypos;

		rotateX += (float)(0.005f * delta_y);

		previous_xpos = xpos;
		previous_ypos = ypos;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			double xpos, ypos;

			glfwGetCursorPos(window, &xpos, &ypos);

			previous_xpos = xpos;
			previous_ypos = ypos;

			rotateEnable = true;
		}
		else if (action == GLFW_RELEASE)
		{
			rotateEnable = false;
		}
	}
}

bool moveback = false;
bool moveforward = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset >= 1.0)
	{
		moveback = true;
		moveforward = false;
	}
	else if (yoffset <= 1.0)
	{
		moveback = false;
		moveforward = true;
	}
}

void mouse_instersections(Camera & cam)
{
	double x = (2.0 * xposition) / window_width - 1.0;
	double y = 1.0f - (2.0 * yposition) / window_height;
	double z = -1.0f;

#ifdef ENABLE_DEBUG_WINDOW_OUTPUT
	std::cout << "init: " << x << " " << y << " " << z << std::endl;
#endif

	glm::mat4 model_matrix = glm::mat4(1.0);


	model_matrix = glm::rotate(model_matrix, glm::pi<float>() * rotateY, glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::pi<float>() * rotateX, glm::vec3(1.0f, 0.0f, 0.0f));



	glm::vec4 newy = cam.getProjectionMatrix() * cam.getViewMatrix() * model_matrix * glm::vec4(0.0, 1, 0, 1);

#ifdef ENABLE_DEBUG_WINDOW_OUTPUT
	std::cout << "rnewy: " << newy.x / newy.w << " " << newy.y / newy.w << " " << newy.z / newy.w << std::endl;
#endif
}

int main()
{
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
		return -1;

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	Window window(window_width, window_height, key_callback, mouse_callback, mouse_button_callback, scroll_callback);

	glfwSwapInterval(1); // Enable vsync

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initlize GLEW" << "\n";
		return 0;
	}

	//BLEND settings
	glEnable(GL_BLEND);
	glFrontFace(GL_CW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera camera(window.GetHandler());
	GLuint program_id = LoadShaders(".\\shaders\\fovs.vert", ".\\shaders\\fovs.frag");
	glUseProgram(program_id);
	GLuint matrix_id = glGetUniformLocation(program_id, "MVP");

	//setup graph objects
	Axes3d axes3d(10.0f, 10.0f, 10.0f);
	axes3d.Setup(program_id);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.GetHandler(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 backgroundColor = ImVec4(0.074f, 0.129f, 0.16f, 1.00f);

	int index = (301 - 1) * 18;
	int index2 = (576 - 1) * 18;

	GLuint texture_program_id = LoadShaders(".\\shaders\\texture.vert", ".\\shaders\\texture.frag");
	glUseProgram(texture_program_id);

	//Readout readout(0, 0, 16);

	while (!glfwWindowShouldClose(window.GetHandler()))
	{
		glfwPollEvents();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;

			ImGui::Begin("ControlWindow");                          
			ImGui::ColorEdit3("clear color", (float*)&backgroundColor); 
			ImGui::Text("X %.3f, Y %0.3f", rotateX, rotateY);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window.GetHandler(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(backgroundColor.x * backgroundColor.w, backgroundColor.y * backgroundColor.w, backgroundColor.z * backgroundColor.w, backgroundColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glEnable(GL_CULL_FACE);
		glUseProgram(program_id);

		camera.computeViewProjectionMatrices(moveback, moveforward);

		mouse_instersections(camera);

		moveback = false;
		moveforward = false;

		glm::mat4 projection_matrix = camera.getProjectionMatrix();
		glm::mat4 view_matrix = camera.getViewMatrix();
		glm::mat4 model_matrix = glm::mat4(1.0);

		model_matrix = glm::rotate(model_matrix, glm::pi<float>() * rotateY, glm::vec3(0.0f, 1.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::pi<float>() * rotateX, glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;

		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

		axes3d.Draw();

		glfwSwapBuffers(window.GetHandler());
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	window.Destroy();
	window.~Window();

	glfwTerminate();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
