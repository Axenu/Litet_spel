#ifdef __APPLE__
// #define GLFW_INCLUDE_COREARB
#define GLFW_INCLUDE_GLCOREARB
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
