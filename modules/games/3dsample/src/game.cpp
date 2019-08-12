// TODO:
// use RAII for GL objects

#define GLEW_STATIC

#include "game.hpp"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <math.h>
#include <chrono>
#include <iostream>

namespace Sample
{
constexpr GLfloat s_vertices[] = {
    -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  // Top-left, Red
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Top-right, Green
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f,  // Bottom-right, Blue
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f   // Bottom-left, White
};
constexpr GLuint s_elements[]     = {0, 1, 2, 2, 3, 0};
const GLchar*    s_vertexSource   = R"glsl(
    #version 150 core

    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main()
    {
        Color = color;
        gl_Position = vec4(position, rand(), 1.0);
    }
)glsl";
const GLchar*    s_fragmentSource = R"glsl(
    #version 150 core

    uniform vec3 triangleColor;

    in vec3 Color;

    out vec4 outColor;

    void main()
    {
        // outColor = vec4(triangleColor, 1.0);

        outColor = vec4(Color, triangleColor.r);
    }
)glsl";
// TODO:
// move to fatty
constexpr auto s_pie      = 3.14159f;
constexpr auto s_sineWave = [](float time, float freq = 1.0f) {
  return (sin(2 * s_pie * freq * time) + 1) / 2;
};

struct Game::Impl
{
  decltype(std::chrono::high_resolution_clock::now()) m_startTimestamp;
  bool                                                m_rightPressed = false;
  bool                                                m_leftPressed  = false;
  size_t                                              m_r            = 0;
  size_t                                              m_g            = 0;
  size_t                                              m_b            = 0;
  GLuint                                              m_vao;
  GLuint                                              m_vbo;
  GLuint                                              m_ebo;
  GLuint                                              m_vertexShader;
  GLuint                                              m_fragmentShader;
  GLuint                                              m_shaderProgram;
  GLint                                               m_uniColor;
  GLint                                               m_posAttrib;
  GLint                                               m_colAttrib;

  Impl()  = default;
  ~Impl() = default;

  void initGlew()
  {
    glewExperimental = GL_TRUE;
    glewInit();

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void initVao()
  {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
  }

  void initVbo()
  {
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices,
                 GL_STATIC_DRAW);
  }

  void initEbo()
  {
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_elements), s_elements,
                 GL_STATIC_DRAW);
  }

  void initVertexShader()
  {
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader, 1, &s_vertexSource, NULL);
    glCompileShader(m_vertexShader);
    GLint status;
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
    char buffer[512];
    glGetShaderInfoLog(m_vertexShader, 512, NULL, buffer);
    std::cout << "vertex shader compilation log:\n" << buffer;
    assert(status == GL_TRUE);
  }

  void initFragmentShader()
  {
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader, 1, &s_fragmentSource, NULL);
    glCompileShader(m_fragmentShader);
    GLint status;
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
    char buffer[512];
    glGetShaderInfoLog(m_fragmentShader, 512, NULL, buffer);
    std::cout << "fragment shader compilation log:\n" << buffer;
    assert(status == GL_TRUE);
  }

  void initProgram()
  {
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glBindFragDataLocation(m_shaderProgram, 0, "outColor");
    glLinkProgram(m_shaderProgram);
    glUseProgram(m_shaderProgram);
  }

  void initAttribsAndUniforms()
  {
    // Specify the layout of the vertex data
    m_posAttrib = glGetAttribLocation(m_shaderProgram, "position");
    glEnableVertexAttribArray(m_posAttrib);
    glVertexAttribPointer(m_posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          0);
    m_colAttrib = glGetAttribLocation(m_shaderProgram, "color");
    glEnableVertexAttribArray(m_colAttrib);
    glVertexAttribPointer(m_colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(2 * sizeof(float)));

    // Get the location of the color uniform
    m_uniColor = glGetUniformLocation(m_shaderProgram, "triangleColor");
  }

  void checkGLError()
  {
    // TODO:
    // understand why this doesnt work, and enable it
    // assert(glGetError() == GL_FALSE);
  }

  void create(Fatty::ThreeDState&)
  {
    initGlew();

    initVao();

    initVbo();

    initEbo();

    initVertexShader();

    initFragmentShader();

    initProgram();

    initAttribsAndUniforms();

    checkGLError();

    m_startTimestamp = std::chrono::high_resolution_clock::now();
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

  void keyPressed(Fatty::ThreeDState&, const sf::Event::KeyEvent& key)
  {
    switch (key.code)
    {
      case sf::Keyboard::Left:
        m_leftPressed = true;
        break;
      case sf::Keyboard::Right:
        m_rightPressed = true;
        break;
      default:
        break;
    }
  }

  void keyReleased(Fatty::ThreeDState&, const sf::Event::KeyEvent& key)
  {
    switch (key.code)
    {
      case sf::Keyboard::Left:
        m_leftPressed = false;
        break;
      case sf::Keyboard::Right:
        m_rightPressed = false;
        break;
      default:
        break;
    }
  }

  void updateBackgroundColor()
  {
    if (m_leftPressed)
    {
      m_r = (m_r + 1) % 100;
    }
    if (m_rightPressed)
    {
      m_g = (m_g + 1) % 100;
    }
  }

  void updateForegroundColor()
  {
    // TODO: move to fatty
    auto  now   = std::chrono::high_resolution_clock::now();
    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(
                      now - m_startTimestamp)
                      .count();
    glUniform3f(m_uniColor, s_sineWave(delta, 0.5), 0.0f, 0.0f);
  }

  void tick(Fatty::ThreeDState&)
  {
    updateBackgroundColor();
    updateForegroundColor();
  }

  void draw(Fatty::ThreeDState& state)
  {
    // Clear the screen
    glClearColor(m_r / 100.0, m_g / 100.0, m_b / 100.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle from the 3 s_vertices
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // Draw two triangles to form a rectangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    checkGLError();
  }
};

Game::Game() : m_impl(new Impl) {}

Game::~Game() = default;

void Game::create(Fatty::ThreeDState& s) { m_impl->create(s); }

void Game::destroy(Fatty::ThreeDState& s) { m_impl->destroy(s); }

void Game::pause(Fatty::ThreeDState& s) { m_impl->pause(s); }

void Game::resume(Fatty::ThreeDState& s) { m_impl->resume(s); }

void Game::keyPressed(Fatty::ThreeDState& s, const sf::Event::KeyEvent& e)
{
  m_impl->keyPressed(s, e);
}

void Game::keyReleased(Fatty::ThreeDState& s, const sf::Event::KeyEvent& e)
{
  m_impl->keyReleased(s, e);
}

void Game::tick(Fatty::ThreeDState& s) { m_impl->tick(s); }

void Game::draw(Fatty::ThreeDState& s) { m_impl->draw(s); }

}  // namespace Sample