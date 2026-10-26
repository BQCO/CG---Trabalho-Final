#define _CRT_SECURE_NO_WARNINGS
#define PI 3.141592654

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>
#include "RgbImage.h"

using namespace std;

// Modelagem Formiga
// Corpo: raios dos elipsoides nos eixos X, Y e Z
float tamanhoTorax[] = {1.30f, 1.05f, 0.85f};
float tamanhoAbdomen[] = {1.60f, 1.05f, 1.10f};
float tamanhoCabeca[] = {1.00f, 0.85f, 0.95f};
float alturaCorpo = 2.15f;

//Pernas
float raioPata = 0.12f;
float raioBasePata = 0.23f;
float tamanhoCoxa = 1.20f;
float tamanhoCanela = 1.70f;

//Antenas
float raioAntena = 0.08;
float tamanhoAntena = 1.5;

// Angulos em torno do eixo Y local: coxa 20 graus abaixo da horizontal.
// A dobra da canela deixa a borda da ponta aproximadamente em Z = 0.
float anguloCoxaY = 110.0f;
float anguloCanelaY = 40.5f;

//Variávies para Câmera
float eyeDistance = 13.0;
float eyeX, eyeY, eyeZ;
float viewAngleX = 45.0;
float viewAngleZ = 25.0;

//Quadrics para desenhar as formas (esferas, cilindros)
GLUquadric *quadSphere;
GLUquadric *quadCylinder;

//Funções de Desenho 
void drawEsfera(float raio){
    gluSphere(quadSphere, raio, 40, 40);
}

void drawCylinder(float raio, float comprimento){
    // O eixo longitudinal do cilindro da GLU e o Z local positivo.
    gluCylinder(quadCylinder, raio, raio, comprimento, 40, 40);
}

void drawElipsoide(float raioX, float raioY, float raioZ){
    glPushMatrix();
        // A escala afeta somente a esfera, preservando o referencial dos filhos.
        glScalef(raioX, raioY, raioZ);
        drawEsfera(1.0f);
    glPopMatrix();
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
        // O segundo segmento herda a rotacao da base e dobra no joelho.
        glRotatef(anguloArticulacao, 0.0f, 1.0f, 0.0f);
        drawCylinder(raio, tamParte2);
    glPopMatrix();
}

//Inicialização
void initRendering(void){
    quadSphere = gluNewQuadric();
    quadCylinder = gluNewQuadric();
    glEnable(GL_DEPTH_TEST); //Faz o que está na frente cobrir o que está atras, gerando profundidade
    glEnable(GL_NORMALIZE); // Mantem as normais unitarias apos a escala dos elipsoides.
    glEnable(GL_COLOR_MATERIAL); // Permite distinguir os olhos pela cor.

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
    eyeZ = alturaCorpo + eyeDistance * sin(viewAngleZ*PI /180);

    if (viewAngleZ < 90)
        gluLookAt(eyeX,eyeY,eyeZ, 0.0, 0.0, alturaCorpo, 0.0, 0.0, 1.0);
    else
        gluLookAt(eyeX,eyeY,eyeZ, 0.0, 0.0, alturaCorpo, 0.0, 0.0, -1.0);

    glColor3f(0.8f, 0.8f, 0.8f); //Dps vou qrr mudar a cor

    //Desenho da Formiga
    glPushMatrix(); // Torax na origem local; o chao corresponde ao Z global = 0.
        glTranslatef(0.0f, 0.0f, alturaCorpo);

        //Torax
        drawElipsoide(tamanhoTorax[0], tamanhoTorax[1], tamanhoTorax[2]);

        //Pata Dianteira Direita
        glPushMatrix();
            glTranslatef(0.85f, -1.00f, -0.20f);
            drawEsfera(raioBasePata);
            glRotatef(-65.0f, 0.0f, 0.0f, 1.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaY, anguloCanelaY);
        glPopMatrix();
        //Pata Dianteira Esquerda
        glPushMatrix();
            glTranslatef(0.85f, 1.00f, -0.20f);
            drawEsfera(raioBasePata);
            glRotatef(65.0f, 0.0f, 0.0f, 1.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaY, anguloCanelaY);
        glPopMatrix();
        //Pata Central Direita
        glPushMatrix();
            glTranslatef(0.00f, -1.00f, -0.20f);
            drawEsfera(raioBasePata);
            glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaY, anguloCanelaY);
        glPopMatrix();
        //Pata Central Esquerda
        glPushMatrix();
            glTranslatef(0.00f, 1.00f, -0.20f);
            drawEsfera(raioBasePata);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaY, anguloCanelaY);
        glPopMatrix();
        //Pata Traseira Direita
        glPushMatrix();
            glTranslatef(-0.85f, -1.00f, -0.20f);
            drawEsfera(raioBasePata);
            glRotatef(-115.0f, 0.0f, 0.0f, 1.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaY, anguloCanelaY);
        glPopMatrix();
        //Pata Traseira Esquerda
        glPushMatrix();
            glTranslatef(-0.85f, 1.00f, -0.20f);
            drawEsfera(raioBasePata);
            glRotatef(115.0f, 0.0f, 0.0f, 1.0f);
            drawMembroArticulado(raioPata, tamanhoCoxa, tamanhoCanela, anguloCoxaY, anguloCanelaY);
        glPopMatrix();

        //Abdomen
        glPushMatrix();
            //movemos para trás (eixoX negativo)
            glTranslatef(-2.75f, 0.0f, 0.10f);
            drawElipsoide(tamanhoAbdomen[0], tamanhoAbdomen[1], tamanhoAbdomen[2]);
        glPopMatrix(); //Volta ao referencial do torax

        //Cabeça
        glPushMatrix();
            //movemos para frente (eixoX positivo)
            glTranslatef(2.15f, 0.0f, 0.18f);
            drawElipsoide(tamanhoCabeca[0], tamanhoCabeca[1], tamanhoCabeca[2]);

            // Olhos simetricos: a escala da cabeca nao altera suas esferas.
            glColor3f(0.05f, 0.05f, 0.05f);
            glPushMatrix();
                glTranslatef(0.55f, -0.67f, 0.25f);
                drawEsfera(0.22f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.55f, 0.67f, 0.25f);
                drawEsfera(0.22f);
            glPopMatrix();
            glColor3f(0.8f, 0.8f, 0.8f);

            //Antena Direita
            glPushMatrix();
                glTranslatef(0.55f, -0.42f, 0.60f);
                glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
                drawMembroArticulado(raioAntena, tamanhoAntena, tamanhoAntena/2.0, 30.0, 45.0);
            glPopMatrix();

            //Antena Esquerda
            glPushMatrix();
                glTranslatef(0.55f, 0.42f, 0.60f);
                glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
                drawMembroArticulado(raioAntena, tamanhoAntena, tamanhoAntena/2.0, 30.0, 45.0);
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
