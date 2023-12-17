#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <meshes.h>
#include <camera.h>
using namespace std; // Standard namespace

//custom colors
#define LightGray  (float) 138 / 255, (float) 147 / 255, (float) 150 / 255
#define MacOsColor  (float) 155 / 255, (float) 66 / 255, (float) 245 / 255
#define LightBulbColor  (float) 252 / 255, (float) 243 / 255, (float) 169 / 255
#define LightBulbObjColor  (float) 252 / 255*10.0f, (float) 243 / 255*10.0f, (float) 169 / 255*10.0f

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
	const char* const WINDOW_TITLE = "Making a mesh out of things"; // Macro for window title

	//Globals
	Camera cam;
	bool firstMouse = true;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float cam_x = -91.0f;
	float cam_y = -7.0f;
	float cam_z = -1.0f;
	float cam_speed = 0.01f;
	float rotateCamX = 0.0f;
	float rotateCamY = 0.0f;
	float sensitivity = 0.05f;
	bool ortho = false;

	// Variables for window width and height
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// Stores the GL data relative to a given mesh
	struct GLMesh
	{
		GLuint vao;         // Handle for the vertex array object
		GLuint vbos[2];     // Handles for the vertex buffer objects
		GLuint nIndices;    // Number of indices of the mesh
	};

	// Main GLFW window
	GLFWwindow* gWindow = nullptr;
	// Triangle mesh data
	//GLMesh gMesh;
	// Shader program
	GLuint gProgramId;

	//Shape Meshes from Professor Brian
	Meshes meshes;
}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char*[], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window, int button, int action, int mods);
void mouseScrollInput(GLFWwindow* window, double xoffset, double yoffset);
void mouse_click(GLFWwindow* window, int button, int action, int mods);
void cursorPos(GLFWwindow* window, double xPos, double yPos);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint &programId);
void UDestroyShaderProgram(GLuint programId);
void loadImg(const char* file, int memoryLoc);
////////////////////////////////////////////////////////////////////////////////////////
// SHADER CODE
/* Vertex Shader Source Code*/
const GLchar * vertexShaderSource = GLSL(440,
	layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 1) in vec3 color;  // Color data from Vertex Attrib Pointer 1
layout(location = 2) in vec2 texCoord; // texture data from vertex Attrib pointer 2
out vec4 vertexColor; // variable to transfer color data to the fragment shader
out vec2 texCoords;
out vec3 normals;
out vec3 curPos;
//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	curPos = vec3(model * vec4(position, 1.0f));
	normals = mat3(transpose(inverse(model))) * color;
	gl_Position = projection * view * vec4(curPos, 1.0f); // transforms vertices to clip coordinates
	vertexColor = vec4(color,1.0f); // references incoming color data
	texCoords = texCoord;
}
);


