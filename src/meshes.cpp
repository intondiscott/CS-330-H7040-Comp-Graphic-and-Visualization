///////////////////////////////////////////////////////////////////////////////
// meshes.cpp
// ========
// create meshes for various 3D primitives: plane, pyramid, cube, cylinder, torus, sphere
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 7th, 2022
///////////////////////////////////////////////////////////////////////////////

#include "meshes.h"

#include <vector>

namespace
{
	const double M_PI = 3.14159265358979323846f;
	const double M_PI_2 = 1.571428571428571;
}

///////////////////////////////////////////////////
//	CreateMeshes()
//
//	Create all the following 3D meshes:
//		plane, pyramid, cube, cylinder, torus, sphere
///////////////////////////////////////////////////
void Meshes::CreateMeshes()
{
	UCreatePlaneMesh(gPlaneMesh);
	UCreatePrismMesh(gPrismMesh);
	UCreateBoxMesh(gBoxMesh);
	UCreateConeMesh(gConeMesh);
	UCreateCylinderMesh(gCylinderMesh);
	UCreateTaperedCylinderMesh(gTaperedCylinderMesh);
	UCreatePyramid3Mesh(gPyramid3Mesh);
	UCreatePyramid4Mesh(gPyramid4Mesh);
	UCreateSphereMesh(gSphereMesh);
	UCreateTorusMesh(gTorusMesh);
}

///////////////////////////////////////////////////
//	DestroyMeshes()
//
//	Destroy the created meshes
///////////////////////////////////////////////////
void Meshes::DestroyMeshes()
{
	UDestroyMesh(gBoxMesh);
	UDestroyMesh(gConeMesh);
	UDestroyMesh(gCylinderMesh);
	UDestroyMesh(gPlaneMesh);
	UDestroyMesh(gPyramid3Mesh);
	UDestroyMesh(gPyramid4Mesh);
	UDestroyMesh(gPrismMesh);
	UDestroyMesh(gSphereMesh);
	UDestroyMesh(gTorusMesh);
}

///////////////////////////////////////////////////
//	UCreatePlaneMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a plane mesh and store it in a VAO/VBO
// 
//  Correct triangle drawing command:
//
//	glDrawElements(GL_TRIANGLES, meshes.gPlaneMesh.nIndices, GL_UNSIGNED_INT, (void*)0);
///////////////////////////////////////////////////
void Meshes::UCreatePlaneMesh(GLMesh &mesh)
{
	// Vertex data
	GLfloat verts[] = {
		// Vertex Positions		// Normals			// Texture coords	// Index
		-1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			//0
		1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			//1
		1.0f,  0.0f, -1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			//2
		-1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,			//3
	};

	// Index data
	GLuint indices[] = {
		0,1,2,
		0,3,2
	};

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	// Generate the VAO for the mesh
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);	// activate the VAO

	// Create VBOs for the mesh
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends data to the GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]); // Activates the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreatePyramid3Mesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a pyramid mesh and store it in a VAO/VBO
//
//  Correct triangle drawing command:
//
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gPyramid3Mesh.nVertices);
///////////////////////////////////////////////////
void Meshes::UCreatePyramid3Mesh(GLMesh &mesh)
{
	// Vertex data
	GLfloat verts[] = {
		// Vertex Positions		// Normals			// Texture coords
		//left side
		0.0f, 0.5f, 0.0f,		-0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		0.0f, -0.5f, -0.5f,		-0.894427180f, 0.0f, -0.447213590f,	0.0f, 0.0f,		//back center
		-0.5f, -0.5f, 0.5f,		-0.894427180f, 0.0f, -0.447213590f,	1.0f, 0.0f,     //front bottom left
		0.0f, 0.5f, 0.0f,		-0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		//right side
		0.0f, 0.5f, 0.0f,		0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		0.5f, -0.5f, 0.5f,		0.894427180f, 0.0f, -0.447213590f,	0.0f, 0.0f,     //front bottom right
		0.0f, -0.5f, -0.5f,		0.894427180f, 0.0f, -0.447213590f,	1.0f, 0.0f,		//back center	
		0.0f, 0.5f, 0.0f,		0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		//front side
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f,		//top point			
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,     //front bottom left	
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,     //front bottom right
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f,		//top point	
		//bottom side
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,     //front bottom right
		0.0f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,		//back center	
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
	};

	const GLuint floatsPerVertex = 3;	// Number of coordinates per vertex
	const GLuint floatsPerColor = 3;	// Number of values per vertex color
	const GLuint floatsPerUV = 2;		// Number of texture coordinate values

	// Calculate total defined vertices
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerUV));

	glGenVertexArrays(1, &mesh.vao);			// Creates 1 VAO
	glGenBuffers(1, mesh.vbos);					// Creates 1 VBO
	glBindVertexArray(mesh.vao);				// Activates the VAO
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);	// Activates the VBO
	// Sends vertex or coordinate data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// Strides between sets of attribute data
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerUV);

	// Creates the Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* (floatsPerVertex + floatsPerColor)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreatePyramid4Mesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a pyramid mesh and store it in a VAO/VBO
//
//  Correct triangle drawing command:
//
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gPyramid4Mesh.nVertices);
///////////////////////////////////////////////////
void Meshes::UCreatePyramid4Mesh(GLMesh &mesh)
{
	// Vertex data
	GLfloat verts[] = {
		// Vertex Positions		// Normals			// Texture coords
		//bottom side
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,		//back bottom left
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,		//back bottom right	
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,     //front bottom right
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,		//back bottom right	
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
		//back side
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	0.5f, 1.0f,		//top point	
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f,		//back bottom right	
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f,		//back bottom left
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	0.5f, 1.0f,		//top point	
		//left side
		0.0f, 0.5f, 0.0f,		-1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		//top point	
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		//back bottom left	
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,     //front bottom left
		0.0f, 0.5f, 0.0f,		-1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		//top point	
		//right side
		0.0f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		//top point	
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,     //front bottom right
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		//back bottom right	
		0.0f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		//top point	
		//front side
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f,		//top point			
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,     //front bottom left	
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,     //front bottom right
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f,		//top point
	};

	const GLuint floatsPerVertex = 3;	// Number of coordinates per vertex
	const GLuint floatsPerColor = 3;	// Number of values per vertex color
	const GLuint floatsPerUV = 2;		// Number of texture coordinate values

	// Calculate total defined vertices
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerUV));

	glGenVertexArrays(1, &mesh.vao);			// Creates 1 VAO
	glGenBuffers(1, mesh.vbos);					// Creates 1 VBO
	glBindVertexArray(mesh.vao);				// Activates the VAO
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);	// Activates the VBO
	// Sends vertex or coordinate data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// Strides between sets of attribute data
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerUV);

	// Creates the Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* (floatsPerVertex + floatsPerColor)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreatePrismMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a pyramid mesh and store it in a VAO/VBO
