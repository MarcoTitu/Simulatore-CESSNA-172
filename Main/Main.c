#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Librerie
#include "..\Main\Librerie\Atmosphere.h"
#include "..\Main\Librerie\interpola.h"
#include "..\Main\Librerie\propel.h"
#include "..\Main\Librerie\read.h"
#include "..\Main\Librerie\stabilita.h"
#include "..\Main\Librerie\trim.h"
#include "..\Main\Librerie\integrazione.h"
#include "..\Main\Librerie\comandi.h"

// Variabili

double engine[7];   // Variabili lette dal file engine.txt
double propeller_data[11], CSI[60], RD[60], CH[60], BA[60]; // Variabili lette dal file propeller.txt
double general_data[30], alpha[126];    // dati generali letti dal file dba.txt

// Derivate aerodinamiche lette dal file dba.txt
double Cx[126], Cy[126], Cz[126], Cl[126], Cm[126], Cn[126];
double Cxa[126], Cxap[126], Cxu[126], Cxq[126], Cxb[126], Cxp[126], Cxr[126];
double Cyb[126], Cybp[126], Cyp[126], Cyr[126], Cya[126], Cyq[126];
double Czalpha[126], Czap[126], Czu[126], Czq[126], Czb[126], Czp[126], Czr[126];
double ClB[126], ClBP[126], ClP[126], ClR[126], ClA[126], ClQ[126];
double CmA[126], CmAP[126], CmU[126], CmQ[126], CmB[126], CmP[126], CmR[126];
double CnB[126], CnBP[126], CnP[126], CnR[126], CnA[126], CnQ[126];
double CXde[126], CXdle[126], CZde[126], CZdle[126], CYda[126], CYdr[126];
double Clda[126], Cldr[126], Cmde[126], Cmdle[126], Cnda[126], Cndr[126];
double CXom[126],CYom[126], CZom[126], Clom[126], Cmom[126], Cnom[126];

