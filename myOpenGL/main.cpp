// based on Learn OpenGL by Joey de Vries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

int main()
{
   // Initialize window
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
   if (window == NULL)
   {
      std::cout << "Failed to create GLFW window\n";
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cout << "Failed to initialize GLAD\n";
      return -1;
   }

   glViewport(0, 0, 800, 600);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


   // Hello Rectangle
   float rectangle[] = {
       0.5f,  0.5f, 0.0f, // top right
       0.5f, -0.5f, 0.0f, // bottom right
      -0.5f,  0.5f, 0.0f, // top left
      -0.5f, -0.5f, 0.0f, // bottom left
   };
   float diamond[] = {
      // pos               // color
       0.0f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f, // top
       0.2f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // right
      -0.2f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // left
       0.0f, -0.4f, 0.0f,  1.0f, 1.0f, 0.0f  // bottom
   };
   uint32_t indices[] = {
      0, 1, 2,
      1, 3, 2
   };

   Shader shader_dynamic("dynamic.vert", "dynamic.frag");
   Shader shader_gradient("gradient.vert", "gradient.frag");

   uint32_t vao[2];
   glGenVertexArrays(2, vao);

   uint32_t vbo[2];
   glGenBuffers(2, vbo);

   uint32_t ebo[2];
   glGenBuffers(2, ebo);

   // rectangle
   glBindVertexArray(vao[0]);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
   glEnableVertexAttribArray(0);

   // diamond
   glBindVertexArray(vao[1]);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(diamond), diamond, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   // position attribute
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
   glEnableVertexAttribArray(0);
   // color attribute
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
   glEnableVertexAttribArray(1);


   // misc
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // render loop
   while (!glfwWindowShouldClose(window))
   {
      processInput(window);

      // render commands
      glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      shader_dynamic.use();
      float time = glfwGetTime();
      float green = sin(time) / 2.0f + 0.5f;
      int vertex_color_location = glGetUniformLocation(shader_dynamic.getId(), "new_color");
      glUniform4f(vertex_color_location, 0.0f, green, 0.0f, 1.0f);
      glBindVertexArray(vao[0]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      shader_gradient.use();
      glBindVertexArray(vao[1]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwTerminate();

   return 0;
}