#ifndef  _LOADOBJ_HPP
#define _LOADOBJ_HPP

#include <vector>
#include <string>
#include <glm\glm.hpp>

#include "Reader.hpp"
#include "Utils.hpp"

using glm::vec3;
using std::string;
using std::vector;

struct Vertex {
	vec3 positions;
	vec3 normals;
};

class ObjLoader
{
	public:
		
		vector<struct Vertex> vertices;
		vector<unsigned int> indices;
	
		vector<string> rawData;

		ObjLoader(std::string filename)
		{
			rawData = Reader(filename).getLine();

			for (string buffer : rawData)
			{
				if (buffer[0] == 'v') {
					
					std::vector<std::string> v = Utils::split(buffer.substr(string("v ").size(), buffer.length() - string("v ").size()), ' ');
					
					auto pos = vec3(
						std::stof(v[0], nullptr),
						std::stof(v[1], nullptr),
						std::stof(v[2], nullptr)
					);
					
					struct Vertex vert;
					vert.positions = pos;

					vertices.push_back(vert);
				}
				else if (buffer[0] == 'f') {
					std::vector<std::string> f = Utils::split(buffer.substr(string("f ").size(), buffer.length() - string("f ").size()), ' ');

					unsigned int f0 = std::stod(f[0], nullptr) - 1;
					unsigned int f1 = std::stod(f[1], nullptr) - 1;
					unsigned int f2 = std::stod(f[2], nullptr) - 1;

					indices.push_back(f0);
					indices.push_back(f1);
					indices.push_back(f2);
					
					auto v1 = vertices[f0].positions;
					auto v2 = vertices[f1].positions;
					auto v3 = vertices[f2].positions;

					auto n = glm::cross(v2 - v1, v3 - v1);

					vertices[f0].normals = n;
					vertices[f1].normals = n;
					vertices[f2].normals = n;
				}
			}
		}
};

#endif // ! _LOADOBJ_HPP