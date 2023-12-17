#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <string>

class app {
public:
	 int width, height;
	 const char* title;
	 app(const char* title, int width, int height);
	 void run();
};
