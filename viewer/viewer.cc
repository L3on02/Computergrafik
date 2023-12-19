#include <GL/glew.h> 
#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <array>
#include "wavefront.h"

const int window_width = 1024;
const int window_height = 768;

SDL_Window * window = nullptr;
SDL_GLContext context;

void init() {
  window = nullptr;
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
      std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
      throw EXIT_FAILURE;
  } 
  window = SDL_CreateWindow( "Simple OpenGL/Wavefront Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  if( window == nullptr ) {
      std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      throw EXIT_FAILURE;
  }
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  context = SDL_GL_CreateContext(window);

  GLenum err = glewInit(); // to be called after OpenGL render context is created
  if (GLEW_OK != err) {
    std::cerr << "Error: " <<  glewGetErrorString(err) << std::endl;
  }

  glEnable(GL_DEPTH_TEST); 

}

void exit() {
  SDL_GL_DeleteContext(context);
  if (window != nullptr) SDL_DestroyWindow(window);

  SDL_Quit();
}

void swap_window() {
  SDL_GL_SwapWindow(window);
}

void delay(int ms) {
  SDL_Delay(ms);
}


void compile_shader(GLint shader, const char * source) {
  glShaderSource(shader, 1, &source, NULL) ;
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    std::cerr << " Shader did not compile." << std::endl;
    char log[512];
    glGetShaderInfoLog( shader, 512, NULL, log) ;
    std::cerr << log << std::endl;
    throw EXIT_FAILURE;
  }
}

void check_link_status(GLint shader_program) {
  GLint status;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &length);
    GLchar *log = new char[length + 1];
    glGetProgramInfoLog(shader_program, length, &length, &log[0]);
    std::cerr << log << std::endl;
    throw EXIT_FAILURE;
  }
}


GLuint shaderProgram;

void create_shaders() {

  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n" 
    "layout (location = 1) in vec3 incolor;\n"
    "layout (location = 2) in vec3 innormal;\n"
    "out vec3 color;\n"
    "out vec4 normal;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "gl_Position = model * vec4(position, 1.0);\n"
    "color = incolor;\n"
    "normal = normalize( model * vec4(innormal, 1.0));\n"
    "}\0";

  // direction to light source is hard coded: (0,1,-4)  
  // Lambertian shading used for vertices of triangle
  // cause during rasterization colors are interpolated, the result is Gouraud-Shading
  const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 outColor;\n"
  "in vec3 color;\n"
  "in vec4 normal;\n"
  "void main () {\n"
  "  outColor = vec4(color * (0.3 + 0.7 * max(0.0, dot(normal, normalize( vec4(0.0, 1.0, -4.0, 0.0))))) , 1.0);\n"
  "}\n\0";
  
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER) ;
  compile_shader(vertexShader, vertexShaderSource);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER) ;
  compile_shader(fragmentShader, fragmentShaderSource);

  shaderProgram = glCreateProgram() ;
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  check_link_status(shaderProgram);
}

void switch_background_color() {
  SDL_Delay(1000);
  glClearColor ( 1.0, 0.0, 0.0, 1.0 );
  glClear ( GL_COLOR_BUFFER_BIT 
            | GL_DEPTH_BUFFER_BIT); // clear depth buffer from last frame!
  swap_window();
  SDL_Delay(1000);
}


// view assumes the vertices are in canonical coordinates
// view direction is along the -z axis
// scale can be used to scale the coordinates up or down to canonical coords.
void view(std::vector<float> & vertices, float scale) {
  static float PI = 3.1415f;
  
  glClearColor ( 0.5, 0.5, 0.5, 1.0 );
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  GLuint vao;  
  glGenVertexArrays(1, &vao); // create a vertex array object (VAO)
  glBindVertexArray(vao); // make vao active

  GLuint vbo;
  glGenBuffers(1, &vbo); // create a new vertex buffer object (VBO)
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // make it active  
  
  // transfer data from RAM to GPU buffer
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(float), 
               vertices.data(),
               GL_STATIC_DRAW);

  /* the following layout is assumed for each vertex of a face (triangle)
    x1 y1 z1 nx1 ny1 nz1 r1 g1 b1 
  */

  // active vbo will be associated to active vao by the following calls
  glVertexAttribPointer(0,
                        3,        // number of vertices (components)
                        GL_FLOAT, //
                        GL_FALSE, // no normalization
                        9 * sizeof(float), // no of bytes between each vertice (component)
                        (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,
                        3,        // no vertices (components)
                        GL_FLOAT, //
                        GL_FALSE, // no normalization
                        9 * sizeof(float), // no of bytes between each color (component)
                        (void*)(6 * sizeof(float)) ); // offset to color data in the vbo
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2,
                        3,        // no vertices (components)
                        GL_FLOAT, //
                        GL_FALSE, // no normalization
                        9 * sizeof(float), // no of bytes between each color (component)
                        (void*)(3 * sizeof(float)) ); // offset to color data in the vbo
  glEnableVertexAttribArray(2);

  
  glUseProgram(shaderProgram);

  for (float angle = 0.0f; angle < 2 * PI; angle += PI / 32.0) {
    glm::mat4 model = glm::mat4(1.0);
    model = glm::scale( model, glm::vec3(scale, scale, scale) );
    model = glm::rotate (model, angle ,
                         glm::vec3 (0.0f, 1.0f, 1.0f) ) ; // rotate along x and z axis


    glClearColor ( 0.5, 0.5, 0.5, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    GLint uniformView = glGetUniformLocation (shaderProgram, "model");
    glUniformMatrix4fv(uniformView, 1 , GL_FALSE , glm::value_ptr(model) ) ;  
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 9);
    swap_window();
  
    delay(100);
  }
  delay(2000);
}


Material default_material = { {1.0f, 1.0f, 1.0f} };

/* assumes that the all i faces are triangles, more than three vertices of a face are ignored
   creates a vector of floats with the following layout
   [ face1 | face2 | ... | facei ]
   where each face consists of a sequence of 3 (=number of triangle vertices) * 9 floats
   x1 y1 z1 nx1 ny1 nz1 r1 g1 b1 | x2 y2 z2 nx2 ny2 nz2 r2 g2 b2 | x3 y3 z3 nx3 ny3 nz3 r3 g3 b3
     vertex   normal     color       vertex    normal     color     vertex    normal     color
     
  if no material is given for a face, then the default_material is used
  
  if you want to use other layouts, then adapt this code
*/     
std::vector<float> create_vertices(WavefrontImporter & wi) {
  std::vector<float> vertices;
  
  for (Face face : wi.get_faces() ) {
    for (ReferenceGroup group : face.reference_groups ) {
      for (size_t i = 0; i < 3; i++) {
        vertices.push_back( group.vertice[i]);
      }
      for (size_t i = 0; i < 3; i++) {
        vertices.push_back( group.normal[i] );
      }
      if (face.material == nullptr) face.material = &default_material;
      for (size_t i = 0; i < 3; i++) {
        vertices.push_back( face.material->ambient[i]);
      }
    } 
  }
  return vertices;
}

int main(int argc, char** argv) {
  // entnehme einen filenamen aus argv, wenn argv etwas enthält
  std::string file = "teapot.obj";
  if (argc > 1) file = argv[1];

  std::fstream in(file);
  WavefrontImporter wi( in );
  wi.parse();
  std::vector<float> vertices = create_vertices(wi);
  init();
  create_shaders();   
  view(vertices, 0.25f); // adapt the scale factor to the objects local coordinates
                         // such that the scaled vertex coordinates fit into the canonical box [-1,1]^3
  exit();
  
  return EXIT_SUCCESS;
}
