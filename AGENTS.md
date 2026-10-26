
# Projeto: Formiga Robótica 3D

## Objetivo
Desenvolver em dupla uma formiga robótica tridimensional para o trabalho final da disciplina de Computação Gráfica, utilizando OpenGL tradicional.

## Materiais de referência
- referencias/CG20261_Trabalho_Final.pdf: requisitos obrigatórios.
- referencias/CG_20261_10_Introdução ao OpenGL.pdf: GLUT, callbacks, interação e temporizador.
- referencias/CG_20261_11_Modelagem e Visualização no OpenGL.pdf: transformações, pilha de matrizes, modelagem e projeção.
- referencias/CG_20261_12_Iluminação e Texturas no OpenGL.pdf: materiais, iluminação, texturas e quádricas.
- referencias/notas_opengl.md: resumo prático dos conceitos e exemplos relevantes.

Consulte as referências relevantes para cada tarefa. Se não conseguir ler alguma parte do PDF, informe a limitação em vez de inventar seu conteúdo.

## Restrições técnicas
- Linguagem: C++.
- Utilizar somente OpenGL, GLUT, GLU e a biblioteca RgbImage fornecida pelo professor, além da biblioteca padrão necessária.
- Preferir glPushMatrix, glPopMatrix, glTranslatef, glRotatef, glScalef, gluSphere e gluCylinder.
- Não utilizar shaders, GLM, motores gráficos ou outras bibliotecas de modelagem.
- Preservar o estilo didático dos códigos das aulas 10, 11 e 12.

## Modelagem
- Eixo X positivo: frente; X negativo: traseira.
- Eixo Y: laterais; eixo Z positivo: para cima.
- Tórax como referência central.
- Formiga com cabeça, dois olhos, duas antenas, tórax, abdômen e seis pernas articuladas.
- Utilizar transformações hierárquicas para as articulações.

## Método de trabalho
- Modificar o código existente em formiga.cpp, evitando reescritas desnecessárias.
- Trabalhar em etapas: geometria, articulações, caminhada, texturas/iluminação e revisão final.
- Alterar somente o necessário para a tarefa solicitada.
- Explicar em português as principais alterações e seus conceitos de Computação Gráfica.
- Não implementar etapas futuras sem solicitação.
- Não modificar os arquivos de RgbImage sem necessidade comprovada.
- Informar o que foi testado e o que depende de verificação visual.
- Preservar os comandos existentes, salvo instrução explícita em contrário.
  