// TODO:
// use RAII for GL objects

#define GLEW_STATIC

#include "game.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>

#include <math.h>
#include <array>
#include <chrono>
#include <iostream>

namespace Sample {
constexpr GLfloat s_vertices[] = {
    //
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   //
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,   //
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
    //
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   //
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,   //
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
    //
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   //
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   //
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    //
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   //
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   //
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    //
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   //
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   //
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    //
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   //
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    //
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   //
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    //
    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //
    1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
    1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,    //
    1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,    //
    -1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   //
    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f   //
                                                        //
};
// constexpr GLuint s_elements[] = {0, 1, 2, 2, 3, 0};
const GLchar *s_vertexSource = R"glsl(
    #version 150 core

    in vec3 position;
    in vec3 color;
    in vec2 texcoord;

    out vec3 Color;
    out vec2 Texcoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    uniform vec3 overrideColor;

    void main()
    {
      Color = overrideColor * color;
      Texcoord = texcoord;
      gl_Position = projection * view * model * vec4(position, 1.0);
      //gl_Position = vec4(position, 1.0);
    }
)glsl";
const GLchar *s_fragmentSource = R"glsl(
    #version 150 core

    uniform float time;
    uniform sampler2D tex1;
    uniform sampler2D tex2;

    in vec3 Color;
    in vec2 Texcoord;

    out vec4 outColor;

    const float pi = 3.14159;

    float sineWave(float time, float freq)
    {
      return (sin(2 * pi * freq * time) + 1) / 2;
    }

    void main()
    {
      vec4 colTex1 = texture(tex1, Texcoord);
      vec4 colTex2 = texture(tex2, Texcoord);
      vec4 colBlended = mix(colTex1, colTex2, sineWave(time, 1.0/8));
      vec4 colColorized = vec4(Color, 1.0) * colBlended;
      vec4 colFaded = colColorized * vec4(1.0, 1.0, 1.0, sineWave(time, 1.0/4));
      outColor = vec4(Color, 1.0) * colTex1;

      // invertion
      // if (Texcoord.y < 0.5)
      //     outColor = texture(tex1, Texcoord);
      // else
      //     outColor = texture(tex1, vec2(Texcoord.x, 1.0 - Texcoord.y));

      // sea effect
      // if (Texcoord.y < 0.5)
      //     outColor = texture(tex1, Texcoord);
      // else
      //     outColor = texture(tex1,
      //         vec2(Texcoord.x + sin(Texcoord.y * 60.0 + time * 2.0) / 30.0, 1.0 - Texcoord.y)
      //     ) * vec4(0.7, 0.7, 1.0, 1.0);
    }
)glsl";
// TODO:
// move to fatty
constexpr auto s_pi = 3.14159f;
constexpr auto s_sineWave = [](float time, float freq = 1.0f) {
  return (sin(2 * s_pi * freq * time) + 1) / 2;
};

struct Game::Impl {
  decltype(std::chrono::high_resolution_clock::now()) m_startTimestamp;
  bool m_rightPressed = false;
  bool m_leftPressed = false;
  size_t m_r = 1.0;
  size_t m_g = 1.0;
  size_t m_b = 1.0;
  GLuint m_vao;
  GLuint m_vbo;
  // GLuint m_ebo;
  GLuint m_textures[2];
  glm::mat4 m_view;
  glm::mat4 m_model;
  GLuint m_vertexShader;
  GLuint m_fragmentShader;
  GLuint m_shaderProgram;
  GLint m_uniTime;
  GLint m_uniModel;
  GLint m_uniView;
  GLint m_uniProjection;
  GLint m_uniColorOverride;
  GLint m_posAttrib;
  GLint m_colAttrib;
  GLint m_texAttrib;

  Impl() = default;
  ~Impl() = default;

  void initGl() {
    glewExperimental = GL_TRUE;
    glewInit();

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
  }

