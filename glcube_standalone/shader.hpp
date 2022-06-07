#include<string>
#include<dirent.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<unistd.h>
//shader code heavily modified from www.opengl-tutorial.org

using namespace std;
void loadShaders(GLuint programID){
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	string VertexShaderCode;
VertexShaderCode=R"(
											//fragmentColor=gl_Position*0.5+0.5;
											attribute vec4 vertexBuf;
											uniform mat4 viewPort;
											uniform mat4 cameraRot;
											attribute vec4 colorBuffer;
											varying vec4 fragmentColor;
											void main(){
												gl_Position=viewPort*cameraRot*vertexBuf;
												//gl_Position=cameraRot*vertexBuf;
												fragmentColor=colorBuffer;
											}
)";

	string FragmentShaderCode;
/*
char cwd[256];
getcwd(cwd, sizeof(cwd));
printf("Current working dir: %s\n", cwd);
DIR *dirp = opendir(".");
dirent *dp;
while ((dp = readdir(dirp)) != NULL)
printf("%s\n",dp->d_name);
closedir(dirp);
*/
FragmentShaderCode=R"(
											#ifdef GL_ES
											precision mediump float;
											#endif
											varying vec4 fragmentColor;
											void main(){
												//gl_FragColor=vec4(0,0,1,1);
												gl_FragColor=fragmentColor;
											}
)";

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling vertex shader\n");
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling fragment shader\n");
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}
