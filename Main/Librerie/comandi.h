#ifndef COMANDI_H
#define COMANDI_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define step 0
#define rampa 1
#define impulso 2
#define doublet 3

#define equili 0
#define aletto 1
#define timone 2
#define manett 3

extern int file_count;
extern char file_count_char[20];

void comandi_trim(double vett_com[]){

    double step_trim = 0.01;
    double time = 0;

    char estensione_file[10] = ".txt";
    char nome_file_comandi[100] = "../Main/Input/commandHistory";
    sprintf(file_count_char, "%d", file_count);
    strcat(nome_file_comandi, file_count_char);
    strcat(nome_file_comandi, estensione_file);

    FILE *fp;
    fp = fopen(nome_file_comandi, "w");

    if(fp == NULL){
        printf("ERRORE -12: Errore creazione del file commandHistory%d.txt per la scrittura\n", file_count);
        exit(-12);
    }

    // fprintf(fp,"%-10s%-10s%-10s%-10s%-10s\n","Tempo [s]","de [deg]","da [deg]","dr [deg]","Man [%]");
    
    for (int i=0;i<100001;i++)
    {
        fprintf(fp,"%lf, %lf, %lf, %lf, %lf\n", time, vett_com[0], vett_com[1], vett_com[2], vett_com[3]/100);
        time = time + step_trim;
    }
    fclose(fp);
}


void saturazione(double cur_com_vec[4], int che_comando, int com_max, int com_min, int check[], int cur_timestep, double timestep) {
    
    char units[4] = "deg";
    char sat_string[14] = "equilibratore";

    switch (che_comando){
        case 0:
            strcpy(sat_string,"equilibratore");
            break;
        
        case 1:
            strcpy(sat_string,"alettone");
            break;

        case 2:
            strcpy(sat_string,"timone");
            break;
        
        case 3:    
            strcpy(sat_string,"manetta");
            com_max = 100;
            com_min = 0;
            strcpy(units, "%");
            break;
    }

    if (cur_com_vec[che_comando] > com_max) {
        cur_com_vec[che_comando] = com_max;
        if (check[che_comando] == 0) {
            printf("\nWARNING: Comando di %s saturato a %d %s al tempo %.2lf s\n", sat_string, com_max, units, cur_timestep * timestep);
            check[che_comando] = 1;
        }
    } else if (cur_com_vec[che_comando] < com_min) {
        cur_com_vec[che_comando] = com_min;
        if (check[che_comando] == 0) {
            printf("\nWARNING: Comando di %s saturato a %d %s al tempo %.2lf s\n", sat_string, com_min, units, cur_timestep * timestep);
            check[che_comando] = 1;
        }
    }
}

