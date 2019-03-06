/*******************************************************************************************
   Trabalho de Computação Gráfica
   Integrantes: Everton Camacho, André Macuco, Leonardo Bernal, Jaqueline Tenguam
   				
   Data de entrega: 13/02/2019
   
   Projeto: Flappy Bird 
   Projeções: Ortogonal / Perspectiva
*******************************************************************************************/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"
#include "GL/glu.h"
#include "GL/gl.h"
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>

//Variáveis Globais

GLUquadricObj *quadric;

//Projeção
int tipoProjecao = 1; //1 - Ortogonal, 2 - Perspectiva

//Status do jogo
int start = 0;
int restart = 0;
int pontosAtual = 0;
int melhorPontuacao = 0;

//Canos
float infoCanos[4][3]; //Armazena a posição em x e as alturas dos canos. | posiçãoX | alturaCanoSuperior | alturaCanoInferior
float alturaCanoSuperior, alturaCanoInferior, canoX = -90.0;

//Cores para o passáro
float corPassaro[6][3];
int skin = 5;

//Física do jogo
float k = 0.0f;
float velocidade = -0.1f;
float tempo = 0.1f;
float acel = -0.2f;
float colisaoCanoEmX = -263.5f + 8.0f + 30.0f; //Considera a posição do passáro em x, tomando como base o tamanho do passáro.
int controle = 0;

float nuvemX = -270.0f, nuvemY = 200.0f, nuvemZ = -50.0f;
float nuvens[9][4];

//Textos
char textoPontos[50];
char textoMelhorPontuacao[50];

//Funções
void drawBitmapText(const char *string, float x, float y, float z, float r, float g, float b) {
	const char *c;
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}

void criaChao() {
	//Parte superior
    glColor3ub(60, 179, 113);
    glPushMatrix();
        glTranslatef(0, -190, 0);
        glScalef(40.0f, 1.0f, 50.0f);
        glutSolidCube(20);
    glPopMatrix();

	//Parte inferior
    glColor3ub(233, 194, 166);
    glPushMatrix();
        glTranslatef(0, -250, 0);
        glScalef(8.0f, 1.0f, 10.0f);
        glutSolidCube(100);
    glPopMatrix();
    
    //Prolonga chão
    glColor3ub(60, 179, 113);
    glPushMatrix();
        glTranslatef(600, -190, 0);
        glScalef(40.0f, 1.0f, 50.0f);
        glutSolidCube(20);
    glPopMatrix();
    glColor3ub(233, 194, 166);
    glPushMatrix();
        glTranslatef(600, -250, 0);
        glScalef(8.0f, 1.0f, 10.0f);
        glutSolidCube(100);
    glPopMatrix();
}

void criaCanos() {
	for(int k = 0; k < 4; k++){
		//Parte superior
		glColor3ub(34, 139, 34);
		glPushMatrix();
	        glTranslatef(infoCanos[k][0], 300 , 0);
	        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	        gluCylinder(quadric, 17.5f, 17.5f, infoCanos[k][1], 50, 50);
	    glPopMatrix();
	
	    //Parte inferior
		glColor3ub(34, 139, 34);
		glPushMatrix();
	        glTranslatef(infoCanos[k][0], -180, 0);
	        glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	        gluCylinder(quadric, 17.5, 17.5f, infoCanos[k][2], 50, 50);
	    glPopMatrix(); 
	    
	    //Prolonga parte superior dos canos
		glColor3ub(34, 139, 34);
		glPushMatrix();
	        glTranslatef(infoCanos[k][0], 300.0f, 0.0f);
	        glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	        gluCylinder(quadric, 17.5f, 17.5f, 2500, 50, 50);
	    glPopMatrix();
	}
}


void criaNuvens(){

	for(int n = 0; n < 4; n++){
		glPushMatrix();
			glTranslatef(nuvens[n][0], nuvens[n][1], nuvens[n][2]);
			if(tipoProjecao == 1){
				glScalef(30, 35, 10);
			}else{
				glScalef(10, 20, 10);
			}
		    glBegin(GL_POLYGON);
		        glColor3f(1.0f,1.0f,1.0f);
		        glVertex3f(-2.0,-2.0,-5.0);
		        glVertex3f(-1.0,-1.5,-5.0);
		        glVertex3f(2.0,-2.0,-5.0);
		        glVertex3f(1.2,-1.5,-5.0);
		    glEnd();
		glPopMatrix();
	}
}