double press0 = 101325, temp0 = 15, rho0 = 1.225, vsuono0 = 340;    // Valori atmosfera a quota 0
double press_h ,temp_h ,rho_h, vsuono_h, P_max_h, mach;    // Valori atmosfera a quota h scelta
double propThrust, propTorque, P_trim;  // Spinta e Coppia motore
double CI[3]={0,0,0};   // Condizioni iniziali di velocità, quota e rampa {V,h,gamma}
double trim_angles[2], RPM_trim, g = 9.81;    // angoli di trim {alfa, delta_e} e RPM di trim
double vett_stato[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // Vettore di stato istante iniziale {u,v,w,p,q,r,phi,theta,psi,h,x,y,massa,T,P}
double vett_comm[4] = {0, 0, 0, 0}; // Vettore dei comandi istante iniziale {delta_e,delta_a,delta_r,RPM}
short int flag_eq_equilibratore = 0;
int flagatm;
double fugoide[4], CP[4];   // Valori di fugoide e corto periodo

int file_count;
char file_count_char[20];

double t_trim = 1000, t_sim;

int main(){
    ////// INTESTAZIONE /////
    // printf("\t\t\t       _____\n");
    // printf("\t\t\t         |\n");
    // printf("\t\t\t\\______0(.)0______/\n\n");
    printf("\t\t\t\t\t|\n");
    printf("\t  ______________________________|_______________________________\n");
    printf("\t\t\t      ----\\--||___||--/----\n");
    printf("\t\t\t\t   \\ :==e==: /\n");
    printf("\t\t\t\t    \\|  o  |/\n");
    printf("\t\t\t\t     \\_____/\n");
    printf("\t\t\t\t     /  |  \\ \n");
    printf("\t\t\t\t   e/   e   \\e\n");
    printf("\t\t\t\t   U    U    U\n");
    printf("\t\t\t        SIMULATORE DI VOLO\n");
    printf("\t\t\t       Politecnico di Torino\n");
    printf("\t\t    Corso di Simulazione del Volo - Luglio 2024\n");
    printf("\t\t  Sviluppato da: GRUPPO 03 per Cessna 172 Skyhawk\n");

    printf("\033[?25l"); // nasconde il cursore
    printf("\nPremere invio per avviare il simulatore\n");
    getchar();
    system("cls");

    ///// LETTURA FILE ////

    printf("Caricamento file database aerodinamico\n");
    readDba();
    printf("Database aerodinamico letto correttamente...\n\n");
    printf("Caricamento file database motore\n");
    readEngine();
    printf("Database motore letto correttamente...\n\n");
    printf("Caricamento file database propeller\n");
    readPropeller();
    printf("Database propeller letto correttamente...\n\n");
    printf("Premere invio per continuare");
    getchar();
    system("cls");

    ////// ANALISI TRIM /////

    // INPUT CONDIZIONI INIZIALI //

    printf("\033[?25h"); // Mostra il cursore
    char input_utente;
    int flag_alfa, flag_RPM, flag_stabilita;
    do{
        do{
            input_utente = '\0';
            do{
                system("cls");
                flagatm =AtmosphereChoice();
            }while(flagatm != 0);

            printf("Conferma Input? [y/n] ");
            scanf(" %c",&input_utente);
            if(input_utente!='y' && input_utente!='n'){
                printf("Inserire y o n! ");
                scanf(" %c",&input_utente);
            }
            system("cls");
        }while(input_utente!='y');

        // CONDIZIONI DI TRIM //

        printf("\nInizio analisi condizioni trim...\n");

        flag_alfa = alpha_trim_calc(CI[0], CI[1], rho_h, CI[2],g, trim_angles);
        
        if(flag_alfa == 0){
            flag_RPM = rpm_trim_calc(trim_angles[0], trim_angles[1], CI[0], CI[1], rho_h, CI[2], g,engine[2], engine[3]);
            flag_stabilita = stabilita(trim_angles[1], CI, rho_h, trim_angles[0], g);
			vett_stat_trim(trim_angles, vett_stato, CI); //Scrivi su vett di stato
			vett_com_trim(trim_angles, vett_comm, RPM_trim); //Scrivi su vett comandi
        }

    }while(flag_alfa != 0 && flag_stabilita !=0);


    // RICHIESTA PER PROCEDERE CON LA MANOVRA // 

    input_utente = '\0';
    printf("\n\nSi vuole procedere con l'analisi delle manovre? [y/n] ");
    scanf(" %c", &input_utente);
    if(input_utente != 'y' && input_utente != 'n'){
        printf("inserire y o n! ");
        scanf(" %c", &input_utente);
    }
    if(input_utente == 'n'){
        printf("\033[?25l"); // nasconde il cursore
        printf("\n\nSIMULATORE ARRESTATO. \nPremere invio per chiudere");
        getchar();
        getchar();
        return 0;
    }

    system("cls");
    printf("\n\t\tANALISI DI MANOVRA\n\n");
    printf("condizione di trim iniziale: | V = %.2lf m/s | h = %.2lf m | Angolo di rampa %.2lf deg | \n", CI[0], CI[1], CI[2]);

    // MANTENIMENTO CONDIZIONI DI TRIM //

    printf("\nSi vuole effettuare l'analisi di mantenimento del trim? [y/n] ");
    scanf(" %c",&input_utente);
    if(input_utente != 'y' && input_utente != 'n'){
        printf("Inserire y o n! ");
        scanf(" %c",&input_utente);
    }

    if(input_utente == 'y'){
        system("cls");
        input_utente = '\0';
        file_count = 0;
        comandi_trim(vett_comm);
        //Integrazione delle equazioni del moto
        printf("MANOVRA - Mantenimento del trim\nSimulazione in corso: \n");
        integrazione(0.01,t_trim);
        printf("\033[?25l"); // nasconde il cursore
        printf("Premere invio per continuare");
        getchar();
        getchar();

    }
    file_count = 1;
    do{
        printf("\033[?25h"); // Mostra il cursore
        input_utente = '\0';
        system("cls");
        printf("condizione di trim iniziale: | V = %.2lf m/s | h = %.2lf m | Angolo di rampa %.2lf deg | \n", CI[0], CI[1], CI[2]);
        printf("\nSi vuole procedere all'analisi di una nuova manovra con queste condizioni iniziali? [y/n] ");
        scanf(" %c",&input_utente);
        if(input_utente!='y' && input_utente!='n'){
            printf("Inserire y o n! ");
            scanf(" %c",&input_utente);
        }
        if(input_utente == 'n'){
            break;
        }
        
        printf("\nInserire la durata della manovra: ");
        int f=scanf("%lf",&t_sim);
        getchar();
        // if(t_sim>600){
        //     printf("Si e' inserito un tempo superiore ai 600 secondi\n La simulazione si arrestera' a 600 secondi\n");
        //     t_sim=600;
        // }
        genera_comandi(t_sim/0.01, 0.01, vett_comm);
        printf("Premere invio per continuare");
        getchar();
        system("cls");
        //Integrazione delle equazioni del moto
        printf("MANOVRA \nSimulazione in corso: \n");
        integrazione(0.01,t_sim);
        printf("\033[?25l"); // nasconde il cursore
        printf("Premere invio per continuare");
        getchar();

        file_count++;

    }while(input_utente != 'n');

    printf("\033[?25l"); // nasconde il cursore
    printf("\nSIMULAZIONE TERMINATA\n");
    printf("Premere invio per chiudere");
    getchar();
    getchar();

    return 0;
}
