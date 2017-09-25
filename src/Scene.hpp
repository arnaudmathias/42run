#pragma once
#include <deque>
#include <limits>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "run.hpp"

struct Floor {
  std::array<int, 81> setup;
  std::vector<glm::vec3> obstacles_pos;
  std::vector<GameObject*> obstacles_pool;
  std::vector<GameObject*> entities;
};

class Scene {
 public:
  Scene(Shader shader, Camera* camera, Renderer* renderer, VAO* cube);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::deque<GameObject*> floors;
  std::vector<Texture*> floor_textures;
  std::vector<Texture*> wall_textures;
  std::vector<Texture*> roof_textures;
  World world;
  VAO* vao_cube;
  void update(InputHandler& inputHandler, float deltaTime);
  void draw();
  void drawPauseUI();
  void populateFloor(GameObject* floor_ptr, const Floor& setup);

  std::vector<Floor> floor_setups;
  GLuint shader;

 private:
  Scene(void);
  void drawUI();
  Camera* _camera;
  Renderer* _renderer;
  GameObject* _player;
  float _meter_counter;
  bool _paused;
};
