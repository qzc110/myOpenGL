#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CamMove
{
   kForward, kBack, kLeft, kRight
};

class Camera
{
public:
   glm::vec3 position_;
   glm::vec3 front_;
   glm::vec3 up_;
   glm::vec3 right_;
   glm::vec3 world_up_;
   float yaw_;
   float pitch_;
   float fov_;
   float move_speed_;
   float mouse_sens_;

   Camera() : position_(0.0f, 0.0f, 0.0f),
              front_(0.0f, 0.0f, -1.0f),
              up_(0.0f, 1.0f, 0.0f),
              right_(1.0f, 0.0f, 0.0f),
              world_up_(0.0f, 1.0f, 0.0f),
              yaw_(-90.0f),
              pitch_(0.0f),
              fov_(45.0f),
              move_speed_(2.0f),
              mouse_sens_(0.1f)
   {}

   glm::mat4 GetViewMatrix()
   {
      return glm::lookAt(position_, position_ + front_, up_);
   }

   void ProcessKeyboard(CamMove direction, float delta_time)
   {
      float velocity = move_speed_ * delta_time;
      if (direction == CamMove::kForward)
         position_ += front_ * velocity;
      if (direction == CamMove::kBack)
         position_ -= front_ * velocity;
      if (direction == CamMove::kLeft)
         position_ -= right_ * velocity;
      if (direction == CamMove::kRight)
         position_ += right_ * velocity;
   }

   void ProcessScroll(double y_offset)
   {
      fov_ -= static_cast<float>(y_offset);
      if (fov_ < 1.0f)
         fov_ = 1.0f;
      if (fov_ > 130.0f)
         fov_ = 130.0f;
   }

   void ProcessMouse(double x_offset, double y_offset)
   {
      yaw_ += static_cast<float>(x_offset) * mouse_sens_;
      pitch_ += static_cast<float>(y_offset) * mouse_sens_;

      if (pitch_ > 89.0f)
         pitch_ = 89.0f;
      if (pitch_ < -89.0f)
         pitch_ = -89.0f;

      glm::vec3 direction = {
         cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
         sin(glm::radians(pitch_)),
         sin(glm::radians(yaw_)) * cos(glm::radians(pitch_))
      };
      front_ = glm::normalize(direction);
   }
};