/* Contador de primos
 *
 * Este programa recebera uma serie de numeros em sua entrada. 
 * Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de primos que recebeu (2,3,5,7,11 são numeros primos)
 * Cada numero deve ser analisado em um thread diiferente
 * usando 4 thread maximo em paralelo
 *Os numeros tem tamanho macimo de 2^64 <=> long long int 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

#define WORKERS 4 //numero de threads em paralelo

//saber se eh primo
bool eh_primo(long long int x){
	bool eh= true;
	// 0 e 1 não são primos
	if(x < 2) eh =false;
	
	for (int i = 2; i < x; i++){
		if (x % i == 0){
      eh=false;
	  break;
		}
	}
	return eh;
}

pthread_mutex_t trava;
int primos =0; // varaivel global

void* funcao_thread(void *arg) {
  long long int *N = (long long int*)(arg);
  long long int M = (*N);
    pthread_mutex_lock(&trava);
	
	if (eh_primo(M))primos++;
	
    pthread_mutex_unlock(&trava);
  return NULL;
}



int main(int argc, char **argv) {
  pthread_t workers[WORKERS];
  int thread_id[WORKERS];

  /* Recuperando numeros */
	long long int entrada[1000];  
	char c; 
	int e = 0;
	while(c != '\n'){
		scanf("%llu", &entrada[e]);
		c = getchar();
		e++;
	}
	int entrada_size = e;
	
  /* Disparanto threads */
	int id =1;
	for (int i_entrada = 0; i_entrada < entrada_size; i_entrada++) {
		//criando a thread
		pthread_create(&(workers[id]), NULL, funcao_thread, &(entrada[i_entrada])); 
		if(id <WORKERS){
			id++;		
		}else{
			/*Esperando todas as threads terminarem os processos*/
			for (int k=0; k<WORKERS; k++) {
				pthread_join(workers[k], NULL);
			}
			id = 0; //volta a usar a primeira thread

		}
	}

	/* Esperando threads */
	for (int i=0; i<WORKERS; i++) {
		pthread_join(workers[i], NULL);
	}
	
  /* Escrevendo  numero de primos*/
    printf("%d\n", primos);


  return 0;
}
