#ifndef TRIM_H
#define TRIM_H

#include "interpola.h"
#include "propel.h"

#define RESET "\033[0m"
#define GIALLO "\033[0;33m"
#define ROSSO "\033[0;31m"


extern double engine[7];
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
double press0, temp0, rho0, vsuono0;    // Valori atmosfera a quota 0 
double press_h ,temp_h ,rho_h, vsuono_h, P_max_h, mach;    // Valori atmosfera a quota h scelta
double trim_angles[2], RPM_trim, P_max_h;
extern short int flag_eq_equilibratore;
extern double propThrust;
extern double propTorque;
extern double P_trim;


double delta_e_calc(double alpha_int){

    // leggere cm vari e verificare equazione di momento

    double delta_e_min = general_data[23];
    double delta_e_max = general_data[22];
    double delta_e = delta_e_min;
    double delta_e_trim = 0;
    double cm_ss = interpola(alpha, Cm, alpha_int);
    double cm_alpha = interpola(alpha, CmA, alpha_int);
    double cm_de = interpola(alpha, Cmde, alpha_int);
    double delta_delta_e = 0.01;     // diminuire se non converge o se da richiesta
    double delta_e_rad;
    double alpha_int_rad = alpha_int*M_PI/180.0;

    double de_analitico = (-cm_ss - cm_alpha*alpha_int_rad)/interpola(alpha, Cmde, alpha_int);
    delta_e = de_analitico;
    delta_e_trim = delta_e*180/M_PI;

    // printf("Delta_e (analitico): %lf", de_analitico*180/M_PI);
    // FINE TEST

    // while (delta_e <= delta_e_max){
    //     delta_e_rad = delta_e*M_PI/180.0;
    //     double condition = cm_ss + cm_alpha*alpha_int_rad + cm_de*delta_e_rad;
    //     double res = 0.0005;          // scegliere in modo piu' furbo la tolleranza

    //     if (fabs(condition) < res){
    //         delta_e_trim = delta_e;
    //         break;
    //     }

    //     else{
    //         delta_e = delta_e + delta_delta_e;
    //     }
    // }
    
    if (delta_e_trim > delta_e_max || delta_e_trim < delta_e_min){
        flag_eq_equilibratore = -1;
    }else{
        flag_eq_equilibratore = 0;
    }

    return delta_e_trim;
}





int alpha_trim_calc(double V, double h, double rho, double gamma, double g, double trim_angles[]){

    /*
    input utente sono V, h, gamma (gamma sara' nullo in primo tentativo)
    si parlava di avere V, h, rho come variabili globali (quindi non c'e' bisogno di passarle)
    */

    double delta_e_min = general_data[23];
    double delta_e_max = general_data[22];
    double m = general_data[0];
    double S = general_data[2];
    double alpha_int = alpha[0];
    double alpha_max = 20;
    double delta_alpha = 0.001;
    double alpha_trim = alpha_int;
    double Cz_delta_e = interpola(alpha, CZde, alpha_int);
    double Cz_alpha = interpola(alpha, Czalpha, alpha_int);
    double delta_e_trim = 0;
    short int flag_alfa = 0;

    printf("\nCalcolo alpha e delta equilibratore...\n");

    while(alpha_int <= alpha_max){
        Cz_delta_e = interpola(alpha, CZde, alpha_int);
        Cz_alpha = interpola(alpha, Czalpha, alpha_int);
        // BISOGNA USARE GLI ANGOLI IN RADIANTI
        double alpha_int_rad = alpha_int*M_PI/180.0;
        // Se alpha viene passato in gradi ricordiamoci la conversione in delta_e_calc
        delta_e_trim = delta_e_calc(alpha_int);

        double Cz_ss_interp = interpola(alpha, Cz, alpha_int);
        double Cz_tot = Cz_ss_interp + Cz_alpha*alpha_int_rad + Cz_delta_e*delta_e_trim*M_PI/180.0;
        // gamma è in gradi e va convertito
        double theta = alpha_int_rad + gamma*M_PI/180.0;
        double condition = m*g*cos(theta) + 0.5*Cz_tot*rho*S*(V*V);
        // DEBUG
        //printf("Alpha corrente: %lf \t Theta (rad): %lf \t  Delta_e: %lf\t Risultante su Z: %lf\n", alpha_int, theta, delta_e_trim, condition);

        double res = 10;  // [N]

        if (fabs(condition) < res){
            alpha_trim = alpha_int;
            if (flag_eq_equilibratore != 0){
                printf(GIALLO "WARNING: Non trovato valore di delta_e di trim\n" RESET);
                if(delta_e_trim<0){
                    delta_e_trim = delta_e_min;
                    printf("delta_e limitato a -20 deg\n");
                }
                if(delta_e_trim>0){
                    delta_e_trim = delta_e_max;
                    printf("delta_e limitato a 20 deg\n");
                }                
            }
            // Rimuovere se si torna a metodo non analitico
            //printf("ATTENZIONE: angolo di equilibratore ottenuto analiticamente!\n");
            // printf("Alpha trim: %lf \t Delta eq. trim: %lf\n", alpha_int, delta_e_trim);
            flag_alfa = 0;
            break;
        }else{
            alpha_int = alpha_int + delta_alpha;
        }
    }

    if (alpha_int > alpha_max){
        printf(ROSSO "ERRORE -5: Non trovato valore di alfa di trim\n" RESET);
        printf("Premere invio per inserire una nuova condizione di trim...\n");
        getchar();
        getchar();
        system("cls");
        flag_alfa = -1;
    }
    trim_angles[0] = alpha_trim;
    trim_angles[1] = delta_e_trim;

    printf("Alpha Trim: \t %lf [deg]\n", trim_angles[0]);
    printf("delta Trim: \t %lf [deg]\n", trim_angles[1]);

    return flag_alfa;
}


