#include <iostream>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif
#include "glhelper.h"
#include "camera.h"
#include "mesh.h"

GLuint vao;
GLuint n_elements;

Camera cam;
GLuint program_id;
GLuint program_tbn_id;

GLuint program_btn;

GLuint program_cube_map;
GLuint skyboxVAO;
GLuint cubemapTexture;
// value for the rotation of the model
bool turn = true;
float angle = 0.0f;

// value to display the TBN basis
bool tbn = true;

//bouton 
GLuint buttonVAO;

//affichage des boutons
bool button0Clicked = false;

void initButtons()
{
    glUseProgram(program_btn);
    GLuint buttonColorLocation = glGetUniformLocation(program_btn, "buttonColor");
    if (button0Clicked)
        glUniform3f(buttonColorLocation, 1.0f, 0.0f, 0.0f); // Rouge
    else
        glUniform3f(buttonColorLocation, 0.0f, 1.0f, 0.0f); // Vert


    
    GLfloat buttonVertices[] = {
    0.5f, 0.5f, 0.0f, // Coin inférieur gauche
    0.5f, 0.7f, 0.0f, // Coin supérieur gauche
    0.7f, 0.7f, 0.0f, // Coin supérieur droit
    0.7f, 0.5f, 0.0f  // Coin inférieur droit
    };

    GLuint buttonVBO;
    glGenVertexArrays(1, &buttonVAO);
    glGenBuffers(1, &buttonVBO);

    glBindVertexArray(buttonVAO);

    glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertices), buttonVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*glBegin(GL_QUADS);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, 0.7);
    glVertex2f(0.7, 0.7);
    glVertex2f(0.7, 0.5);
    glEnd();*/

    
    //glBegin(GL_QUADS);
    //glVertex2f(10, 10);
    //glVertex2f(60, 10);
    //glVertex2f(60, 60);
    //glVertex2f(10, 60);
    //glEnd();

}




void init()
{
  program_tbn_id = glhelper::create_program_from_file("./shaders/texture.vert","./shaders/normal.geom", "./shaders/color.frag");
  program_id = glhelper::create_program_from_file("./shaders/texture.vert","./shaders/texture.frag");
  program_btn = glhelper::create_program_from_file("./shaders/basic.vert", "./shaders/basic.frag");
  program_cube_map = glhelper::create_program_from_file("./shaders/cube_map.vert", "./shaders/cube_map.frag");
  glUseProgram(program_id);
  glActiveTexture(GL_TEXTURE0);
  glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Color.png");
  glUniform1i(glGetUniformLocation(program_id, "texture0"), 0);

  glActiveTexture(GL_TEXTURE1);
  glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_AmbientOcclusion.png");
  glUniform1i(glGetUniformLocation(program_id, "texture1"), 1);

  glActiveTexture(GL_TEXTURE2);
  glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Displacement.png");
  glUniform1i(glGetUniformLocation(program_id, "texture2"), 2);

  glActiveTexture(GL_TEXTURE3);
  glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Normal.png");
  glUniform1i(glGetUniformLocation(program_id, "texture3"), 3);

  glActiveTexture(GL_TEXTURE4);
  glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Roughness.png");
  glUniform1i(glGetUniformLocation(program_id, "texture4"), 4);


  glUniform1i(glGetUniformLocation(program_id, "switch_text"), 0);

  Mesh m = Mesh::create_sphere(200, 200);
  n_elements = m.size_element();
  vao = m.load_to_gpu();


  //cubemap
  glUseProgram(program_cube_map);
  std::vector<std::string> faces = {
      "./data/skybox/right.jpg",
          "./data/skybox/left.jpg",
          "./data/skybox/bottom.jpg",
          "./data/skybox/top.jpg",     
          "./data/skybox/front.jpg",
          "./data/skybox/back.jpg"
  };
  cubemapTexture = glhelper::loadCubemap(faces);


  float skyboxVertices[] = {
      // positions          
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f
  };


  // Création du VAO pour le cube de la skybox
  GLuint skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);

  glBindVertexArray(skyboxVAO);

  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

  // Positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  initButtons();

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);



}