void posicaoNuvem(){

	for(int n = 0; n < 4; n++){
		nuvens[n][0] = nuvemX;
		nuvens[n][1] = nuvemY;
		nuvens[n][2] = nuvemZ;
		
		if(n > 1){
			nuvemY -= 70.0f;
		}else {
			nuvemY += 50.0f;
		}
		nuvemX += 200.0f;
	}
}

void informacoesCanos(){
	for(int k = 0; k < 4; k++){
		alturaCanoSuperior = (rand() % 200) + 100.0;
		alturaCanoInferior = 480 - alturaCanoSuperior - 100;
		infoCanos[k][0] = canoX;
		infoCanos[k][1] = alturaCanoSuperior;
		infoCanos[k][2] = alturaCanoInferior;
		canoX += 200.0;
	}
}

void atualizaAlturaCano(int cano){
	float alturaCanoSuperior, alturaCanoInferior, canoX = -90.0;
	
	alturaCanoSuperior = (rand() % 200) + 100.0;
	alturaCanoInferior = 480 - alturaCanoSuperior - 100;
		
	infoCanos[cano][1] = alturaCanoSuperior;
	infoCanos[cano][2] = alturaCanoInferior;
}

void decrementaPosCano(){
	float alturaCanoSuperior, alturaCanoInferior;
	
	infoCanos[0][0] -= 0.25;
	infoCanos[1][0] -= 0.25;
	infoCanos[2][0] -= 0.25;
	infoCanos[3][0] -= 0.25;
	
	if(infoCanos[0][0] == -390){
		infoCanos[0][0] = 410;
		atualizaAlturaCano(0);
	}
		
	if(infoCanos[1][0] == -390){
		infoCanos[1][0] = 410;
		atualizaAlturaCano(1);
	}
	
	if(infoCanos[2][0] == -390){
		infoCanos[2][0] = 410;
		atualizaAlturaCano(2);
	}
	
	if(infoCanos[3][0] == -390){
		infoCanos[3][0] = 410;
		atualizaAlturaCano(3);
	}
}

void criaPassaro(){
	//Corpo
	glColor3ub(corPassaro[skin][0], corPassaro[skin][1], corPassaro[skin][2]);
	glPushMatrix();
	    glTranslatef(-263.5f, 0.0f, 0.0f);
	    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	    glutSolidSphere(15.0f, 8.0f, 20.0f);
	glPopMatrix();
	
	//Bico
	glColor3ub(255, 153, 0);
	glPushMatrix();
	    glTranslatef(-250.0f, 0.0f, 0.0f);
	    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	    glutSolidCone(7.5f, 8.0f, 20.0f, 20.0f);
	glPopMatrix();
}

void limitePuloEmY(){
	k = k >= 300 ? 300 : k;
}

void pulaPassaro(){
	velocidade = 4.5;
}

void carregaMelhorPontuacao(){
	FILE *arq;
	arq = fopen("score.txt", "r");
	
	if(arq == NULL){
		//printf("Erro ao abrir o arquivo!\n");
	}else{
		fscanf(arq,"%d", &melhorPontuacao);
	}
	 
	fclose(arq);
}

void salvaPontuacao(){
	
	FILE *score = fopen("score.txt", "w");
	
	if(score == NULL){
  		//printf("Erro ao abrir o arquivo!\n");
  	}else{
	  	fprintf(score, "%d", pontosAtual);	
  	}
  	
  	fclose(score);
}