void user_request(short int flag [4][4], int time_of_start[4][4], int time_of_end[4][4], double amplitude[4][4], double back_to_base[4],int final_timestep, double timestep){
    printf("\nSi vuole inserire un comando di equilibratore? [y/n]: ");
    char input;
    short int com_selection = -1;
    input = getchar();
    getchar();
    while(input!='y' && input!='n'){
        printf("\nInserire y o n! \n"); input = getchar(); 
        getchar();
    }
    if (input == 'y'){
        printf("\nComando equilibratore\n");
        printf("\nSelezionare tipologia comando\n1. Gradino\n2. Rampa\n3. Impulso\n4. Doublet\n");
        scanf("%d",&com_selection);
        getchar();
        while(com_selection<1 || com_selection>4){
            printf("\nInput non valido, inserire un numero tra 1 e 4\n"); scanf("%d",&com_selection);; 
            getchar();
        }
        switch (com_selection){
            case 1:
                printf("\nSi e' scelto di inserire un gradino di equilibratore.\nA quale secondo (intero) della simulazione si desidera che avvenga?\n");
                scanf("%d",&time_of_start[step][equili]);
                getchar();
                while((time_of_start[step][equili] > final_timestep*timestep)||(time_of_start[step][equili] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[step][equili]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il gradino?\n");
                scanf("%d",&time_of_end[step][equili]);
                getchar();       
                while((time_of_end[step][equili] > final_timestep*timestep)||(time_of_end[step][equili] < 0)||(time_of_end[step][equili] < time_of_start[step][equili])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[step][equili]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[step][equili]);
                getchar();
                flag[step][equili] = 1;
                break;
            
            case 2:
                printf("\nSi e' scelto di inserire una rampa di equilibratore.\nA quale secondo (intero) della simulazione si desidera che inizi la rampa?\n");
                scanf("%d",&time_of_start[rampa][equili]);
                getchar();
                while((time_of_start[rampa][equili] > final_timestep*timestep)||(time_of_start[rampa][equili] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[rampa][equili]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca la rampa?\n");
                scanf("%d",&time_of_end[rampa][equili]);
                getchar();       
                while((time_of_end[rampa][equili] > final_timestep*timestep)||(time_of_end[rampa][equili] < 0)||(time_of_end[rampa][equili] < time_of_start[rampa][equili])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[rampa][equili]);
                    getchar();
                }    
                printf("\nA quale secondo (intero) della simulazione si desidera che il comando ritorni alla condizione precedente?\n");
                scanf("%lf",&back_to_base[equili]);
                getchar();
                while(back_to_base[equili] < time_of_end[rampa][equili]){
                    printf("L'Istante di ritorno del comando alla condizione di trim risulta precedente all'istante di fine rampa, inserire nuovamente!\n");
                    scanf("%lf",&back_to_base[equili]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[rampa][equili]);
                getchar();
                flag[rampa][equili] = 1;
                break;

            case 3:
                printf("\nSi e' scelto di inserire un impulso di equilibratore.\nA quale secondo (intero) della simulazione si desidera che inizi l'impulso?\n");
                scanf("%d",&time_of_start[impulso][equili]);
                getchar();         
                while((time_of_start[impulso][equili] > final_timestep*timestep)||(time_of_start[impulso][equili] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[impulso][equili]);
                    getchar();
                }  
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[impulso][equili]);
                getchar();
                flag[impulso][equili] = 1;
                break;

            case 4:
                printf("\nSi e' scelto di inserire un doublet di equilibratore.\nA quale secondo (intero) della simulazione si desidera che inizi il doublet?\n");
                scanf("%d",&time_of_start[doublet][equili]);
                getchar();
                while((time_of_start[doublet][equili] > final_timestep*timestep)||(time_of_start[doublet][equili] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[doublet][equili]);
                    getchar();
                }  
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il doublet?\n");
                scanf("%d",&time_of_end[doublet][equili]);
                getchar();            
                while((time_of_end[doublet][equili] > final_timestep*timestep)||(time_of_end[doublet][equili] < 0)||(time_of_end[doublet][equili] < time_of_start[doublet][equili])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[doublet][equili]);
                    getchar();
                } 
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[doublet][equili]);
                getchar();
                flag[doublet][equili] = 1;
                break;

            
            default:
                printf("\nErrore generico nell'input dell'utente\n");
                getchar();
                break;

        }
    }

    printf("\nSi vuole inserire un comando di alettone? [y/n]: ");
    input = getchar();
    getchar();
    com_selection = -1;
    while(input!='y' && input!='n'){
        printf("\nInserire y o n! \n");
        input = getchar(); 
        getchar();
    }
    if (input == 'y'){
        printf("\nChe tipo di comando si vuole inserire?\n1. Gradino\n2. Rampa\n3. Impulso\n4. Doublet\n");
        scanf("%d",&com_selection);
        getchar();
        switch (com_selection){
            case 1:
                printf("Si e' scelto di inserire un gradino di alettone.\nA quale secondo (intero) della simulazione si desidera che avvenga?\n");
                scanf("%d",&time_of_start[step][aletto]);
                getchar();
                while((time_of_start[step][aletto] > final_timestep*timestep)||(time_of_start[step][aletto] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[step][aletto]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il gradino?\n");
                scanf("%d",&time_of_end[step][aletto]);
                getchar();       
                while((time_of_end[step][aletto] > final_timestep*timestep)||(time_of_end[step][aletto] < 0)||(time_of_end[step][aletto] < time_of_start[step][aletto])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[step][aletto]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[step][aletto]);
                getchar();
                flag[step][aletto] = 1;
                break;
            
            case 2:
                printf("Si e' scelto di inserire una rampa di alettone.\nA quale secondo (intero) della simulazione si desidera che inizi la rampa?\n");
                scanf("%d",&time_of_start[rampa][aletto]);
                getchar();
                while((time_of_start[rampa][aletto] > final_timestep*timestep)||(time_of_start[rampa][aletto] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[rampa][aletto]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca la rampa?\n");
                scanf("%d",&time_of_end[rampa][aletto]);
                getchar();       
                while((time_of_end[rampa][aletto] > final_timestep*timestep)||(time_of_end[rampa][aletto] < 0)||(time_of_end[rampa][aletto] < time_of_start[rampa][aletto])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[rampa][aletto]);
                    getchar();
                }    
                printf("\nA quale secondo (intero) della simulazione si desidera che il comando ritorni alla condizione precedente?\n");
                scanf("%lf",&back_to_base[aletto]);
                getchar();
                while(back_to_base[aletto] < time_of_end[rampa][aletto]){
                    printf("L'Istante di ritorno del comando alla condizione di trim risulta precedente all'istante di fine rampa, inserire nuovamente!\n");
                    scanf("%lf",&back_to_base[aletto]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[rampa][aletto]);
                getchar();
                flag[rampa][aletto] = 1;
                break;

            case 3:
                printf("Si e' scelto di inserire un impulso di alettone.\nA quale secondo (intero) della simulazione si desidera che inizi l'impulso?\n");
                scanf("%d",&time_of_start[impulso][aletto]);
                getchar();         
                while((time_of_start[impulso][aletto] > final_timestep*timestep)||(time_of_start[impulso][aletto] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[impulso][aletto]);
                    getchar();
                }   
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[impulso][aletto]);
                getchar();
                flag[impulso][aletto] = 1;
                break;

            case 4:
                printf("Si e' scelto di inserire un doublet di alettone.\nA quale secondo (intero) della simulazione si desidera che inizi il doublet?\n");
                scanf("%d",&time_of_start[doublet][aletto]);
                getchar();
                while((time_of_start[doublet][aletto] > final_timestep*timestep)||(time_of_start[doublet][aletto] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[doublet][aletto]);
                    getchar();
                }  
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il doublet?\n");
                scanf("%d",&time_of_end[doublet][aletto]);
                getchar();            
                while((time_of_end[doublet][aletto] > final_timestep*timestep)||(time_of_end[doublet][aletto] < 0)||(time_of_end[doublet][aletto] < time_of_start[doublet][aletto])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[doublet][aletto]);
                    getchar();
                }         
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[doublet][aletto]);
                getchar();
                flag[doublet][aletto] = 1;
                break;

            default:
                printf("\nErrore generico nell'input da utente!\n");

                break;
        }
    }

    printf("\nSi vuole inserire un comando di timone? [y/n]: ");
    input = getchar();
    getchar();
    com_selection = -1;
    while(input!='y' && input!='n'){
        printf("\nInserire y o n! \n");
        input = getchar(); 
        getchar();
    }
    if (input == 'y'){
        printf("\nChe tipo di comando si vuole inserire?\n1. Gradino\n2. Rampa\n3. Impulso\n4. Doublet\n");
        scanf("%d",&com_selection);
        getchar();
        switch (com_selection){
            case 1:
                printf("Si e' scelto di inserire un gradino di timone.\nA quale secondo (intero) della simulazione si desidera che avvenga?\n");
                scanf("%d",&time_of_start[step][timone]);
                getchar();
                while((time_of_start[step][timone] > final_timestep*timestep)||(time_of_start[step][timone] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[step][timone]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il gradino?\n");
                scanf("%d",&time_of_end[step][timone]);
                getchar();       
                while((time_of_end[step][timone] > final_timestep*timestep)||(time_of_end[step][timone] < 0)||(time_of_end[step][timone] < time_of_start[step][timone])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[step][timone]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[step][timone]);
                getchar();
                flag[step][timone] = 1;
                break;
            
            case 2:
                printf("Si e' scelto di inserire una rampa di timone.\nA quale secondo (intero) della simulazione si desidera che inizi la rampa?\n");
                scanf("%d",&time_of_start[rampa][timone]);
                getchar();
                while((time_of_start[rampa][timone] > final_timestep*timestep)||(time_of_start[rampa][timone] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[rampa][timone]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca la rampa?\n");
                scanf("%d",&time_of_end[rampa][timone]);
                getchar();       
                while((time_of_end[rampa][timone] > final_timestep*timestep)||(time_of_end[rampa][timone] < 0)||(time_of_end[rampa][timone] < time_of_start[rampa][timone])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[rampa][timone]);
                    getchar();
                }          
                printf("\nA quale secondo (intero) della simulazione si desidera che il comando ritorni alla condizione precedente?\n");
                scanf("%lf",&back_to_base[timone]);
                getchar();
                while(back_to_base[timone] < time_of_end[rampa][timone]){
                    printf("L'Istante di ritorno del comando alla condizione di trim risulta precedente all'istante di fine rampa, inserire nuovamente!\n");
                    scanf("%lf",&back_to_base[timone]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[rampa][timone]);
                getchar();
                flag[rampa][timone] = 1;
                break;

            case 3:
                printf("Si e' scelto di inserire un impulso di timone.\nA quale secondo (intero) della simulazione si desidera che inizi l'impulso?\n");
                scanf("%d",&time_of_start[impulso][timone]);
                getchar();    
                while((time_of_start[impulso][timone] > final_timestep*timestep)||(time_of_start[impulso][timone] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[impulso][timone]);
                    getchar();
                }         
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[impulso][timone]);
                getchar();
                flag[impulso][timone] = 1;
                break;

            case 4:
                printf("Si e' scelto di inserire un doublet di timone.\nA quale secondo (intero) della simulazione si desidera che inizi il doublet?\n");
                scanf("%d",&time_of_start[doublet][timone]);
                getchar();
                while((time_of_start[doublet][timone] > final_timestep*timestep)||(time_of_start[doublet][timone] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[doublet][timone]);
                    getchar();
                }  
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il doublet?\n");
                scanf("%d",&time_of_end[doublet][timone]);
                getchar();            
                while((time_of_end[doublet][timone] > final_timestep*timestep)||(time_of_end[doublet][timone] < 0)||(time_of_end[doublet][timone] < time_of_start[doublet][timone])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[doublet][timone]);
                    getchar();
                }           
                printf("\nQuale ampiezza deve avere il comando (gradi)?\n");
                scanf("%lf",&amplitude[doublet][timone]);
                getchar();
                flag[doublet][timone] = 1;
                break;

            default:
                printf("\nErrore nell'input da utente!\n");

                break;
        }
    }

    printf("\nSi vuole inserire un comando di manetta? [y/n]: ");
    input = getchar();
    getchar();
    com_selection = -1;
    while(input!='y' && input!='n'){
        printf("\nInserire y o n! \n");
        input = getchar(); 
        getchar();
    }
    if (input == 'y'){
        printf("\nChe tipo di comando si vuole inserire?\n1. Gradino\n2. Rampa\n3. Impulso\n4. Doublet\n");
        scanf("%d",&com_selection);
        getchar();
        switch (com_selection){
            case 1:
                printf("Si e' scelto di inserire un gradino di manetta.\nA quale secondo (intero) della simulazione si desidera che avvenga?\n");
                scanf("%d",&time_of_start[step][manett]);
                getchar();
                while((time_of_start[step][manett] > final_timestep*timestep)||(time_of_start[step][manett] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[step][manett]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il gradino?\n");
                scanf("%d",&time_of_end[step][manett]);
                getchar();       
                while((time_of_end[step][manett] > final_timestep*timestep)||(time_of_end[step][manett] < 0)||(time_of_end[step][manett] < time_of_start[step][manett])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[step][manett]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (%% aggiuntiva)?\n");
                scanf("%lf",&amplitude[step][manett]);
                getchar();
                flag[step][manett] = 1;
                break;
            
            case 2:
                printf("Si e' scelto di inserire una rampa di manetta.\nA quale secondo (intero) della simulazione si desidera che inizi la rampa?\n");
                scanf("%d",&time_of_start[rampa][manett]);
                getchar();
                while((time_of_start[rampa][manett] > final_timestep*timestep)||(time_of_start[rampa][manett] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[rampa][manett]);
                    getchar();
                }
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca la rampa?\n");
                scanf("%d",&time_of_end[rampa][manett]);
                getchar();       
                while((time_of_end[rampa][manett] > final_timestep*timestep)||(time_of_end[rampa][manett] < 0)||(time_of_end[rampa][manett] < time_of_start[rampa][manett])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[rampa][manett]);
                    getchar();
                }          
                printf("\nA quale secondo (intero) della simulazione si desidera che il comando ritorni alla condizione precedente?\n");
                scanf("%lf",&back_to_base[manett]);
                getchar();
                while(back_to_base[manett] < time_of_end[rampa][manett]){
                    printf("L'Istante di ritorno del comando alla condizione di trim risulta precedente all'istante di fine rampa, inserire nuovamente!\n");
                    scanf("%lf",&back_to_base[manett]);
                    getchar();
                }
                printf("\nQuale ampiezza deve avere il comando (%% aggiuntiva)?\n");
                scanf("%lf",&amplitude[rampa][manett]);
                getchar();
                flag[rampa][manett] = 1;
                break;

            case 3:
                printf("Si e' scelto di inserire un impulso di manetta.\nA quale secondo (intero) della simulazione si desidera che inizi l'impulso?\n");
                scanf("%d",&time_of_start[impulso][manett]);
                getchar();    
                while((time_of_start[impulso][manett] > final_timestep*timestep)||(time_of_start[impulso][manett] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[impulso][manett]);
                    getchar();
                }       
                printf("\nQuale ampiezza deve avere il comando (%% aggiuntiva)?\n");
                scanf("%lf",&amplitude[impulso][manett]);
                getchar();
                flag[impulso][manett] = 1;
                break;

            case 4:
                printf("Si e' scelto di inserire un doublet di manetta.\nA quale secondo (intero) della simulazione si desidera che inizi il doublet?\n");
                scanf("%d",&time_of_start[doublet][manett]);
                getchar();
                while((time_of_start[doublet][manett] > final_timestep*timestep)||(time_of_start[doublet][manett] < 0)){
                    printf("Istante di inizio comando fuori dal tempo minimo o massimo di simulazione, inserire nuovamente!\n");
                    scanf("%d",&time_of_start[doublet][manett]);
                    getchar();
                }  
                printf("\nA quale secondo (intero) della simulazione si desidera che finisca il doublet?\n");
                scanf("%d",&time_of_end[doublet][manett]);
                getchar();            
                while((time_of_end[doublet][manett] > final_timestep*timestep)||(time_of_end[doublet][manett] < 0)||(time_of_end[doublet][manett] < time_of_start[doublet][manett])){
                    printf("Istante di fine comando fuori dal tempo minimo/massimo di simulazione, oppure istante di fine precedente all'istante di inizio, inserire nuovamente!\n");
                    scanf("%d",&time_of_end[doublet][manett]);
                    getchar();
                }            
                printf("\nQuale ampiezza deve avere il comando (%% aggiuntiva)?\n");
                scanf("%lf",&amplitude[doublet][manett]);
                getchar();
                flag[doublet][manett] = 1;
                break;

            default:
                printf("\nErrore generico nell'input da utente!\n");

                break;
        }
    }
}

int genera_comandi(int final_timestep, double timestep, double baseline_com_vec[4]){
    /*
    Funzione che genera la time history dei comandi.
    int final_timestep = ultimo timestep della simulazione
    double timestep = passo dell'integrazione
    double baseline_com_vec = vettore dei comandi derivato dal trim
    */

    // ELIMINARE, SOLO PER TEST
    // int final_timestep = 300;
    // double timestep = 0.1;

    // da trim, MODIFICARE OPPORTUNAMENTE
    // double baseline_com_vec[4] = {0,0,0,0};

    // Definizione matrici che contengono parametri dei comandi
    int time_of_start[4][4];                // tempo a cui succedono cose relative a [input][superficie]
    int time_of_end[4][4];                  // tempo a cui finiscono cose relative a [input][superficie]
    short int flag[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};   // flag presenza manovra, stessa sintassi
    double amplitude[4][4];                 // ampiezza, stessa sintassi
    double back_to_base[4];

    user_request(flag, time_of_start, time_of_end, amplitude, back_to_base, final_timestep, timestep);

    int timestep_of_start[4][4];            // memorizza conversione da tempo inizio comando a timestep;
    int timestep_of_end[4][4];              // memorizza conversione da tempo fine comando a timestep
    double ang_coeff[4];                    // coefficienti angolari delle rampe per i vari eq, alet, timone, e manetta
    int impulse_duration_timesteps = 5;     // durata in timesteps dell'impulso
    int delta_timestep_doublet[4];
    int timestep_of_doublet_inversion[4];

    double com_max,com_min;                 // estremi escursione equilibratore, alettone e timone (sempre +-20 deg)
    com_max = 20; com_min = -20;
    int check[4]= {0,0,0,0};
    
    // Apertura file

    char estensione_file[10] = ".txt";
    char nome_file_comandi[100] = "../Main/Input/commandHistory";
    //char nome_file_comandi [] = "output_com.txt";
    sprintf(file_count_char, "%d", file_count);
    strcat(nome_file_comandi, file_count_char);
    strcat(nome_file_comandi, estensione_file);
    
    FILE *fileCH;
    fileCH = fopen(nome_file_comandi, "w");

    if(fileCH == NULL){
        printf("ERRORE -19: Errore apertura del file commandHistory%d.txt per la scrittura\n", file_count);
        exit(-19);
    }

    // DEFINIZIONE STEP
    timestep_of_start[step][equili] = time_of_start[step][equili]/timestep;
    timestep_of_end[step][equili] = time_of_end[step][equili]/timestep;
    timestep_of_start[step][aletto] = time_of_start[step][aletto]/timestep;
    timestep_of_end[step][aletto] = time_of_end[step][aletto]/timestep;
    timestep_of_start[step][timone] = time_of_start[step][timone]/timestep;
    timestep_of_end[step][timone] = time_of_end[step][timone]/timestep;
    timestep_of_start[step][manett] = time_of_start[step][manett]/timestep;
    timestep_of_end[step][manett] = time_of_end[step][manett]/timestep;

    // DEFINIZIONE RAMPA
    // Equilibratore
    timestep_of_start[rampa][equili] = time_of_start[rampa][equili]/timestep;
    timestep_of_end[rampa][equili] = time_of_end[rampa][equili]/timestep;
    // calcolo coeff. angolare (ampiezza/lunghezza_in_timestep)
    ang_coeff[equili] =((double) amplitude[rampa][equili])/((double) timestep_of_end[rampa][equili]-timestep_of_start[rampa][equili]);
    back_to_base[equili] = ((double)back_to_base[equili])/timestep;
    // Alettone
    timestep_of_start[rampa][aletto] = time_of_start[rampa][aletto]/timestep;
    timestep_of_end[rampa][aletto] = time_of_end[rampa][aletto]/timestep;
    ang_coeff[aletto] =((double) amplitude[rampa][aletto])/((double) timestep_of_end[rampa][aletto]-timestep_of_start[rampa][aletto]);
    back_to_base[aletto] = ((double)back_to_base[aletto])/timestep;
    // Timone
    timestep_of_start[rampa][timone] = time_of_start[rampa][timone]/timestep;
    timestep_of_end[rampa][timone] = time_of_end[rampa][timone]/timestep;
    ang_coeff[timone] =((double) amplitude[rampa][timone])/((double) timestep_of_end[rampa][timone]-timestep_of_start[rampa][timone]);
    back_to_base[timone] = ((double)back_to_base[timone])/timestep;
    // Manetta
    timestep_of_start[rampa][manett] = time_of_start[rampa][manett]/timestep;
    timestep_of_end[rampa][manett] = time_of_end[rampa][manett]/timestep;
    ang_coeff[manett] =((double) amplitude[rampa][manett])/((double) timestep_of_end[rampa][manett]-timestep_of_start[rampa][manett]);
    back_to_base[manett] = ((double)back_to_base[manett])/timestep;


    // DEFINIZIONE IMPULSO
    // Equilibratore
    time_of_end[impulso][equili] = time_of_start[impulso][equili] + impulse_duration_timesteps;
    timestep_of_start[impulso][equili] = time_of_start[impulso][equili]/timestep;
    timestep_of_end[impulso][equili] = (timestep_of_start[impulso][equili]) + impulse_duration_timesteps;
    // Alettone
    time_of_end[impulso][aletto] = time_of_start[impulso][aletto] + impulse_duration_timesteps;
    timestep_of_start[impulso][aletto] = time_of_start[impulso][aletto]/timestep;
    timestep_of_end[impulso][aletto] = (timestep_of_start[impulso][aletto]) + impulse_duration_timesteps;
    // Timone
    time_of_end[impulso][timone] = time_of_start[impulso][timone] + impulse_duration_timesteps;
    timestep_of_start[impulso][timone] = time_of_start[impulso][timone]/timestep;
    timestep_of_end[impulso][timone] = (timestep_of_start[impulso][timone]) + impulse_duration_timesteps;
    // Manetta
    time_of_end[impulso][manett] = time_of_start[impulso][manett] + impulse_duration_timesteps;
    timestep_of_start[impulso][manett] = time_of_start[impulso][manett]/timestep;
    timestep_of_end[impulso][manett] = (timestep_of_start[impulso][manett]) + impulse_duration_timesteps;

    // DEFINIZIONE DOUBLET
    // Equilibratore
    timestep_of_start[doublet][equili] = time_of_start[doublet][equili]/timestep;
    timestep_of_end[doublet][equili] = time_of_end[doublet][equili]/timestep;
    delta_timestep_doublet[equili] = timestep_of_end[doublet][equili] - timestep_of_start[doublet][equili];
    timestep_of_doublet_inversion[equili] = timestep_of_start[doublet][equili] + (delta_timestep_doublet[equili])/2;
    // Alettone
    timestep_of_start[doublet][aletto] = time_of_start[doublet][aletto]/timestep;
    timestep_of_end[doublet][aletto] = time_of_end[doublet][aletto]/timestep;
    delta_timestep_doublet[aletto] = timestep_of_end[doublet][aletto] - timestep_of_start[doublet][aletto];
    timestep_of_doublet_inversion[aletto] = timestep_of_start[doublet][aletto] + (delta_timestep_doublet[aletto])/2;
    // Timone
    timestep_of_start[doublet][timone] = time_of_start[doublet][timone]/timestep;
    timestep_of_end[doublet][timone] = time_of_end[doublet][timone]/timestep;
    delta_timestep_doublet[timone] = timestep_of_end[doublet][timone] - timestep_of_start[doublet][timone];
    timestep_of_doublet_inversion[timone] = timestep_of_start[doublet][timone] + (delta_timestep_doublet[timone])/2;
    // Manetta
    timestep_of_start[doublet][manett] = time_of_start[doublet][manett]/timestep;
    timestep_of_end[doublet][manett] = time_of_end[doublet][manett]/timestep;
    delta_timestep_doublet[manett] = timestep_of_end[doublet][manett] - timestep_of_start[doublet][manett];
    timestep_of_doublet_inversion[manett] = timestep_of_start[doublet][manett] + (delta_timestep_doublet[manett])/2;

    // stampa condizioni trim con tempo = 0
    fprintf(fileCH, "0,%.4lf,%.4lf,%.4lf,%.4lf\n",baseline_com_vec[0],baseline_com_vec[1],baseline_com_vec[2],baseline_com_vec[3]/100);

    // qui si cicla sul tempo fino alla fine della simulazione
    for (int cur_timestep = 1; cur_timestep < (final_timestep + 1); cur_timestep++){

        double cur_com_vec[4];
        memcpy(cur_com_vec, baseline_com_vec, sizeof(cur_com_vec));

        // 0 equilibratore, 1 alettone, 2 timone, 3 manetta
        for (int che_comando = 0; che_comando < 4; che_comando++){
            
            // gradino
            if ((flag[step][che_comando]) && (cur_timestep >= timestep_of_start[step][che_comando])){
                if (cur_timestep >= timestep_of_end[step][che_comando]){
                    // ritorno alla condizione pre-gradino
                    cur_com_vec[che_comando] = baseline_com_vec[che_comando];
                }else{
                    cur_com_vec[che_comando] = cur_com_vec[che_comando] + amplitude[step][che_comando];
                }               
                // qui segnaliamo la eventuale saturazione del comando
                saturazione(cur_com_vec,che_comando,com_max,com_min,check,cur_timestep,timestep);

                // printf("\n");
                // break; uncomment se è giusto smettere di scrivere nel caso il comando saturi
            }
            // rampa
            else if ((flag[rampa][che_comando]) && (cur_timestep >= timestep_of_start[rampa][che_comando])){
                if (cur_timestep >= timestep_of_end[rampa][che_comando]){
                    // saturo la rampa
                    cur_com_vec[che_comando] = cur_com_vec[che_comando] + amplitude[rampa][che_comando];
                    if (cur_timestep >= back_to_base[che_comando]){
                        cur_com_vec[che_comando] = baseline_com_vec[che_comando];
                    }
                }else{
                    cur_com_vec[che_comando] = cur_com_vec[che_comando] + ang_coeff[che_comando]*(double)(cur_timestep-timestep_of_start[rampa][che_comando]);
                }

                // segnalazione saturazione comando
                saturazione(cur_com_vec,che_comando,com_max,com_min,check,cur_timestep,timestep);
                
            // impulso
            }else if ((flag[impulso][che_comando]) && (cur_timestep >= timestep_of_start[impulso][che_comando])){
                if (cur_timestep >= timestep_of_end[impulso][che_comando]){
                    // ritorno alla condizione pre-impulso
                    cur_com_vec[che_comando] = baseline_com_vec[che_comando];
                }else{
                    cur_com_vec[che_comando] = cur_com_vec[che_comando] + amplitude[impulso][che_comando];
                }

                // segnalazione saturazione comando
                saturazione(cur_com_vec,che_comando,com_max,com_min,check,cur_timestep,timestep);

            // doublet
            }else if ((flag[doublet][che_comando]) && (cur_timestep >= timestep_of_start[doublet][che_comando])){
                if (cur_timestep >= timestep_of_end[doublet][che_comando]){
                    // se il doublet è finito, ritorno alla condizione pre-doublet
                    cur_com_vec[che_comando] = baseline_com_vec[che_comando];
                }else if (cur_timestep < timestep_of_doublet_inversion[che_comando]){
                    // se siamo nella parte positiva del doublet
                    cur_com_vec[che_comando] = cur_com_vec[che_comando] + amplitude[doublet][che_comando];
                }else{
                    // altrimenti siamo per forza nella parte negativa del doublet
                    cur_com_vec[che_comando] = cur_com_vec[che_comando] - amplitude[doublet][che_comando];
                }

                // segnalazione saturazione comando
                saturazione(cur_com_vec,che_comando,com_max,com_min,check,cur_timestep,timestep);
            }
            
            // Stampa su file
            if (che_comando == 0){
                fprintf(fileCH, "%.4lf, %.4lf, ", (double)cur_timestep * timestep, cur_com_vec[che_comando]);
                // printf("%d, %.2lf, ",cur_timestep, cur_com_vec[che_comando]);
            }
            else if (che_comando != 3){
                fprintf(fileCH,"%.4lf, ",cur_com_vec[che_comando]);
                // printf("%.2lf, ",cur_com_vec[che_comando]);
            }
            else{
                cur_com_vec[che_comando] = cur_com_vec[che_comando]/100.0;
                fprintf(fileCH,"%.4lf\n",cur_com_vec[che_comando]);
                // printf("%.2lf\n",cur_com_vec[che_comando]);
            }
            
        }
    } 
    // Segnalo fine scrittura su file e chiudo file
    printf("\nScrittura file comandi terminata!\n");
    fclose(fileCH);
    return 0;
}
#endif