// Include standard headers
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "MarchingCubes.hpp"



// #define RAYMARCH


GLfloat Zoom=3.5;

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Zoom = Zoom * (yoffset>=0 ? 0.9 : 1.1);
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "SDFcad", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetScrollCallback(window, ScrollCallback);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
#ifdef RAYMARCH
	GLuint programID = LoadShaders( "TransformVertexShader.vertshader", "RayMarch.fragshader" );
#else
	GLuint programID = LoadShaders( "TransformVertexShader.vertshader", "Mesh.fragshader" );
#endif
	if (programID==(GLuint)-1)
	{
		return -1;
	}


	GLuint MvpID = glGetUniformLocation(programID, "MVP");

#ifdef RAYMARCH
	GLuint viewMatID = glGetUniformLocation(programID, "ViewMat"); 


	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,0.0f,
		-1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		
		-1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

#else
	std::cout << "Generating Marching Cubes...\n";
	std::vector<vec3> vertices = MarchingCubes();
	std::cout << "Marching Cubes generated (" << vertices.size() << " vertices)\n";
	
	GLfloat g_vertex_buffer_data[vertices.size()*3];

	for (unsigned int i=0; i<vertices.size(); i++)
	{
		g_vertex_buffer_data[i*3+0] = vertices[i].x;
		g_vertex_buffer_data[i*3+1] = vertices[i].y;
		g_vertex_buffer_data[i*3+2] = vertices[i].z;
		// std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << "\n";
	}
	std::cout << "Copied vertices\n";

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size()*3, g_vertex_buffer_data, GL_STATIC_DRAW);
#endif

	std::cout << "Entering Main Loop\n";
	do
	{
		static GLfloat pitch=0, yaw=0;

		{
			static bool last = 0;
			static double startx, starty;
			static GLfloat startpitch=0, startyaw=0;

			int press = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
			if (press == GLFW_RELEASE && last)
			{
				last = 0;
			}
			else if (press == GLFW_PRESS)
			{
				double x, y;
				glfwGetCursorPos(window, &x, &y);

				if (!last)
				{
					startx = x;
					starty = y;
					startpitch = pitch;
					startyaw = yaw;
					last=1;
				}
				else
				{
					pitch = startpitch + (2*3.1415)*(starty-y)/768;
					yaw = startyaw + (2*3.1415)*(startx-x)/1024;
				}
			}
		}

		// for (int i=0; i<4; i++)
		// {
		// 	for (int j=0; j<4; j++)
		// 	{
		// 		printf("%f ", viewMat[i][j]);
		// 	}
		// 	printf("\n");
		// }
		// printf("\n");


		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

#ifdef RAYMARCH
		glm::vec3 camPosition = {0, 0, 0};
		glm::vec3 camTarget = {0, 0, -1};
		glm::mat4 viewMat = glm::lookAt(camPosition, camTarget, glm::vec3{0,1,0});
		viewMat = glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0)) * 
			glm::rotate(glm::mat4(1), pitch, glm::vec3(1, 0, 0)) * 
			glm::translate(glm::mat4(1), glm::vec3(0, 0, Zoom)) *
			viewMat;

		glm::mat4 mvp = glm::mat4(1);
		glUniformMatrix4fv(MvpID, 1, false, &mvp[0][0]);
		glUniformMatrix4fv(viewMatID, 1, false, &viewMat[0][0]);
#else
		glm::vec3 camPosition = {0, 0, 0};
		glm::vec3 camTarget = {0, 0, -1};
		glm::mat4 viewMat = glm::lookAt(camPosition, camTarget, glm::vec3{0,1,0});
		viewMat = 
			glm::translate(glm::mat4(1), glm::vec3(0, 0, -Zoom)) *
			glm::rotate(glm::mat4(1), -pitch, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) *
			viewMat;

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 mvp        = Projection * viewMat; // Remember, matrix multiplication is the other way around
		glUniformMatrix4fv(MvpID, 1, false, &mvp[0][0]);
#endif

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangles!
		
#ifdef RAYMARCH
		glDrawArrays(GL_TRIANGLES, 0, 2*3); // 12*3 indices starting at 0 -> 12 triangles
#else
		glDrawArrays(GL_TRIANGLES, 0, vertices.size()*3);
#endif

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

