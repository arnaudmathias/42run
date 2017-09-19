#include "GameObject.hpp"

void World::update(InputHandler& inputHandler, float deltaT) {
  this->deltaTime = deltaT;
  for (auto& entity : entities) {
    entity->update(*this, inputHandler);
  }
}

bool World::collide(GameObject& gameObject) {
  for (auto& entity : entities) {
    if (entity != &gameObject) {
      glm::vec4 aabb1_min_wspace =
          gameObject.getTransform() * glm::vec4(gameObject.getAABBmin(), 1.0f);
      glm::vec4 aabb1_max_wspace =
          gameObject.getTransform() * glm::vec4(gameObject.getAABBmax(), 1.0f);

      glm::vec4 aabb2_min_wspace =
          entity->getTransform() * glm::vec4(entity->getAABBmin(), 1.0f);
      glm::vec4 aabb2_max_wspace =
          entity->getTransform() * glm::vec4(entity->getAABBmax(), 1.0f);

      return (aabb1_max_wspace.x > aabb2_min_wspace.x &&
              aabb1_min_wspace.x < aabb2_max_wspace.x &&
              aabb1_max_wspace.y > aabb2_min_wspace.y &&
              aabb1_min_wspace.y < aabb2_max_wspace.y &&
              aabb1_max_wspace.z > aabb2_min_wspace.z &&
              aabb1_min_wspace.z < aabb2_max_wspace.z);
    }
  }
  return (false);
}

GameObject::GameObject(void){};

GameObject::GameObject(GLuint shader, VAO* vao, Texture* texture,
                       InputComponent* inputComponent,
                       PhysicsComponent* physicsComponent, GameObject* parent,
                       glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
    : position(pos),
      rotation(rot),
      scale(sca),
      parent(parent),
      inputComponent(inputComponent),
      physicsComponent(physicsComponent) {
  this->_renderAttrib.shader = shader;
  this->_renderAttrib.vao = vao;
  this->_renderAttrib.transforms.push_back(glm::mat4(1.0f));
  this->_renderAttrib.texture = texture;
  if (vao != nullptr) {
    this->_aabb_min = vao->aabb_min;
    this->_aabb_max = vao->aabb_max;
  } else {
    this->_aabb_min = glm::vec3(0.0f, 0.0f, 0.0f);
    this->_aabb_max = glm::vec3(0.0f, 0.0f, 0.0f);
  }
  updateTransform();
}

GameObject::GameObject(GameObject const& src) {
  std::cout << "cpy cstor" << std::endl;
  *this = src;
}

GameObject::~GameObject(void) {}

GameObject& GameObject::operator=(GameObject const& rhs) {
  std::cout << "assign op" << std::endl;
  if (this != &rhs) {
    this->_renderAttrib = rhs._renderAttrib;
    this->_renderAttrib.shader = rhs._renderAttrib.shader;
    this->_renderAttrib.vao = rhs._renderAttrib.vao;
    this->_renderAttrib.transforms = rhs._renderAttrib.transforms;
    this->_renderAttrib.texture = rhs._renderAttrib.texture;
    this->inputComponent = rhs.inputComponent;
    this->physicsComponent = rhs.physicsComponent;
    this->position = rhs.position;
    this->rotation = rhs.rotation;
    this->scale = rhs.scale;
    this->parent = rhs.parent;
    this->positionRelative = rhs.positionRelative;
  }
  return (*this);
}

void GameObject::update(World& world, InputHandler& inputHandler) {
  glm::vec3 oldPosition = this->position;
  if (inputComponent != nullptr) {
    inputComponent->update(*this, world, inputHandler, this->physicsComponent);
  }
  if (physicsComponent != nullptr) {
    physicsComponent->update(*this, world);
  }
  /*
  if (this->parent != nullptr) {
    this->updateTransform();
    this->parent->updateTransform();
    this->setTransform(this->parent->getTransform() * this->getTransform());
    // print_matrix(this->getTransform());
  } */
  // this->updateTransform();
  updateTransform();
  this->positionRelative = this->position - oldPosition;
}

void GameObject::setTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca) {
  this->position = pos;
  this->rotation = rot;
  this->scale = sca;
  updateTransform();
}

void GameObject::setTransform(glm::mat4 transform) {
  this->_renderAttrib.transforms[0] = transform;
}

glm::mat4 GameObject::getTransform() {
  // updateTransform();
  return (this->_renderAttrib.transforms[0]);
}

glm::vec3 GameObject::getAABBmin() { return (this->_aabb_min); }
glm::vec3 GameObject::getAABBmax() { return (this->_aabb_max); }

void GameObject::updateTransform() {
  glm::mat4 mat_translation = glm::translate(position);
  glm::mat4 mat_rotation =
      glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
  glm::mat4 mat_scale = glm::scale(scale);
  this->_renderAttrib.transforms[0] =
      mat_translation * mat_rotation * mat_scale;
  if (this->parent != nullptr) {
    this->parent->updateTransform();
    this->_renderAttrib.transforms[0] =
        this->parent->getTransform() * this->_renderAttrib.transforms[0];
  }
}

const RenderAttrib GameObject::getRenderAttrib() const {
  return (this->_renderAttrib);
}

void GameObject::setTexture(Texture* texture) {
  this->_renderAttrib.texture = texture;
}

PhysicsComponent::PhysicsComponent(void) : speed(5.0f){};

PhysicsComponent::~PhysicsComponent(void){};

PhysicsComponent::PhysicsComponent(PhysicsComponent const& src) { *this = src; }

PhysicsComponent& PhysicsComponent::operator=(PhysicsComponent const& rhs) {
  if (this != &rhs) {
    this->velocity = rhs.velocity;
    this->speed = rhs.speed;
  }
  return (*this);
}

void PhysicsComponent::update(GameObject& gameObject, World& world) {
  glm::vec3 backupPosition = gameObject.position;
  this->speed += 0.1f * world.deltaTime;
  gameObject.position.z += log(this->speed) * 2.0f * world.deltaTime;
  this->velocity.y -= 0.81f * world.deltaTime;
  gameObject.position += velocity * world.deltaTime;

  gameObject.position.y = glm::clamp(gameObject.position.y, 0.0f, 1.0f);
  this->velocity.y = glm::clamp(this->velocity.y, -10.0f, 3.0f);
  /*
  if (world.collide(gameObject)) {
    gameObject.position = backupPosition;  // Rollback position
  } */
}

InputComponent::InputComponent(void){};

InputComponent::~InputComponent(void){};

InputComponent::InputComponent(InputComponent const& src) { *this = src; }

InputComponent& InputComponent::operator=(InputComponent const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

void InputComponent::update(GameObject& gameObject, World& world,
                            InputHandler& inputHandler,
                            PhysicsComponent* physicsComponent) {
  if (physicsComponent != nullptr) {
    if (inputHandler.keys[GLFW_KEY_A] || inputHandler.keys[GLFW_KEY_LEFT]) {
      physicsComponent->velocity.x += 1.0f * world.deltaTime;
      // gameObject.position.x += 1.0f * world.deltaTime;
    }
    if (inputHandler.keys[GLFW_KEY_D] || inputHandler.keys[GLFW_KEY_RIGHT]) {
      physicsComponent->velocity.x -= 1.0f * world.deltaTime;
      // gameObject.position.x -= 1.0f * world.deltaTime;
    }
    if (inputHandler.keys[GLFW_KEY_SPACE]) {
      if (gameObject.position.y == 0.0f) {
        physicsComponent->velocity.y = 0.7f;
      }
    }
  }
}
