
#include "Common.hpp"

#include <GL/glew.h> //Include GLEW
#include <GLFW/glfw3.h> //Include GLFW

#include "shader.hpp"
#include "SDFlib/SignedDistanceHelper.hpp"


GLFWwindow* Window;
int WindowWidth=1024, WindowHeight=768;

GLfloat Zoom=3.5;

void ScrollCallback(GLFWwindow* Window, double xoffset, double yoffset)
{
	Zoom = Zoom * (yoffset>=0 ? 0.9 : 1.1);
}

void ResizeCallback(GLFWwindow* Window, int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;

	glViewport(0, 0, WindowWidth, WindowHeight);
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


	// Open a Window and create its OpenGL context
	Window = glfwCreateWindow(WindowWidth, WindowHeight, "SDFcad", NULL, NULL);
	if (Window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW Window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);

	glfwSetWindowSizeCallback(Window, ResizeCallback);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetScrollCallback(Window, ScrollCallback);

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
	GLuint programID = LoadShaders( "TransformVertexShader.vertshader", "Mesh.fragshader" );
	if (programID==(GLuint)-1)
	{
		return -1;
	}


	GLuint MvpID = glGetUniformLocation(programID, "MVP");


	GLuint lookDirID = glGetUniformLocation(programID, "LookDir");


	Time t1 = GetTime();
	InitSignedDistance((char *)"SignedDistance.cpp");

	std::cout << "Generating Marching Cubes...\n";
	unsigned int numEntries;
	UserConstructSignedDistance();
	float *vertexData = MarchingCubes(&numEntries);
	UserDestructSignedDistance();
	std::cout << "Marching Cubes generated (" << numEntries/3 << " vertices) (" << (numEntries/3)/3 << " triangles)\n";

	float *normalData = (float *)malloc(sizeof(float)*numEntries);
	for (unsigned int i=0; i<numEntries; i+=3*3)
	{
		vec3 p1 = vec3(vertexData[i+0*3+0], vertexData[i+0*3+1], vertexData[i+0*3+2]);
		vec3 p2 = vec3(vertexData[i+1*3+0], vertexData[i+1*3+1], vertexData[i+1*3+2]);
		vec3 p3 = vec3(vertexData[i+2*3+0], vertexData[i+2*3+1], vertexData[i+2*3+2]);

		vec3 n1 = cross(p2-p1, p3-p1);
		// float n1mag = length(n1);
		n1 = normalize(n1);

		// vec3 n2 = cross(p1-p2, p3-p2);
		// float n2mag = length(n2);
		// n2 = normalize(n2);
		
		// vec3 n3 = cross(p1-p3, p2-p3);
		// float n3mag = length(n3);
		// n3 = normalize(n3);

		normalData[i+0*3+0]=n1.x; normalData[i+0*3+1]=n1.y; normalData[i+0*3+2]=n1.z;
		normalData[i+1*3+0]=n1.x; normalData[i+1*3+1]=n1.y; normalData[i+1*3+2]=n1.z;
		normalData[i+2*3+0]=n1.x; normalData[i+2*3+1]=n1.y; normalData[i+2*3+2]=n1.z;
	}
	Time t2 = GetTime();

	std::cout << "Total Time: " << DurationString(t1, t2) << "\n";

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numEntries, vertexData, GL_STATIC_DRAW);
	
	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numEntries, normalData, GL_STATIC_DRAW);

	free(vertexData);
	free(normalData);
	
	ShutdownSignedDistance();

	std::cout << "Entering Main Loop\n";
	do
	{
		static GLfloat pitch=0, yaw=0;
		static vec3 pos;

		{

			int mousepress = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_MIDDLE);
			int shiftpress = glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(Window, GLFW_KEY_RIGHT_SHIFT);
			if (shiftpress)
			{
				static bool hold = 0;
				static double startx, starty;
				static vec3 startpos;

				mat4 looktmp = (rotate(mat4(1), yaw, vec3(0, 1, 0))*rotate(mat4(1), pitch, vec3(1, 0, 0)));

				if (mousepress == GLFW_RELEASE && hold)
				{
					hold = 0;
				}
				else if (mousepress == GLFW_PRESS)
				{
					double x, y;
					glfwGetCursorPos(Window, &x, &y);

					if (!hold)
					{
						startx = x;
						starty = y;
						startpos = pos;
						hold=1;
					}
					else
					{
						vec4 newpos = looktmp*vec4(-(startx-x)/WindowHeight, (starty-y)/WindowHeight, 0, 1);
						pos = startpos+vec3(newpos.x, newpos.y, newpos.z)*Zoom;
					}
				}
			}
			else
			{
				static bool hold = 0;
				static double startx, starty;
				static GLfloat startpitch=0, startyaw=0;

				if (mousepress == GLFW_RELEASE && hold)
				{
					hold = 0;
				}
				else if (mousepress == GLFW_PRESS)
				{
					double x, y;
					glfwGetCursorPos(Window, &x, &y);

					if (!hold)
					{
						startx = x;
						starty = y;
						startpitch = pitch;
						startyaw = yaw;
						hold=1;
					}
					else
					{
						pitch = startpitch + (2*3.1415)*(starty-y)/WindowHeight;
						yaw = startyaw + (2*3.1415)*(startx-x)/WindowWidth;
					}
				}
			}
		}


		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		vec3 camPosition = {0, 0, 0};
		vec3 camTarget = {0, 0, -1};
		mat4 viewMat = lookAt(camPosition, camTarget, vec3{0,1,0});
		viewMat = 
			translate(mat4(1), vec3(0, 0, -Zoom)) *
			rotate(mat4(1), -pitch, vec3(1, 0, 0)) *
			rotate(mat4(1), -yaw, vec3(0, 1, 0)) *
			translate(mat4(1), pos) *
			viewMat;

		mat4 Projection = perspective(radians(45.0f), (float)WindowWidth/WindowHeight, 0.001f, 100.0f);
		mat4 mvp        = Projection * viewMat; // Remember, matrix multiplication is the other way around
		glUniformMatrix4fv(MvpID, 1, false, &mvp[0][0]);

		vec4 looktmp = (rotate(mat4(1), yaw, vec3(0, 1, 0)) * rotate(mat4(1), pitch, vec3(1, 0, 0)) * vec4(0,0,1,1));
		vec3 lookDir = normalize(vec3(looktmp.x, looktmp.y, looktmp.z));
		glUniform3f(lookDirID, lookDir.x, lookDir.y, lookDir.z);

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

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangles!
		glDrawArrays(GL_TRIANGLES, 0, numEntries);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		
		// Swap buffers
		glfwSwapBuffers(Window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the Window was closed
	while (glfwGetKey(Window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(Window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL Window and terminate GLFW
	glfwTerminate();

	return 0;
}

