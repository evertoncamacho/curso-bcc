#include <stdio.h>
#include <stdlib.h>

/* 		
	Enunciado: Ler o arquivo, calcular a média de pesos dos bois e salvar num segundo arquivo
	os seguintes dados: identificador peso sentido(+/-) valor
		
	Aluno: Everton de Melo Camacho
	Disciplina: Algoritmos e Técnicas de Programação II
*/

// Estrutura para representar as informações de um boi
typedef struct boi{
	char nome[40];
	char id[10];
	double peso;
}BOI;

// Função para ler o arquivo
void leArquivo(BOI gado[500], int *contador){
	FILE *boi = NULL;
	
	char arquivoEntrada[40];
	
	printf("Qual o arquivo de entrada? \n");
	scanf("%s", arquivoEntrada);
	
	boi = fopen(arquivoEntrada, "r");
	
	while(!feof(boi)){
		fscanf(boi, "%s %s %lf", gado[*contador].nome, gado[*contador].id, &gado[*contador].peso);
		(*contador)++;
	}
	
	fclose(boi);		
}

// Função para calcular a média
double calculaMedia(BOI boi[500], int contador){
	
	double somaPesos = 0;
	int i;
	
	for(i = 0; i < contador; i++){
		somaPesos += boi[i].peso;
	}
	
	return somaPesos / contador;
}

// Função para salvar o arquivo
void salvaArquivo(BOI gado[500], double media, int contador){
	FILE *boi = NULL;
	
	int i;
	char arquivoSaida[40], sentido;
	double dif;
	
	printf("\nQual o arquivo de saida?\n");
	scanf("%s", arquivoSaida);
	
	boi = fopen(arquivoSaida, "w");
	
	for(i = 0; i < contador; i++){
		sentido = gado[i].peso > media ? '+' : '-';
		dif = abs(gado[i].peso - media);
		fprintf(boi, "%s %lf %c %lf \n", gado[i].id, gado[i].peso, sentido, dif);	
	}
	
	printf("\nO arquivo foi salvo com sucesso!");
	
	fclose(boi);
}

int main(){

	int i, cont = 0;
	double media;
	BOI gado[500];

	// Chamada das funções
	leArquivo(gado, &cont);
	media = calculaMedia(gado, cont);
	salvaArquivo(gado, media, cont);
}
