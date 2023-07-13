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
         frag_shader_stream << frag_shader_file.rdbuf();
         frag_shader_file.close();

         vertex_code = vert_shader_stream.str();
         fragment_code = frag_shader_stream.str();
      }
      catch (std::ifstream::failure e)
      {
         std::cout << "SHADER::File read error\n";
      }
      const char* vert_shader_code = vertex_code.c_str();
      const char* frag_shader_code = fragment_code.c_str();


      // compile shaders
      uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vert_shader_code, NULL);
      glCompileShader(vertex);
      checkCompileError(vertex, Type::kVertex);

      uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &frag_shader_code, NULL);
      glCompileShader(fragment);
      checkCompileError(fragment, Type::kFragment);

      id_ = glCreateProgram();
      glAttachShader(id_, vertex);
      glAttachShader(id_, fragment);
      glLinkProgram(id_);
      checkCompileError(id_, Type::kProgram);

      glDeleteShader(vertex);
      glDeleteShader(fragment);
   }

   uint32_t getId() { return id_; }

   void use() { glUseProgram(id_); }

   void setBool(const std::string& name, bool value) const
   {
      glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
   }
   void setInt(const std::string& name, int value) const
   {
      glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
   }
   void setFloat(const std::string& name, float value) const
   {
      glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
   }

private:
   enum class Type { kVertex, kFragment, kProgram };
   uint32_t id_;

   void checkCompileError(uint32_t obj, Shader::Type type)
   {
      int success;
      char info_log[512];

      switch (type)
      {
         case Type::kVertex:
            glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
            if (success != GL_TRUE) {
               glGetShaderInfoLog(obj, 512, NULL, info_log);
               std::cout << "SHADER::Vertex compilation failed\n" << info_log << std::endl;
            }
            break;

         case Type::kFragment:
            glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
            if (success != GL_TRUE) {
               glGetShaderInfoLog(obj, 512, NULL, info_log);
               std::cout << "SHADER::Fragment compilation failed\n" << info_log << std::endl;
            }
            break;

         case Type::kProgram:
            glGetProgramiv(obj, GL_COMPILE_STATUS, &success);
            if (success != GL_TRUE) {
               glGetProgramInfoLog(obj, 512, NULL, info_log);
               std::cout << "SHADER::Program linking failed\n" << info_log << std::endl;
            }
            break;

         default:
            std::cout << "SHADER::Unknown shader compilation error\n";
      }
   }
};