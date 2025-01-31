#include <windows.h>
#include<windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <gl/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "WindowManager.h"
#include "Logger.h"
#include "Timer.h"
#include "camera.h"
#include "Shader.h"



//*** Globle Function Declarations ***
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//*** Global Variable Declaration ***
WindowManager* pWindow = NULL;
Camera* camera = NULL;
float lastX = WindowManager::SCR_WIDTH / 2.0f;
float lastY = WindowManager::SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


GLuint vaoPiramid = 0;
GLuint vbo_position_piramid = 0;
GLuint vbo_color_piramid = 0;
GLuint mvpMatrixUniform = 0;
glm::mat4 perspectiveProjectionMatrix;
GLfloat anglePiramid = 0.0f;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    BOOL LoadGLTexture(GLuint * texture, GLsizei width, GLsizei height, void* data);
	pWindow = new WindowManager();
	MSG msg = { 0 };
	camera = new Camera();



	Logger::Init();

	TIMER_INIT("Window");
	pWindow->initialize();
	TIMER_END(); 
	LOG_INFO("Window Initialized in %.6f seconds", TIMER_GET("Window"));



	///======================== OpenGL ==============================///
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader shaderRed("shaders/advanced_glsl.vs", "shaders/red.fs");
	Shader shaderGreen("shaders/advanced_glsl.vs", "shaders/green.fs");
	Shader shaderBlue("shaders/advanced_glsl.vs", "shaders/blue.fs");
	Shader shaderYellow("shaders/advanced_glsl.vs", "shaders/yellow.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[] = {
		// positions         
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// configure a uniform buffer object
	// ---------------------------------
	// first. We get the relevant block indices
	unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
	unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
	unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.ID, "Matrices");
	// then we link each shader's uniform block to this uniform binding point
	glUniformBlockBinding(shaderRed.ID, uniformBlockIndexRed, 0);
	glUniformBlockBinding(shaderGreen.ID, uniformBlockIndexGreen, 0);
	glUniformBlockBinding(shaderBlue.ID, uniformBlockIndexBlue, 0);
	glUniformBlockBinding(shaderYellow.ID, uniformBlockIndexYellow, 0);
	// Now actually create the buffer
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	// store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
	glm::mat4 projection = glm::perspective(45.0f, (float)WindowManager::SCR_WIDTH / (float)WindowManager::SCR_HEIGHT, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);




	//*** Game LOOP ***
	while (pWindow->isRunning == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				pWindow->isRunning = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// per-frame time logic
			// --------------------
			float currentFrame = static_cast<float>(Timer::getAppRunTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;


			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
			glm::mat4 view = camera->GetViewMatrix();
			glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// draw 4 cubes 
			// RED
			glBindVertexArray(cubeVAO);
			shaderRed.use();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
			shaderRed.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			// GREEN
			shaderGreen.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
			shaderGreen.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			// YELLOW
			shaderYellow.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
			shaderYellow.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			// BLUE
			shaderBlue.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
			shaderBlue.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			
			
			glBindVertexArray(0);
			glUseProgram(0);
            
			
            pWindow->swapDisplayBuffer();

			///================== UPDATE =======================//
			anglePiramid = anglePiramid + 0.01f;
			
		}
	}
	//Delete VBO Color
	if (vbo_color_piramid)
	{
		glDeleteBuffers(1, &vbo_color_piramid);
		vbo_color_piramid = 0;
	}


	//Delete VBO Position
	if (vbo_position_piramid)
	{
		glDeleteBuffers(1, &vbo_position_piramid);
		vbo_position_piramid = 0;
	}


	//Delete VAO
	if (vaoPiramid)
	{
		glDeleteVertexArrays(1, &vaoPiramid);
		vaoPiramid = 0;
	}


	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//*** Function Declaration ***
	void ToggleFullscreen(void);
	void resize(int, int);


	//*** Code ***
	switch (iMsg)
	{
	case WM_SETFOCUS:
		break;

	case WM_KILLFOCUS:
		break;

	case WM_SIZE:
        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_MOUSEMOVE:
		{
			float xpos = static_cast<float>(GET_X_LPARAM(lParam));
			float ypos = static_cast<float>(GET_Y_LPARAM(lParam));

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
			camera->ProcessMouseMovement(xoffset, yoffset);
		}
		
		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			
			break;

		case 'W':
		case 'w':
			camera->ProcessKeyboard(FORWARD, deltaTime * 0.1f);
			break;

		case 'A':
		case 'a':
			camera->ProcessKeyboard(LEFT, deltaTime * 0.1f);
			break;

		case 'S':
		case 's':
			camera->ProcessKeyboard(BACKWARD, deltaTime * 0.1f);
			break;

		case 'D':
		case 'd':
			camera->ProcessKeyboard(RIGHT, deltaTime * 0.1f);
			break;

		
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}


	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}