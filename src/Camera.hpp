#pragma once
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "env.hpp"

class Camera {
 public:
  glm::vec3 pos;
  glm::vec3 dir;
  glm::vec3 right;
  glm::vec3 up;
  glm::mat4 proj;
  glm::mat4 view;
  int width;
  int height;
  float mouseXpos;
  float mouseYpos;

  Camera(glm::vec3 pos, glm::vec3 target, int width = 0, int height = 0);
  void queryInput(std::array<bool, 1024> keys, float mouse_x, float mouse_y);
  void update();
  void rotate(float hor, float ver);  // in radians

 private:
  bool mouseMoved = false;
  float horAngle;
  float verAngle;
  float speed = 3.0f;
  float deltaTime;
  float lastTime;
};
