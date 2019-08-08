#define GLEW_STATIC

#include "game.hpp"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <math.h>
#include <chrono>
#include <iostream>

namespace Sample
{
constexpr GLfloat s_vertices[]     = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
const GLchar*     s_vertexSource   = R"glsl(
    #version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar*     s_fragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;
    uniform vec3 triangleColor;
    void main()
    {
        outColor = vec4(triangleColor, 1.0);
    }
)glsl";
constexpr auto    s_pie            = 3.14159f;
constexpr auto    s_sine_wave      = [](float time, float freq = 1.0f) {
  return (sin(2 * s_pie * freq * time) + 1) / 2;
};

struct Game::Impl
{
  decltype(std::chrono::high_resolution_clock::now()) m_start_timestamp;
  bool                                                m_right_pressed = false;
  bool                                                m_left_pressed  = false;
  size_t                                              m_r             = 0;
  size_t                                              m_g             = 0;
  size_t                                              m_b             = 0;
  GLuint                                              m_vao;
  GLuint                                              m_vbo;
  GLuint                                              m_vertexShader;
  GLuint                                              m_fragmentShader;
  GLuint                                              m_shaderProgram;
  GLint                                               m_uniColor;
  GLint                                               m_posAttrib;

  Impl()  = default;
  ~Impl() = default;

  void create(Fatty::ThreeDState&)
  {
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create Vertex Array Object
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices,
                 GL_STATIC_DRAW);

    // Create and compile the vertex shader
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader, 1, &s_vertexSource, NULL);
    glCompileShader(m_vertexShader);
    GLint status;
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
    char buffer[512];
    glGetShaderInfoLog(m_vertexShader, 512, NULL, buffer);
    std::cout << "vertex shader compilation log:\n" << buffer;
    assert(status == GL_TRUE);

    // Create and compile the fragment shader
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader, 1, &s_fragmentSource, NULL);
    glCompileShader(m_fragmentShader);
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(m_fragmentShader, 512, NULL, buffer);
    std::cout << "fragment shader compilation log:\n" << buffer;
    assert(status == GL_TRUE);

    // Link the vertex and fragment shader into a shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glBindFragDataLocation(m_shaderProgram, 0, "outColor");
    glLinkProgram(m_shaderProgram);
    glUseProgram(m_shaderProgram);

    // Specify the layout of the vertex data
    m_posAttrib = glGetAttribLocation(m_shaderProgram, "position");
    glEnableVertexAttribArray(m_posAttrib);
    glVertexAttribPointer(m_posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Get the location of the color uniform
    m_uniColor = glGetUniformLocation(m_shaderProgram, "triangleColor");

    // assert(glGetError() == GL_FALSE);

    m_start_timestamp = std::chrono::high_resolution_clock::now();
  }

  void destroy(Fatty::ThreeDState&)
  {
    glDeleteProgram(m_shaderProgram);
    glDeleteShader(m_fragmentShader);
    glDeleteShader(m_vertexShader);

    glDeleteBuffers(1, &m_vbo);

    glDeleteVertexArrays(1, &m_vao);
  }

  void pause(Fatty::ThreeDState&) {}

  void resume(Fatty::ThreeDState&) {}

  void key_pressed(Fatty::ThreeDState&, const sf::Event::KeyEvent& key)
  {
    switch (key.code)
    {
      case sf::Keyboard::Left:
        m_left_pressed = true;
        break;
      case sf::Keyboard::Right:
        m_right_pressed = true;
        break;
      default:
        break;
    }
  }

  void key_released(Fatty::ThreeDState&, const sf::Event::KeyEvent& key)
  {
    switch (key.code)
    {
      case sf::Keyboard::Left:
        m_left_pressed = false;
        break;
      case sf::Keyboard::Right:
        m_right_pressed = false;
        break;
      default:
        break;
    }
  }

  void tick(Fatty::ThreeDState&)
  {
    if (m_left_pressed)
    {
      m_r = (m_r + 1) % 100;
    }
    if (m_right_pressed)
    {
      m_g = (m_g + 1) % 100;
    }
  }

  void draw(Fatty::ThreeDState& state)
  {
    // Set the color of the triangle
    auto  now   = std::chrono::high_resolution_clock::now();
    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(
                      now - m_start_timestamp)
                      .count();

    glUniform3f(m_uniColor, s_sine_wave(delta), 0.0f, 0.0f);

    // Clear the screen to black
    glClearColor(m_r / 100.0, m_g / 100.0, m_b / 100.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle from the 3 s_vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // assert(glGetError() == GL_FALSE);
  }
};

Game::Game() : m_impl(new Impl) {}

Game::~Game() = default;

void Game::create(Fatty::ThreeDState& s) { m_impl->create(s); }

void Game::destroy(Fatty::ThreeDState& s) { m_impl->destroy(s); }

void Game::pause(Fatty::ThreeDState& s) { m_impl->pause(s); }

void Game::resume(Fatty::ThreeDState& s) { m_impl->resume(s); }

void Game::key_pressed(Fatty::ThreeDState& s, const sf::Event::KeyEvent& e)
{
  m_impl->key_pressed(s, e);
}

void Game::key_released(Fatty::ThreeDState& s, const sf::Event::KeyEvent& e)
{
  m_impl->key_released(s, e);
}

void Game::tick(Fatty::ThreeDState& s) { m_impl->tick(s); }

void Game::draw(Fatty::ThreeDState& s) { m_impl->draw(s); }

}  // namespace Sample