#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "read.h"
#include "interpola.h"
#include "stabilita.h"
#include "atmosphere.h"
#include "trim.h"

// Variabili

double engine[7];                                           // Variabili lette dal file engine.txt
double propeller_data[11], CSI[60], RD[60], CH[60], BA[60]; // Variabili lette dal file propeller.txt
double general_data[30], alpha[126];                        // dati generali letti dal file dba.txt

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
double CXom[126], CYom[126], CZom[126], Clom[126], Cmom[126], Cnom[126];

double press0 = 101325, temp0 = 15, rho0 = 1.225, vsuono0 = 340;       // Valori atmosfera a quota 0
double press_h, temp_h, rho_h, vsuono_h, P_max_h, mach;                // Valori atmosfera a quota h scelta
double propThrust, propTorque, P_trim;                                 // Spinta e Coppia motore
double CI[3] = {0, 0, 0};                                              // Condizioni iniziali di velocità, quota e rampa {V,h,gamma}
double trim_angles[2], RPM_trim, g = 9.81;                             // angoli di trim {alfa, delta_e} e RPM di trim
double vett_stato[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Vettore di stato istante iniziale {u,v,w,p,q,r,phi,theta,psi,h,x,y,massa,T,P}
double vett_comm[4] = {0, 0, 0, 0};                                    // Vettore dei comandi istante iniziale {delta_e,delta_a,delta_r,RPM}
short int flag_eq_equilibratore = 0;
int flagatm;
double fugoide[4], CP[4]; // Valori di fugoide e corto periodo

int file_count;
char file_count_char[20];

int indice_numero_vicino = 0;

double t_trim = 1000, t_sim;

int main()
{
    // Lettura Engine.txt
    readEngine();

    // Lettura Propeller.txt
    readPropeller();

    // Lettura Dba.txt
    readDba();

    printf("<><><><><><><><><><><><><><><><><><><><><><><><>");
    printf("\nValidazione (Trim)");
    printf("\n<><><><><><><><><><><><><><><><><><><><><><><><>\n\n");

    printf("Inserire la quota di volo in m [0-4116]\t\t");
    scanf("%lf", &CI[1]);
    printf("Inserire la velocita' di volo in m/s [30-75]\t");
    scanf("%lf", &CI[0]);
    printf("Inserire l'angolo di rampa gamma in gradi:\t");
    scanf("%lf", &CI[2]);
    printf("\033[?25l"); // nasconde il cursore

    atmospherecalc(CI[1], CI[0]);
    alpha_trim_calc(CI[0], CI[1], rho_h, CI[2], g, trim_angles);
    rpm_trim_calc(trim_angles[0], trim_angles[1], CI[0], CI[1], rho_h, CI[2], g, engine[2], engine[3]);

    double alpha_trim = trim_angles[0];
    double delta_e_trim = trim_angles[1];

    printf("\npremere invio per procedere con validazione stabilita'\n");
    getchar();
    getchar();
    system("cls");

    printf("<><><><><><><><><><><><><><><><><><><><><><><><>");
    printf("\nValidazione (stabilita' dinamica longitudinale)");
    printf("\n<><><><><><><><><><><><><><><><><><><><><><><><>");
    printf("\n\nCondizione di trim di riferimento:");
    printf("\nV: %lf [m/s]", CI[0]);
    printf("\nAoA: %lf [deg]", alpha_trim);
    printf("\ndelta_eq: %lf [deg]", delta_e_trim);
    printf("\nQuota: %lf [m]\n\n", CI[1]);

    stabilita(trim_angles[1], CI, rho_h, trim_angles[0], g);

    system("pause");
    return 0;
}
