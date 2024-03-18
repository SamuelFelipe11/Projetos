#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/**/
typedef struct{
    int dia;
    int mes;
    int ano;
}DATA_ATUAL;

DATA_ATUAL obtemdata(void){
    DATA_ATUAL data;
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    data.dia = tm.tm_mday;
    data.mes = tm.tm_mon + 1;
    data.ano = tm.tm_year + 1900;

    return data;
}//fim função

bool verificaIdade(int dia, int mes, int ano){
    DATA_ATUAL Data;
    if(ano <= (Data.ano - 12)) return false;
    if(mes <= Data.mes)  return false;
    if(dia < Data.dia)  return false;

    /*if(ano <= (Data.ano - 12)){
        if(mes <= Data.mes){
            if(dia < Data.dia){
                return false;
            }
        }
    }*/

    return true;
}//fim função

int main(){
    DATA_ATUAL Data;
    Data = obtemdata();    
    printf("Data: %d/%d/%d\n", Data.dia, Data.mes, Data.ano);


    return EXIT_SUCCESS;
}