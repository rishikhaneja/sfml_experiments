#define GLEW_STATIC

#include "game.hpp"
#include <GL/glew.h>
#include <math.h>
#include <SFML/OpenGL.hpp>
#include <chrono>

namespace Sample
{
// Shader sources
const GLchar* vertexSource   = R"glsl(
    #version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;
    uniform vec3 triangleColor;
    void main()
    {
        outColor = vec4(triangleColor, 1.0);
    }
)glsl";

bool   right_pressed = false;
bool   left_pressed  = false;
size_t r             = 0;
size_t g             = 0;
size_t b             = 0;

decltype(std::chrono::high_resolution_clock::now()) t_start;

GLuint  vao;
GLuint  vbo;
GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
GLuint  vertexShader;
GLuint  fragmentShader;
GLuint  shaderProgram;
GLint   uniColor;
GLint   posAttrib;

void Game::create(Fatty::ThreeDState&)
{
  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  // Create Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // Create and compile the fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Link the vertex and fragment shader into a shader program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Specify the layout of the vertex data
  posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // Get the location of the color uniform
  uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

  t_start = std::chrono::high_resolution_clock::now();
}

void Game::destroy(Fatty::ThreeDState&)
{
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &vbo);

  glDeleteVertexArrays(1, &vao);
}

void Game::pause(Fatty::ThreeDState&) {}

void Game::resume(Fatty::ThreeDState&) {}

void Game::key_pressed(Fatty::ThreeDState&, const sf::Event::KeyEvent& key)
{
  switch (key.code)
  {
    case sf::Keyboard::Left:
      left_pressed = true;
      break;
    case sf::Keyboard::Right:
      right_pressed = true;
      break;
    default:
      break;
  }
}

void Game::key_released(Fatty::ThreeDState&, const sf::Event::KeyEvent& key)
{
  switch (key.code)
  {
    case sf::Keyboard::Left:
      left_pressed = false;
      break;
    case sf::Keyboard::Right:
      right_pressed = false;
      break;
    default:
      break;
  }
}

void Game::tick(Fatty::ThreeDState&)
{
  if (left_pressed)
  {
    r = (r + 1) % 100;
  }
  if (right_pressed)
  {
    g = (g + 1) % 100;
  }
}

void Game::draw(Fatty::ThreeDState& state)
{
  // Set the color of the triangle
  auto  t_now = std::chrono::high_resolution_clock::now();
  float time =
      std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start)
          .count();

  glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

  // Clear the screen to black
  glClearColor(r / 100.0, g / 100.0, b / 100.0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw a triangle from the 3 vertices
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

}  // namespace Sample