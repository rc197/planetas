#define __USE_MINGW_ANSI_STDIO 0

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shaders.h"
#include "Mesh.h"
#include "Texture.h"

#define I glm::mat4(1.0f)

void funInit();
void funDestroy();
void funReshape(int w, int h);
void funDisplay();

void funKeyboard(unsigned char key, int x, int y);
void funSpecial(int key, int x, int y);
void funMouse(int button, int state, int x, int y);
void funMotion(int x, int y);
void funTimer(int value);


void setLights(glm::mat4 P, glm::mat4 V);
void drawPlanetas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSun(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawMercury(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVenus(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawEarth(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawMars(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawJupiter(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSaturn(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawUranus(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawNeptune(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPluto(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Viewport
int w = 500;
int h = 500;
GLfloat distancia = 30.0f;
// Dimensiones
GLfloat radSol = 695.508 / 15;
GLfloat radMer = 2.44 / 3;
GLfloat radVen = 6.052 / 3;
GLfloat radTier = 6.371 / 3;
GLfloat radMar = 3.39 / 3;
GLfloat radJup = 69.911 / 3;
GLfloat radSat = 58.23273;
GLfloat radUr = 25.362 / 3;
GLfloat radNep = 24.622 / 3;
GLfloat radPlu = 1.188 / 3;

GLfloat distMer = 5.791f;
GLfloat distVen = 108.2f;
GLfloat distTier = 149.6f;
GLfloat distMar = 227.9f;
GLfloat distJup = 778.5f;
GLfloat distSat = 1434.0f;
GLfloat distUr = 2871.0f;
GLfloat distNep = 4495.0f;
GLfloat distPlu = 5934.456f;


// Animaciones
GLint speed = 50;
GLfloat rotPlaneta = 0.0f;
bool dibujar = true;
GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat desZ = 0.0f;
GLfloat zoom = 1.0f;
GLfloat Xc = 0.0f;
GLfloat Yc = 0.0f;
bool leftbutton = false;

// Shaders
Shaders *objShaders;

// Modelos
Mesh *plane;
Mesh *sphere;

// Luces
#define NLD 1
#define NLP 1
#define NLF 2
Light lightG;
Light lightD[NLD];
Light lightP[NLP];
Light lightF[NLF];

// Materiales
Material matLuces;
Material matRuby;
Material matGold;

// Texturas
Texture *texSun;
Texture *texMercury;
Texture *texVenus;
Texture *texEarth;
Texture *texMars;
Texture *texJupiter;
Texture *texSaturn;
Texture *texUranus;
Texture *texNeptune;
Texture *texPluto;

int main(int argc, char** argv) {

    // Inicializamos GLUT y el contexto de OpenGL
    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    // Inicializamos el FrameBuffer y la ventana   
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Sesion 07");

    // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

    // Inicializaciones específicas
    funInit();

    // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutKeyboardFunc(funKeyboard);
    glutSpecialFunc(funSpecial);
    glutMouseFunc(funMouse);
    glutMotionFunc(funMotion);
    glutTimerFunc(speed, funTimer, 0);

    // Bucle principal
    glutMainLoop();

    // Liberamos memoria
    funDestroy();

    return 0;
}

void funInit() {

    // Test de profundidad
    glEnable(GL_DEPTH_TEST);

    // Shaders
    objShaders = new Shaders("resources/shaders/vshader_phong.glsl", "resources/shaders/fshader_phong.glsl");

    // Modelos
    plane = new Mesh("resources/models/plane.obj");
    plane->createVao();
    sphere = new Mesh("resources/models/sphere.obj");
    sphere->createVao();

    // Texturas
    texSun = new Texture(2, "resources/textures/sunmap.jpg");
    texMercury = new Texture(2, "resources/textures/mercurymap.jpg");
    texVenus = new Texture(2, "resources/textures/venusmap.jpg");
    texEarth = new Texture(2, "resources/textures/imgEarth.bmp");
    texMars = new Texture(2, "resources/textures/mars_1k_color.jpg");
    texJupiter = new Texture(2, "resources/textures/jupitermap.jpg");
    texSaturn = new Texture(2, "resources/textures/saturnmap.jpg");
    texUranus = new Texture(2, "resources/textures/uranusmap.jpg");
    texNeptune = new Texture(2, "resources/textures/neptunemap.jpg");
    texPluto = new Texture(2, "resources/textures/plutomap1k.jpg");

    // Luz ambiental global
    lightG.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    // Luces direccionales
    lightD[0].direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    lightD[0].ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    lightD[0].diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    lightD[0].specular = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);

    // Luces posicionales
    lightP[0].position = glm::vec3(0.0f, 3.0f, 3.0f);
    lightP[0].ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    lightP[0].diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    lightP[0].specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    lightP[0].c0 = 1.00f;
    lightP[0].c1 = 0.22f;
    lightP[0].c2 = 0.20f;

    // Luces focales
    lightF[0].position = glm::vec3(-2.0f, 2.0f, 5.0f);
    lightF[0].direction = glm::vec3(2.0f, -2.0f, -5.0f);
    lightF[0].ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    lightF[0].diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    lightF[0].specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    lightF[0].innerCutOff = 10.0f;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0f;
    lightF[0].c0 = 1.000f;
    lightF[0].c1 = 0.090f;
    lightF[0].c2 = 0.032f;
    lightF[1].position = glm::vec3(2.0f, 2.0f, 5.0f);
    lightF[1].direction = glm::vec3(-2.0f, -2.0f, -5.0f);
    lightF[1].ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    lightF[1].diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    lightF[1].specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    lightF[1].innerCutOff = 5.0f;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0f;
    lightF[1].c0 = 1.000f;
    lightF[1].c1 = 0.090f;
    lightF[1].c2 = 0.032f;

    // Materiales
    matLuces.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    matLuces.diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    matLuces.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    matLuces.shininess = 10.0f;
    matRuby.ambient = glm::vec4(0.174500f, 0.011750f, 0.011750f, 0.55f);
    matRuby.diffuse = glm::vec4(0.614240f, 0.041360f, 0.041360f, 0.55f);
    matRuby.specular = glm::vec4(0.727811f, 0.626959f, 0.626959f, 0.55f);
    matRuby.shininess = 76.8f;
    matGold.ambient = glm::vec4(0.247250f, 0.199500f, 0.074500f, 1.00f);
    matGold.diffuse = glm::vec4(0.751640f, 0.606480f, 0.226480f, 1.00f);
    matGold.specular = glm::vec4(0.628281f, 0.555802f, 0.366065f, 1.00f);
    matGold.shininess = 51.2f;

}

void funDestroy() {

    delete objShaders;
    delete plane;
    delete sphere;

}

void funReshape(int wnew, int hnew) {

    // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

    // Captura de w y h
    w = wnew;
    h = hnew;

}

void funDisplay() {

    // Borramos el buffer de color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matriz de Proyección P (Perspectiva)
    GLfloat fovy = 150.0f;
    GLfloat nplane = 0.1f;
    GLfloat fplane = 6000.0f;
    GLfloat aspectRatio = (GLfloat) w / (GLfloat) h;
    glm::mat4 P = glm::perspective(glm::radians(fovy * zoom), aspectRatio, nplane, fplane);

    // Matriz de Vista V (Cámara)
    GLfloat PI = glm::pi<float>();
    GLfloat x = 40.0f * glm::cos(Yc * PI / 180.0f) * glm::sin(Xc * PI / 180.0f);
    GLfloat y = 50.0f* glm::cos(Yc * PI / 180.0f) * glm::cos(Xc * PI / 180.0f);
    GLfloat z = 40.0f ;
    glm::vec3 pos(x, y, z);
    glm::vec3 lookat(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::mat4 V = glm::lookAt(pos, lookat, up);

    // Fijamos los shaders a utilizar junto con la posición de la cámara
    objShaders->use();
    objShaders->setVec3("uposc", pos);

    // Fijamos las luces
    setLights(P, V);

    // Dibujamos la escena

    glm::mat4 Ry = glm::rotate(I, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Rx = glm::rotate(I, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f, 0.0f, desZ));
    drawPlanetas(P, V, Tz * Rx * Ry);

    // Intercambiamos los buffers
    glutSwapBuffers();

}

void setLights(glm::mat4 P, glm::mat4 V) {

    objShaders->setLight("ulightG", lightG);
    for (int i = 0; i < NLD; i++) objShaders->setLight("ulightD[" + std::to_string(i) + "]", lightD[i]);
    for (int i = 0; i < NLP; i++) objShaders->setLight("ulightP[" + std::to_string(i) + "]", lightP[i]);
    for (int i = 0; i < NLF; i++) objShaders->setLight("ulightF[" + std::to_string(i) + "]", lightF[i]);

    for (int i = 0; i < NLP; i++) {
        glm::mat4 M = glm::scale(glm::translate(I, lightP[i].position), glm::vec3(0.10f));
        //drawObject(sphere,matLuces,texLight,0.0f,P,V,M);
    }

    for (int i = 0; i < NLF; i++) {
        glm::mat4 M = glm::scale(glm::translate(I, lightF[i].position), glm::vec3(0.025f));
        //drawObject(sphere,matLuces,texLight,0.0f,P,V,M);
    }

}

void drawPlanetas(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    drawSun(matRuby, texSun, 0.75f, P, V, M);
    drawMercury(matRuby, texMercury, 1.0f, P, V, M);
    drawVenus(matRuby, texVenus, 1.0f, P, V, M);
    drawEarth(matRuby, texEarth, 1.0f, P, V, M);
    drawMars(matRuby, texMars, 1.0f, P, V, M);
    drawJupiter(matRuby, texJupiter, 1.0f, P, V, M);
    drawSaturn(matRuby, texSaturn, 1.0f, P, V, M);
    drawUranus(matRuby, texUranus, 1.0f, P, V, M);
    drawNeptune(matRuby, texNeptune, 1.0f, P, V, M);
    drawPluto(matRuby, texPluto, 1.0f, P, V, M);
}

void drawSun(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    M = glm::translate(M, glm::vec3(-0.5, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(5.0f, 5.0f, 5.0f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawMercury(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(distancia/2, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(0.6f, 0.6f, 0.6f));
    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawVenus(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(2*distancia/2, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(1.7f, 1.7f, 1.7f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawEarth(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(3*distancia/2, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(1.9f, 1.9f, 1.9f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawMars(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(4*distancia/2, 0.0f, 0.0f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawJupiter(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(6*distancia, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(3.5f, 3.5f, 3.5f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawSaturn(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(8*distancia, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(3.0f, 3.0f, 3.0f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawUranus(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(10*distancia, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(2.5f, 2.5f, 2.5f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawNeptune(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(12*distancia, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(2.4f, 2.4f, 2.4f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void drawPluto(Material material, Texture *texture, GLfloat mixValue, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    M = glm::rotate(M, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::translate(M, glm::vec3(14*distancia, 0.0f, 0.0f));
    M = glm::scale(M, glm::vec3(0.2f, 0.2f, 0.2f));
    objShaders->setMat4("uN", glm::transpose(glm::inverse(M)));
    objShaders->setMat4("uM", M);
    objShaders->setMat4("uPVM", P * V * M);
    objShaders->setMaterial("umaterial", material);
    texture->Activate();
    objShaders->setTexture("utex", texture->getIdTexture());
    objShaders->setFloat("umixValue", mixValue);
    sphere->render(GL_FILL);
}

void funKeyboard(unsigned char key, int x, int y) {

    /*switch (key) {
        case 'e': desZ -= 0.1f;
            break;
        case 'd': desZ += 0.1f;
            break;
        default: desZ = 0.0f;
            break;
    }
    glutPostRedisplay();*/

}

void funSpecial(int key, int x, int y) {

    /*switch (key) {
        case GLUT_KEY_UP: rotX -= 5.0f;
            break;
        case GLUT_KEY_DOWN: rotX += 5.0f;
            break;
        case GLUT_KEY_LEFT: rotY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT: rotY += 5.0f;
            break;
        default:
            rotX = 0.0f;
            rotY = 0.0f;
            break;
    }
    glutPostRedisplay();*/

}

void funMouse(int button, int state, int x, int y) {

    leftbutton = button == GLUT_LEFT_BUTTON && state == GLUT_DOWN;
    switch (button) {
        case 3: zoom += (zoom < 0.05) ? 0.05f: -0.05f;
            break;
        case 4: zoom += (zoom > 1.55) ? -0.05f: 0.05f;
            break;
    }
    glutPostRedisplay();

}

void funMotion(int x, int y) {

    if (leftbutton) {
        GLfloat Xo = (GLfloat) w / 2.0f;
        GLfloat Yo = (GLfloat) h / 2.0f;
        Xc = ((GLfloat) x - Xo) / -5.0f;
        Yc = (Yo - (GLfloat) y) / -5.0f;
        glutPostRedisplay();
    }

}

void funTimer(int ignore) {

    if (dibujar) {
        rotPlaneta += 1.0f;
        dibujar = false;
    } else {
        rotPlaneta += 1.0f;
        dibujar = true;
    }
    glutPostRedisplay();
    glutTimerFunc(speed, funTimer, 0);

}