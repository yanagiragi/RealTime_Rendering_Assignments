#pragma once
#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "ObjLoader.hpp"
#include "Reader.hpp"
#include "Camera.hpp"

#include "Quaternion.hpp"
#include <glm\gtx\quaternion.hpp>

class Behaviour 
{
	public:

		/* Variables */

		unsigned int VAO, VBO, EBO;
		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;				
		
		mat4 MVP;
		Camera mainCamera;
		
		// Load & Parse Obj
		ObjLoader loader = ObjLoader("Resources/teapot.obj");

		float previous_time = glfwGetTime();

		quaternion q;

		/* Functions */
		
		void Behaviour::setupCamera()
		{
			vec3 cameraPos = vec3(0.0, 0.0, 15.0);
			vec3 cameraFront = vec3(0.0, 0.0, -1.0);
			vec3 cameraUp = vec3(-1.0, 0.0, 0.0);

			float fov = 45.0f;
			float aspect = (float)(Utils::SCR_HEIGHT) / (float)(Utils::SCR_WIDTH);
			float nearDistance = 0.1f;
			float farDistance = 1000.0f;

			mainCamera = Camera(cameraPos, cameraFront, cameraUp, fov, nearDistance, farDistance, aspect);
		}

		void Behaviour::setupShader() 
		{
			// Create Shader program and setup buffers
			Shader generator = Shader();

			Reader vertexShaderReader("Resources/vertex.glsl");
			generator.CreateShader(vertexShader, GL_VERTEX_SHADER, vertexShaderReader.getLineBuffer().c_str());

			Reader fragmentShaderReader("Resources/fragment.glsl");
			generator.CreateShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderReader.getLineBuffer().c_str());

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

			// 暫時先不丟normal

			// 0 is the index of vertex attribute			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void *)0);
			glEnableVertexAttribArray(0);

			// vertex normals
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, positions));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		// Prepare data
		void Behaviour :: Start()
		{
			// Setup Shader, VAO, VBO, EBO
			setupShader();

			// Setup Camera settings
			setupCamera();

			glEnable(GL_DEPTH_TEST);

			// initate quaternion
			q = quaternion(1.0, 0.0, 0.0, 0.0);
		}


		void Behaviour::Update()
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgram);

			// Setup Model Matrixs
			glm::mat4 M(1.0f);
			M = glm::translate(M, vec3(0, 0, 10));

			#pragma region "Done it native way"
			/*
				glm::quat MyQuaternion;
				float RotationAngle = glfwGetTime();
			
				// 公轉
				MyQuaternion = glm::angleAxis(RotationAngle, vec3(0.0, 1.0, 0.0));
				// q.w = MyQuaternion.w; q.x = MyQuaternion.x; q.y = MyQuaternion.y; q.z = MyQuaternion.z;

				// 自轉
				MyQuaternion = glm::angleAxis(RotationAngle, vec3(1.0, 0.0, 0.0)) * MyQuaternion;
				q.w = MyQuaternion.w; q.x = MyQuaternion.x; q.y = MyQuaternion.y; q.z = MyQuaternion.z;

				M = M * glm::toMat4(glm::quat(q.w, q.x, q.y, q.z));
			*/
			#pragma endregion			

			#pragma region "Done it Yanagi's class way"
			/*
				quaternion MyQuaternion;
				float RotationAngle = glfwGetTime();

				// 公轉
				MyQuaternion = q.rotate(RotationAngle, vec3(0.0, 1.0, 0.0));
				// q.w = MyQuaternion.w; q.x = MyQuaternion.x; q.y = MyQuaternion.y; q.z = MyQuaternion.z;

				// 自轉
				MyQuaternion = q.rotate(RotationAngle, vec3(1.0, 0.0, 0.0)).mul(MyQuaternion);
				q.w = MyQuaternion.w; q.x = MyQuaternion.x; q.y = MyQuaternion.y; q.z = MyQuaternion.z;

				M = M * glm::toMat4(glm::quat(q.w, q.x, q.y, q.z));
			*/
			/*
				// Failed way
				glm::vec3 res = q.rotate(glm::vec3(sin(y), cos(y), 0.0));
				q.x = res.x;
				q.y = res.y;
				q.z = res.z;
				q.Normalize();
			*/
			#pragma endregion

			#pragma region "Done it Yanagi's Shader way"
				glUniform1f(glGetUniformLocation(shaderProgram, "Time"), previous_time);
			#pragma endregion			

			// Setting MVPs
			MVP = mainCamera.getP() * mainCamera.getV() * M;
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, loader.indices.size(), GL_UNSIGNED_INT, 0);

			float current_time = glfwGetTime();
			std::cout << "\r FPS : " << 1.0 / (current_time - previous_time);
			previous_time = current_time;
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