double rpm_trim_calc(double alpha_trim, double delta_e_trim, double V, double h, double rho, double gamma, double g, double RPM_min, double RPM_max){
    
    double omega_trim;
    int flag_RPM;
    double m = general_data[0];
    double S = general_data[2];
    double RPM = RPM_min;
    double delta_RPM = 1;  // [rpm] come prima approx, poi a discrezione utente
    double Cx_delta_e = interpola(alpha, CXde, alpha_trim);
    double Cx_alpha = interpola(alpha, Cxa, alpha_trim);
    double theta_trim = alpha_trim + gamma;
    theta_trim = theta_trim*M_PI/180.0;
    P_max_h = engine[0]* pow(rho_h/(rho0),engine[1]);

    printf("\nCalcolo spinta necessaria...\n");
    double Cx_tot = interpola(alpha, Cx, alpha_trim) + Cx_alpha*alpha_trim*M_PI/180.0 + Cx_delta_e*delta_e_trim*M_PI/180.0;
    double T_trim = m*g*sin(theta_trim) - 0.5*Cx_tot*rho*S*(V*V);
    printf("Spinta: \t %lf [N]\n", T_trim);


    printf("\nCalcolo Parametri Motore...\n");

    while (RPM <= RPM_max){
        propel(V, RPM, rho);  // da propel.c
        double T = propThrust;
        double condition = T_trim - T;
        double res = 2;

        if (fabs(condition) <= res){
            RPM_trim = RPM;
            omega_trim = 2*M_PI/60*RPM_trim;
            P_trim = propTorque*omega_trim/1000; // [kW]
            if (P_trim > P_max_h){
            printf(GIALLO "WARNING: Potenza massima superata\nPotenza limitata alla potenza massima\n" RESET);//Messaggio di warning
            P_trim=P_max_h;
            }
            break;
        }
        else{
            RPM = RPM + delta_RPM;
        }
    }

    if (RPM > RPM_max){
        printf(GIALLO "WARNING: Convergenza non raggiunta su RPM; RPM impostati al valore medio\n" RESET);
        RPM_trim = (RPM_max + RPM_min)/2;
        propel(V, RPM_trim, rho);  // da propel.c
        omega_trim = 2*M_PI/60*RPM_trim;
        P_trim = propTorque*omega_trim/1000; // [kW]
    }
    printf("Thrust: \t %lf [N]\n", propThrust);
    printf("Torque: \t %lf [Nm]\n", propTorque);
    printf("RPM: \t\t %lf\n", RPM_trim);
    printf("Power: \t\t %lf [kW]\n", P_trim);

    return flag_RPM;
}

void vett_stat_trim(double trim_angles[], double vett_stato[],double CI[]){
    double alpha_trim = trim_angles[0];
    double delta_e_trim = trim_angles[1];
    double V = CI[0];
    double h = CI[1];
    double gamma = CI[2];

    vett_stato[0] = V*cos(alpha_trim*M_PI/180);
    vett_stato[1] = 0;
    vett_stato[2] = V*sin(alpha_trim*M_PI/180);
    vett_stato[3] = 0;
    vett_stato[4] = 0;
    vett_stato[5] = 0;
    vett_stato[6] = 0;
    vett_stato[7] = alpha_trim + gamma;
    vett_stato[8] = 0;
    vett_stato[9] = h;
    vett_stato[10] = 0;
    vett_stato[11] = 0;
    vett_stato[12] = general_data[0];
    vett_stato[13] = propThrust;
    vett_stato[14] = P_trim*1000;
}

void vett_com_trim(double trim_angles[], double vett_comm[],double RPM_trim){
    vett_comm[0] = trim_angles[1];
    vett_comm[3] = (RPM_trim-engine[2])/(engine[3]-engine[2])*100;
}
#endif


