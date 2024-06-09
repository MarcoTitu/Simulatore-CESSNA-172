#include <stdio.h>
#include <stdlib.h>
#include "read.h"
#include "verificaLettura.h"
#include "interpola.h"

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



int main()
{
	double Cx_int, Cy_int, Cz_int, Cl_int, Cm_int, Cn_int;
    double Cxa_int, Cxap_int, Cxu_int, Cxq_int, Cxb_int, Cxp_int, Cxr_int;
    double Cyb_int, Cybp_int, Cyp_int, Cyr_int, Cya_int, Cyq_int;
    double Czalpha_int, Czap_int, Czu_int, Czq_int, Czb_int, Czp_int, Czr_int;
    double ClB_int, ClBP_int, ClP_int, ClR_int, ClA_int, ClQ_int;
    double CmA_int, CmAP_int, CmU_int, CmQ_int, CmB_int, CmP_int, CmR_int;
    double CnB_int, CnBP_int, CnP_int, CnR_int, CnA_int, CnQ_int;
    double CXde_int, CXdle_int, CZde_int, CZdle_int, CYda_int, CYdr_int;
    double Clda_int, Cldr_int, Cmde_int, Cmdle_int, Cnda_int, Cndr_int;
    double CXom_int,CYom_int, CZom_int, Clom_int, Cmom_int, Cnom_int;

    double alpha_des, differenza;
    int indice_numero_vicino = 0, j=1;
    double Cx_int2, Cxa_int2, Cyb_int2, Czalpha_int2, ClB_int2, CmA_int2, CnB_int2, CXde_int2, Clda_int2, CXom_int2;



    //Lettura Engine.txt
    readEngine();

    //Lettura Propeller.txt
    readPropeller();

    //Lettura Dba.txt
    readDba();


    printf("\n==============================\nVERIFICA LETTURA FILE\n==============================\n");
    verificaLettura();

    printf("\n==============================\nVERIFICA INTERPOLAZIONE\n==============================\n");

	printf("Inserire un angolo alfa desiderato (si ricorda che deve essere un valore compreso tra -5 e +20)\n");

    scanf ("%lf", &alpha_des);
    while (alpha_des < -5 || alpha_des > 20)
    {
        printf("E' stato inserito un valore di alfa al di fuori dell'intervallo accettabile. Si richiede, quindi, di digitare nuovamente l'alfa desiderato (tra -5 e +20)\n");
        scanf("%lf", &alpha_des);
    }

    // Vengono calcolate le derivate aerodinamiche in corrispondenza di alfa desiderato
    // attraverso l'interpolazione lineare dei valori tabulati nel database aerodinamico
    Cx_int = interpola(alpha, Cx, alpha_des);

    Cxa_int = interpola(alpha, Cxa, alpha_des);

    Cyb_int = interpola(alpha, Cyb, alpha_des);

    ClB_int = interpola(alpha, ClB, alpha_des);

    CmA_int = interpola(alpha, CmA, alpha_des);

    CnB_int = interpola(alpha, CnB, alpha_des);

    CXde_int = interpola(alpha, CXde, alpha_des);

    Clda_int = interpola(alpha, Clda, alpha_des);

    CXom_int = interpola(alpha, CXom, alpha_des);


    // Si effettua l'interpolazione lineare delle derivate aerdinamiche tramite un metodo differente da quello
    // implementato nella funzione "interpola" così da poter confrontare i risultati che si ottengono e verificarne
    // il corretto funzionamento.
    // In particolare, mentre nella funzione "interpola" l'interpolazione viene effettuata tramite l'equazione della
    // retta passante per due punti (y-y0 = m(x - x0), dove y rappresenta una derivata aerodinamica e x l'angolo alfa),
    // di seguito si sfrutta la relazione "y(x) = y_j * (x_(j+1) - x)/(x_(j+1) - x_j)  +  y_(j+1) * (x - x_j)/(x_(j+1) - x_j)".
    differenza = alpha_des - alpha[0];
    while(differenza >= 0 && j<126){
    differenza = alpha_des - alpha[j];   // Si effettua questo ciclo while per individuare l'intervallo in cui l'alfa desiderato si trova
    j++;
    }
    indice_numero_vicino = j-2;
    Cx_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Cx[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Cx[indice_numero_vicino+1];

    Cxa_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Cxa[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Cxa[indice_numero_vicino+1];

    Cyb_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Cyb[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Cyb[indice_numero_vicino+1];

    ClB_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*ClB[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*ClB[indice_numero_vicino+1];

    CmA_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CmA[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CmA[indice_numero_vicino+1];

    CnB_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CnB[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CnB[indice_numero_vicino+1];

    CXde_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CXde[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CXde[indice_numero_vicino+1];

    Clda_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Clda[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*Clda[indice_numero_vicino+1];

    CXom_int2 = (alpha[indice_numero_vicino+1] - alpha_des)/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CXom[indice_numero_vicino] + (alpha_des - alpha[indice_numero_vicino])/(alpha[indice_numero_vicino+1] - alpha[indice_numero_vicino])*CXom[indice_numero_vicino+1];


    printf("Di seguito viene riportato una verifica a campione di derivate aerodinamiche calcolate tramite la funzione 'interpola'\n");
    printf("(colonna di sinistra) e tramite un metodo alternativo di verifica (seconda colonna da sinistra). Nelle altre due\n");
    printf("colonne, invece, vengono riportati i due valori di derivate aerodinamiche tratti dal database aerodinamico e che\n");
    printf("corrispondono agli estremi dell'intervallo entro cui la derivata selezionata e' compresa\n");
    // Si scrivono i valori delle derivate aerodinamiche calcolate sul file denominato "derivate_interpolate.txt"
    printf ("Cx = %.8lf \tCx di verifica = %.8lf\tCx_dba = %.8lf\tCx_dba = %.8lf\n", Cx_int, Cx_int2, Cx[indice_numero_vicino], Cx[indice_numero_vicino+1]);

    printf ("Cxa = %.8lf\tCxa di verifica = %.8lf\tCxa_dba = %.8lf\tCxa_dba = %.8lf\n", Cxa_int, Cxa_int2, Cxa[indice_numero_vicino], Cxa[indice_numero_vicino+1]);

    printf ("Cyb = %.8lf\tCxyb di verifica = %.8lf\tCyb_dba = %.8lf\tCyb_dba = %.8lf\n", Cyb_int, Cyb_int2, Cyb[indice_numero_vicino], Cyb[indice_numero_vicino+1]);

    printf ("ClB = %.8lf\tClB di verifica = %.8lf\tClB_dba = %.8lf\tClB_dba = %.8lf\n", ClB_int, ClB_int2, ClB[indice_numero_vicino], ClB[indice_numero_vicino+1]);

    printf ("CmA = %.8lf\tCmA di verifica = %.8lf\tCmA_dba = %.8lf\tCmA_dba = %.8lf\n", CmA_int, CmA_int2, CmA[indice_numero_vicino], CmA[indice_numero_vicino+1]);

    printf ("CnB = %.8lf\tCnB di verifica = %.8lf\tCnB_dba = %.8lf\tCnB_dba = %.8lf\n", CnB_int, CnB_int2, CnB[indice_numero_vicino], CnB[indice_numero_vicino+1]);

    printf ("CXde = %.8lf\tCXde di verifica = %.8lf\tCXde_dba = %.8lf\tCXde_dba = %.8lf\n", CXde_int, CXde_int2, CXde[indice_numero_vicino], CXde[indice_numero_vicino+1]);

    printf ("Clda= %.8lf\tClda di verifica = %.8lf\tClda_dba = %.8lf\tClda_dba = %.8lf\n", Clda_int, Clda_int2, Clda[indice_numero_vicino], Clda[indice_numero_vicino+1]);

    printf ("CXom= %.8lf\tCXom di verifica = %.8lf\tCXom_dba = %.8lf\tCXom_dba = %.8lf\n", CXom_int, CXom_int2, CXom[indice_numero_vicino], CXom[indice_numero_vicino+1]);


    system("pause");
    return 0;
}
