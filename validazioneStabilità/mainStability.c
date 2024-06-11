#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "read.h"
#include "interpola.h"
#include "stabilita.h"

double engine[7], propeller_data[11], CSI[60], RD[60], CH[60], BA[60], general_data[30], alpha[126];
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

double fugoide[4], CP[4];
int indice_numero_vicino=0;

int main()
{

    double CI[3] = {52, 1000, 0};      // CI(0) = velocità, CI(1)= quota, CI(2)=gamma

    //Lettura Engine.txt
    readEngine();

    //Lettura Propeller.txt
    readPropeller();

    //Lettura Dba.txt
    readDba();

    double alpha_trim = 1.90;
    double delta_e_trim = -4.46;
    double rho = 1.111;             // densità in corrispondenza di h = 1000 m

	printf("\n<><><><><><><><><><><><><><><><><><><><><><><><>");
	printf("\nValidazione (stabilita' dinamica longitudinale)");
	printf("\n<><><><><><><><><><><><><><><><><><><><><><><><>");
	printf("\n\nCondizione di trim di riferimento:");
	printf("\nV: 52.00 [m/s]");
	printf("\nAoA: %lf [deg]", alpha_trim);
	printf("\ndelta_eq: %lf [deg]", delta_e_trim);
	printf("\nQuota: 1000 [m]\n\n\n");

    double CmA_int = interpola(alpha, CmA, alpha_trim);
    double Cm_int = interpola(alpha, Cm, alpha_trim);
    double Cmde_int = interpola(alpha, Cmde, alpha_trim);
    double CXA_int = interpola(alpha, Cxa, alpha_trim);
    double Czalpha_int = interpola(alpha, Czalpha, alpha_trim);
    double CmAP_int = interpola(alpha, CmAP, alpha_trim);
    double CmQ_int = interpola(alpha, CmQ, alpha_trim);
    double Czap_int = interpola(alpha, Czap, alpha_trim);

    stabilita (CmA_int, Cm_int, Cmde_int, delta_e_trim, CI, CXA_int, Czalpha_int, CmAP_int, CmQ_int, Czap_int, rho, alpha_trim);
    printf("\n MODO FUGOIDE:\n pulsazione (parte immaginaria dell'autovalore) = %lf\n smorzamento = %lf\n periodo = %lf\n tempo di dimezzamento = %lf\n", fugoide[0], fugoide[1], fugoide[2], fugoide[3]);
    printf("\n MODO DI CORTO PERIODO:\n pulsazione (parte immaginaria dell'autovalore) = %lf\n smorzamento = %lf\n periodo = %lf\n tempo di dimezzamento = %lf\n\n", CP[0], CP[1], CP[2], CP[3]);


    system("pause");
    return 0;
}
