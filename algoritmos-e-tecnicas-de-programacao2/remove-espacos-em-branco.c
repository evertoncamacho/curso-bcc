#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	
	char frase[100];
	int tamanhoPalavra, i, k;
	
	printf("Informe uma frase com espacos em branco:\n");
	scanf("%[^\n]s", frase);
	
	tamanhoPalavra = strlen(frase);
	
	for(i = 0; i < tamanhoPalavra; i++){
		if(frase[i] == ' ' && frase[i+1] == ' '){
			for(k = i; k < tamanhoPalavra; k++){
				frase[k] = frase[k+1];
			}
			i = 0;
		}
	}
	
	printf("\nFrase: %s", frase);
}
