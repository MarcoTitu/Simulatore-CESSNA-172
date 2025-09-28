#ifndef STABILITA_H_INCLUDED
#define STABILITA_H_INCLUDED

#define g 9.81

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Il vettore CI � il vettore delle condizioni iniziali cos� definito: [V, h, gamma]
// Abbiamo definito come costante g

void stabilita (double CmA_int, double Cm_int, double Cmde_int, double delta_e_trim,
                double CI[3], double CXA_int, double Czalpha_int, double CmAP_int,
                double CmQ_int, double Czap_int, double rho, double alpha_trim) {

    extern double general_data[30], alpha[126];
    extern double fugoide[4], CP[4];

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
        printf("ERROR: le condizioni iniziali digitate (V, h, gamma) non permettono di avere un velivolo dinamicamente stabile. \nLa simulazione viene quindi interrotta.\n");
        exit(EXIT_FAILURE);
    }

    else{
        double omegan_FUG = (2*CI[0]*CWe)/(sqrt(2)*mu*c);
        double zita_FUG = -CTu/(2*sqrt(2)*CWe);
        double eta_FUG = -zita_FUG*omegan_FUG;
        double omega_FUG = omegan_FUG*sqrt(1 - pow(zita_FUG,2));
        double T_FUG = (2*M_PI)/omega_FUG;
        double t_dimFUG = log(0.5)/eta_FUG;
        fugoide[0] = omega_FUG;
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
    }

    }


#endif
