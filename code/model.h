#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>
using namespace std;

const float modelYAW = 35.0f;
const float modelPITCH = 0.0f;
const float modelROLL = 0.0f;
const float modelSPEED = 0.01f;
const float rotateSPEED = 1.0f;

class Model 
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    map<int, glm::vec3> Vertices;
    int Nvertices;
    //glm::vec3 geomvec;
    vector<glm::vec3> OrigCC;

    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Position;
    glm::vec3 Front;
    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;
    // camera options
    float MovementSpeed;
    float RotateSpeed;
    Model();

    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false);
    void clearVec();

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    void updateVec();


    // draws the model, and thus all its meshes
    void Draw(Shader& shader);

    //glm::vec3 getGeometric()
    //{
    //    glm::vec3 geometric = glm::vec3(0.0f, 0.0f, 0.0f);
    //    std::map<int, glm::vec3>::iterator it = Vertices.begin(), itEnd = Vertices.end();
    //    while (it != itEnd) {
    //        geometric += it->second;
    //        it++;
    //    }
    //    geometric /= Vertices.size();
    //    return geometric;
    //}

    void origCC();

    vector<glm::vec3> getCC(glm::mat4 model);

    void Translate();
    //void Rotate(double r)
    //{
    //    rotate = glm::radians(r);

    //}

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};


#endif