/* Fragment Shader Source Code*/
const GLchar * fragmentShaderSource = GLSL(440,
	in vec4 vertexColor; // Variable to hold incoming color data from vertex shader
in vec2 texCoords;
in vec3 curPos;
in vec3 normals;
out vec4 fragmentColor;
out  vec4 fragmentTexture;
//out  vec4 lightBulb;

uniform vec4 objectColor;
uniform vec3 lightBulbPos;
uniform vec3 lightScreenPos;
uniform sampler2D myTexture;
uniform vec4 lightBulbColor;
uniform vec4 lightScreenColor;
uniform vec3 viewDirection;
void main()
{
	//fragmentColor = vec4(vertexColor);
	//fragmentColor = vec4(objectColor);
	vec3 normal = normalize(normals);
	vec3 lightBulbDir = normalize(lightBulbPos - curPos);
	vec3 lightScreenDir = normalize(lightScreenPos - curPos);
	float ambientBrightness = 0.1f;
	vec4 ambientLighting = (ambientBrightness * lightBulbColor)+(ambientBrightness * lightScreenColor);
	float difForBulb = max(dot(normal, lightBulbDir), 0.0f);
	float difForScreen = max(dot(normal, lightScreenDir), 0.0f);
	vec4 diffuse = (difForBulb*lightScreenColor)+(difForScreen * lightBulbColor);
	float specularLighting = 2.0f;
	vec3 viewDir = normalize(viewDirection - curPos);
	vec3 reflectDirLightBulb = reflect(-lightBulbDir, normal);
	vec3 reflectDirScreen = reflect(-lightScreenDir, normal);
	float specLightBulb = pow(max(dot(viewDir, reflectDirLightBulb), 0.0f), 32);
	float specScreen = pow(max(dot(viewDir, reflectDirScreen), 0.0f), 32);
	vec4 specular = (specularLighting * specLightBulb * lightBulbColor);

	fragmentTexture = texture(myTexture, texCoords) * objectColor * (ambientLighting+diffuse+specular);
	
}
);
///////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;

	// Create the mesh
	//UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object
	meshes.CreateMeshes();

	//load textures
	loadImg("old-concrete-texture-with-blue-paint.JPG", 0);
	loadImg("pen_holder.JPG", 1);
	loadImg("gray-smooth-textured-background.JPG", 2);
	loadImg("gray-lined-paper-texture.JPG", 3);
	loadImg("wooden-flooring-textured-background-design.JPG", 4);
	loadImg("buttons.JPG", 5);
	loadImg("wave1.jpg", 6);
	loadImg("back_of_mac.JPG", 7);
	loadImg("mac_os.JPG", 8);
	loadImg("multi-colored-psychedelic-background.JPG", 9);
	// Create the shader program
	if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
		return EXIT_FAILURE;

	// Sets the background color of the window to black (it will be implicitely used by glClear)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(gWindow))
	{
		// input
		// -----
		UProcessInput(gWindow);

		glfwPollEvents();
	}

	// Release mesh data
	//UDestroyMesh(gMesh);
	meshes.DestroyMeshes();

	// Release shader program
	UDestroyShaderProgram(gProgramId);

	exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
	// GLFW: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// GLFW: window creation
	// ---------------------
	*window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);
	
	// GLEW: initialize
	// ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	// Displays GPU OpenGL version
	cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;
	
	return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{   
	//init URender with globals
	URender();
	//void mouseInput(GLFWwindow * window, int button, int action, int mods);
	glfwSetScrollCallback(window, mouseScrollInput);
	glfwSetMouseButtonCallback(window, mouse_click);
	glfwSetCursorPosCallback(window, cursorPos);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_A)) cam.ProcessKeyboard(RIGHT, cam_speed);
	if (glfwGetKey(window, GLFW_KEY_D)) cam.ProcessKeyboard(LEFT, cam_speed);
	if (glfwGetKey(window, GLFW_KEY_W)) cam.ProcessKeyboard(FORWARD, cam_speed);
	if (glfwGetKey(window, GLFW_KEY_S)) cam.ProcessKeyboard(BACKWARD, cam_speed);
	if (glfwGetKey(window, GLFW_KEY_Q)) cam.ProcessKeyboard(UP, cam_speed);
	if (glfwGetKey(window, GLFW_KEY_E)) cam.ProcessKeyboard(DOWN, cam_speed);
	if (glfwGetKey(window, GLFW_KEY_P)) ortho = true;
	if (glfwGetKey(window, GLFW_KEY_O)) ortho = false;
}
void mouseInput(GLFWwindow* window, int button, int action, int mods) {
	std::cout << button << " button..." << std::endl;
	std::cout << action << " action..." << std::endl;
	std::cout << mods << " mods..." << std::endl;
}
void mouseScrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0) cam_speed += 0.01;
	if (yoffset < 0) cam_speed -= 0.01;
	if (cam_speed < 0.01) cam_speed = 0.01f;
	std::cout << "speed of camera: " << cam_speed << std::endl;
}

