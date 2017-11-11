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

class ObjLoader
{
	public:
		vector<vec3> vertices;
		vector<vec3> normals;
		vector<vec3> indices;
	
		vector<string> rawData;

		ObjLoader(std::string filename)
		{
			rawData = Reader(filename).getLine();

			for (string buffer : rawData)
			{
				if (buffer[0] == 'v') {
					
					std::vector<std::string> v = Utils::split(buffer.substr(string("v ").size(), buffer.length() - string("v ").size()), ' ');
					
					auto vertex = vec3(
						std::stof(v[0], nullptr),
						std::stof(v[1], nullptr),
						std::stof(v[2], nullptr)
					);
					
					vertices.push_back(vertex);
				}
				else if (buffer[0] == 'f') {
					std::vector<std::string> f = Utils::split(buffer.substr(string("f ").size(), buffer.length() - string("f ").size()), ' ');

					auto faces = vec3(
						std::stod(f[0], nullptr),
						std::stod(f[1], nullptr),
						std::stod(f[2], nullptr)
					);

					indices.push_back(faces);
				}
			}
		}
};

#endif // ! _LOADOBJ_HPP