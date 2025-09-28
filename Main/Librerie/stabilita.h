#ifndef STABILITA_H_INCLUDED
#define STABILITA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Il vettore CI � il vettore delle condizioni iniziali cos� definito: [V, h, gamma]
// Abbiamo definito come costante g

int stabilita (double delta_e_trim, double CI[], double rho, double alpha_trim, double g) {
    
    extern double general_data[30], alpha[126];
    extern double fugoide[4], CP[4];
    extern double trim_angles[2];
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

    int flag_stabilita = 0;

    double CmA_int = interpola(alpha, CmA, trim_angles[0]);
    double Cm_int = interpola(alpha, Cm, trim_angles[0]);
    double Cmde_int = interpola(alpha, Cmde, trim_angles[0]);
    double CXA_int = interpola(alpha, Cxa, trim_angles[0]);
    double Czalpha_int = interpola(alpha, Czalpha, trim_angles[0]);
    double CmAP_int = interpola(alpha, CmAP, trim_angles[0]);
    double CmQ_int = interpola(alpha, CmQ, trim_angles[0]);
    double Czap_int = interpola(alpha, Czap, trim_angles[0]);

    double m = general_data[0];
    double S = general_data[2];
    double I_y = general_data[14];
    double c = general_data[3];
    double mu =(2*m)/(rho*S*c);
    double I_yad =(8*I_y)/(rho*S*pow(c,3));

    double CLap = - Czap_int*cos(alpha_trim*M_PI/180);
    double CL_alpha =CXA_int*sin(alpha_trim*M_PI/180) - Czalpha_int*cos(alpha_trim*M_PI/180);
    double CWe = (2*m*g)/(rho*pow(CI[0],2)*S);
    double CLe = CWe; // da verificare se non ha più senso imporre CLe=CWe
    double CDe = 0.0235 + 0.047*pow(CLe,2);
    double CTu = -3*CDe;
    double CD_alpha = 2*0.047*CL_alpha*CLe;

    double A1 = 2*mu*I_yad*(2*mu + CLap);
    double B1 = 2*mu*I_yad*(CL_alpha + CDe - CTu) - I_yad*CTu*CLap - 2*mu*CmQ_int*CLap - 4*pow(mu,2)*(CmQ_int + CmAP_int);
    double C1 = 2*mu*(CmQ_int*(CTu - CL_alpha - CDe) - 2*mu*CmA_int + CmAP_int*CTu) + I_yad*(2*CWe*(CWe - CD_alpha) + CTu*CL_alpha + CDe*CL_alpha) + CmQ_int*CLap*CTu;
    double D1 = -2*pow(CWe,2)*CmAP_int + 2*mu*CTu*CmA_int + CTu*CmQ_int*CL_alpha - 2*CWe*CmQ_int*(CLe - CD_alpha) + 2*CDe*CmQ_int*CTu;
    double E1 = -2*pow(CWe,2)*CmA_int;
    double delta = B1*C1*D1 - A1*pow(D1,2) - pow(B1,2)*E1;

    if (E1<=0 || delta<=0){
        printf("ERRORE -15: Velivolo instabile secondo il criterio di Routh\n");
        flag_stabilita = -1;
    }

    else{
        printf("\nIl velivolo risulta staticamente stabile.\n");
        printf("Il velivolo risulta dinamicamente stabile secondo il criterio di Routh\n");

        double omegan_FUG = (2*CI[0]*CWe)/(sqrt(2)*mu*c);
        double zita_FUG = -CTu/(2*sqrt(2)*CWe);
        double eta_FUG = -zita_FUG*omegan_FUG;
        double omega_FUG = omegan_FUG*sqrt(1 - pow(zita_FUG,2));
        double T_FUG = (2*M_PI)/omega_FUG;
        double t_dimFUG = log(0.5)/eta_FUG;
        fugoide[0] = omegan_FUG;
        fugoide[1] = zita_FUG;
        fugoide[2] = T_FUG;
        fugoide[3] = t_dimFUG;

        double omegan_CP = sqrt((-CmA_int/I_yad))*(2*CI[0]/c);
        double zita_CP = (I_yad*CL_alpha - 2*mu*(CmQ_int + CmAP_int))/(2*sqrt(-2*mu*I_yad*(2*mu*CmA_int + CmQ_int*CL_alpha)));
        double eta_CP = -zita_CP*omegan_CP;
        double omega_CP = omegan_CP*sqrt(1 - pow(zita_CP,2));
        double T_CP = (2*M_PI)/omega_CP;
        double t_dimCP = log(0.5)/eta_CP;
        CP[0] = omegan_CP;
        CP[1] = zita_CP;
        CP[2] = T_CP;
        CP[3] = t_dimCP;
        printf("\n*************** MODI LONGITUDINALI ***************\n\n");
        printf("FUGOIDE:		Pulsazione propria	= %lf rad/s\n", omegan_FUG);
        printf("			Smorzamento		= %lf\n", zita_FUG);
        printf("			Periodo			= %lf secondi\n", T_FUG);
        printf("			Tempo di dimezzamento	= %lf secondi\n", t_dimFUG);

        printf("CORTO PERIODO:	        Pulsazione propria	= %lf rad/s\n", omegan_CP);
        printf("			Smorzamento		= %lf\n", zita_CP);
        printf("			Periodo			= %lf secondi\n", T_CP);
        printf("			Tempo di dimezzamento	= %lf secondi\n", t_dimCP);
    }
    return flag_stabilita;
}


#endif