static void display_callback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program_cube_map);
    // Désactivez le test de profondeur pour dessiner la skybox derrière tout
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(skyboxVAO);

    // Utilisez le programme de la skybox
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glUniform1i(glGetUniformLocation(program_cube_map, "skybox"), 0);
    
  
    //glhelper::set_uniform_mat4(program_id, "Model", glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    glhelper::set_uniform_mat4(program_cube_map, "Perspective", cam.projection());
    glhelper::set_uniform_mat4(program_cube_map, "View", glm::mat4(glm::mat3(cam.view())));
    // Dessinez le cube de la skybox
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Réactivez le test de profondeur
    glEnable(GL_DEPTH_TEST);


  

  cam.draw_frame();
  
  //bouton
  glUseProgram(program_btn);
  GLuint buttonColorLocation = glGetUniformLocation(program_btn, "buttonColor");
  if (button0Clicked)
      glUniform3f(buttonColorLocation, 1.0f, 0.0f, 0.0f); // Rouge
  else
      glUniform3f(buttonColorLocation, 0.0f, 1.0f, 0.0f); // Vert

  // Liaison du VAO du rectangle
  glBindVertexArray(buttonVAO);

  // Rendu du rectangle
  glDrawArrays(GL_QUADS, 0, 4);

  // Libération du VAO
  glBindVertexArray(0);

  glUseProgram(program_id);
  glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
  glhelper::set_uniform_mat4(program_id, "Model", model);
  glhelper::set_uniform_mat4(program_id, "View", cam.view());
  glhelper::set_uniform_mat4(program_id, "Perspective", cam.projection());
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);

  glm::vec3 view_pos = cam.position();
  glUniform3f(glGetUniformLocation(program_btn, "view_pos"), view_pos.r, view_pos.g, view_pos.b); // Rouge
  
  if(tbn)
  {
    glUseProgram(program_tbn_id);
    glhelper::set_uniform_mat4(program_tbn_id, "Model", model);
    glhelper::set_uniform_mat4(program_tbn_id, "View", cam.view());
    glhelper::set_uniform_mat4(program_tbn_id, "Perspective", cam.projection());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
  }

  CHECK_GL_ERROR();

  
  glBindVertexArray(0);
  glutSwapBuffers();
}

static void keyboard_callback(unsigned char key, int, int)
{
  int viewport[4];
  int polygonMode;

  switch (key)
  {
    case 'o':
      turn = !turn;
   break;
    case 'f':
      tbn = !tbn;
    break;
    case 'w':
      glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
      glPolygonMode(GL_FRONT_AND_BACK, polygonMode == GL_LINE ? GL_FILL : GL_LINE);
   break;
    case 'p':
      glGetIntegerv(GL_VIEWPORT, viewport);
      glhelper::print_screen(viewport[2], viewport[3]);
      break;
    case '0':
        glUniform1i(glGetUniformLocation(program_id, "switch_text"), 0);
        break;
    case '1':
        glUniform1i(glGetUniformLocation(program_id, "switch_text"), 1);
        break;
    case '2':
        glUniform1i(glGetUniformLocation(program_id, "switch_text"), 2);
        break;
    case '3':
        glUniform1i(glGetUniformLocation(program_id, "switch_text"), 3);
        break;
    case '4':
        glUniform1i(glGetUniformLocation(program_id, "switch_text"), 4);
        break;
    case 'q':
    case 'Q':
       
    case 27:
      exit(0);
  }
}

static void reshape_callback(int width, int height)
{
  cam.common_reshape(width,height);
  glViewport(0,0, width, height);
}
static void mouse_callback(int button, int action, int x, int y)
{
    cam.common_mouse(button, action, x, y);

    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
    {
        int viewport[4];
        double modelview[16];
        double projection[16];
        double objX, objY, objZ;

        // Récupérer les matrices modelview, projection et viewport
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        // Inverser la transformation des coordonnées de la souris à l'écran en coordonnées OpenGL
        gluUnProject(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 0.0, modelview, projection, viewport, &objX, &objY, &objZ);

        // Exemple : Vérifier si les coordonnées sont dans la zone d'un bouton
        if (objX >= 0.5 && objX <= 0.7 && objY >= 0.5 && objY <= 0.7)
        {
            button0Clicked = !button0Clicked;
            if (button0Clicked)
            {
                std::cout << "parralax" << std::endl;
                glUniform1i(glGetUniformLocation(program_id, "parallax"), 1);
            }      
            else
            {
                std::cout << "not parralax" << std::endl;
                glUniform1i(glGetUniformLocation(program_id, "parallax"), 0);
            }
                
        }
    }
}

static void motion_callback(int x, int y)
{
  cam.common_motion(x, y);
}
static void timer_callback(int)
{
  if(turn) angle += 0.01;
  glutPostRedisplay();
  glutTimerFunc(25, timer_callback, 0);
}
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(cam.width(), cam.height());
  glutCreateWindow("opengl");
  glutDisplayFunc(display_callback);
  glutMotionFunc(motion_callback);
  glutMouseFunc(mouse_callback);
  glutKeyboardFunc(keyboard_callback);
  glutReshapeFunc(reshape_callback);
  glutTimerFunc(25, timer_callback, 0);
  glewExperimental=true;
  glewInit();

  int major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  std::cout << "OpenGL Version: " << major <<"."<< minor << std::endl;

  init();
  glutMainLoop();

  return 0;
}