void cursorPos(GLFWwindow* window, double xPosIn, double yPosIn) {
	float xpos = static_cast<float>(xPosIn);
	float ypos = static_cast<float>(yPosIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	cam.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_click(GLFWwindow* window, int button, int action, int mods) 
{	
	switch (button)
	{
	case 0:
		std::cout << "left click" << std::endl;
		break;
	case 1:
		std::cout << "right click" << std::endl;
		break;
	case 2:
		std::cout << "scroll-wheel click" << std::endl;
		break;
	default:
		std::cout << "button: " << button << "pressed" << std::endl;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// Functioned called to render a frame
void URender()
{
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::mat4 translation;
	glm::mat4 model;
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 rotateX = glm::mat4(1.0f);
	glm::mat4 rotateY = glm::mat4(1.0f);
	glm::vec3 lightBulbPos = glm::vec3(-10.0f, 20.0f, 0.0f);
	glm::vec3 lightScreenPos = glm::vec3(-12.0f, 15.0f, 20.0f);
	glm::vec3 viewDir = glm::vec3(cam.Position.x, cam.Position.y, cam.Position.z);
	GLint modelLoc;
	GLint viewLoc;
	GLint projLoc;
	GLint objectColorLoc;
	GLint lightBulbLoc;
	GLint lightScreenLoc;
	GLint objectTexLoc;
	GLint viewDirLoc;

	// Enable z-depth
	glEnable(GL_DEPTH_TEST);

	// Clear the frame and z buffers

	glClearColor(0,0,0, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transforms the camera
	view = cam.GetViewMatrix();
	
	// Creates a orthographic projection
	ortho ? projection = glm::ortho(-40.0f, (float)WINDOW_WIDTH/10, -20.0f,(float)WINDOW_HEIGHT/10,0.1f, 100.0f) :
		projection = glm::perspective(glm::radians(45.0f), (GLfloat) WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

	// Set the shader to be used
	glUseProgram(gProgramId);

	// Retrieves and passes transform matrices to the Shader program
	modelLoc = glGetUniformLocation(gProgramId, "model");
	viewLoc = glGetUniformLocation(gProgramId, "view");
	projLoc = glGetUniformLocation(gProgramId, "projection");
	objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	lightBulbLoc = glGetUniformLocation(gProgramId, "lightBulbColor");
	lightScreenLoc = glGetUniformLocation(gProgramId, "lightScreenColor");
	viewDirLoc = glGetUniformLocation(gProgramId, "viewDirection");
	glUniform3f(glGetUniformLocation(gProgramId, "viewDir"), viewDir.x,viewDir.y, viewDir.z);
	glUniform3f(glGetUniformLocation(gProgramId, "lightBulbPos"), lightBulbPos.x, lightBulbPos.y, lightBulbPos.z);
	glProgramUniform4f(gProgramId, lightBulbLoc, LightBulbColor, 1.0f);
	glUniform3f(glGetUniformLocation(gProgramId, "lightScreenPos"), lightScreenPos.x, lightScreenPos.y, lightScreenPos.z);
	glProgramUniform4f(gProgramId, lightScreenLoc, MacOsColor,1.0f);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	///////////////////////////////////////////////////////////////////////////////
	// desk rendering														    //	
	/////////////////////////////////////////////////////////////////////////////

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gPlaneMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(20.0f, 10.0f, 10.0f));
	// 2. Rotate the object
	rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(0.0f, -10.0f, -20.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	
	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 0);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gPlaneMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gPlaneMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
	// 2. Rotate the object
	rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(30.0f, -10.0f, -20.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	
	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 4);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
	
	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gPlaneMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);
	
	///////////////////////////////////////////////////////////////////////////////
	// pen holder and pen rendering											    //	
	/////////////////////////////////////////////////////////////////////////////

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(0.6f, 8.0f, 0.6f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(15.0f, -1.0f, -28.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 5);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(0.6f, 8.0f, 0.6f));
	// 2. Rotate the object
	rotation = glm::rotate(-0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(16.0f, -1.0f, -27.5f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 5);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(0.6f, 8.0f, 0.6f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(13.5f, -1.0f, -28.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 5);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gCylinderMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(3.0f, 9.0f, 2.0f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(15.0f, -10.0f, -28.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 1);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// Activate the VBOs contained within the mesh's VAO
	/*glBindVertexArray(meshes.gTaperedCylinderMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(-1.2f, 1.0f, 5.5f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glProgramUniform4f(gProgramId, objectColorLoc, 0.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);
	*/
	// Activate the VBOs contained within the mesh's VAO

	///////////////////////////////////////////////////////////////////////////////
	// sphere redering														    //	
	/////////////////////////////////////////////////////////////////////////////

	glBindVertexArray(meshes.gTorusMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(2.3f, 2.3f, 5.0f));
	// 2. Rotate the object
	rotation = glm::rotate(glm::radians(87.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(20.0f, -10.0f, -17.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 6);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.gTorusMesh.nVertices);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);
	
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gSphereMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(3.6f, 4.6f, 3.6f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(20.0f, -6.0f, -17.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//loadImg("reflective_chrome_low_res.JPG","myTexture", 3);
	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 9);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gSphereMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	///////////////////////////////////////////////////////////////////////////////
	// computer redering														//	
	/////////////////////////////////////////////////////////////////////////////
	
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(20.0f, 1.0f, 13.0f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(0.0f, -9.5f, -18.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	
	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 2);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(20.0f, 13.0f, 0.01f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(0.0f, -3.0f, -24.7f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 8);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(20.0f, 13.0f, 0.99f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(0.0f, -3.0f, -25.3f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	
	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 7);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// render trackpad
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(6.0f, 0.01f, 5.0f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(0.0f, -9.0f, -14.5f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	
	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 3);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	//render keyboard buttons 
	for (float key = 0; key < 72.0f; key+=1.5f) {
		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(meshes.gBoxMesh.vao);

		// 1. Scales the object
		scale = glm::scale(glm::vec3(1.3f, 0.2f, 1.3f));
		// 2. Rotate the object
		rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
		// 3. Position the object
		
		if (key < 17) translation = glm::translate(glm::vec3(key - 8.3f, -8.8f, -22.0f));
		 
		if (key > 17) translation = glm::translate(glm::vec3((key - 18.3) - 8.0f, -8.8f, -20.0f));
		
		if (key > 35) translation = glm::translate(glm::vec3((key - 36.3) - 8.0f, -8.8f, -18.0f));
		
		if (key > 53)
		{
			scale = glm::scale(glm::vec3(1.3f, 0.2f, 0.5f));
			translation = glm::translate(glm::vec3((key - 54.0) - 8.3f, -8.8f, -23.5f));
		}
		// Model matrix: transformations are applied right-to-left order
		model = translation * rotation * scale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//lightSourceLoc = glGetUniformLocation(gProgramId, "lightSourceColor");
		glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 5);
		glProgramUniform4f(gProgramId, objectColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);

		// Draws the triangles
		glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

		// Deactivate the Vertex Array Object
		glBindVertexArray(0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
    // render light source                                                                //
    ///////////////////////////////////////////////////////////////////////////////////////
	
	//LightBulb
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gSphereMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(3.0f, 3.0f, 3.0f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(25.0f, 10.0f, -21.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 3);
	glProgramUniform4f(gProgramId, objectColorLoc, LightBulbObjColor, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gSphereMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);
    
	//LightBulb holder
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gConeMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(9.0f, 6.0f, 6.0f));
	// 2. Rotate the object
	rotation = glm::rotate(glm::radians(-30.0f), glm::vec3(0.0, 0.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(26.0f, 10.0f, -21.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 3);
	glProgramUniform4f(gProgramId, objectColorLoc, 0,0,0,0);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	//light stand to light holder
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(1.0f, 7.0f, 0.6f));
	// 2. Rotate the object
	rotation = glm::rotate(glm::radians(40.0f), glm::vec3(0.0, 0.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(31.0f, 12.0f, -20.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 5);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	//light stand to base
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(1.0f, 20.0f, 0.6f));
	// 2. Rotate the object
	rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(33.0f, 1.0f, -20.0f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 5);
	glProgramUniform4f(gProgramId, objectColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	// Draws the triangles
	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	//light base
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gConeMesh.vao);

	// 1. Scales the object
	scale = glm::scale(glm::vec3(6.0f, 3.0f, 6.0f));
	// 2. Rotate the object
	rotation = glm::rotate(glm::radians(-0.0f), glm::vec3(0.0, 0.0f, 1.0f));
	// 3. Position the object
	translation = glm::translate(glm::vec3(33.0f, -9.0f, -20.5f));
	// Model matrix: transformations are applied right-to-left order
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	glUniform1i(glGetUniformLocation(gProgramId, "myTexture"), 3);
	glProgramUniform4f(gProgramId, objectColorLoc, 0, 0, 0, 0);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];

	// Create a Shader program object.
	programId = glCreateProgram();

	// Create the vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Retrive the shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// Compile the vertex shader, and print compilation errors (if any)
	glCompileShader(vertexShaderId); // compile the vertex shader
	// check for shader compile errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glCompileShader(fragmentShaderId); // compile the fragment shader
	// check for shader compile errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	// Attached compiled shaders to the shader program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);   // links the shader program
	// check for linking errors
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(programId);    // Uses the shader program

	return true;
}

void UDestroyShaderProgram(GLuint programId)
{
	glDeleteProgram(programId);
}

void loadImg(const char* file, int memoryLoc)
{
	unsigned int textures;
	//create our image texture
	glGenTextures(1, &textures);
	
	// OpenGL has 32(at least in this version) available textures starting from 33984-34015
	GLenum activeTexture = 33984+memoryLoc;
	std::cout << "GL_TEXTURE" << activeTexture - 33984 << " = " << activeTexture << std::endl;
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, textures);

	//texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data =
		stbi_load
		(
			file,
			&width,
			&height,
			&nrChannels,
			0
		);
	if (data)
	{
		std::cout << "loaded image..." << std::endl;
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0, GL_RGB, width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture failed to load..." << std::endl;
	}
	stbi_set_flip_vertically_on_load(1);
	stbi_image_free(data);
	
}
