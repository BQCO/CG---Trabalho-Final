#define _CRT_SECURE_NO_WARNINGS
#define PI 3.141592654

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>
#include "RgbImage.h"

using namespace std;

// Modelagem Formiga
// Corpo
float tamanhoTorax = 2.0;
float tamanhoAbdomen = 3.0;
float tamanhoCabeca = 1.5;

//Pernas
float raioPata = 0.2;
float tamanhoCoxa = 2.0;
float tamanhoCanela = 2.5;

//Antenas
float raioAntena = 0.08;
float tamanhoAntena = 1.5;

//Angulos iniciais
float anguloCoxaZ = 30.0;
float anguloCanelaZ = -80.0;

//Variávies para Câmera
float eyeDistance = 20.0;
float eyeX, eyeY, eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 15.0;

//Quadrics para desenhar as formas (esferas, cilindros)
GLUquadric *quadSphere;
GLUquadric *quadCylinder;

//Funções de Desenho 
void drawEsfera(float diamentro){
    gluSphere(quadSphere, diamentro, 40, 40);
}

void drawCylinder(float diamentro, float largura){
    gluCylinder(quadCylinder, diamentro, diamentro, largura, 40, 40);
}

void drawMembroArticulado(float raio, float tamParte1, float tamParte2, float anguloBase, float anguloArticulacao){
    glPushMatrix();
        //Coxa/Base Antena
        glRotatef(anguloBase, 0.0f, 1.0f, 0.0f);
        drawCylinder(raio, tamParte1);
        //Articulação
        glTranslatef(0.0f, 0.0f, tamParte1);
        drawEsfera(raio*1.2);
        //Canela/Ponta Antena
        glRotatef(anguloArticulacao, 1.0f, 1.0f, 0.0f);
        drawCylinder(raio, tamParte2);
    glPopMatrix();
}

//Inicialização
void initRendering(void){
    quadSphere = gluNewQuadric();
    quadCylinder = gluNewQuadric();
    glEnable(GL_DEPTH_TEST); //Faz o que está na frente cobrir o que está atras, gerando profundidade

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = {10.0f, 10.0f, 10.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void handleResize(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/ (float)h, 1.0, 50.0);
}

void handleKeypress(unsigned char key, int x, int y){
    switch(key){
        case 27:
            exit(0);
        case 'w':
            if (viewAngleZ < 175) viewAngleZ += 3;
            glutPostRedisplay();
            break;
        case 's':
            if (viewAngleZ > 5) viewAngleZ -= 3;
            glutPostRedisplay();
            break;
        case 'a': 
            viewAngleX -= 3;
            glutPostRedisplay();
            break;
        case 'd':
            viewAngleX += 3;
            glutPostRedisplay();
            break;
        case 'q':
            if(eyeDistance > 5.0) eyeDistance -= 1.0;
            glutPostRedisplay();
            break;
        case 'e':
            if(eyeDistance < 50.0) eyeDistance += 1.0;
            glutPostRedisplay();
            break;
    }
}

void drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Configuração Camera
    eyeX = eyeDistance * cos(viewAngleZ*PI /180) * cos(viewAngleX*PI /180);
    eyeY = eyeDistance * cos(viewAngleZ*PI /180) * sin(viewAngleX*PI /180);
    eyeZ = eyeDistance * sin(viewAngleZ*PI /180);

    if (viewAngleZ < 90)
        gluLookAt(eyeX,eyeY,eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    else
        gluLookAt(eyeX,eyeY,eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

    glColor3f(0.8f, 0.8f, 0.8f); //Dps vou qrr mudar a cor

    //Desenho da Formiga
    glPushMatrix(); //Salva a posição original (torax no centro)

        //Torax
        drawEsfera(tamanhoTorax);

        //Pata Dianteira Direita
        glPushMatrix();
            glTranslatef(tamanhoTorax/2.0, 0.0f, 0.0f);
            glRotatef(45.0, 0.0f, 0.0f, 1.0f);
            glRotatef(90.0, 1.0f, 0.0f, 0.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaZ, anguloCanelaZ);
        glPopMatrix();
        //Pata Dianteira Esquerda
        glPushMatrix();
            glTranslatef(tamanhoTorax/2.0, 0.0f, 0.0f);
            glRotatef(135.0, 0.0f, 0.0f, 1.0f);
            glRotatef(90.0, 1.0f, 0.0f, 0.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaZ, anguloCanelaZ);
        glPopMatrix();
        //Pata Central Direita
        glPushMatrix();
            glRotatef(0.0, 0.0f, 0.0f, 1.0f);
            glRotatef(90.0, 1.0f, 0.0f, 0.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaZ, anguloCanelaZ);
        glPopMatrix();
        //Pata Central Esquerda
        glPushMatrix();
            glRotatef(180.0, 0.0f, 0.0f, 1.0f);
            glRotatef(90.0, 1.0f, 0.0f, 0.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaZ, anguloCanelaZ);
        glPopMatrix();
        //Pata Traseira Direita
        glPushMatrix();
            glTranslatef(-tamanhoTorax/2.0, 0.0f, 0.0f);
            glRotatef(-45.0, 0.0f, 0.0f, 1.0f);
            glRotatef(90.0, 1.0f, 0.0f, 0.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaZ, anguloCanelaZ);
        glPopMatrix();
        //Pata Traseira Esquerda
        glPushMatrix();
            glTranslatef(-tamanhoTorax/2.0, 0.0f, 0.0f);
            glRotatef(225.0, 0.0f, 0.0f, 1.0f);
            glRotatef(90.0, 1.0f, 0.0f, 0.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaZ, anguloCanelaZ);
        glPopMatrix();

        //Abdomen
        glPushMatrix();
            //movemos para trás (eixoX negativo)
            glTranslatef(-(tamanhoTorax + tamanhoAbdomen/2.0), 0.0f, 0.0f);
            drawEsfera(tamanhoAbdomen);
        glPopMatrix(); //Volta ao referencial do torax

        //Cabeça
        glPushMatrix();
            //movemos para frente (eixoX positivo)
            glTranslatef((tamanhoTorax + tamanhoCabeca/2.0), 0.0f, 0.0f);
            drawEsfera(tamanhoCabeca);

            //Antena Direita
            glPushMatrix();
                glTranslatef(tamanhoCabeca/2.0, 0.0f, tamanhoCabeca/2.0);
                glRotatef(-45.0, 0.0f, 1.0f, 0.0f);
                drawMembroArticulado(raioAntena, tamanhoAntena, tamanhoAntena/2.0, 30.0, 45.0);
            glPopMatrix();

            //Antena Esquerda
            glPushMatrix();
                glTranslatef(tamanhoCabeca/2.0, 0.0f, -tamanhoCabeca/2.0);
                glRotatef(45.0, 0.0f, 1.0f, 0.0f);
                drawMembroArticulado(raioAntena, tamanhoAntena, tamanhoAntena/2.0, -30.0, 45.0);
            glPopMatrix();
        glPopMatrix();
    
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Formiga Robo 3D");

    initRendering();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeypress);

    glutMainLoop();
    return 0;
}