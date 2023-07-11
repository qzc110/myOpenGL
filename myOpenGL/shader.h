#pragma once

#include <stdint.h>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
   uint32_t id;

   Shader(const char* vertex_path, const char* fragment_path)
   {
      std::string vertex_code;
      std::string fragment_code;
      std::ifstream vert_shader_file;
      std::ifstream frag_shader_file;

      // get shader source from files
      vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try
      {
         std::stringstream vert_shader_stream;
         std::stringstream frag_shader_stream;

         vert_shader_file.open(vertex_path);
         vert_shader_stream << vert_shader_file.rdbuf();
         vert_shader_file.close();

         frag_shader_file.open(fragment_path);
         frag_shader_stream << vert_shader_file.rdbuf();
         frag_shader_file.close();

         vertex_code = vert_shader_stream.str();
         fragment_code = frag_shader_stream.str();
      }
      catch (std::ifstream::failure e)
      {
         std::cout << "SHADER::FILE READ ERROR\n";
      }
      const char* vert_shader_code = vertex_code.c_str();
      const char* frag_shader_code = fragment_code.c_str();


      // compile shaders
      int success;
      char info_log[512];

      uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vert_shader_code, NULL);
      glCompileShader(vertex);
      glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
      checkCompileError(vertex, 0);

      uint32_t fragment = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vert_shader_code, NULL);
      glCompileShader(vertex);
      glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
      checkCompileError(fragment, 0);

      id = glCreateProgram();
      glAttachShader(id, vertex);
      glAttachShader(id, fragment);
      glLinkProgram(id);
      glGetProgramiv(id, GL_LINK_STATUS, &success);
      checkCompileError(id, 1);
   }

   void use()
   {
      glUseProgram(id);
   }

   void setBool(const std::string& name, bool value) const
   {
      glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
   }
   void setInt(const std::string& name, int value) const
   {
      glUniform1i(glGetUniformLocation(id, name.c_str()), value);
   }
   void setFloat(const std::string& name, float value) const
   {
      glUniform1i(glGetUniformLocation(id, name.c_str()), value);
   }

private:
   void checkCompileError(uint32_t shader, int type)
   {
      int success;
      char info_log[512];
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
         if (type == 0)
         {
            glGetShaderInfoLog(shader, 512, NULL, info_log);
            std::cout << "SHADER::SHADER::COMPILATION FAILED\n" << info_log << std::endl;
         }
         else
         {
            glGetProgramInfoLog(shader, 512, NULL, info_log);
            std::cout << "SHADER::PROGRAM::LINKING FAILED\n" << info_log << std::endl;
         }
      }
   }
};