//
//	Correct triangle drawing command:
//
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gPrismMesh.nVertices);
///////////////////////////////////////////////////
void Meshes::UCreatePrismMesh(GLMesh &mesh)
{
	// Vertex data
	GLfloat verts[] = {
		//Positions				//Normals				//Texture Coords
		// ------------------------------------------------------

		//Back Face				//Negative Z Normal  
		0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,

		//Bottom Face			//Negative Y Normal
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.0f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		0.5f, 1.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,

		//Left Face/slanted		//Normals
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		-0.5f, 0.5f,  -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 1.0f,
		0.0f, 0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		0.0f, -0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 0.0f,
		0.0f, 0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,

		//Right Face/slanted	//Normals
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.0f, -0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,

		//Top Face				//Positive Y Normal		//Texture Coords.
		0.5f, 0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		0.0f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		0.5f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,

	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(1, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreateBoxMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a cube mesh and store it in a VAO/VBO
//
//	Correct triangle drawing command:
//
//	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);
///////////////////////////////////////////////////
void Meshes::UCreateBoxMesh(GLMesh &mesh)
{
	// Position and Color data
	GLfloat verts[] = {
	//Positions				//Normals
	// ------------------------------------------------------

	//Back Face				//Negative Z Normal  Texture Coords.
	0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,  0.0f, 1.0f,   //0
	0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,  0.0f, 0.0f,   //1
	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,  1.0f, 0.0f,   //2
	-0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,  1.0f, 1.0f,   //3

	//Bottom Face			//Negative Y Normal
	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  //4
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,  0.0f, 0.0f,  //5
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  //6
	0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,  1.0f, 1.0f, //7

	//Left Face				//Negative X Normal
	-0.5f, 0.5f, -0.5f,		1.0f,  0.0f,  0.0f,  0.0f, 1.0f,      //8
	-0.5f, -0.5f,  -0.5f,	1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  //9
	-0.5f,  -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //10
	-0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  //11

	//Right Face			//Positive X Normal
	0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  //12
	0.5f,  -0.5f, 0.5f,		1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  //13
	0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //14
	0.5f, 0.5f, -0.5f,		1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  //15

	//Top Face				//Positive Y Normal
	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //16
	-0.5f,  0.5f, 0.5f,		0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //17
	0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //18
	0.5f,  0.5f,  -0.5f,	0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //19
	
	//Front Face			//Positive Z Normal
	-0.5f, 0.5f,  0.5f,	    0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //20
	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //21
	0.5f,  -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //22
	0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //23
	};

	// Index data
	GLuint indices[] = {
		0,1,2,
		0,3,2,
		4,5,6,
		4,7,6,
		8,9,10,
		8,11,10,
		12,13,14,
		12,15,14,
		16,17,18,
		16,19,18,
		20,21,22,
		20,23,22
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]); // Activates the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreateConeMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a cone mesh and store it in a VAO/VBO
//
//  Correct triangle drawing commands:
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
//	glDrawArrays(GL_TRIANGLE_STRIP, 36, 108);	//sides
///////////////////////////////////////////////////
void Meshes::UCreateConeMesh(GLMesh &mesh)
{
	GLfloat verts[] = {
		// cone bottom			// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cone sides		// normals									// texture coords
		1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, -0.116841137f, 		1.0f,0.5f,
		0.0f, 1.0f, 0.0f,		0.993150651f, 0.0f, -0.116841137f, 		0.5f, 0.5f,
		.98f, 0.0f, -0.17f,		0.993150651f, 0.0f, -0.116841137f, 		0.983f,0.6f,
		.98f, 0.0f, -0.17f,		0.973417103f, 0.0f, -0.229039446f, 		0.983f,0.6f,
		0.0f, 1.0f, 0.0f,		0.973417103f, 0.0f, -0.229039446f, 		0.5f, 0.5f,
		.94f, 0.0f, -0.34f,		0.973417103f, 0.0f, -0.229039446f, 		0.96f,0.68f,
		.94f, 0.0f, -0.34f,		0.916157305f, 0.0f, -0.400818795f, 		0.96f,0.68f,
		0.0f, 1.0f, 0.0f,		0.916157305f, 0.0f, -0.400818795f, 		0.5f, 0.5f,
		.87f, 0.0f, -0.5f,		0.916157305f, 0.0f, -0.400818795f, 		0.92f,0.77f,
		.87f, 0.0f, -0.5f,		0.813733339f, 0.0f, -0.581238329f, 		0.92f,0.77f,
		0.0f, 1.0f, 0.0f,		0.813733339f, 0.0f, -0.581238329f, 		0.5f, 0.5f,
		.77f, 0.0f, -0.64f,		0.813733339f, 0.0f, -0.581238329f, 		0.87f, 0.83f,
		.77f, 0.0f, -0.64f,		0.707106769f, 0.0f, -0.707106769f, 		0.87f, 0.83f,
		0.0f, 1.0f, 0.0f,		0.707106769f, 0.0f, -0.707106769f, 		0.5f, 0.5f,
		.64f, 0.0f, -0.77f,		0.707106769f, 0.0f, -0.707106769f, 		0.83f, 0.87f,
		.64f, 0.0f, -0.77f,		0.581238329f, 0.0f, -0.813733339f, 		0.83f, 0.87f,
		0.0f, 1.0f, 0.0f,		0.581238329f, 0.0f, -0.813733339f, 		0.5f, 0.5f,
		.5f, 0.0f, -0.87f,		0.581238329f, 0.0f, -0.813733339f, 		0.77f, 0.92f,
		.5f, 0.0f, -0.87f,		0.400818795f, 0.0f, -0.916157305f, 		0.77f, 0.92f,
		0.0f, 1.0f, 0.0f,		0.400818795f, 0.0f, -0.916157305f, 		0.5f, 0.5f,
		.34f, 0.0f, -0.94f,		0.400818795f, 0.0f, -0.916157305f, 		0.68f, 0.96f,
		.34f, 0.0f, -0.94f,		0.229039446f, 0.0f, -0.973417103f, 		0.68f, 0.96f,
		0.0f, 1.0f, 0.0f,		0.229039446f, 0.0f, -0.973417103f, 		0.5f, 0.5f,
		.17f, 0.0f, -0.98f,		0.229039446f, 0.0f, -0.973417103f, 		0.6f, 0.983f,
		.17f, 0.0f, -0.98f,		0.116841137f, 0.0f, -0.993150651f, 		0.6f, 0.983f,
		0.0f, 1.0f, 0.0f,		0.116841137f, 0.0f, -0.993150651f, 		0.5f, 0.5f,
		0.0f, 0.0f, -1.0f,		0.116841137f, 0.0f, -0.993150651f, 		0.5f, 1.0f,

		0.0f, 0.0f, -1.0f,		-0.116841137f, 0.0f, -0.993150651f, 		0.5f, 1.0f,
		0.0f, 1.0f, 0.0f,		-0.116841137f, 0.0f, -0.993150651f, 		0.5f, 0.5f,
		-.17f, 0.0f, -0.98f,	-0.116841137f, 0.0f, -0.993150651f, 		0.41f, 0.983f,
		-.17f, 0.0f, -0.98f,	-0.229039446f, 0.0f, -0.973417103f, 		0.41f, 0.983f,
		0.0f, 1.0f, 0.0f,		-0.229039446f, 0.0f, -0.973417103f, 		0.5f, 0.5f,
		-.34f, 0.0f, -0.94f,	-0.229039446f, 0.0f, -0.973417103f, 		0.33f, 0.96f,
		-.34f, 0.0f, -0.94f,	-0.400818795f, 0.0f, -0.916157305f, 		0.33f, 0.96f,
		0.0f, 1.0f, 0.0f,		-0.400818795f, 0.0f, -0.916157305f, 		0.5f, 0.5f,
		-.5f, 0.0f, -0.87f,		-0.400818795f, 0.0f, -0.916157305f, 		0.25f, 0.92f,
		-.5f, 0.0f, -0.87f,		-0.581238329f, 0.0f, -0.813733339f, 		0.25f, 0.92f,
		0.0f, 1.0f, 0.0f,		-0.581238329f, 0.0f, -0.813733339f, 		0.5f, 0.5f,
		-.64f, 0.0f, -0.77f,	-0.581238329f, 0.0f, -0.813733339f, 		0.17f, 0.87f,
		-.64f, 0.0f, -0.77f,	-0.707106769f, 0.0f, -0.707106769f, 		0.17f, 0.87f,
		0.0f, 1.0f, 0.0f,		-0.707106769f, 0.0f, -0.707106769f, 		0.5f, 0.5f,
		-.77f, 0.0f, -0.64f,	-0.707106769f, 0.0f, -0.707106769f, 		0.13f, 0.83f,
		-.77f, 0.0f, -0.64f,	-0.813733339f, 0.0f, -0.581238329f, 		0.13f, 0.83f,
		0.0f, 1.0f, 0.0f,		-0.813733339f, 0.0f, -0.581238329f, 		0.5f, 0.5f,
		-.87f, 0.0f, -0.5f,		-0.813733339f, 0.0f, -0.581238329f, 		0.08f, 0.77f,
		-.87f, 0.0f, -0.5f,		-0.916157305f, 0.0f, -0.400818795f, 		0.08f, 0.77f,
		0.0f, 1.0f, 0.0f,		-0.916157305f, 0.0f, -0.400818795f, 		0.5f, 0.5f,
		-.94f, 0.0f, -0.34f,	-0.916157305f, 0.0f, -0.400818795f, 		0.04f, 0.68f,
		-.94f, 0.0f, -0.34f,	-0.973417103f, 0.0f, -0.229039446f, 		0.04f, 0.68f,
		0.0f, 1.0f, 0.0f,		-0.973417103f, 0.0f, -0.229039446f, 		0.5f, 0.5f,
		-.98f, 0.0f, -0.17f,	-0.973417103f, 0.0f, -0.229039446f, 		0.017f, 0.6f,
		-.98f, 0.0f, -0.17f,	-0.993150651f, 0.0f, -0.116841137f, 		0.017f, 0.6f,
		0.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f, 		0.5f, 0.5f,
		-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f, 		0.0f, 0.5f,
		-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f, 		0.0f, 0.5f,
		0.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, 0.116841137f, 		0.5f, 0.5f,
		-.98f, 0.0f, 0.17f,		-0.993150651f, 0.0f, 0.116841137f, 		0.017f, 0.41f,
		-.98f, 0.0f, 0.17f,		-0.973417103f, 0.0f, 0.229039446f, 		0.017f, 0.41f,
		0.0f, 1.0f, 0.0f,		-0.973417103f, 0.0f, 0.229039446f, 		0.5f, 0.5f,
		-.94f, 0.0f, 0.34f,		-0.973417103f, 0.0f, 0.229039446f, 		0.04f, 0.33f,
		-.94f, 0.0f, 0.34f,		-0.916157305f, 0.0f, 0.400818795f, 		0.04f, 0.33f,
		0.0f, 1.0f, 0.0f,		-0.916157305f, 0.0f, 0.400818795f, 		0.5f, 0.5f,
		-.87f, 0.0f, 0.5f,		-0.916157305f, 0.0f, 0.400818795f, 		0.08f, 0.25f,
		-.87f, 0.0f, 0.5f,		-0.813733339f, 0.0f, 0.581238329f, 		0.08f, 0.25f,
		0.0f, 1.0f, 0.0f,		-0.813733339f, 0.0f, 0.581238329f, 		0.5f, 0.5f,
		-.77f, 0.0f, 0.64f,		-0.813733339f, 0.0f, 0.581238329f, 		0.13f, 0.17f,
		-.77f, 0.0f, 0.64f,		-0.707106769f, 0.0f, 0.707106769f, 		0.13f, 0.17f,
		0.0f, 1.0f, 0.0f,		-0.707106769f, 0.0f, 0.707106769f, 		0.5f, 0.5f,
		-.64f, 0.0f, 0.77f,		-0.707106769f, 0.0f, 0.707106769f, 		0.17f, 0.13f,
		-.64f, 0.0f, 0.77f,		-0.581238329f, 0.0f, 0.813733339f, 		0.17f, 0.13f,
		0.0f, 1.0f, 0.0f,		-0.581238329f, 0.0f, 0.813733339f, 		0.5f, 0.5f,
		-.5f, 0.0f, 0.87f,		-0.581238329f, 0.0f, 0.813733339f, 		0.25f, 0.08f,
		-.5f, 0.0f, 0.87f,		-0.400818795f, 0.0f, 0.916157305f, 		0.25f, 0.08f,
		0.0f, 1.0f, 0.0f,		-0.400818795f, 0.0f, 0.916157305f, 		0.5f, 0.5f,
		-.34f, 0.0f, 0.94f,		-0.400818795f, 0.0f, 0.916157305f, 		0.33f, 0.04f,
		-.34f, 0.0f, 0.94f,		-0.229039446f, 0.0f, 0.973417103f, 		0.33f, 0.04f,
		0.0f, 1.0f, 0.0f,		-0.229039446f, 0.0f, 0.973417103f, 		0.5f, 0.5f,
		-.17f, 0.0f, 0.98f,		-0.229039446f, 0.0f, 0.973417103f, 		0.41f, 0.017f,
		-.17f, 0.0f, 0.98f,		-0.116841137f, 0.0f, 0.993150651f, 		0.41f, 0.017f,
		0.0f, 1.0f, 0.0f,		-0.116841137f, 0.0f, 0.993150651f, 		0.5f, 0.5f,
		0.0f, 0.0f, 1.0f,		-0.116841137f, 0.0f, 0.993150651f, 		0.5f, 0.0f,
		0.0f, 0.0f, 1.0f,		0.116841137f, 0.0f, 0.993150651f, 	0.5f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.116841137f, 0.0f, 0.993150651f, 	0.5f, 0.5f,
		.17f, 0.0f, 0.98f,		0.116841137f, 0.0f, 0.993150651f, 	0.6f, 0.017f,
		.17f, 0.0f, 0.98f,		0.229039446f, 0.0f, 0.973417103f, 	0.6f, 0.017f,
		0.0f, 1.0f, 0.0f,		0.229039446f, 0.0f, 0.973417103f, 	0.5f, 0.5f,
		.34f, 0.0f, 0.94f,		0.229039446f, 0.0f, 0.973417103f, 	0.68f, 0.04f,
		.34f, 0.0f, 0.94f,		0.400818795f, 0.0f, 0.916157305f, 	0.68f, 0.04f,
		0.0f, 1.0f, 0.0f,		0.400818795f, 0.0f, 0.916157305f, 	0.5f, 0.5f,
		.5f, 0.0f, 0.87f,		0.400818795f, 0.0f, 0.916157305f, 	0.77f, 0.08f,
		.5f, 0.0f, 0.87f,		0.581238329f, 0.0f, 0.813733339f, 	0.77f, 0.08f,
		0.0f, 1.0f, 0.0f,		0.581238329f, 0.0f, 0.813733339f, 	0.5f, 0.5f,
		.64f, 0.0f, 0.77f,		0.581238329f, 0.0f, 0.813733339f, 	0.83f, 0.13f,
		.64f, 0.0f, 0.77f,		0.707106769f, 0.0f, 0.707106769f, 	0.83f, 0.13f,
		0.0f, 1.0f, 0.0f,		0.707106769f, 0.0f, 0.707106769f, 	0.5f, 0.5f,
		.77f, 0.0f, 0.64f,		0.707106769f, 0.0f, 0.707106769f, 	0.87f, 0.17f,
		.77f, 0.0f, 0.64f,		0.813733339f, 0.0f, 0.581238329f, 	0.87f, 0.17f,
		0.0f, 1.0f, 0.0f,		0.813733339f, 0.0f, 0.581238329f, 	0.5f, 0.5f,
		.87f, 0.0f, 0.5f,		0.813733339f, 0.0f, 0.581238329f, 	0.92f, 0.25f,
		.87f, 0.0f, 0.5f,		0.916157305f, 0.0f, 0.400818795f, 	0.92f, 0.25f,
		0.0f, 1.0f, 0.0f,		0.916157305f, 0.0f, 0.400818795f, 	0.5f, 0.5f,
		.94f, 0.0f, 0.34f,		0.916157305f, 0.0f, 0.400818795f, 	0.96f, 0.33f,
		.94f, 0.0f, 0.34f,		0.973417103f, 0.0f, 0.229039446f, 	0.96f, 0.33f,
		0.0f, 1.0f, 0.0f,		0.973417103f, 0.0f, 0.229039446f, 	0.5f, 0.5f,
		.98f, 0.0f, 0.17f,		0.973417103f, 0.0f, 0.229039446f, 	0.983f, 0.41f,
		.98f, 0.0f, 0.17f,		0.993150651f, 0.0f, 0.116841137f, 	0.983f, 0.41f,
		0.0f, 1.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f, 	0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f, 	1.0f, 0.5f
	};

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.nIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create VBO
	glGenBuffers(1, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::CalculateTriangleNormal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	glm::vec3 Normal(0, 0, 0);
	float v1x = p1.x - p0.x;
	float v1y = p1.y - p0.y;
	float v1z = p1.z - p0.z;
	float v2x = p2.x - p1.x;
	float v2y = p2.y - p1.y;
	float v2z = p2.z - p1.z;
	Normal.x = v1y * v2z - v1z * v2y;
	Normal.y = v1z * v2x - v1x * v2z;
	Normal.y = v1x * v2y - v1y * v2x;
	float len = (float)sqrt(Normal.x * Normal.x + Normal.y * Normal.y + Normal.z * Normal.z);
	if (len == 0)
	{
		//throw Exception();
	}
	else
	{
		Normal.x /= len;
		Normal.y /= len;
		Normal.z /= len;
	}
	//return Normal;
}

///////////////////////////////////////////////////
//	UCreateCylinderMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a cylinder mesh and store it in a VAO/VBO
//
//  Correct triangle drawing commands:
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
//	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
//	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
///////////////////////////////////////////////////
void Meshes::UCreateCylinderMesh(GLMesh &mesh)
{
	GLfloat verts[] = {
		// cylinder bottom		// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder top			// normals			// texture coords
		1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f,1.0f,
		.98f, 1.0f, -0.17f,		0.0f, 1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 1.0f, -0.34f,		0.0f, 1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 1.0f, -0.64f,		0.0f, 1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 1.0f, -0.77f,		0.0f, 1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 1.0f, -0.87f,		0.0f, 1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 1.0f, -0.94f,		0.0f, 1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 1.0f, -0.98f,		0.0f, 1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 1.0f, -0.98f,	0.0f, 1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 1.0f, -0.94f,	0.0f, 1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 1.0f, -0.87f,		0.0f, 1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 1.0f, -0.77f,	0.0f, 1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 1.0f, -0.64f,	0.0f, 1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 1.0f, -0.34f,	0.0f, 1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 1.0f, -0.17f,	0.0f, 1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 1.0f, 0.34f,		0.0f, 1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 1.0f, 0.64f,		0.0f, 1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 1.0f, 0.77f,		0.0f, 1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 1.0f, 0.87f,		0.0f, 1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 1.0f, 0.94f,		0.0f, 1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 1.0f, 0.98f,		0.0f, 1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 1.0f, 0.98f,		0.0f, 1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 1.0f, 0.94f,		0.0f, 1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 1.0f, 0.87f,		0.0f, 1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 1.0f, 0.77f,		0.0f, 1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 1.0f, 0.64f,		0.0f, 1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 1.0f, 0.34f,		0.0f, 1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder body		// normals							// texture coords
		1.0f, 1.0f, 0.0f,		0.993150651f, 0.5f, -0.116841137f,	0.0,1.0,
		1.0f, 0.0f, 0.0f,		0.993150651f, 0.5f, -0.116841137f,	0.0,0.0,
		.98f, 0.0f, -0.17f,		0.993150651f, 0.5f, -0.116841137f,	0.0277,0.0,
		1.0f, 1.0f, 0.0f,		0.993150651f, 0.5f, -0.116841137f,	0.0,1.0,
		.98f, 1.0f, -0.17f,		0.973417103f, 0.5f, -0.229039446f,	0.0277,1.0,
		.98f, 0.0f, -0.17f,		0.973417103f, 0.5f, -0.229039446f,	0.0277,0.0,
		.94f, 0.0f, -0.34f,		0.973417103f, 0.0f, -0.229039446f,	0.0554,0.0,
		.98f, 1.0f, -0.17f,		0.973417103f, 0.0f, -0.229039446f,	0.0277,1.0,
		.94f, 1.0f, -0.34f,		0.916157305f, 0.0f, -0.400818795f,	0.0554,1.0,
		.94f, 0.0f, -0.34f,		0.916157305f, 0.0f, -0.400818795f,	0.0554,0.0,
		.87f, 0.0f, -0.5f,		0.916157305f, 0.0f, -0.400818795f,	0.0831,0.0,
		.94f, 1.0f, -0.34f,		0.916157305f, 0.0f, -0.400818795f,	0.0554,1.0,
		.87f, 1.0f, -0.5f,		0.813733339f, 0.0f, -0.581238329f,	0.0831,1.0,
		.87f, 0.0f, -0.5f,		0.813733339f, 0.0f, -0.581238329f,	0.0831,0.0,
		.77f, 0.0f, -0.64f,		0.813733339f, 0.0f, -0.581238329f,	0.1108,0.0,
		.87f, 1.0f, -0.5f,		0.813733339f, 0.0f, -0.581238329f,	0.0831,1.0,
		.77f, 1.0f, -0.64f,		0.707106769f, 0.0f, -0.707106769f,	0.1108,1.0,
		.77f, 0.0f, -0.64f,		0.707106769f, 0.0f, -0.707106769f,	0.1108,0.0,
		.64f, 0.0f, -0.77f,		0.707106769f, 0.0f, -0.707106769f,	0.1385,0.0,
		.77f, 1.0f, -0.64f,		0.707106769f, 0.0f, -0.707106769f,	0.1108,1.0,
		.64f, 1.0f, -0.77f,		0.581238329f, 0.0f, -0.813733339f,	0.1385,1.0,
		.64f, 0.0f, -0.77f,		0.581238329f, 0.0f, -0.813733339f,	0.1385,0.0,
		.5f, 0.0f, -0.87f,		0.581238329f, 0.0f, -0.813733339f,	0.1662,0.0,
		.64f, 1.0f, -0.77f,		0.581238329f, 0.0f, -0.813733339f,	0.1385, 1.0,
		.5f, 1.0f, -0.87f,		0.400818795f, 0.0f, -0.916157305f,	0.1662, 1.0,
		.5f, 0.0f, -0.87f,		0.400818795f, 0.0f, -0.916157305f,	0.1662, 0.0,
		.34f, 0.0f, -0.94f,		0.400818795f, 0.0f, -0.916157305f,	0.1939, 0.0,
		.5f, 1.0f, -0.87f,		0.400818795f, 0.0f, -0.916157305f,	0.1662, 1.0,
		.34f, 1.0f, -0.94f,		0.229039446f, 0.0f, -0.973417103f,	0.1939, 1.0,
		.34f, 0.0f, -0.94f,		0.229039446f, 0.0f, -0.973417103f,	0.1939, 0.0,
		.17f, 0.0f, -0.98f,		0.229039446f, 0.0f, -0.973417103f,	0.2216, 0.0,
		.34f, 1.0f, -0.94f,		0.229039446f, 0.0f, -0.973417103f,	0.1939, 1.0,
		.17f, 1.0f, -0.98f,		0.116841137f, 0.0f, -0.993150651f,	0.2216, 1.0,
		.17f, 0.0f, -0.98f,		0.116841137f, 0.0f, -0.993150651f,	0.2216, 0.0,
		0.0f, 0.0f, -1.0f,		0.116841137f, 0.0f, -0.993150651f,	0.2493, 0.0,
		.17f, 1.0f, -0.98f,		0.116841137f, 0.0f, -0.993150651f,	0.2216, 1.0,
		0.0f, 1.0f, -1.0f,		0.116841137f, 0.0f, -0.993150651f,	0.2493, 1.0,
		0.0f, 0.0f, -1.0f,		0.116841137f, 0.0f, -0.993150651f,	0.2493, 0.0,
		-.17f, 0.0f, -0.98f,	-0.116841137f, 0.0f, -0.993150651f,	0.277, 0.0,
		0.0f, 1.0f, -1.0f,		-0.116841137f, 0.0f, -0.993150651f,	0.2493, 1.0,
		-.17f, 1.0f, -0.98f,	-0.229039446f, 0.0f, -0.973417103f,	0.277, 1.0,
		-.17f, 0.0f, -0.98f,	-0.229039446f, 0.0f, -0.973417103f,	0.277, 0.0,
		-.34f, 0.0f, -0.94f,	-0.229039446f, 0.0f, -0.973417103f,	0.3047, 0.0,
		-.17f, 1.0f, -0.98f,	-0.229039446f, 0.0f, -0.973417103f,	0.277, 1.0,
		-.34f, 1.0f, -0.94f,	-0.400818795f, 0.0f, -0.916157305f,	0.3047, 1.0,
		-.34f, 0.0f, -0.94f,	-0.400818795f, 0.0f, -0.916157305f,	0.3047, 0.0,
		-.5f, 0.0f, -0.87f,		-0.400818795f, 0.0f, -0.916157305f,	0.3324, 0.0,
		-.34f, 1.0f, -0.94f,	-0.400818795f, 0.0f, -0.916157305f,	0.3047, 1.0,
		-.5f, 1.0f, -0.87f,		-0.581238329f, 0.0f, -0.813733339f,	0.3324, 1.0,
		-.5f, 0.0f, -0.87f,		-0.581238329f, 0.0f, -0.813733339f,	0.3324, 0.0,
		-.64f, 0.0f, -0.77f,	-0.581238329f, 0.0f, -0.813733339f,	0.3601, 0.0,
		-.5f, 1.0f, -0.87f,		-0.581238329f, 0.0f, -0.813733339f,	0.3324, 1.0,
		-.64f, 1.0f, -0.77f,	-0.707106769f, 0.0f, -0.707106769f,	0.3601, 1.0,
		-.64f, 0.0f, -0.77f,	-0.707106769f, 0.0f, -0.707106769f,	0.3601, 0.0,
		-.77f, 0.0f, -0.64f,	-0.707106769f, 0.0f, -0.707106769f,	0.3878, 0.0,
		-.64f, 1.0f, -0.77f,	-0.707106769f, 0.0f, -0.707106769f,	0.3601, 1.0,
		-.77f, 1.0f, -0.64f,	-0.813733339f, 0.0f, -0.581238329f,	0.3878, 1.0,
		-.77f, 0.0f, -0.64f,	-0.813733339f, 0.0f, -0.581238329f,	0.3878, 0.0,
		-.87f, 0.0f, -0.5f,		-0.813733339f, 0.0f, -0.581238329f,	0.4155, 0.0,
		-.77f, 1.0f, -0.64f,	-0.813733339f, 0.0f, -0.581238329f,	0.3878, 1.0,
		-.87f, 1.0f, -0.5f,		-0.916157305f, 0.0f, -0.400818795f,	0.4155, 1.0,
		-.87f, 0.0f, -0.5f,		-0.916157305f, 0.0f, -0.400818795f,	0.4155, 0.0,
		-.94f, 0.0f, -0.34f,	-0.916157305f, 0.0f, -0.400818795f,	0.4432, 0.0,
		-.87f, 1.0f, -0.5f,		-0.916157305f, 0.0f, -0.400818795f,	0.4155, 1.0,
		-.94f, 1.0f, -0.34f,	-0.973417103f, 0.0f, -0.229039446f,	0.4432, 1.0,
		-.94f, 0.0f, -0.34f,	-0.973417103f, 0.0f, -0.229039446f,	0.4432, 0.0,
		-.98f, 0.0f, -0.17f,	-0.973417103f, 0.0f, -0.229039446f,	0.4709, 0.0,
		-.94f, 1.0f, -0.34f,	-0.973417103f, 0.0f, -0.229039446f,	0.4432, 1.0,
		-.98f, 1.0f, -0.17f,	-0.993150651f, 0.0f, -0.116841137f,	0.4709, 1.0,
		-.98f, 0.0f, -0.17f,	-0.993150651f, 0.0f, -0.116841137f,	0.4709, 0.0,
		-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f,	0.4986, 0.0,
		-.98f, 1.0f, -0.17f,	-0.993150651f, 0.0f, -0.116841137f,	0.4709, 1.0,
		-1.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f,	0.4986, 1.0,
		-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f,	0.4986, 0.0,
		-.98f, 0.0f, 0.17f,		-0.993150651f, 0.0f, 0.116841137f,	0.5263, 0.0,
		-1.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, 0.116841137f,	0.4986, 1.0,
		-.98f, 1.0f, 0.17f,		-0.973417103f, 0.0f, 0.229039446f,	0.5263, 1.0,
		-.98f, 0.0f, 0.17f,		-0.973417103f, 0.0f, 0.229039446f,	0.5263, 0.0,
		-.94f, 0.0f, 0.34f,		-0.973417103f, 0.0f, 0.229039446f,	0.554, 0.0,
		-.98f, 1.0f, 0.17f,		-0.973417103f, 0.0f, 0.229039446f,	0.5263, 1.0,
		-.94f, 1.0f, 0.34f,		-0.916157305f, 0.0f, 0.400818795f,	0.554, 1.0,
		-.94f, 0.0f, 0.34f,		-0.916157305f, 0.0f, 0.400818795f,	0.554, 0.0,
		-.87f, 0.0f, 0.5f,		-0.916157305f, 0.0f, 0.400818795f,	0.5817, 0.0,
		-.94f, 1.0f, 0.34f,		-0.916157305f, 0.0f, 0.400818795f,	0.554, 1.0,
		-.87f, 1.0f, 0.5f,		-0.813733339f, 0.0f, 0.581238329f,	0.5817, 1.0,
		-.87f, 0.0f, 0.5f,		-0.813733339f, 0.0f, 0.581238329f,	0.5817, 0.0,
		-.77f, 0.0f, 0.64f,		-0.813733339f, 0.0f, 0.581238329f,	0.6094, 0.0,
		-.87f, 1.0f, 0.5f,		-0.813733339f, 0.0f, 0.581238329f,	0.5817, 1.0,
		-.77f, 1.0f, 0.64f,		-0.707106769f, 0.0f, 0.707106769f,	0.6094, 1.0,
		-.77f, 0.0f, 0.64f,		-0.707106769f, 0.0f, 0.707106769f,	0.6094, 0.0,
		-.64f, 0.0f, 0.77f,		-0.707106769f, 0.0f, 0.707106769f,	0.6371, 0.0,
		-.77f, 1.0f, 0.64f,		-0.707106769f, 0.0f, 0.707106769f,	0.6094, 1.0,
		-.64f, 1.0f, 0.77f,		-0.581238329f, 0.0f, 0.813733339f,	0.6371, 1.0,
		-.64f, 0.0f, 0.77f,		-0.581238329f, 0.0f, 0.813733339f,	0.6371, 0.0,
		-.5f, 0.0f, 0.87f,		-0.581238329f, 0.0f, 0.813733339f,	0.6648, 0.0,
		-.64f, 1.0f, 0.77f,		-0.581238329f, 0.0f, 0.813733339f,	0.6371, 1.0,
		-.5f, 1.0f, 0.87f,		-0.400818795f, 0.0f, 0.916157305f,	0.6648, 1.0,
		-.5f, 0.0f, 0.87f,		-0.400818795f, 0.0f, 0.916157305f,	0.6648, 0.0,
		-.34f, 0.0f, 0.94f,		-0.400818795f, 0.0f, 0.916157305f,	0.6925, 0.0,
		-.5f, 1.0f, 0.87f,		-0.400818795f, 0.0f, 0.916157305f,	0.6648, 1.0,
		-.34f, 1.0f, 0.94f,		-0.229039446f, 0.0f, 0.973417103f,	0.6925, 1.0,
		-.34f, 0.0f, 0.94f,		-0.229039446f, 0.0f, 0.973417103f,	0.6925, 0.0,
		-.17f, 0.0f, 0.98f,		-0.229039446f, 0.0f, 0.973417103f,	0.7202, 0.0,
		-.34f, 1.0f, 0.94f,		-0.229039446f, 0.0f, 0.973417103f,	0.6925, 1.0,
		-.17f, 1.0f, 0.98f,		-0.116841137f, 0.0f, 0.993150651f,	0.7202, 1.0,
		-.17f, 0.0f, 0.98f,		-0.116841137f, 0.0f, 0.993150651f,	0.7202, 0.0,
		0.0f, 0.0f, 1.0f,		0.116841137f, 0.0f, 0.993150651f,	0.7479, 0.0,
		-.17f, 1.0f, 0.98f,		-0.116841137f, 0.0f, 0.993150651f,	0.7202, 1.0,
		0.0f, 1.0f, 1.0f,		0.116841137f, 0.0f, 0.993150651f,	0.7479, 1.0,
		0.0f, 0.0f, 1.0f,		0.116841137f, 0.0f, 0.993150651f,	0.7479, 0.0,
		.17f, 0.0f, 0.98f,		0.116841137f, 0.0f, 0.993150651f,	0.7756, 0.0,
		0.0f, 1.0f, 1.0f,		0.116841137f, 0.0f, 0.993150651f,	0.7479, 1.0,
		.17f, 1.0f, 0.98f,		0.229039446f, 0.0f, 0.973417103f,	0.7756, 1.0,
		.17f, 0.0f, 0.98f,		0.229039446f, 0.0f, 0.973417103f,	0.7756, 0.0,
		.34f, 0.0f, 0.94f,		0.229039446f, 0.0f, 0.973417103f,	0.8033, 0.0,
		.17f, 1.0f, 0.98f,		0.229039446f, 0.0f, 0.973417103f,	0.7756, 1.0,
		.34f, 1.0f, 0.94f,		0.400818795f, 0.0f, 0.916157305f,	0.8033, 1.0,
		.34f, 0.0f, 0.94f,		0.400818795f, 0.0f, 0.916157305f,	0.8033, 0.0,
		.5f, 0.0f, 0.87f,		0.400818795f, 0.0f, 0.916157305f,	0.831, 0.0,
		.34f, 1.0f, 0.94f,		0.400818795f, 0.0f, 0.916157305f,	0.8033, 1.0,
		.5f, 1.0f, 0.87f,		0.581238329f, 0.0f, 0.813733339f,	0.831, 1.0,
		.5f, 0.0f, 0.87f,		0.581238329f, 0.0f, 0.813733339f,	0.831, 0.0,
		.64f, 0.0f, 0.77f,		0.581238329f, 0.0f, 0.813733339f,	0.8587, 0.0,
		.5f, 1.0f, 0.87f,		0.581238329f, 0.0f, 0.813733339f,	0.831, 1.0,
		.64f, 1.0f, 0.77f,		0.707106769f, 0.0f, 0.707106769f,	0.8587, 1.0,
		.64f, 0.0f, 0.77f,		0.707106769f, 0.0f, 0.707106769f,	0.8587, 0.0,
		.77f, 0.0f, 0.64f,		0.707106769f, 0.0f, 0.707106769f,	0.8864, 0.0,
		.64f, 1.0f, 0.77f,		0.707106769f, 0.0f, 0.707106769f,	0.8587, 1.0,
		.77f, 1.0f, 0.64f,		0.813733339f, 0.0f, 0.581238329f,	0.8864, 1.0,
		.77f, 0.0f, 0.64f,		0.813733339f, 0.0f, 0.581238329f,	0.8864, 0.0,
		.87f, 0.0f, 0.5f,		0.813733339f, 0.0f, 0.581238329f,	0.9141, 0.0,
		.77f, 1.0f, 0.64f,		0.813733339f, 0.0f, 0.581238329f,	0.8864, 1.0,
		.87f, 1.0f, 0.5f,		0.916157305f, 0.0f, 0.400818795f,	0.9141, 1.0,
		.87f, 0.0f, 0.5f,		0.916157305f, 0.0f, 0.400818795f,	0.9141, 0.0,
		.94f, 0.0f, 0.34f,		0.916157305f, 0.0f, 0.400818795f,	0.9418, 0.0,
		.87f, 1.0f, 0.5f,		0.916157305f, 0.0f, 0.400818795f,	0.9141, 1.0,
		.94f, 1.0f, 0.34f,		0.973417103f, 0.0f, 0.229039446f,	0.9418, 1.0,
		.94f, 0.0f, 0.34f,		0.973417103f, 0.0f, 0.229039446f,	0.9418, 0.0,
		.98f, 0.0f, 0.17f,		0.973417103f, 0.0f, 0.229039446f,	0.9695, 0.0,
		.94f, 1.0f, 0.34f,		0.973417103f, 0.0f, 0.229039446f,	0.9418, 1.0,
		.98f, 1.0f, 0.17f,		0.993150651f, 0.0f, 0.116841137f,	0.9695, 1.0,
		.98f, 0.0f, 0.17f,		0.993150651f, 0.0f, 0.116841137f,	0.9695, 0.0,
		1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f,	1.0, 0.0,
		.98f, 1.0f, 0.17f,		0.993150651f, 0.0f, 0.116841137f,	0.9695, 1.0,
		1.0f, 1.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f,	1.0, 1.0,
		1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f,	1.0, 0.0
	};

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.nIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create VBO
	glGenBuffers(1, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreateTaperedCylinderMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a tapered cylinder mesh and store it in a VAO/VBO
//
//  Correct triangle drawing commands:
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
//	glDrawArrays(GL_TRIANGLE_FAN, 36, 72);		//top
//	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
///////////////////////////////////////////////////
void Meshes::UCreateTaperedCylinderMesh(GLMesh &mesh)
{
	GLfloat verts[] = {
		// cylinder bottom		// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder top			// normals			// texture coords
		0.5f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f,1.0f,
		.49f, 1.0f, -0.085f,	0.0f, 1.0f, 0.0f,	0.41f, 0.983f,
		.47f, 1.0f, -0.17f,		0.0f, 1.0f, 0.0f,	0.33f, 0.96f,
		.435f, 1.0f, -0.25f,	0.0f, 1.0f, 0.0f,	0.25f, 0.92f,
		.385f, 1.0f, -0.32f,	0.0f, 1.0f, 0.0f,	0.17f, 0.87f,
		.32f, 1.0f, -0.385f,	0.0f, 1.0f, 0.0f,	0.13f, 0.83f,
		.25f, 1.0f, -0.435f,	0.0f, 1.0f, 0.0f,	0.08f, 0.77f,
		.17f, 1.0f, -0.47f,		0.0f, 1.0f, 0.0f,	0.04f, 0.68f,
		.085f, 1.0f, -0.49f,	0.0f, 1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f,0.5f,
		-.085f, 1.0f, -0.49f,	0.0f, 1.0f, 0.0f,	0.017f, 0.41f,
		-.17f, 1.0f, -0.47f,	0.0f, 1.0f, 0.0f,	0.04f, 0.33f,
		-.25f, 1.0f, -0.435f,	0.0f, 1.0f, 0.0f,	0.08f, 0.25f,
		-.32f, 1.0f, -0.385f,	0.0f, 1.0f, 0.0f,	0.13f, 0.17f,
		-.385f, 1.0f, -0.32f,	0.0f, 1.0f, 0.0f,	0.17f, 0.13f,
		-.435f, 1.0f, -0.25f,	0.0f, 1.0f, 0.0f,	0.25f, 0.08f,
		-.47f, 1.0f, -0.17f,	0.0f, 1.0f, 0.0f,	0.33f, 0.04f,
		-.49f, 1.0f, -0.085f,	0.0f, 1.0f, 0.0f,	0.41f, 0.017f,
		-0.5f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f, 0.0f,
		-.49f, 1.0f, 0.085f,	0.0f, 1.0f, 0.0f,	0.6f, 0.017f,
		-.47f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.68f, 0.04f,
		-.435f, 1.0f, 0.25f,	0.0f, 1.0f, 0.0f,	0.77f, 0.08f,
		-.385f, 1.0f, 0.32f,	0.0f, 1.0f, 0.0f,	0.83f, 0.13f,
		-.32f, 1.0f, 0.385f,	0.0f, 1.0f, 0.0f,	0.87f, 0.17f,
		-.25f, 1.0f, 0.435f,	0.0f, 1.0f, 0.0f,	0.92f, 0.25f,
		-.17f, 1.0f, 0.47f,		0.0f, 1.0f, 0.0f,	0.96f, 0.33f,
		-.085f, 1.0f, 0.49f,	0.0f, 1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.5f,
		.085f, 1.0f, 0.49f,		0.0f, 1.0f, 0.0f,	0.983f, 0.6f,
		.17f, 1.0f, 0.47f,		0.0f, 1.0f, 0.0f,	0.96f, 0.68f,
		.25f, 1.0f, 0.435f,		0.0f, 1.0f, 0.0f,	0.92f, 0.77f,
		.32f, 1.0f, 0.385f,		0.0f, 1.0f, 0.0f,	0.87f, 0.83f,
		.385f, 1.0f, 0.32f,		0.0f, 1.0f, 0.0f,	0.83f, 0.87f,
		.435f, 1.0f, 0.25f,		0.0f, 1.0f, 0.0f,	0.77f, 0.92f,
		.47f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.68f, 0.96f,
		.49f, 1.0f, 0.085f,		0.0f, 1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder body		// normals							// texture coords
		0.5f, 1.0f, 0.0f,		0.993150651, 0.5f, -0.116841137f,	0.25,1.0,
		1.0f, 0.0f, 0.0f,		0.993150651, 0.5f, -0.116841137f,	0.0,0.0,
		.98f, 0.0f, -0.17f,		0.993150651, 0.5f, -0.116841137f,	0.0277,0.0,
		0.5f, 1.0f, 0.0f,		0.993150651, 0.5f, -0.116841137f, 	0.25,1.0,
		.49f, 1.0f, -0.085f,	0.993150651, 0.5f, -0.116841137f, 	0.2635,1.0,
		.98f, 0.0f, -0.17f,		0.993150651, 0.5f, -0.116841137f,	0.0277,0.0,
		.94f, 0.0f, -0.34f,		0.993417103f, 0.5f, -0.229039446f,	0.0554,0.0,
		.49f, 1.0f, -0.085f,	0.993417103f, 0.5f, -0.229039446f,	0.2635,1.0,
		.47f, 1.0f, -0.17f,		0.993417103f, 0.5f, -0.229039446f,	0.277,1.0,
		.94f, 0.0f, -0.34f,		0.993417103f, 0.5f, -0.229039446f,	0.0554,0.0,
		.87f, 0.0f, -0.5f,		0.993417103f, 0.5f, -0.229039446f,	0.0831,0.0,
		.47f, 1.0f, -0.17f,		0.993417103f, 0.5f, -0.229039446f,	0.277,1.0,
		.435f, 1.0f, -0.25f,	0.813733339f, 0.5f, -0.581238329f,	0.2905,1.0,
		.87f, 0.0f, -0.5f,		0.813733339f, 0.5f, -0.581238329f,	0.0831,0.0,
		.77f, 0.0f, -0.64f,		0.813733339f, 0.5f, -0.581238329f,	0.1108,0.0,
		.435f, 1.0f, -0.25f,	0.813733339f, 0.5f, -0.581238329f,	0.2905,1.0,
		.385f, 1.0f, -0.32f,	0.813733339f, 0.5f, -0.581238329f,	0.304,1.0,
		.77f, 0.0f, -0.64f,		0.813733339f, 0.5f, -0.581238329f,	0.1108,0.0,
		.64f, 0.0f, -0.77f,		0.707106769f, 0.5f, -0.707106769f,	0.1385,0.0,
		.385f, 1.0f, -0.32f,	0.707106769f, 0.5f, -0.707106769f,	0.304,1.0,
		.32f, 1.0f, -0.385f,	0.707106769f, 0.5f, -0.707106769f,	0.3175,1.0,
		.64f, 0.0f, -0.77f,		0.707106769f, 0.5f, -0.707106769f,	0.1385,0.0,
		.5f, 0.0f, -0.87f,		0.707106769f, 0.5f, -0.707106769f,	0.1662,0.0,
		.32f, 1.0f, -0.385f,	0.707106769f, 0.5f, -0.707106769f,	0.3175, 1.0,
		.25f, 1.0f, -0.435f,	0.400818795f, 0.5f, -0.916157305f,	0.331, 1.0,
		.5f, 0.0f, -0.87f,		0.400818795f, 0.5f, -0.916157305f,	0.1662, 0.0,
		.34f, 0.0f, -0.94f,		0.400818795f, 0.5f, -0.916157305f,	0.1939, 0.0,
		.25f, 1.0f, -0.435f,	0.400818795f, 0.5f, -0.916157305f,	0.331, 1.0,
		.17f, 1.0f, -0.47f,		0.400818795f, 0.5f, -0.916157305f,	0.3445, 1.0,
		.34f, 0.0f, -0.94f,		0.400818795f, 0.5f, -0.916157305f,	0.1939, 0.0,
		.17f, 0.0f, -0.98f,		0.229039446f, 0.5f, -0.973417103f,	0.2216, 0.0,
		.17f, 1.0f, -0.47f,		0.229039446f, 0.5f, -0.973417103f,	0.3445, 1.0,
		.085f, 1.0f, -0.49f,	0.229039446f, 0.5f, -0.973417103f,	0.358, 1.0,
		.17f, 0.0f, -0.98f,		0.229039446f, 0.5f, -0.973417103f,	0.2216, 0.0,
		0.0f, 0.0f, -1.0f,		0.229039446f, 0.5f, -0.973417103f,	0.2493, 0.0,
		.085f, 1.0f, -0.49f,	0.229039446f, 0.5f, -0.973417103f,	0.358, 1.0,
		0.0f, 1.0f, -0.5f,		-0.116841137f, 0.5f, -0.993150651f,	0.3715, 1.0,
		0.0f, 0.0f, -1.0f,		-0.116841137f, 0.5f, -0.993150651f,	0.2493, 0.0,
		-.17f, 0.0f, -0.98f,	-0.116841137f, 0.5f, -0.993150651f,	0.277, 0.0,
		0.0f, 1.0f, -0.5f,		-0.116841137f, 0.5f, -0.993150651f,	0.3715, 1.0,
		-.085f, 1.0f, -0.49f,	-0.116841137f, 0.5f, -0.993150651f,	0.385, 1.0,
		-.17f, 0.0f, -0.98f,	-0.116841137f, 0.5f, -0.993150651f,	0.277, 0.0,
		-.34f, 0.0f, -0.94f,	-0.229039446f, 0.5f, -0.973417103f,	0.3047, 0.0,
		-.085f, 1.0f, -0.49f,	-0.229039446f, 0.5f, -0.973417103f,	0.385, 1.0,
		-.17f, 1.0f, -0.47f,	-0.229039446f, 0.5f, -0.973417103f,	0.3985, 1.0,
		-.34f, 0.0f, -0.94f,	-0.229039446f, 0.5f, -0.973417103f,	0.3047, 0.0,
		-.5f, 0.0f, -0.87f,		-0.229039446f, 0.5f, -0.973417103f,	0.3324, 0.0,
		-.17f, 1.0f, -0.47f,	-0.229039446f, 0.5f, -0.973417103f,	0.3985, 1.0,
		-.25f, 1.0f, -0.435f,	-0.581238329f, 0.5f, -0.581238329f,	0.412, 1.0,
		-.5f, 0.0f, -0.87f,		-0.581238329f, 0.5f, -0.581238329f,	0.3324, 0.0,
		-.64f, 0.0f, -0.77f,	-0.581238329f, 0.5f, -0.581238329f,	0.3601, 0.0,
		-.25f, 1.0f, -0.435f,	-0.581238329f, 0.5f, -0.581238329f,	0.412, 1.0,
		-.32f, 1.0f, -0.385f,	-0.581238329f, 0.5f, -0.581238329f,	0.4255, 1.0,
		-.64f, 0.0f, -0.77f,	-0.581238329f, 0.5f, -0.581238329f,	0.3601, 0.0,
		-.77f, 0.0f, -0.64f,	-0.707106769f, 0.5f, -0.707106769f,	0.3878, 0.0,
		-.32f, 1.0f, -0.385f,	-0.707106769f, 0.5f, -0.707106769f,	0.4255, 1.0,
		-.385f, 1.0f, -0.32f,	-0.707106769f, 0.5f, -0.707106769f,	0.439, 1.0,
		-.77f, 0.0f, -0.64f,	-0.707106769f, 0.5f, -0.707106769f,	0.3878, 0.0,
		-.87f, 0.0f, -0.5f,		-0.707106769f, 0.5f, -0.707106769f,	0.4155, 0.0,
		-.385f, 1.0f, -0.32f,	-0.707106769f, 0.5f, -0.707106769f,	0.439, 1.0,
		-.435f, 1.0f, -0.25f,	-0.916157305f, 0.5f, -0.400818795f,	0.4525, 1.0,
		-.87f, 0.0f, -0.5f,		-0.916157305f, 0.5f, -0.400818795f,	0.4155, 0.0,
		-.94f, 0.0f, -0.34f,	-0.916157305f, 0.5f, -0.400818795f,	0.4432, 0.0,
		-.435f, 1.0f, -0.25f,	-0.916157305f, 0.5f, -0.400818795f,	0.4525, 1.0,
		-.47f, 1.0f, -0.17f,	-0.916157305f, 0.5f, -0.400818795f,	0.466, 1.0,
		-.94f, 0.0f, -0.34f,	-0.916157305f, 0.5f, -0.400818795f,	0.4432, 0.0,
		-.98f, 0.0f, -0.17f,	-0.973417103f, 0.5f, -0.229039446f,	0.4709, 0.0,
		-.47f, 1.0f, -0.17f,	-0.973417103f, 0.5f, -0.229039446f,	0.466, 1.0,
		-.49f, 1.0f, -0.085f,	-0.973417103f, 0.5f, -0.229039446f,	0.4795, 1.0,
		-.98f, 0.0f, -0.17f,	-0.973417103f, 0.5f, -0.229039446f,	0.4709, 0.0,
		-1.0f, 0.0f, 0.0f,		-0.973417103f, 0.5f, -0.229039446f,	0.4986, 0.0,
		-.49f, 1.0f, -0.085f,	-0.973417103f, 0.5f, -0.229039446f,	0.4795, 1.0,
		-0.5f, 1.0f, 0.0f,		-0.993150651f, 0.5f, -0.116841137f,	0.493, 1.0,
		-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.5f, -0.116841137f,	0.4986, 0.0,
		-.98f, 0.0f, 0.17f,		-0.993150651f, 0.5f, 0.116841137f,	0.5263, 0.0,
		-0.5f, 1.0f, 0.0f,		-0.993150651f, 0.5f, 0.116841137f,	0.493, 1.0,
		-.49f, 1.0f, 0.085f,	-0.993150651f, 0.5f, 0.116841137f,	0.5065, 1.0,
		-.98f, 0.0f, 0.17f,		-0.993150651f, 0.5f, 0.116841137f,	0.5263, 0.0,
		-.94f, 0.0f, 0.34f,		-0.973417103f, 0.5f, 0.229039446f,	0.554, 0.0,
		-.49f, 1.0f, 0.085f,	-0.973417103f, 0.5f, 0.229039446f,	0.5065, 1.0,
		-.47f, 1.0f, 0.17f,		-0.973417103f, 0.5f, 0.229039446f,	0.52, 1.0,
		-.94f, 0.0f, 0.34f,		-0.973417103f, 0.5f, 0.229039446f,	0.554, 0.0,
		-.87f, 0.0f, 0.5f,		-0.973417103f, 0.5f, 0.229039446f,	0.5817, 0.0,
		-.47f, 1.0f, 0.17f,		-0.973417103f, 0.5f, 0.229039446f,	0.52, 1.0,
		-.435f, 1.0f, 0.25f,	-0.813733339f, 0.5f, 0.581238329f,	0.5335, 1.0,
		-.87f, 0.0f, 0.5f,		-0.813733339f, 0.5f, 0.581238329f,	0.5817, 0.0,
		-.77f, 0.0f, 0.64f,		-0.813733339f, 0.5f, 0.581238329f,	0.6094, 0.0,
		-.435f, 1.0f, 0.25f,	-0.813733339f, 0.5f, 0.581238329f,	0.5335, 1.0,
		-.385f, 1.0f, 0.32f,	-0.813733339f, 0.5f, 0.581238329f,	0.547, 1.0,
		-.77f, 0.0f, 0.64f,		-0.813733339f, 0.5f, 0.581238329f,	0.6094, 0.0,
		-.64f, 0.0f, 0.77f,		-0.707106769f, 0.5f, 0.707106769f,	0.6371, 0.0,
		-.385f, 1.0f, 0.32f,	-0.707106769f, 0.5f, 0.707106769f,	0.547, 1.0,
		-.32f, 1.0f, 0.385f,	-0.707106769f, 0.5f, 0.707106769f,	0.5605, 1.0,
		-.64f, 0.0f, 0.77f,		-0.707106769f, 0.5f, 0.707106769f,	0.6371, 0.0,
		-.5f, 0.0f, 0.87f,		-0.707106769f, 0.5f, 0.707106769f,	0.6648, 0.0,
		-.32f, 1.0f, 0.385f,	-0.707106769f, 0.5f, 0.707106769f,	0.5605, 1.0,
		-.25f, 1.0f, 0.435f,	-0.400818795f, 0.5f, 0.916157305f,	0.574, 1.0,
		-.5f, 0.0f, 0.87f,		-0.400818795f, 0.5f, 0.916157305f,	0.6648, 0.0,
		-.34f, 0.0f, 0.94f,		-0.400818795f, 0.5f, 0.916157305f,	0.6925, 0.0,
		-.25f, 1.0f, 0.435f,	-0.400818795f, 0.5f, 0.916157305f,	0.574, 1.0,
		-.17f, 1.0f, 0.47f,		-0.400818795f, 0.5f, 0.916157305f,	0.5875, 1.0,
		-.34f, 0.0f, 0.94f,		-0.400818795f, 0.5f, 0.916157305f,	0.6925, 0.0,
		-.17f, 0.0f, 0.98f,		-0.229039446f, 0.5f, 0.973417103f,	0.7202, 0.0,
		-.17f, 1.0f, 0.47f,		-0.229039446f, 0.5f, 0.973417103f,	0.5875, 1.0,
		-.085f, 1.0f, 0.49f,	-0.229039446f, 0.5f, 0.973417103f,	0.601, 1.0,
		-.17f, 0.0f, 0.98f,		-0.229039446f, 0.5f, 0.973417103f,	0.7202, 0.0,
		0.0f, 0.0f, 1.0f,		-0.229039446f, 0.5f, 0.973417103f,	0.7479, 0.0,
		-.085f, 1.0f, 0.49f,	-0.229039446f, 0.5f, 0.973417103f,	0.601, 1.0,
		0.0f, 1.0f, 0.5f,		-0.116841137f, 0.5f, 0.993150651f,	0.6145, 1.0,
		0.0f, 0.0f, 1.0f,		-0.116841137f, 0.5f, 0.993150651f,	0.7479, 0.0,
		.17f, 0.0f, 0.98f,		0.116841137f, 0.5f, 0.993150651f,	0.7756, 0.0,
		0.0f, 1.0f, 0.5f,		0.116841137f, 0.5f, 0.993150651f,	0.6145, 1.0,
		.085f, 1.0f, 0.49f,		0.116841137f, 0.5f, 0.993150651f,	0.628, 1.0,
		.17f, 0.0f, 0.98f,		0.116841137f, 0.5f, 0.993150651f,	0.7756, 0.0,
		.34f, 0.0f, 0.94f,		0.229039446f, 0.5f, 0.973417103f,	0.8033, 0.0,
		.085f, 1.0f, 0.49f,		0.229039446f, 0.5f, 0.973417103f,	0.628, 1.0,
		.17f, 1.0f, 0.47f,		0.229039446f, 0.5f, 0.973417103f,	0.6415, 1.0,
		.34f, 0.0f, 0.94f,		0.229039446f, 0.5f, 0.973417103f,	0.8033, 0.0,
		.5f, 0.0f, 0.87f,		0.229039446f, 0.5f, 0.973417103f,	0.831, 0.0,
		.17f, 1.0f, 0.47f,		0.229039446f, 0.5f, 0.973417103f,	0.6415, 1.0,
		.25f, 1.0f, 0.435f,		0.581238329f, 0.5f, 0.813733339f,	0.655, 1.0,
		.5f, 0.0f, 0.87f,		0.581238329f, 0.5f, 0.813733339f,	0.831, 0.0,
		.64f, 0.0f, 0.77f,		0.581238329f, 0.5f, 0.813733339f,	0.8587, 0.0,
		.25f, 1.0f, 0.435f,		0.581238329f, 0.5f, 0.813733339f,	0.655, 1.0,
		.32f, 1.0f, 0.385f,		0.581238329f, 0.5f, 0.813733339f,	0.6685, 1.0,
		.64f, 0.0f, 0.77f,		0.581238329f, 0.5f, 0.813733339f,	0.8587, 0.0,
		.77f, 0.0f, 0.64f,		0.707106769f, 0.5f, 0.707106769f,	0.8864, 0.0,
		.32f, 1.0f, 0.385f,		0.707106769f, 0.5f, 0.707106769f,	0.6685, 1.0,
		.385f, 1.0f, 0.32f,		0.707106769f, 0.5f, 0.707106769f,	0.682, 1.0,
		.77f, 0.0f, 0.64f,		0.707106769f, 0.5f, 0.707106769f,	0.8864, 0.0,
		.87f, 0.0f, 0.5f,		0.707106769f, 0.5f, 0.707106769f,	0.9141, 0.0,
		.385f, 1.0f, 0.32f,		0.707106769f, 0.5f, 0.707106769f,	0.682, 1.0,
		.435f, 1.0f, 0.25f,		0.916157305f, 0.5f, 0.400818795f,	0.6955, 1.0,
		.87f, 0.0f, 0.5f,		0.916157305f, 0.5f, 0.400818795f,	0.9141, 0.0,
		.94f, 0.0f, 0.34f,		0.916157305f, 0.5f, 0.400818795f,	0.9418, 0.0,
		.435f, 1.0f, 0.25f,		0.916157305f, 0.5f, 0.400818795f,	0.6955, 1.0,
		.47f, 1.0f, 0.17f,		0.916157305f, 0.5f, 0.400818795f,	0.709, 1.0,
		.94f, 0.0f, 0.34f,		0.916157305f, 0.5f, 0.400818795f,	0.9418, 1.0,
		.98f, 0.0f, 0.17f,		0.973417103f, 0.5f, 0.229039446f,	0.9695, 0.0,
		.47f, 1.0f, 0.17f,		0.973417103f, 0.5f, 0.229039446f,	0.709, 0.0,
		.49f, 1.0f, 0.085f,		0.973417103f, 0.5f, 0.229039446f,	0.7225, 1.0,
		.98f, 0.0f, 0.17f,		0.973417103f, 0.5f, 0.229039446f,	0.9695, 0.0,
		1.0f, 0.0f, 0.0f,		0.973417103f, 0.5f, 0.229039446f,	1.0, 0.0,
		.49f, 1.0f, 0.085f,		0.973417103f, 0.5f, 0.229039446f,	0.7225, 1.0,
		0.5f, 1.0f, 0.0f,		0.993150651f, 0.5f, 0.116841137f,	0.75, 1.0,
		1.0f, 0.0f, 0.0f,		0.993150651f, 0.5f, 0.116841137f,	1.0, 0.0
	};

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.nIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create VBO
	glGenBuffers(1, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreateTorusMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a torus mesh and store it in a VAO/VBO
//
//	Correct triangle drawing command:
//
//	glDrawArrays(GL_TRIANGLES, 0, meshes.gTorusMesh.nVertices);
///////////////////////////////////////////////////
void Meshes::UCreateTorusMesh(GLMesh &mesh)
{
	int _mainSegments = 30;
	int _tubeSegments = 30;
	float _mainRadius = 1.0f;
	float _tubeRadius = .1f;

	auto mainSegmentAngleStep = glm::radians(360.0f / float(_mainSegments));
	auto tubeSegmentAngleStep = glm::radians(360.0f / float(_tubeSegments));

	std::vector<glm::vec3> vertex_list;
	std::vector<std::vector<glm::vec3>> segments_list;
	std::vector<glm::vec2> texture_coords;
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 normal;
	glm::vec3 vertex;
	glm::vec2 text_coord;

	// generate the torus vertices
	auto currentMainSegmentAngle = 0.0f;
	for (auto i = 0; i < _mainSegments; i++)
	{
		// Calculate sine and cosine of main segment angle
		auto sinMainSegment = sin(currentMainSegmentAngle);
		auto cosMainSegment = cos(currentMainSegmentAngle);
		auto currentTubeSegmentAngle = 0.0f;
		std::vector<glm::vec3> segment_points;
		for (auto j = 0; j < _tubeSegments; j++)
		{
			// Calculate sine and cosine of tube segment angle
			auto sinTubeSegment = sin(currentTubeSegmentAngle);
			auto cosTubeSegment = cos(currentTubeSegmentAngle);

			// Calculate vertex position on the surface of torus
			auto surfacePosition = glm::vec3(
				(_mainRadius + _tubeRadius * cosTubeSegment)*cosMainSegment,
				(_mainRadius + _tubeRadius * cosTubeSegment)*sinMainSegment,
				_tubeRadius*sinTubeSegment);

			//vertex_list.push_back(surfacePosition);
			segment_points.push_back(surfacePosition);

			// Update current tube angle
			currentTubeSegmentAngle += tubeSegmentAngleStep;
		}
		segments_list.push_back(segment_points);
		segment_points.clear();

		// Update main segment angle
		currentMainSegmentAngle += mainSegmentAngleStep;
	}

	float horizontalStep = 1.0 / _mainSegments;
	float verticalStep = 1.0 / _tubeSegments;
	float u = 0.0;
	float v = 0.0;

	// connect the various segments together, forming triangles
	for (int i = 0; i < _mainSegments; i++)
	{
		for (int j = 0; j < _tubeSegments; j++)
		{
			if (((i + 1) < _mainSegments) && ((j + 1) < _tubeSegments))
			{
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i][j + 1]);
				texture_coords.push_back(glm::vec2(u, v+verticalStep));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				texture_coords.push_back(glm::vec2(u+horizontalStep, v+verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i + 1][j]);
				texture_coords.push_back(glm::vec2(u+horizontalStep, v));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				texture_coords.push_back(glm::vec2(u+horizontalStep, v-verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
			}
			else
			{
				if (((i + 1) == _mainSegments) && ((j + 1) == _tubeSegments))
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[0][0]);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][0]);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((i + 1) == _mainSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][j + 1]);
					texture_coords.push_back(glm::vec2(u, v+verticalStep));
					vertex_list.push_back(segments_list[0][j + 1]);
					texture_coords.push_back(glm::vec2(0, v+verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][j + 1]);
					texture_coords.push_back(glm::vec2(0, v+verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((j + 1) == _tubeSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[i + 1][0]);
					texture_coords.push_back(glm::vec2(u+horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i + 1][j]);
					texture_coords.push_back(glm::vec2(u+horizontalStep, v));
					vertex_list.push_back(segments_list[i + 1][0]);
					texture_coords.push_back(glm::vec2(u+horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}
				
			}
			v += verticalStep;
		}
		v = 0.0;
		u += horizontalStep;
	}

	std::vector<GLfloat> combined_values;

	// combine interleaved vertices, normals, and texture coords
	for (int i = 0; i < vertex_list.size(); i++)
	{
		vertex = vertex_list[i];
		normal = normalize(vertex);
		text_coord = texture_coords[i];
		combined_values.push_back(vertex.x);
		combined_values.push_back(vertex.y);
		combined_values.push_back(vertex.z);
		combined_values.push_back(normal.x);
		combined_values.push_back(normal.y);
		combined_values.push_back(normal.z);
		combined_values.push_back(text_coord.x);
		combined_values.push_back(text_coord.y);
	}

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.nVertices = vertex_list.size();
	mesh.nIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create VBOs
	glGenBuffers(1, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * combined_values.size(), combined_values.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreateSphereMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a sphere mesh and store it in a VAO/VBO
//
//  Correct triangle drawing command:
//
//	glDrawElements(GL_TRIANGLES, meshes.gSphereMesh.nIndices, GL_UNSIGNED_INT, (void*)0);
///////////////////////////////////////////////////
void Meshes::UCreateSphereMesh(GLMesh &mesh)
{
	GLfloat verts[] = {
		// vertex data					// texture coords			// index
		// top center point
		0.0f, 1.0f, 0.0f,				0.5f, 1.0f,					//0
		// ring 1
		0.0f, 0.9808f, 0.1951f,			0.5f, 0.9375f,				//1
		0.0747f, 0.9808f, 0.1802f,		0.51219375f, 0.9375f,		//2
		0.1379f, 0.9808f, 0.1379f,		0.5243875f, 0.9375f,		//3
		0.1802f, 0.9808f, 0.0747f,		0.53658125f, 0.9375f,		//4
		0.1951f, 0.9808, 0.0f,			0.548775f, 0.9375f,			//5
		0.1802f, 0.9808f, -0.0747f,		0.56096875f, 0.9375f,		//6
		0.1379f, 0.9808f, -0.1379f,		0.5731625f, 0.9375f,		//7
		0.0747f, 0.9808f, -0.1802f,		0.58535625f, 0.9375f,		//8
		0.0f, 0.9808f, -0.1951f,		0.59755f, 0.9375f,			//9 - seam
		0.0f, 0.9808f, -0.1951f,		0.40245f, 0.9375f,			//10 - seam
		-0.0747f, 0.9808f, -0.1802f,	0.41464375f, 0.9375f,		//11
		-0.1379f, 0.9808f, -0.1379f,	0.4268375f, 0.9375f,		//12
		-0.1802f, 0.9808f, -0.0747f,	0.43903125f, 0.9375f,		//13
		-0.1951f, 0.9808, 0.0f,			0.451225f, 0.9375f,			//14
		-0.1802f, 0.9808f, 0.0747f,		0.46341875f, 0.9375f,		//15
		-0.1379f, 0.9808f, 0.1379f,		0.4756125f, 0.9375f,		//16
		-0.0747f, 0.9808f, 0.1802f,		0.48780625f, 0.9375f,		//17
		// ring 2
		0.0f, 0.9239f, 0.3827f,			0.5f, 0.875f,				//18
		0.1464f, 0.9239f, 0.3536f,		0.52391875f, 0.875f,		//19
		0.2706f, 0.9239f, 0.2706f,		0.5478375f, 0.875f,			//20
		0.3536f, 0.9239f, 0.1464f,		0.57175625f, 0.875f,		//21
		0.3827f, 0.9239f, 0.0f,			0.5956755f, 0.875f,			//22
		0.3536f, 0.9239f, -0.1464f,		0.61959425f, 0.875f,		//23
		0.2706f, 0.9239f, -0.2706f,		0.643513f, 0.875f,			//24
		0.1464f, 0.9239f, -0.3536f,		0.66743175f, 0.875f,		//25
		0.0f, 0.9239f, -0.3827f,		0.6913505f, 0.875f,			//26 - seam
		0.0f, 0.9239f, -0.3827f,		0.3086495f, 0.875f,			//27 - seam
		-0.1464f, 0.9239f, -0.3536f,	0.33256825f, 0.875f,		//28
		-0.2706f, 0.9239f, -0.2706f,	0.356487f, 0.875f,			//29
		-0.3536f, 0.9239f, -0.1464f,	0.38040575f, 0.875f,		//30
		-0.3827f, 0.9239f, 0.0f,		0.4043245f, 0.875f,			//31
		-0.3536f, 0.9239f, 0.1464f,		0.42824325f, 0.875f,		//32
		-0.2706f, 0.9239f, 0.2706f,		0.452162f, 0.875f,			//33
		-0.1464f, 0.9239f, 0.3536f,		0.47608075f, 0.875f,		//34
		// ring 3
		0.0f, 0.8315f, 0.5556f,			0.5f, 0.8125f,				//35
		0.2126f, 0.8315f, 0.5133f,		0.534725f, 0.8125f,			//36
		0.3928f, 0.8315f, 0.3928f,		0.56945f, 0.8125f,			//37
		0.5133f, 0.8315f, 0.2126f,		0.604175f, 0.8125f,			//38
		0.5556f, 0.8315f, 0.0f,			0.6389f, 0.8125f,			//39
		0.5133f, 0.8315f, -0.2126f,		0.673625f, 0.8125f,			//40
		0.3928f, 0.8315f, -0.3928f,		0.70835f, 0.8125f,			//41
		0.2126f, 0.8315f, -0.5133f,		0.743075f, 0.8125f,			//42
		0.0f, 0.8315f, -0.5556f,		0.7778f, 0.8125f,			//43 - seam
		0.0f, 0.8315f, -0.5556f,		0.2222f, 0.8125f,			//44 - seam
		-0.2126f, 0.8315f, -0.5133f,	0.256925f, 0.8125f,			//45
		-0.3928f, 0.8315f, -0.3928f,	0.29165f, 0.8125f,			//46
		-0.5133f, 0.8315f, -0.2126f,	0.326375f, 0.8125f,			//47
		-0.5556f, 0.8315f, 0.0f,		0.3611f, 0.8125f,			//48
		-0.5133f, 0.8315f, 0.2126f,		0.395825f, 0.8125f,			//49
		-0.3928f, 0.8315f, 0.3928f,		0.43055f, 0.8125f,			//50
		-0.2126f, 0.8315f, 0.5133f,		0.465275f, 0.8125f,			//51
		// ring 4
		0.0f, 0.7071f, 0.7071f,			0.5f, 0.75f,				//52
		0.2706f, 0.7071f, 0.6533f,		0.54419375f, 0.75f,			//53
		0.5f, 0.7071f, 0.5f,			0.5883875f, 0.75f,			//54
		0.6533f, 0.7071f, 0.2706f,		0.63258125f, 0.75f,			//55
		0.7071f, 0.7071f, 0.0f,			0.676775f, 0.75f,			//56
		0.6533f, 0.7071f, -0.2706f,		0.72096875f, 0.75f,			//57
		0.5f, 0.7071f, -0.5f,			0.7651625f, 0.75f,			//58
		0.2706f, 0.7071f, -0.6533f,		0.80935625f, 0.75f,			//59
		0.0f, 0.7071f, -0.7071f,		0.85355f, 0.75f,			//60 - seam
		0.0f, 0.7071f, -0.7071f,		0.14645f, 0.75f,			//61 - seam
		-0.2706f, 0.7071f, -0.6533f,	0.19064375f, 0.75f,			//62
		-0.5f, 0.7071f, -0.5f,			0.2348375f, 0.75f,			//63
		-0.6533f, 0.7071f, -0.2706f,	0.27903135f, 0.75f,			//64
		-0.7071f, 0.7071f, 0.0f,		0.323225f, 0.75f,			//65
		-0.6533f, 0.7071f, 0.2706f,		0.36741875f, 0.75f,			//66
		-0.5f, 0.7071f, 0.5f,			0.4116125f, 0.75f,			//67
		-0.2706f, 0.7071f, 0.6533f,		0.45580625f, 0.75f,			//68
		// ring 5
		0.0f, 0.5556f, 0.8315f,			0.5f, 0.6875f,				//69
		0.3182f, 0.5556f, 0.7682f,		0.55196875f, 0.6875f,		//70
		0.5879f, 0.5556f, 0.5879f,		0.6039375f, 0.6875f,		//71
		0.7682f, 0.5556f, 0.3182f,		0.65590625f, 0.6875f,		//72
		0.8315f, 0.5556f, 0.0f,			0.707875f, 0.6875f,			//73
		0.7682f, 0.5556f, -0.3182f,		0.75984375f, 0.6875f,		//74
		0.5879f, 0.5556f, -0.5879f,		0.8118125f, 0.6875f,		//75
		0.3182f, 0.5556f, -0.7682f,		0.86378125f, 0.6875f,		//76
		0.0f, 0.5556f, -0.8315f,		0.91575f, 0.6875f,			//77 - seam
		0.0f, 0.5556f, -0.8315f,		0.08425f, 0.6875f,			//78 - seam
		-0.3182f, 0.5556f, -0.7682f,	0.13621875f, 0.6875f,		//79
		-0.5879f, 0.5556f, -0.5879f,	0.1881875f, 0.6875f,		//80
		-0.7682f, 0.5556f, -0.3182f,	0.24015625f, 0.6875f,		//81
		-0.8315f, 0.5556f, 0.0f,		0.292125f, 0.6875f,			//82
		-0.7682f, 0.5556f, 0.3182f,		0.34409375f, 0.6875f,		//83
		-0.5879f, 0.5556f, 0.5879f,		0.3960625f, 0.6875f,		//84
		-0.3182f, 0.5556f, 0.7682f,		0.44803125f, 0.6875f,		//85
		//ring 6
		0.0f, 0.3827f, 0.9239f,			0.5f, 0.625f,				//86
		0.3536f, 0.3827f, 0.8536f,		0.55774375f, 0.625f,		//87
		0.6533f, 0.3827f, 0.6533f,		0.6154875f, 0.625f,			//88
		0.8536f, 0.3827f, 0.3536f,		0.67323125f, 0.625f,		//89
		0.9239f, 0.3827f, 0.0f,			0.730975f, 0.625f,			//90
		0.8536f, 0.3827f, -0.3536f,		0.78871875f, 0.625f,		//91
		0.6533f, 0.3827f, -0.6533f,		0.8464625f, 0.625f,			//92
		0.3536f, 0.3827f, -0.8536f,		0.90420625f, 0.625f,		//93
		0.0f, 0.3827f, -0.9239f,		0.96195f, 0.625f,			//94 - seam
		0.0f, 0.3827f, -0.9239f,		0.03805f, 0.625f,			//95 - seam
		-0.3536f, 0.3827f, -0.8536f,	0.09579375f, 0.625f,		//96
		-0.6533f, 0.3827f, -0.6533f,	0.1535375f, 0.625f,			//97
		-0.8536f, 0.3827f, -0.3536f,	0.21128125f, 0.625f,		//98
		-0.9239f, 0.3827f, 0.0f,		0.269025f, 0.625f,			//99
		-0.8536f, 0.3827f, 0.3536f,		0.32676875f, 0.625f,		//100
		-0.6533f, 0.3827f, 0.6533f,		0.3845125f, 0.625f,			//101
		-0.3536f, 0.3827f, 0.8536f,		0.44225625f, 0.625f,		//102
		// ring 7
		0.0f, 0.1951f, 0.9808f,			0.5f, 0.5625f,				//103
		0.3753f, 0.1915f, 0.9061f,		0.5613f, 0.5625f,			//104
		0.6935f, 0.1915f, 0.6935f,		0.6226f, 0.5625f,			//105
		0.9061f, 0.1915f, 0.3753f,		0.6839f, 0.5625f,			//106
		0.9808f, 0.1915f, 0.0f,			0.7452f, 0.5625f,			//107
		0.9061f, 0.1915f, -0.3753f,		0.8065f, 0.5625f,			//108
		0.6935f, 0.1915f, -0.6935f,		0.8678f, 0.5625f,			//109
		0.3753f, 0.1915f, -0.9061f,		0.9291f, 0.5625f,			//110
		0.0f, 0.1915f, -0.9808f,		0.9904f, 0.5625f,			//111 - seam
		0.0f, 0.1915f, -0.9808f,		0.0096f, 0.5625f,			//112 - seam
		-0.3753f, 0.1915f, -0.9061f,	0.0709f, 0.5625f,			//113
		-0.6935f, 0.1915f, -0.6935f,	0.1322f, 0.5625f,			//114
		-0.9061f, 0.1915f, -0.3753f,	0.1935f, 0.5625f,			//115
		-0.9808f, 0.1915f, 0.0f,		0.2548f, 0.5625f,			//116
		-0.9061f, 0.1915f, 0.3753f,		0.3161f, 0.5625f,			//117
		-0.6935f, 0.1915f, 0.6935f,		0.3774f, 0.5625f,			//118
		-0.3753f, 0.1915f, 0.9061f,		0.4387f, 0.5625f,			//119
		// ring 8
		0.0f, 0.0f, 1.0f,				0.5f, 0.5f,					//120
		0.3827f, 0.0f, 0.9239f,			0.5625f, 0.5f,				//121
		0.7071f, 0.0f, 0.7071f,			0.625f, 0.5f,				//122
		0.9239f, 0.0f, 0.3827f,			0.6875f, 0.5f,				//123
		1.0f, 0.0f, 0.0f,				0.75f, 0.5f,				//124
		0.9239f, 0.0f, -0.3827f,		0.8125f, 0.5f,				//125
		0.7071f, 0.0f, -0.7071f,		0.875f, 0.5f,				//126
		0.3827f, 0.0f, -0.9239f,		0.9375f, 0.5f,				//127
		0.0f, 0.0f, -1.0f,				1.0f, 0.5f,					//128 - seam
		0.0f, 0.0f, -1.0f, 				0.0f, 0.5f,					//129 - seam
		-0.3827f, 0.0f, -0.9239f,		0.0625f, 0.5f,				//130
		-0.7071f, 0.0f, -0.7071f,		0.125f, 0.5f,				//131
		-0.9239f, 0.0f, -0.3827f,		0.1875f, 0.5f,				//132
		-1.0f, 0.0f, 0.0f,				0.25f, 0.5f,				//133
		-0.9239f, 0.0f, 0.3827f,		0.3125f, 0.5f,				//134
		-0.7071, 0.0, 0.7071f,			0.375f, 0.5f,				//135
		-0.3827f, 0.0f, 0.9239f,		0.4375f, 0.5f,				//136
		// ring 9
		0.0f, -0.1915f, 0.9808f,		0.5f, 0.4375f,				//137
		0.3753f, -0.1915f, 0.9061f,		0.5613f, 0.4375f,			//138
		0.6935f, -0.1915f, 0.6935f,		0.6226f, 0.4375f,			//139
		0.9061f, -0.1915f, 0.3753f,		0.6839f, 0.4375f,			//140
		0.9808f, -0.1915f, 0.0f,		0.7452f, 0.4375f,			//141
		0.9061f, -0.1915f, -0.3753f,	0.8065f, 0.4375f,			//142
		0.6935f, -0.1915f, -0.6935f,	0.8678f, 0.4375f,			//143
		0.3753f, -0.1915f, -0.9061f,	0.9261f, 0.4375f,			//144
		0.0f, -0.1915f, -0.9808f,		0.9904f, 0.4375f,			//145 - seam
		0.0f, -0.1915f, -0.9808f,		0.0096f, 0.4375f,			//146 - seam
		-0.3753f, -0.1915f, -0.9061f,	0.0709f, 0.4375f,			//147
		-0.6935f, -0.1915f, -0.6935f,	0.1322f, 0.4375f,			//148
		-0.9061f, -0.1915f, -0.3753f,	0.1935f, 0.4375f,			//149
		-0.9808f, -0.1915f, 0.0f,		0.2548f, 0.4375f,			//150
		-0.9061f, -0.1915f, 0.3753f,	0.3161f, 0.4375f,			//151
		-0.6935f, -0.1915f, 0.6935f,	0.3774f, 0.4375f,			//152
		-0.3753f, -0.1915f, 0.9061f,	0.4387f, 0.4375f,			//153
		// ring 10
		0.0f, -0.3827f, 0.9239f,		0.5f, 0.375f,				//154
		0.3536f, -0.3827f, 0.8536f,		0.55774375f, 0.375f,		//155
		0.6533f, -0.3827f, 0.6533f,		0.6154875f, 0.375f,			//156
		0.8536f, -0.3827f, 0.3536f,		0.67323125f, 0.375f,		//157
		0.9239f, -0.3827f, 0.0f,		0.730975f, 0.375f,			//158
		0.8536f, -0.3827f, -0.3536f,	0.78871875f, 0.375f,		//159
		0.6533f, -0.3827f, -0.6533f,	0.8464625f, 0.375f,			//160
		0.3536f, -0.3827f, -0.8536f,	0.90420625f, 0.375f,		//161
		0.0f, -0.3827f, -0.9239f,		0.96195f, 0.375f,			//162 - seam
		0.0f, -0.3827f, -0.9239f,		0.03805f, 0.375f,			//163 - seam
		-0.3536f, -0.3827f, -0.8536f,	0.09579375f, 0.375f,		//164
		-0.6533f, -0.3827f, -0.6533f,	0.1535375f, 0.375f,			//165
		-0.8536f, -0.3827f, -0.3536f,	0.21128125f, 0.375f,		//166
		-0.9239f, -0.3827f, 0.0f,		0.269025f, 0.375f,			//167
		-0.8536f, -0.3827f, 0.3536f,	0.32676875f, 0.375f,		//168
		-0.6533f, -0.3827f, 0.6533f,	0.3845125f, 0.375f,			//169
		-0.3536f, -0.3827f, 0.8536f,	0.44225625f, 0.375f,		//170
		// ring 11
		0.0f, -0.5556f, 0.8315f,		0.5f, 0.3125f,				//171
		0.3182f, -0.5556f, 0.7682f,		0.55196875f, 0.3125f,		//172
		0.5879f, -0.5556f, 0.5879f,		0.6039375f, 0.3125f,		//173
		0.7682f, -0.5556f, 0.3182f,		0.65590625f, 0.3125f,		//174
		0.8315f, -0.5556f, 0.0f,		0.707875f, 0.3125f,			//175
		0.7682f, -0.5556f, -0.3182f,	0.75984375f, 0.3125f,		//176
		0.5879f, -0.5556f, -0.5879f,	0.8118125f, 0.3125f,		//177
		0.3182f, -0.5556f, -0.7682f,	0.86378125f, 0.3125f,		//178
		0.0f, -0.5556f, -0.8315f,		0.91575f, 0.3125f,			//179 - seam
		0.0f, -0.5556f, -0.8315f,		0.08425f, 0.3125f,			//180 - seam
		-0.3182f, -0.5556f, -0.7682f,	0.13621875f, 0.3125f,		//181
		-0.5879f, 0.5556f, -0.5879f,	0.1881875f, 0.3125f,		//182
		-0.7682f, -0.5556f, -0.3182f,	0.24015625f, 0.3125f,		//183
		-0.8315f, -0.5556f, 0.0f,		0.292125f, 0.3125f,			//184
		-0.7682f, -0.5556f, 0.3182f,	0.34409375f, 0.3125f,		//185
		-0.5879f, -0.5556f, 0.5879f,	0.3960625f, 0.3125f,		//186
		-0.3182f, -0.5556f, 0.7682f,	0.44803125f, 0.3125f,		//187
		// ring 12
		0.0f, -0.7071f, 0.7071f,		0.5f, 0.25f,				//188
		0.2706f, -0.7071f, 0.6533f,		0.54419375f, 0.25f,			//189
		0.5f, -0.7071f, 0.5f,			0.5883875f, 0.25f,			//190
		0.6533f, -0.7071f, 0.2706f,		0.63258125f, 0.25f,			//191
		0.7071f, -0.7071f, 0.0f,		0.676775f, 0.25f,			//192
		0.6533f, -0.7071f, -0.2706f,	0.72096875f, 0.25f,			//193
		0.5f, -0.7071f, -0.5f,			0.7651625f, 0.25f,			//194
		0.2706f, -0.7071f, -0.6533f,	0.80935625f, 0.25f,			//195
		0.0f, -0.7071f, -0.7071f,		0.85355f, 0.25f,			//196 - seam
		0.0f, -0.7071f, -0.7071f,		0.14645f, 0.25f,			//197 - seam
		-0.2706f, -0.7071f, -0.6533f,	0.19064375f, 0.25f,			//198
		-0.5f, -0.7071f, -0.5f,			0.2348375f, 0.25f,			//199
		-0.6533f, -0.7071f, -0.2706f,	0.27903135f, 0.25f,			//200
		-0.7071f, -0.7071f, 0.0f,		0.323225f, 0.25f,			//201
		-0.6533f, -0.7071f, 0.2706f,	0.36741875f, 0.25f,			//202
		-0.5f, -0.7071f, 0.5f,			0.4116125f, 0.25f,			//203
		-0.2706f, -0.7071f, 0.6533f,	0.45580625f, 0.25f,			//204
		// ring 13
		0.0f, -0.8315f, 0.5556f,		0.5f, 0.1875f,				//205
		0.2126f, -0.8315f, 0.5133f,		0.534725f, 0.1875f,			//206
		0.3928f, -0.8315f, 0.3928f,		0.56945f, 0.1875f,			//207
		0.5133f, -0.8315f, 0.2126f,		0.604175f, 0.1875f,			//208
		0.5556f, -0.8315f, 0.0f,		0.6389f, 0.1875f,			//209
		0.5133f, -0.8315f, -0.2126f,	0.673625f, 0.1875f,			//210
		0.3928f, -0.8315f, -0.3928f,	0.70835f, 0.1875f,			//211
		0.2126f, -0.8315f, -0.5133f,	0.743075f, 0.1875f,			//212
		0.0f, -0.8315f, -0.5556f,		0.7778f, 0.1875f,			//213 - seam
		0.0f, -0.8315f, -0.5556f,		0.2222f, 0.1875f,			//214 - seam
		-0.2126f, -0.8315f, -0.5133f,	0.256925f, 0.1875f,			//215
		-0.3928f, -0.8315f, -0.3928f,	0.29165f, 0.1875f,			//216
		-0.5133f, -0.8315f, -0.2126f,	0.326375f, 0.1875f,			//217
		-0.5556f, -0.8315f, 0.0f,		0.3611f, 0.1875f,			//218
		-0.5133f, -0.8315f, 0.2126f,	0.395825f, 0.1875f,			//219
		-0.3928f, -0.8315f, 0.3928f,	0.43055f, 0.1875f,			//220
		-0.2126f, -0.8315f, 0.5133f,	0.465275f, 0.1875f,			//221
		// ring 14
		0.0f, -0.9239f, 0.3827f,		0.5f, 0.125f,				//222
		0.1464f, -0.9239f, 0.3536f,		0.52391875f, 0.125f,		//223
		0.2706f, -0.9239f, 0.2706f,		0.5478375f, 0.125f,			//224
		0.3536f, -0.9239f, 0.1464f,		0.57175625f, 0.125f,		//225
		0.3827f, -0.9239f, 0.0f,		0.5956755f, 0.125f,			//226
		0.3536f, -0.9239f, -0.1464f,	0.61959425f, 0.125f,		//227
		0.2706f, -0.9239f, -0.2706f,	0.643513f, 0.125f,			//228
		0.1464f, -0.9239f, -0.3536f,	0.66743175f, 0.125f,		//229
		0.0f, -0.9239f, -0.3827f,		0.6913505f, 0.125f,			//230 - seam
		0.0f, -0.9239f, -0.3827f,		0.3086495f, 0.125f,			//231 - seam
		-0.1464f, -0.9239f, -0.3536f,	0.33256825f, 0.125f,		//232
		-0.2706f, -0.9239f, -0.2706f,	0.356487f, 0.125f,			//233
		-0.3536f, -0.9239f, -0.1464f,	0.38040575f, 0.125f,		//234
		-0.3827f, -0.9239f, 0.0f,		0.4043245f, 0.125f,			//235
		-0.3536f, -0.9239f, 0.1464f,	0.42824325f, 0.125f,		//236
		-0.2706f, -0.9239f, 0.2706f,	0.452162f, 0.125f,			//237
		-0.1464f, -0.9239f, 0.3536f,	0.47608075f, 0.125f,		//238
		// ring 15
		0.0f, -0.9808f, 0.1951f,		0.5f, 0.0625f,				//239
		0.0747f, -0.9808f, 0.1802f,		0.51219375f, 0.0625f,		//240
		0.1379f, -0.9808f, 0.1379f,		0.5243875f, 0.0625f,		//241
		0.1802f, -0.9808f, 0.0747f,		0.53658125f, 0.0625f,		//242
		0.1951f, -0.9808, 0.0f,			0.548775f, 0.0625f,			//243
		0.1802f, -0.9808f, -0.0747f,	0.56096875f, 0.0625f,		//244
		0.1379f, -0.9808f, -0.1379f,	0.5731625f, 0.0625f,		//245
		0.0747f, -0.9808f, -0.1802f,	0.58535625f, 0.0625f,		//246
		0.0f, -0.9808f, -0.1951f,		0.59755f, 0.0625f,			//247 - seam
		0.0f, -0.9808f, -0.1951f,		0.40245f, 0.0625f,			//248 - seam
		-0.0747f, -0.9808f, -0.1802f,	0.41464375f, 0.0625f,		//249
		-0.1379f, -0.9808f, -0.1379f,	0.4268375f, 0.0625f,		//250
		-0.1802f, -0.9808f, -0.0747f,	0.43903125f, 0.0625f,		//251
		-0.1951f, -0.9808, 0.0f,		0.451225f, 0.0625f,			//252
		-0.1802f, -0.9808f, 0.0747f,	0.46341875f, 0.0625f,		//253
		-0.1379f, -0.9808f, 0.1379f,	0.4756125f, 0.0625f,		//254
		-0.0747f, -0.9808f, 0.1802f,	0.48780625f, 0.0625f,		//255
		// bottom center point
		0.0f, -1.0f, 0.0f,				0.5f, 0.0f					//256
	};

	// index data
	GLuint indices[] = {
		//ring 1 - top
		0,10,11,
		0,11,12,
		0,12,13,
		0,13,14,
		0,14,15,
		0,15,16,
		0,16,17,
		0,17,1,
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,9,
		0,9,10,

		// ring 1 to ring 2
		10,27,28,
		10,11,28,
		11,28,29,
		11,12,29,
		12,29,30,
		12,13,30,
		13,30,31,
		13,14,31,
		14,31,32,
		14,15,32,
		15,32,33,
		15,16,33,
		16,33,34,
		16,17,34,
		17,34,18,
		17,1,18,
		1,18,19,
		1,2,19,
		2,19,20,
		2,3,20,
		3,20,21,
		3,4,21,
		4,21,22,
		4,5,22,
		5,22,23,
		5,6,23,
		6,23,24,
		6,7,24,
		7,24,25,
		7,8,25,
		8,25,26,
		8,9,26,
		9,26,27,
		9,10,27,

		// ring 2 to ring 3
		27,44,45,
		27,28,45,
		28,45,46,
		28,29,46,
		29,46,47,
		29,30,47,
		30,47,48,
		30,31,48,
		31,48,49,
		31,32,49,
		32,49,50,
		32,33,50,
		33,50,51,
		33,34,51,
		34,51,35,
		34,18,35,
		18,35,36,
		18,19,36,
		19,36,37,
		19,20,37,
		20,37,38,
		20,21,38,
		21,38,39,
		21,22,39,
		22,39,40,
		22,23,40,
		23,40,41,
		23,24,41,
		24,41,42,
		24,25,42,
		25,42,43,
		25,26,43,
		26,43,44,
		26,27,44,

		// ring 3 to ring 4
		44,61,62,
		44,45,62,
		45,62,63,
		45,46,63,
		46,63,64,
		46,47,64,
		47,64,65,
		47,48,65,
		48,65,66,
		48,49,66,
		49,66,67,
		49,50,67,
		50,67,68,
		50,51,68,
		51,68,52,
		51,35,52,
		35,52,53,
		35,36,53,
		36,53,54,
		36,37,54,
		37,54,55,
		37,38,55,
		38,55,56,
		38,39,56,
		39,56,57,
		39,40,57,
		40,57,58,
		40,41,58,
		41,58,59,
		41,42,59,
		42,59,60,
		42,43,60,
		43,60,61,
		43,44,61,
		
		// ring 4 to ring 5
		61,78,79,
		61,62,79,
		62,79,80,
		62,63,80,
		63,80,81,
		63,64,81,
		64,81,82,
		64,65,82,
		65,82,83,
		65,66,83,
		66,83,84,
		66,67,84,
		67,84,85,
		67,68,85,
		68,85,69,
		68,52,69,
		52,69,70,
		52,53,70,
		53,70,71,
		53,54,71,
		54,71,72,
		54,55,72,
		55,72,73,
		55,56,73,
		56,73,74,
		56,57,74,
		57,74,75,
		57,58,75,
		58,75,76,
		58,59,76,
		59,76,77,
		59,60,77,
		60,77,78,
		60,61,78,

		// ring 5 to ring 6
		78,95,96,
		78,79,96,
		79,96,97,
		79,80,97,
		80,97,98,
		80,81,98,
		81,98,99,
		81,82,99,
		82,99,100,
		82,83,100,
		83,100,101,
		83,84,101,
		84,101,102,
		84,85,102,
		85,102,86,
		85,69,86,
		69,86,87,
		69,70,87,
		70,87,88,
		70,71,88,
		71,88,89,
		71,72,89,
		72,89,90,
		72,73,90,
		73,90,91,
		73,74,91,
		74,91,92,
		74,75,92,
		75,92,93,
		75,76,93,
		76,93,94,
		76,77,94,
		77,94,95,
		77,78,95,

		// ring 6 to ring 7
		95,112,113,
		95,96,113,
		96,113,114,
		96,97,114,
		97,114,115,
		97,98,115,
		98,115,116,
		98,99,116,
		99,116,117,
		99,100,117,
		100,117,118,
		100,101,118,
		101,118,119,
		101,102,119,
		102,119,103,
		102,86,103,
		86,103,104,
		86,87,104,
		87,104,105,
		87,88,105,
		88,105,106,
		88,89,106,
		89,106,107,
		89,90,107,
		90,107,108,
		90,91,108,
		91,108,109,
		91,92,109,
		92,109,110,
		92,93,110,
		93,110,111,
		93,94,111,
		94,111,112,
		94,95,112,

		// ring 7 to ring 8
		112,129,130,
		112,113,130,
		113,130,131,
		113,114,131,
		114,131,132,
		114,115,132,
		115,132,133,
		115,116,133,
		116,133,134,
		116,117,134,
		117,134,135,
		117,118,135,
		118,135,136,
		118,119,136,
		119,136,120,
		119,103,120,
		103,120,121,
		103,104,121,
		104,121,122,
		104,105,122,
		105,122,123,
		105,106,123,
		106,123,124,
		106,107,124,
		107,124,125,
		107,108,125,
		108,125,126,
		108,109,126,
		109,126,127,
		109,110,127,
		110,127,128,
		110,111,128,
		111,128,129,
		111,112,129,

		// ring 8 to ring 9
		129,146,147,
		129,130,147,
		130,147,148,
		130,131,148,
		131,148,149,
		131,132,149,
		132,149,150,
		132,133,150,
		133,150,151,
		133,134,151,
		134,151,152,
		134,135,152,
		135,152,153,
		135,136,153,
		136,153,137,
		136,120,137,
		120,137,138,
		120,121,138,
		121,138,139,
		121,122,139,
		122,139,140,
		122,123,140,
		123,140,141,
		123,124,141,
		124,141,142,
		124,125,142,
		125,142,143,
		125,126,143,
		126,143,144,
		126,127,144,
		127,144,145,
		127,128,145,
		128,145,146,
		128,129,146,

		// ring 9 to ring 10
		146,163,164,
		146,147,164,
		147,164,165,
		147,148,165,
		148,165,166,
		148,149,166,
		149,166,167,
		149,150,167,
		150,167,168,
		150,151,168,
		151,168,169,
		151,152,169,
		152,169,170,
		152,153,170,
		153,170,154,
		153,137,154,
		137,154,155,
		137,138,155,
		138,155,156,
		138,139,156,
		139,156,157,
		139,140,157,
		140,157,158,
		140,141,158,
		141,158,159,
		141,142,159,
		142,159,160,
		142,143,160,
		143,160,161,
		143,144,161,
		144,161,162,
		144,145,162,
		145,162,163,
		145,146,163,

		// ring 10 to ring 11
		163,180,181,
		163,164,181,
		164,181,182,
		164,165,182,
		165,182,183,
		165,166,183,
		166,183,184,
		166,167,184,
		167,184,185,
		167,168,185,
		168,185,186,
		168,169,186,
		169,186,187,
		169,170,187,
		170,187,171,
		170,154,171,
		154,171,172,
		154,155,172,
		155,172,173,
		155,156,173,
		156,173,174,
		156,157,174,
		157,174,175,
		157,158,175,
		158,175,176,
		158,159,176,
		159,176,177,
		159,160,177,
		160,177,178,
		160,161,178,
		161,178,179,
		161,162,179,
		162,179,180,
		162,163,180,

		// ring 11 to ring 12
		180,197,198,
		180,181,198,
		181,198,199,
		181,182,199,
		182,199,200,
		182,183,200,
		183,200,201,
		183,184,201,
		184,201,202,
		184,185,202,
		185,202,203,
		185,186,203,
		186,203,204,
		186,187,204,
		187,204,188,
		187,171,188,
		171,188,189,
		171,172,189,
		172,189,190,
		172,173,190,
		173,190,191,
		173,174,191,
		174,191,192,
		174,175,192,
		175,192,193,
		175,176,193,
		176,193,194,
		176,177,194,
		177,194,195,
		177,178,195,
		178,195,196,
		178,179,196,
		179,196,197,
		179,180,197,

		// ring 12 to ring 13
		197,214,215,
		197,198,215,
		198,215,216,
		198,199,216,
		199,216,217,
		199,200,217,
		200,217,218,
		200,201,218,
		201,218,219,
		201,202,219,
		202,219,220,
		202,203,220,
		203,220,221,
		203,204,221,
		204,221,205,
		204,188,205,
		188,205,206,
		188,189,206,
		189,206,207,
		189,190,207,
		190,207,208,
		190,191,208,
		191,208,209,
		191,192,209,
		192,209,210,
		192,193,210,
		193,210,211,
		193,194,211,
		194,211,212,
		194,195,212,
		195,212,213,
		195,196,213,
		196,213,214,
		196,197,214,

		// ring 13 to ring 14
		214,231,232,
		214,215,232,
		215,232,233,
		215,216,233,
		216,233,234,
		216,217,234,
		217,234,235,
		217,218,235,
		218,235,236,
		218,219,236,
		219,236,237,
		219,220,237,
		220,237,238,
		220,221,238,
		221,238,222,
		221,205,222,
		205,222,223,
		205,206,223,
		206,223,224,
		206,207,224,
		207,224,225,
		207,208,225,
		208,225,226,
		208,209,226,
		209,226,227,
		209,210,227,
		210,227,228,
		210,211,228,
		211,228,229,
		211,212,229,
		212,229,230,
		212,213,230,
		213,230,231,
		213,214,231,

		// ring 14 to ring 15
		231,248,249,
		231,232,249,
		232,249,250,
		232,233,250,
		233,250,251,
		233,234,251,
		234,251,252,
		234,235,252,
		235,252,253,
		235,236,253,
		236,253,254,
		236,237,254,
		237,254,255,
		237,238,255,
		238,255,239,
		238,222,239,
		222,239,240,
		222,223,240,
		223,240,241,
		223,224,241,
		224,241,242,
		224,225,242,
		225,242,243,
		225,226,243,
		226,243,244,
		226,227,244,
		227,244,245,
		227,228,245,
		228,245,246,
		228,229,246,
		229,246,247,
		229,230,247,
		230,247,248,
		230,231,248,

		// ring 15 - bottom
		248,256,249,
		249,256,250,
		250,256,251,
		251,256,252,
		252,256,253,
		253,256,254,
		254,256,255,
		255,256,239,
		239,256,240,
		240,256,241,
		241,256,242,
		242,256,243,
		243,256,244,
		244,256,245,
		245,256,246,
		246,256,247,
		247,256,248
	};

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex));
	mesh.nIndices = sizeof(indices) / (sizeof(indices[0]));

	glm::vec3 normal;
	glm::vec3 vert;
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	float u, v;
	std::vector<GLfloat> combined_values;

	// combine interleaved vertices, normals, and texture coords
	for (int i = 0; i < sizeof(verts) / (sizeof(verts[0])); i += 5)
	{
		vert = glm::vec3(verts[i], verts[i + 1], verts[i + 2]);
		normal = normalize(vert - center);
		//u = atan2(normal.x, normal.z) / (2 * M_PI) + 0.5;
		//v = normal.y * 0.5 + 0.5;
		combined_values.push_back(vert.x);
		combined_values.push_back(vert.y);
		combined_values.push_back(vert.z);
		combined_values.push_back(normal.x);
		combined_values.push_back(normal.y);
		combined_values.push_back(normal.z);
		combined_values.push_back(verts[i + 3]);
		combined_values.push_back(verts[i + 4]);
	}

	// Create VAO
	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create VBOs
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * combined_values.size(), combined_values.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]); // Activates the index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::UDestroyMesh(GLMesh &mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}