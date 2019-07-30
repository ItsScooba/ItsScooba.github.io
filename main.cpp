#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "soil.h"


//Macro for setting the window title.
#define WINDOW_TITLE "Modern OpenGL"

//Vertex and fragment shader source macro.
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif


//Variables for window size.
GLint shaderProgram, windowWidth = 800, windowHeight = 600;
GLuint VBO, VAO, EBO, texture;

//Total number of elements to draw.
int totalElements = -1;

//Function prototypes, defined after main, used to manage window.
void UResizeWindow(int, int);
void URenderGraphics();
void UCreateBuffers();
void UCreateShaders();
void UGenerateTexture();

//Variables used for tracking the rotation of the object being drawn on the screen.
float modelRotationX = 0.0f, modelRotationY = 0.0f, modelRotationZ = 0.0f;

//Vertex shader source code.
const GLchar * VertexShader = GLSL(330, 
	in layout(location = 0) vec3 vertex_position;
	in layout(location = 1) vec2 textureCoordinate;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	out vec2 mobileTextureCoordinate;

		void main() {
			gl_Position = projection * view * model * vec4(vertex_position, 1.0);
			mobileTextureCoordinate = textureCoordinate;
		}
);


//Fragment shader source code.
const GLchar * FragmentShader = GLSL(330,

	in vec2 mobileTextureCoordinate;

	out vec4 vertex_color;

	uniform sampler2D uTexture;

	void main() {
		vertex_color = texture(uTexture, mobileTextureCoordinate);
	}
);


//Main function.
int main(int argc, char **argv) {
	
	//Initialize OpenGL and it's helper libraries, and create a window for the application.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow(WINDOW_TITLE);

	//Pass glut the function address used to handle resizing the window.
	glutReshapeFunc(UResizeWindow);

	//Make sure GLEW initializes properly.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW." << std::endl;
		return -1;
	}

	//Initialize what is needed to draw on the screen using OpenGL, such as shader programs, buffer objects, and load textures.
	UCreateShaders();
	UCreateBuffers();
	UGenerateTexture();

	//Tell OpenGL which shader program to use to draw.
	glUseProgram(shaderProgram);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set the callback function used to draw graphics, and begin the main loop.
	glutDisplayFunc(URenderGraphics);
	glutMainLoop();
	

	//Cleanup and freeing memory used during the program.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}




//Create a Vertex Buffer Object to draw with.
void UCreateBuffers() {

	//Allocate memory for and generate the buffer objects used to send vertex data to the graphics shader.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Generate vertices and indices for a 3D triangle.
	GLfloat vertices[] = {
		0.5f, 0.0f, 0.5f,		0.0f, 0.0f, //BR 0
		-0.5f, 0.0f, 0.5f,		1.0f, 0.0f, //BL 1
		0.5f, 0.0f, -0.5f,		1.0f, 0.0f, //BR BACK 2
		-0.5f, 0.0f, -0.5f,		0.0f, 0.0f, //BL BACK 3

		0.0f, 1.0f, 0.0f,		0.5f, 1.0f, //CENTER TIP 4
	};

	//Index buffer data used to minimize vertex data sent to the shader.
	GLuint indexBufferData[] = {

		0, 1, 2, 
		1, 2, 3,
		0, 1, 4,
		0, 2, 4,
		2, 3, 4,
		1, 3, 4

	};

	//Calculate the total amount of indices that need to be drawn.
	totalElements = ( sizeof(indexBufferData) / sizeof(GLuint));

	//Bind the buffers generated above, and populate them with the vertex data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferData), indexBufferData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}




//Generate and load a texture object from an image file.
void UGenerateTexture()
{
	//Allocate memory for and generate the texture object used for loading and drawing textures.
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width = -1, height = -1;

	//Load the texture data from the hard drive.
	unsigned char* image = SOIL_load_image("snhu.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	//Set the textures parameters and free up the image data now that the texture object is generated.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}




//Create the shader programs used to draw on the screen.
void UCreateShaders() {

	//Generate new shader programs for the individual vertex and fragment shaders. Compiles and links the shader source code from above.
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShaderID, 1, &VertexShader, NULL);
	glCompileShader(vertexShaderID);
	
	glShaderSource(fragmentShaderID, 1, &FragmentShader, NULL);
	glCompileShader(fragmentShaderID);

	shaderProgram = glCreateProgram();


	//Attach the now compiled shaders to the shader program.
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);

	//Link the individual shaders to the main shader program, which is then used to draw by OpenGL.
	glLinkProgram(shaderProgram);

	//Clean up memory and delete the individual shader programs, as they are no longer needed once linked.
	glDeleteProgram(vertexShaderID);
	glDeleteProgram(fragmentShaderID);
}




//Implements UResizeWindow to resize the application window.
void UResizeWindow(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}




//Callback function passed to OpenGL to tell it how to handle rendering the vertices.
void URenderGraphics() {
	
	glEnable(GL_DEPTH_TEST);

	//Clear the buffer than display it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);

	//Set the model's position, rotation, and scale.
	modelRotationX += 0.005f;
	modelRotationY += 0.003f;
	modelRotationZ += 0.004f;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, modelRotationX, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, modelRotationY, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, modelRotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));


	//Set the view matrix, which represents the camera's state.
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3( 0.0f, 0.0f, -5.0f ));

	//Set the projection matrix, which is used to specify positions on the application window.
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);


	//Send the required data to the shaders used for positions, rotations, and scale.
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();

	//Draw the triangle using the VBO.
	glDrawElements(GL_TRIANGLES, totalElements, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);

	//Flip the buffers so that the one just drawn on is displayed to the screen.
	glutSwapBuffers();
}