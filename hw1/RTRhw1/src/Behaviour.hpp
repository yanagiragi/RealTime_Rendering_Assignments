#pragma once
#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "ObjLoader.hpp"
#include "Reader.hpp"

class Behaviour 
{
	public:

		/* Variables */

		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 pos;\n"
			"layout (location = 0) in vec3 normal;\n"
			"out vec4 ourColor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
			"	ourColor = vec4(normal.x, normal.y, normal.z, 0.0);"
			"}\0";

		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec4 ourColor;\n"
			"void main()\n"
			"{\n"
			"	vec4 Red = vec4(1.0, 0.0, 0.0, 1.0);"
			"   FragColor = ourColor;\n"
			"}\n\0";

		unsigned int VAO, VBO, EBO;
		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;
		
		// Load & Parse Obj
		ObjLoader loader = ObjLoader("Resources/teapot.obj");


		/* Functions */

		void Behaviour::setupShader() 
		{
			// Create Shader program and setup buffers
			Shader generator = Shader();

			generator.CreateShader(vertexShader, GL_VERTEX_SHADER, vertexShaderSource);
			generator.CreateShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderSource);

			generator.CreateProgram(shaderProgram, 2, vertexShader, fragmentShader);

			// VAO = vertex attribute object, 1 stands for one buffer
			glGenVertexArrays(1, &VAO);
			// VBO = vertex buffer object
			glGenBuffers(1, &VBO);
			// EBO = element buffer object
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// vertex buffer type: GL_ARRAY_BUFFER
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			// copy vertex data into buffer's memory
			// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
			glBufferData(GL_ARRAY_BUFFER, loader.vertices.size() * sizeof(Vertex), &(loader.vertices[0]), GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.indices.size() * sizeof(unsigned int), &(loader.indices[0]), GL_STATIC_DRAW);

			// ¼È®É¥ý¤£¥ánormal

			// 0 is the index of vertex attribute
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void *)0);

			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, normals));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		// Prepare data
		void Behaviour :: Start()
		{
			// Setup Shader, VAO, VBO, EBO
			setupShader();
		}

		void Behaviour :: Update()
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Wireframe Mode
			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			// Normal Mode
			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// update the uniform of fragment shader
			float time = glfwGetTime();
			float greenValue = (sin(time) / 2.0f) + 0.5f;
			int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

			glUseProgram(shaderProgram);
			//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, loader.indices.size(), GL_UNSIGNED_INT, 0);
		}

		// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
		void Behaviour:: Input(GLFWwindow *window)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}

		void Behaviour :: Destroy()
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}
};

#endif