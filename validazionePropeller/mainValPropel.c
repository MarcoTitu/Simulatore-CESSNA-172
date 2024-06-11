#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "read.h"
#include "propel.h"


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

//Test trim
double propThrust, propTorque;
double V,RPM,rho,h;

int flag=0;

int main()
{

	//Lettura Propeller.txt
    readPropeller();
    printf("\n==================================================");
	printf("\nValidazione implementazione Blade Element Theory");
	printf("\n==================================================");

	printf("\nInserire 1 per utilizzare condizioni di riferimento predefinite, \ninserire 2 per inserire condizioni di riferimento personalizzate: ");
	scanf("%d", &flag);

    if(flag==1){
	printf("\nAlcune condizioni di riferimento sono riportate per verificare l'implementazione BET");




	printf("\n\n<><><><><><><><><><><><");
	V=52;
	RPM=1643;
	h=1000;
	rho=1.111;
	printf("\nCondizione di trim:\nV[m/s]: %lf\nRPM: %lf\nh[m]: %lf", V, RPM, h);

	propel(V,RPM,rho);

	printf("\nRisultati BET: ");
	printf("\nSpinta[N]= %lf \nCoppia[Nm]= %lf\n", propThrust, propTorque);

    printf("\n\n<><><><><><><><><><><><");
	V=56;
	RPM=1773;
	h=3000;
	rho=0.909;
	printf("\nCondizione di trim:\nV[m/s]: %lf\nRPM: %lf\nh[m]: %lf", V, RPM, h);

	propel(V,RPM,rho);

	printf("\nRisultati BET: ");
	printf("\nSpinta[N]= %lf \nCoppia[Nm]= %lf\n", propThrust, propTorque);
	}

	if(flag==2){

	printf("\nInserire la velocita' di volo: ");
	scanf("%lf", &V);

	printf("\nInserire gli RPM: ");
	scanf("%lf", &RPM);

	printf("\nInserire la densita' dell'aria: ");
	scanf("%lf", &rho);

	propel(V,RPM,rho);

	printf("\nRisultati BET: ");
	printf("\nSpinta[N]= %lf \nCoppia[Nm]= %lf\n", propThrust, propTorque);

	}

	if(flag!=1 && flag!=2){
		printf("\nErrore input! Inserire 1 o 2!\n Programma terminato.");
	}

    system("pause");
    return 0;
}
