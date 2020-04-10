#include <iostream>
#include <fstream>
#include "mge/core/ShaderProgram.hpp"

ShaderProgram::ShaderProgram():_programId(0), _shaderIds() {
    //why does opengl use glCreateProgram and not glGenProgram (1, &_programID)? Who knows:) *shrugs*
    _programId = glCreateProgram();
 //   std::cout << '\n' << "Program created with id:" << _programId << '\n';
}

ShaderProgram::~ShaderProgram() {}

void ShaderProgram::addShader (GLuint pShaderType, const std::string &pShaderPath) {
    std::string shaderCode = _readFile(pShaderPath);
    GLuint shaderId = _compileShader(pShaderType, shaderCode);

    if (shaderId != 0) {
        _shaderIds.push_back (shaderId);
    }
}

std::string ShaderProgram::_readFile(const std::string& pShaderPath)
{
	std::string contents;
	std::ifstream file (pShaderPath, std::ios::in);
	if(file.is_open()){
//		std::cout << "Reading shader file... " << pShaderPath << '\n';
		std::string line = "";
		while(getline(file, line)) contents += "\n" + line;
		file.close();
	} else {
	//	std::cout << "Error reading shader " << pShaderPath << '\n';
//		contents = "";
	}
	return contents;
}

// compile the code, and detect errors.
GLuint ShaderProgram::_compileShader(GLuint pShaderType, const std::string& pShaderSource)
{
//	std::cout << "Compiling shader... " << '\n';
	const char * sourcePointer = pShaderSource.c_str();
	GLuint shaderId = glCreateShader(pShaderType);
	glShaderSource(shaderId, 1, &sourcePointer, NULL );
	glCompileShader(shaderId);

	GLint compilerResult = GL_FALSE;
	glGetShaderiv( shaderId, GL_COMPILE_STATUS, &compilerResult);

	if (compilerResult) {
//		std::cout << "Shader compiled ok." << '\n';
		return shaderId;
	} else { // get error message
//	    std::cout << "Shader error:" << '\n';
		int infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* errorMessage = new char[ infoLogLength+1 ];
		glGetShaderInfoLog( shaderId, infoLogLength, NULL, errorMessage);
	//	std::cout << errorMessage << '\n' << '\n';
		delete[] errorMessage;
		return 0;
	}
}

void ShaderProgram::finalize() {
    for (size_t i = 0; i < _shaderIds.size();++i) {
        glAttachShader(_programId, _shaderIds[i]);
    }

    glLinkProgram(_programId);

    // Check the program
    GLint linkResult = GL_FALSE;
    glGetProgramiv( _programId, GL_LINK_STATUS, &linkResult);

    if ( linkResult ) {
  //      std::cout << "Program linked ok." << '\n' << '\n';
    } else { // error, show message
 //       std::cout << "Program error:" << '\n';

        int infoLogLength;
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* errorMessage = new char[infoLogLength+1];
        glGetProgramInfoLog(_programId,infoLogLength,NULL,errorMessage);
  //      std::cout << errorMessage << '\n' << '\n';
        delete[] errorMessage;
    }

    for (size_t i = 0; i < _shaderIds.size();++i) {
        glDeleteShader(_shaderIds[i]);
    }
}

GLuint ShaderProgram::getUniformLocation (const std::string& pName) {
    return glGetUniformLocation (_programId, pName.c_str());
}

GLuint ShaderProgram::getAttribLocation (const std::string& pName) {
    return glGetAttribLocation (_programId, pName.c_str());
}

void ShaderProgram::use()
{
    glUseProgram(_programId);
}
