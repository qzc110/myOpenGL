// based on Learn OpenGL by Joey de Vries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

void checkCompileError(uint32_t shader, int type)
{
   int success;
   char info_log[512];
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      if (type == 0)
         glGetShaderInfoLog(shader, 512, NULL, info_log);
      else
         glGetProgramInfoLog(shader, 512, NULL, info_log);
      std::cout << "COMPILATION_FAILED\n" << info_log << std::endl;
   }
}

const char* vertex_shader_source =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragment_shader_orange_source =
"#version 330 core\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"  frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragment_shader_blue_source =
"#version 330 core\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"  frag_color = vec4(0.0f, 0.5f, 1.0f, 1.0f);\n"
"}\0";

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


   // Compile shaders
   uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
   glCompileShader(vertex_shader);
   checkCompileError(vertex_shader, 0);

   // orange shader
   uint32_t fragment_shader_orange = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader_orange, 1, &fragment_shader_orange_source, NULL);
   glCompileShader(fragment_shader_orange);
   checkCompileError(fragment_shader_orange, 0);

   uint32_t shader_program_orange = glCreateProgram();
   glAttachShader(shader_program_orange, vertex_shader);
   glAttachShader(shader_program_orange, fragment_shader_orange);
   glLinkProgram(shader_program_orange);
   checkCompileError(shader_program_orange, 1);

   // blue shader
   uint32_t fragment_shader_blue = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader_blue, 1, &fragment_shader_blue_source, NULL);
   glCompileShader(fragment_shader_blue);
   checkCompileError(fragment_shader_blue, 0);

   uint32_t shader_program_blue = glCreateProgram();
   glAttachShader(shader_program_blue, vertex_shader);
   glAttachShader(shader_program_blue, fragment_shader_blue);
   glLinkProgram(shader_program_blue);
   checkCompileError(shader_program_blue, 1);

   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader_orange);
   glDeleteShader(fragment_shader_blue);


   // Hello Rectangle
   float rectangle[] = {
       0.5f,  0.5f, 0.0f, // top right
       0.5f, -0.5f, 0.0f, // bottom right
      -0.5f,  0.5f, 0.0f, // top left
      -0.5f, -0.5f, 0.0f, // bottom left
   };
   float diamond[] = {
       0.0f,  0.4f, 0.0f, // top
       0.2f,  0.0f, 0.0f, // right
      -0.2f,  0.0f, 0.0f, // left
       0.0f, -0.4f, 0.0f  // bottom
   };
   uint32_t indices[] = {
      0, 1, 2,
      1, 3, 2
   };

   uint32_t vao[2];
   glGenVertexArrays(2, vao);

   uint32_t vbo[2];
   glGenBuffers(2, vbo);

   uint32_t ebo[2];
   glGenBuffers(2, ebo);

   glBindVertexArray(vao[0]);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glBindVertexArray(vao[1]);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(diamond), diamond, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


   // render loop
   while (!glfwWindowShouldClose(window))
   {
      processInput(window);

      // render commands
      glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shader_program_orange);
      glBindVertexArray(vao[0]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glUseProgram(shader_program_blue);
      glBindVertexArray(vao[1]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwTerminate();

   return 0;
}