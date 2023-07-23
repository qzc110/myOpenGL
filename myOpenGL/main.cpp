// based on Learn OpenGL by Joey de Vries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <fstream>
#include <iostream>

#include "camera.h"
#include "shader.h"

const uint32_t kWidth = 800;
const uint32_t kHeight = 600;
const float kAspectRatio = static_cast<float>(kWidth) / static_cast<float>(kHeight);

float delta_time = 0.0f;
float last_frame = 0.0f;

glm::vec2 cube_rotation = { 0.0f, 0.0f };

glm::vec3 cam_pos = { 0.0f, 0.0f, 3.0f };
glm::vec3 cam_front = { 0.0f, 0.0f, -1.0f };
glm::vec3 cam_up = { 0.0f, 1.0f, 0.0f };

// mouse settings
Camera camera;
bool first_mouse = true;
float last_x = static_cast<float>(kWidth) / 2.0f;
float last_y = static_cast<float>(kHeight) / 2.0f;

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
   camera.ProcessScroll(y_offset);
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
   float x_posf = static_cast<float>(x_pos);
   float y_posf = static_cast<float>(y_pos);

   if (first_mouse)
   {
      last_x = x_posf;
      last_y = y_posf;
      first_mouse = false;
   }

   camera.ProcessMouse(x_posf - last_x, last_y - y_posf);

   last_x = static_cast<float>(x_pos);
   last_y = static_cast<float>(y_pos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
   if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      cube_rotation.x -= 1.0f;
   if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      cube_rotation.x += 1.0f;
   if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
      cube_rotation.y -= 1.0f;
   if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
      cube_rotation.y += 1.0f;

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(CamMove::kForward, delta_time);
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(CamMove::kBack, delta_time);
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(CamMove::kRight, delta_time);
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(CamMove::kLeft, delta_time);
}

int main()
{
   // Initialize window
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "LearnOpenGL", NULL, NULL);
   if (window == NULL)
   {
      std::cout << "Failed to create GLFW window\n";
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cout << "Failed to initialize GLAD\n";
      return -1;
   }

   glEnable(GL_DEPTH_TEST);

   // Shaders
   Shader our_shader("texture.vert", "texture.frag");

   // Cube
   float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
   };

   glm::vec3 cube_positions[] = {
        glm::vec3(0.0f,  3.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
   };
   
   uint32_t vao;
   glGenVertexArrays(1, &vao);

   uint32_t vbo;
   glGenBuffers(1, &vbo);

   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

   int stride = 5 * sizeof(float);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void*>(0));
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);


   // Textures
   stbi_set_flip_vertically_on_load(true);
   int width, height, nr_channels;

   uint32_t texture[2];
   glGenTextures(2, texture);

   stbi_uc* data = stbi_load("container.jpg", &width, &height, &nr_channels, 0);  
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   if (data)
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   }
   else
   {
      std::cout << "Failed to load texture file\n";
   }
   stbi_image_free(data);

   data = stbi_load("awesomeface.png", &width, &height, &nr_channels, 0);
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   if (data)
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   }
   else
   {
      std::cout << "Failed to load texture file\n";
   }
   stbi_image_free(data);

   our_shader.Use();
   our_shader.SetInt("texture1", 0);
   our_shader.SetInt("texture2", 1);


   // misc
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


   // render loop
   while (!glfwWindowShouldClose(window))
   {
      // delta time
      float curr_frame = static_cast<float>(glfwGetTime());
      delta_time = curr_frame - last_frame;
      last_frame = curr_frame;

      ProcessInput(window);

      // render commands
      glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture[0]);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture[1]);


      // Transform 
      glm::mat4 model(1.0f);
      glm::mat4 view(1.0f);
      glm::mat4 projection(1.0f);
      model = glm::rotate(model, glm::radians(cube_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, glm::radians(cube_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      view = camera.GetViewMatrix();
      projection = glm::perspective(glm::radians(camera.fov_), kAspectRatio, 0.1f, 100.0f);

      our_shader.Use();

      uint32_t model_loc = glGetUniformLocation(our_shader.GetId(), "model");
      uint32_t view_loc = glGetUniformLocation(our_shader.GetId(), "view");
      uint32_t projection_loc = glGetUniformLocation(our_shader.GetId(), "projection");
      glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      for (int i = 0; i < 10; i++)
      {
         glm::mat4 model(1.0f);
         model = glm::translate(model, cube_positions[i]);
         float angle = 20.0f * i;
         model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
         glUniformMatrix4fv(
            glGetUniformLocation(our_shader.GetId(), "model"), 1, GL_FALSE, glm::value_ptr(model)
         );
         glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwTerminate();

   return 0;
}