void exibeMenuInicial(float x, float y, float z){
	glPushMatrix();
		drawBitmapText("Comandos:", x, y, 0.0f, 0.0f, 0.0f, 0.0f); 
		drawBitmapText("I - Iniciar", x-70.0f, y-35.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		drawBitmapText("S - Sair", x-70.0f, y-55.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		drawBitmapText("P - Alterar camera", x-70.0f, y-75.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		drawBitmapText("C - Alterar skin", x-70.0f, y-95.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void exibeMenuGameOver(float x, float y, float z){
	glPushMatrix();
		drawBitmapText("R - Reiniciar", x, y, 0.0f, 0.0f, 0.0f, 0.0f); 
		drawBitmapText("S - Sair", x, y-20, 0.0f, 0.0f, 0.0f, 0.0f);
		drawBitmapText("P - Alterar camera", x, y-40, 0.0f, 0.0f, 0.0f, 0.0f);
		drawBitmapText("C - Alterar skin", x, y-60, 0.0f, 0.0f, 0.0f, 0.0f);
	glPopMatrix();
	
	glPushMatrix();
		drawBitmapText("Game Over!", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	glPopMatrix();	
}

void exibePontos(float x, float y, float z){
	snprintf (textoPontos, 50, "%s%i", "Pontos: ", pontosAtual);
	
	glPushMatrix();
		drawBitmapText(textoPontos, x, y, z, 0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void exibeMelhorPontuacao(float x, float y, float z){
	snprintf (textoMelhorPontuacao, 50, "%s%i", "Melhor Pontuacao: ", melhorPontuacao);
	
	glPushMatrix();
		drawBitmapText(textoMelhorPontuacao, x, y, z, 0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void gameOver(){
	
	MessageBeep(MB_ICONERROR); 
	
	//Verifica se vai salvar a nova pontuação
	if(pontosAtual > melhorPontuacao){
		salvaPontuacao();
	}
	
	canoX = -90.0f;
	informacoesCanos();
	carregaMelhorPontuacao();
	k = 0.0f;
	velocidade = -0.1f;
	tempo = 0.1f;
	acel = -0.2f;
	restart = 1;
}

void verificaColisaoChao(){
	if (k <= -165) {
		gameOver();
	}
}

void verificaColisaoCano(){  
	for(int c = 0; c < 4; c++){
		//Possível Colisão na parte externa dos canos 
		if((infoCanos[c][0] == colisaoCanoEmX)){
			controle = 0; controle = infoCanos[c][0] - 75;
			//Se está na parte externa dos canos e não for o limite para passar: Game Over! 
			if(!((k + 8.5 <= 300 - infoCanos[c][1]) && (k - 8.5 >= (300 - infoCanos[c][1] - 100)))){	
				gameOver();	 
			}
		}
		
		//Possível colisão no espaço interno dos canos
		if((infoCanos[c][0] >= controle) && infoCanos[c][0] < colisaoCanoEmX){
			//Se conseguiu passar incrementa os pontos.
			if(infoCanos[c][0] == controle){
				pontosAtual++;
				MessageBeep(MB_OK); 
			}else{
				if(!((k + 14 <= 300 - infoCanos[c][1]) && (k - 14 >= (300 - infoCanos[c][1] - 100)))){
					gameOver();	
				}
			}
		}
	}
}

void keyboard(unsigned char key, int x, int y) {
	if(key == 'i' || key == 'I'){
		start = 1;
	}else if (key == 32) { //Tecla espaço para o pássaro pular.
		pulaPassaro();
	}else if (key == 's' || key == 'S'){ //Sai do jogo
		exit(1);
	}else if(key == 'p' || key == 'P'){ //Altera a projeção
		tipoProjecao = tipoProjecao == 1 ? 2 : 1; 
	}else if(key == 'c' || key == 'C'){ //Altera a cor do passáro
		skin = (rand() % 5);
	}else if(key == 'r' || key == 'R'){ //Reinicia o jogo
		pontosAtual = 0.0f;
		restart = 0;
	}
	
	glutPostRedisplay();
}

void Display() {
	// Inicializa parâmetros de rendering
	// Define a cor de fundo da janela de visualização como preta
	
	//RGB usado = (112,219,219)
	glClearColor(0.6784313725490196f, 0.8470588235294118f, 0.9019607843137255f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_SMOOTH);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION); /*glMatrixMode()- define qual matriz será alterada. SEMPRE defina o tipo de apresentação (Ortogonal ou Perspectiva) na matriz PROJECTION.*/
	glLoadIdentity(); //"Limpa" ou "transforma" a matriz em identidade, reduzindo possíveis erros.
	
	if(tipoProjecao == 1){
		glOrtho(-(800/2.0), (800/2), -(600/2), (600/2), -400, 400);
		gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0); //Define a posição da câmera, para onde olha e qual eixo está na vertical.
	}else if(tipoProjecao == 2){
		gluPerspective(45, 800/600, 75, 5000);
		gluLookAt(-525, k, 350, 0, k, 0, 0, 1, 0); //Define a posição da câmera, para onde olha e qual eixo está na vertical.
	}
	
	glMatrixMode(GL_MODELVIEW); //Define qual matriz será alterada -> MODELVIEW (onde o desenho ocorrerá).
	glLoadIdentity(); //"Limpa" ou "transforma" a matriz em identidade.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers 
	
	//Prepara o cenário
	criaChao();
	criaCanos();
	criaNuvens();
	
	glPushMatrix();
		glTranslatef(0, k, 0);
			glPushMatrix();
				criaPassaro();
			glPopMatrix();
		glTranslatef(0, -k, 0);
	glPopMatrix();
	
	//Verifica se o jogo iniciou
	if(start == 0){
		if(tipoProjecao == 1){
			exibeMenuInicial(-310.0f, 265.0f, 30.0f);
		}
	}else if(start == 1 && restart == 0){
		//Movimento de parábola
		k = k + velocidade*tempo + (0.5)*(acel)*pow(tempo,2);
		velocidade = velocidade + acel*tempo;
		
		if(tipoProjecao == 1){
			exibePontos(-50.0f, 270.0f, 30.0f);	
		}else{
			exibePontos(-100.0f, 100.0f, 50.0f);	
		}
		
		decrementaPosCano();
		limitePuloEmY();
		
		//Verifica possíveis colisões
		verificaColisaoCano();
		verificaColisaoChao();	
	}
	
	if(restart == 1){
		if(tipoProjecao == 1){
			exibeMenuGameOver(-380.0f, 230.0f, 0.0f);
			exibeMelhorPontuacao(-50.0f, 270.0f, 30.0f); 
			exibePontos(-20.0f, 250.0f, 30.0f);
		}else{
			exibeMenuGameOver(-270.0f, 150.0f, 50.0f); 
			exibeMelhorPontuacao(-100.0f, 200.0f, 50.0f);
			exibePontos(-70.0f, 190.0f, 50.0f);
		}	
	}
	glutSwapBuffers(); //Executa a Cena. SwapBuffers dá suporte para mais de um buffer, permitindo execução de animações sem cintilações. 
	glutPostRedisplay();
}

int main(int argc, char **argv){
	quadric = gluNewQuadric();
	srand(time(NULL));
	
	//Preparação inicial das informações referentes aos canos
	informacoesCanos();
	
	//Carrega melhor pontuação se houver
	carregaMelhorPontuacao();
	
	//Carrega info nuvem
	posicaoNuvem();

	//Para mudar a cor do passáro
	corPassaro[0][0] = 55; corPassaro[0][1] = 204; corPassaro[0][2] = 0;
	corPassaro[1][0] = 51; corPassaro[1][1] = 51; corPassaro[1][2] = 0;
	corPassaro[2][0] = 204; corPassaro[2][1] = 0; corPassaro[2][2] = 204;
	corPassaro[3][0] = 204; corPassaro[3][1] = 0; corPassaro[3][2] = 0;
	corPassaro[4][0] = 153; corPassaro[4][1] = 102; corPassaro[4][2] = 0;
	corPassaro[5][0] = 51; corPassaro[5][1] = 51; corPassaro[5][2] = 51;
	
	glutInit(&argc, argv); //Inicializa glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /* Define as características do espaço vetorial. Nesse caso, permite animações (sem cintilações)| cores compostas por Verm. Verde e Azul|Buffer que permite trablhar com profundidade e elimina faces escondidas. */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(200, 80);
	glutCreateWindow("Flappy Bird");
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