  void initVao() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
  }

  void initVbo() {
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices,
                 GL_STATIC_DRAW);
  }

  void initEbo() {
    // glGenBuffers(1, &m_ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_elements), s_elements,
    //              GL_STATIC_DRAW);
  }

  void initVertexShader() {
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

  void initFragmentShader() {
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

  void initProgram() {
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glBindFragDataLocation(m_shaderProgram, 0, "outColor");
    glLinkProgram(m_shaderProgram);
    glUseProgram(m_shaderProgram);
  }

  void initAttribsAndUniforms() {
    // Specify the layout of the vertex data
    m_posAttrib = glGetAttribLocation(m_shaderProgram, "position");
    glEnableVertexAttribArray(m_posAttrib);
    glVertexAttribPointer(m_posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          0);
    m_colAttrib = glGetAttribLocation(m_shaderProgram, "color");
    glEnableVertexAttribArray(m_colAttrib);
    glVertexAttribPointer(m_colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));

    m_texAttrib = glGetAttribLocation(m_shaderProgram, "texcoord");
    glEnableVertexAttribArray(m_texAttrib);
    glVertexAttribPointer(m_texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));

    m_uniTime = glGetUniformLocation(m_shaderProgram, "time");
    m_uniModel = glGetUniformLocation(m_shaderProgram, "model");
    m_uniView = glGetUniformLocation(m_shaderProgram, "view");
    m_uniProjection = glGetUniformLocation(m_shaderProgram, "projection");

    m_uniColorOverride = glGetUniformLocation(m_shaderProgram, "overrideColor");
    glUniform3f(m_uniColorOverride, 1.0, 1.0, 1.0);
  }

  void initTextures() {
    glGenTextures(2, m_textures);
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_textures[0]);
      {
        sf::Image image;
        bool success = image.loadFromFile("assets/lenna.png");
        assert(success);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x,
                     image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     image.getPixelsPtr());
      }
      glUniform1i(glGetUniformLocation(m_shaderProgram, "tex1"), 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, m_textures[1]);
      {
        sf::Image image;
        bool success = image.loadFromFile("assets/ana.jpg");
        assert(success);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x,
                     image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     image.getPixelsPtr());
      }
      glUniform1i(glGetUniformLocation(m_shaderProgram, "tex2"), 1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
  }

  void initTransforms() {
    // model
    m_model = glm::mat4(1.0f);
    glUniformMatrix4fv(m_uniModel, 1, GL_FALSE, glm::value_ptr(m_model));

    // view
    m_view =
        glm::lookAt(glm::vec3(2.5f, 2.5f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f));
    m_uniView = glGetUniformLocation(m_shaderProgram, "view");
    glUniformMatrix4fv(m_uniView, 1, GL_FALSE, glm::value_ptr(m_view));

    // projection
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    m_uniProjection = glGetUniformLocation(m_shaderProgram, "projection");
    glUniformMatrix4fv(m_uniProjection, 1, GL_FALSE,
                       glm::value_ptr(projection));
  }

  void checkGLError() {
    // TODO:
    // understand why this doesnt work, and enable it
    // assert(glGetError() == GL_FALSE);
  }

  void create(Fatty::ThreeDState &) {
    initGl();

    initVao();

    initVbo();

    initEbo();

    initVertexShader();

    initFragmentShader();

    initProgram();

    initAttribsAndUniforms();

    initTextures();

    initTransforms();

    checkGLError();

    m_startTimestamp = std::chrono::high_resolution_clock::now();
  }

  void destroy(Fatty::ThreeDState &) {
    glDeleteProgram(m_shaderProgram);
    glDeleteShader(m_fragmentShader);
    glDeleteShader(m_vertexShader);

    glDeleteBuffers(1, &m_vbo);

    glDeleteVertexArrays(1, &m_vao);
  }

  void pause(Fatty::ThreeDState &) {}

  void resume(Fatty::ThreeDState &) {}

  void keyPressed(Fatty::ThreeDState &, const sf::Event::KeyEvent &key) {
    switch (key.code) {
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

  void keyReleased(Fatty::ThreeDState &, const sf::Event::KeyEvent &key) {
    switch (key.code) {
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

  void updateBackgroundColor() {
    if (m_leftPressed) {
      m_r = (m_r + 1) % 100;
    }
    if (m_rightPressed) {
      m_g = (m_g + 1) % 100;
    }
  }

  void updateTransforms(float delta) {
    // rotate model
    m_model = glm::rotate(glm::mat4(1.0f), delta, glm::vec3(0.0f, 0.0f, 1.0f));

    // scale model
    // GLfloat scale = sin(delta * 5.0f) * 0.25f + 0.75f;
    // model = glm::scale(model, glm::vec3(scale, scale, scale));

    // update model
    glUniformMatrix4fv(m_uniModel, 1, GL_FALSE, glm::value_ptr(m_model));

    // rotate view
    // m_view =
    //     glm::lookAt(glm::vec3(1.2f, 1.2f, 1.2f), glm::vec3(0.0f, 0.0f, 0.0f),
    //                 glm::vec3(0.0f, 0.0f, 1.0f));
    // m_view = glm::rotate(m_view, delta, glm::vec3(1.0f, 1.0f, 0.0f));

    // update view
    // glUniformMatrix4fv(m_uniView, 1, GL_FALSE, glm::value_ptr(m_view));
  }

  void tick(Fatty::ThreeDState &) {
    // TODO: move to fatty
    auto now = std::chrono::high_resolution_clock::now();
    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(
                      now - m_startTimestamp)
                      .count();

    glUniform1f(m_uniTime, delta);

    updateBackgroundColor();

    updateTransforms(delta);
  }

  void draw(Fatty::ThreeDState &state) {
    // Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    {
      glEnable(GL_STENCIL_TEST);

      // Draw floor
      {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Always set to 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glDepthMask(GL_FALSE);
        glDrawArrays(GL_TRIANGLES, 36, 6);
        glDepthMask(GL_TRUE);
      }

      // Draw reflection
      {
        glUniform3f(m_uniColorOverride, 0.3, 0.3, 0.3);
        glStencilFunc(GL_EQUAL, 1, 0xFF);  // Pass test if stencil value is 1
        glStencilMask(0x00);  // Don't write anything to stencil buffer
        m_model = glm::scale(glm::translate(m_model, glm::vec3(0, 0, -1)),
                             glm::vec3(1, 1, -1));
        glUniformMatrix4fv(m_uniModel, 1, GL_FALSE, glm::value_ptr(m_model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glStencilMask(0xFF);  // Set back to default
        glUniform3f(m_uniColorOverride, 1.0, 1.0, 1.0);
      }

      glDisable(GL_STENCIL_TEST);
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    checkGLError();
  }
};

Game::Game() : m_impl(new Impl) {}

Game::~Game() = default;

void Game::create(Fatty::ThreeDState &s) { m_impl->create(s); }

void Game::destroy(Fatty::ThreeDState &s) { m_impl->destroy(s); }

void Game::pause(Fatty::ThreeDState &s) { m_impl->pause(s); }

void Game::resume(Fatty::ThreeDState &s) { m_impl->resume(s); }

void Game::keyPressed(Fatty::ThreeDState &s, const sf::Event::KeyEvent &e) {
  m_impl->keyPressed(s, e);
}

void Game::keyReleased(Fatty::ThreeDState &s, const sf::Event::KeyEvent &e) {
  m_impl->keyReleased(s, e);
}

void Game::tick(Fatty::ThreeDState &s) { m_impl->tick(s); }

void Game::draw(Fatty::ThreeDState &s) { m_impl->draw(s); }

}  // namespace Sample