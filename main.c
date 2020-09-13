//Fernando Garrote De La Macorra
//A01027503

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct{
    pid_t pidH;
    int promedio;
} proceso_t;

void impHistograma(proceso_t *, int, int);

void impHistograma(proceso_t * proc, int num, int max){
    printf("\n PID Hijo \t Promedio \t Histograma");

    for(proceso_t * aux = proc; aux< proc+num; aux++){
        printf("%-20d%-20d", aux->pidH, aux->promedio);
        int ast = 20-(2*(max- aux->promedio));
        for(int i=0; i<ast; i++){
          printf("*");
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char * const * argv) {

    int dato = 0, avg = 0, max = 0, convNum = 0, tot = 0, cont;
    char *numVal = NULL;
    proceso_t *ultimo;
    pid_t pid;

    opterr=0;

    while ((dato = getopt (argc, argv, "n:")) != -1){
        switch (dato){
        case 'n':
            numVal = optarg;
             if(atoi(numVal) == 0){
                printf("\nSe requiere un numero entero");
                exit(0);
            }
            else{
                convNum = atoi(numVal);
            }
            break;
        case '?':
            if (optopt == 'n')
              fprintf(stderr, "\nInserte un argumento a la opción -%c",optopt);
            else if (isprint (optopt))
              fprintf(stderr, "\nLa opción -%c no es válida.", optopt);
            else
                fprintf (stderr, "\nOpción desconocida '\\x%x'.", optopt);
                return 1;

        default:
            abort ();
    }
  }

    proceso_t * proc= malloc(convNum*sizeof(proceso_t));
    cont = convNum;
    proceso_t *aux;
    aux = proc;

    while(tot < convNum){
        pid = fork();
        if (pid == -1){
            printf("\nError al crear hijo");
            cont--;
            break;
        }
        if (pid == 0){
            int promedio = (getpid() + getppid())/2;
            printf("\nSoy el proceso hijo con PID %d, con PPID %d y  promedio de = %d.", getpid(), getppid() ,promedio);
            exit(promedio);
        }
        else{
            sleep(3);
            aux->pidH =  pid;
            aux++;
        }
        tot++;
    }

    ultimo = proc + cont;

    for(aux = proc; aux < ultimo; aux++){
            if (waitpid(aux->pidH, &avg, 0) != -1){
                if (WIFEXITED(avg)){
                    aux->promedio = WEXITSTATUS(avg);
                    if(aux->promedio > max){
                        max = aux->promedio;
                    }
                }
            }
    }

    impHistograma(proc, convNum, max);
    free(proc);

    return 0;
}
