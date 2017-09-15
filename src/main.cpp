#include "Camera.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "env.hpp"
#include "run.hpp"

std::vector<Vertex> cube_vertices = {
    {{-0.5f, -0.5f, 0.5f}, {0.0, 0.0}},   {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0}},
    {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},   {{-0.5f, 0.5f, 0.5f}, {0.0, 1.0f}},
    {{-0.5f, 0.5f, 0.5f}, {0.0, 0.0}},    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  {{-0.5f, 0.5f, -0.5f}, {0.0, 1.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0, 0.0}},   {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}}, {{0.5f, 0.5f, -0.5f}, {0.0, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0, 0.0}},  {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0}},
    {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},  {{-0.5f, -0.5f, 0.5f}, {0.0, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0, 0.0}},  {{-0.5f, -0.5f, 0.5f}, {0.5f, 0.0}},
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},  {{-0.5f, 0.5f, -0.5f}, {0.0, 0.5f}},
    {{0.5f, -0.5f, 0.5f}, {0.0, 0.0}},    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  {{0.5f, 0.5f, 0.5f}, {0.0, 1.0f}}};

std::vector<GLuint> cube_elements = {
    0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
    12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
};

int main() {
  Env env(1280, 720);
  Shader shader("shaders/run.vert", "shaders/run.frag");

  Camera camera(glm::vec3(0.0f, 0.3f, -1.0f), glm::vec3(0.0f, 0.3f, 0.0f),
                env.width, env.height);
  Texture tex_ground("textures/0.png");
  Texture tex_wall("textures/grey_wall.jpg");
  VAO vao_cube(cube_vertices, cube_elements);
  Model cubeModel =
      Model(shader.id, &vao_cube, &tex_wall, glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
  Renderer renderer(env.width, env.height);
  Scene scene(shader, &renderer);
  scene.floor_textures.push_back(&tex_ground);
  scene.wall_textures.push_back(&tex_wall);
  scene.cubeModel = &cubeModel;
  scene.init();
  while (!glfwWindowShouldClose(env.window)) {
    env.updateFpsCounter();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    /*
       camera.queryInput(env.inputHandler.keys, env.inputHandler.mousex,
       env.inputHandler.mousey); */
    camera.update();
    renderer.view = camera.view;
    renderer.proj = camera.proj;
    scene.update(env.inputHandler.keys, env.getDeltaTime());
    scene.draw();
    renderer.renderText(env.width - 100.0f, env.height - 50.0f, 1.0f, "0",
                        glm::vec3(0.0f, 0.0f, 0.0f));
    glfwSwapBuffers(env.window);
    GL_DUMP_ERROR("draw loop");
    if (glfwGetKey(env.window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(env.window, 1);
    }
  }
  glfwTerminate();
}
