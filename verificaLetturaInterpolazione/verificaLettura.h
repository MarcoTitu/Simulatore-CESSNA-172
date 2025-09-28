#ifndef VERIFICALETT_H
#define VERIFICALETT_H


void verificaLettura();

void verificaLettura(){
//Variabili esterne da manipolare
extern double engine[7];

extern double propeller_data[11], CSI[60], RD[60], CH[60], BA[60];

extern double general_data[30];
extern double alpha[126];
extern double Cx[126], Cy[126], Cz[126], Cl[126], Cm[126], Cn[126];
extern double Cxa[126], Cxap[126], Cxu[126], Cxq[126], Cxb[126], Cxp[126], Cxr[126];
extern double Cyb[126], Cybp[126], Cyp[126], Cyr[126], Cya[126], Cyq[126];
extern double Czalpha[126], Czap[126], Czu[126], Czq[126], Czb[126], Czp[126], Czr[126];
extern double ClB[126], ClBP[126], ClP[126], ClR[126], ClA[126], ClQ[126];
extern double CmA[126], CmAP[126], CmU[126], CmQ[126], CmB[126], CmP[126], CmR[126];
extern double CnB[126], CnBP[126], CnP[126], CnR[126], CnA[126], CnQ[126];
extern double CXde[126], CXdle[126], CZde[126], CZdle[126], CYda[126], CYdr[126];
extern double Clda[126], Cldr[126], Cmde[126], Cmdle[126], Cnda[126], Cndr[126];
extern double CXom[126],CYom[126], CZom[126], Clom[126], Cmom[126], Cnom[126];

int i=0;

    printf("ENGINE\n\n");
    for(int k=0; k<7; k++){
        printf("%.10lf \n", engine[k]);
    }

    printf("\n\n");

    printf("\n\nPROPELLER \n\n");
    for(int k=0; k<11; k++){
        printf("%lf \n", propeller_data[k]);
    }

    printf("\n\n");

   i = 0;
    while (i < 60) {
        printf("%lf %lf %lf %lf\n", CSI[i], RD[i], CH[i], BA[i]);
        i++;
    }


    printf("\n\n");

    printf("\n\nDBA \n\n");
    for(int k=0; k<30; k++){
        printf("%lf \n", general_data[k]);
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf\n", alpha[i], Cx[i], Cy[i], Cz[i], Cl[i], Cm[i], Cn[i]);
        i++;
    }



    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf %lf\n", alpha[i], Cxa[i], Cxap[i], Cxu[i], Cxq[i], Cxb[i], Cxp[i], Cxr[i]);
        i++;
    }



    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf\n", alpha[i], Cyb[i], Cybp[i], Cyp[i], Cyr[i], Cya[i], Cyq[i]);
        i++;
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf %lf\n", alpha[i], Czalpha[i], Czap[i], Czu[i], Czq[i], Czb[i], Czp[i], Czr[i]);
        i++;
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf\n", alpha[i], ClB[i], ClBP[i], ClP[i], ClR[i], ClA[i], ClQ[i]);
        i++;
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf %lf\n", alpha[i], CmA[i], CmAP[i], CmU[i], CmQ[i], CmB[i], CmP[i], CmR[i]);
        i++;
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf\n", alpha[i], CnB[i], CnBP[i], CnP[i], CnR[i], CnA[i], CnQ[i]);
        i++;
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf\n", alpha[i], CXde[i], CXdle[i], CZde[i], CZdle[i], CYda[i], CYdr[i]);
        i++;
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf\n", alpha[i], Clda[i], Cldr[i], Cmde[i], Cmdle[i], Cnda[i], Cndr[i]);
        i++;
    }


    printf("\n\n");

    i = 0;
    while (i < 126) {
        printf("%lf %lf %lf %lf %lf %lf %lf\n", alpha[i], CXom[i],CYom[i], CZom[i], Clom[i], Cmom[i], Cnom[i]);
        i++;
    }


}
#endif
