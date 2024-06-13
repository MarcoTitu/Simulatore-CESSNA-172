#ifndef SIMULATORE_ATMOSPHERE_H
#define SIMULATORE_ATMOSPHERE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define RESET "\033[0m"
#define GIALLO "\033[0;33m"
#define ROSSO "\033[0;31m"



int atmospherecalc(double h, double V){
    extern double press0, temp0, rho0, vsuono0;    // Valori atmosfera a quota 0 
    extern double press_h ,temp_h ,rho_h, vsuono_h, mach;    // Valori atmosfera a quota h scelta
    extern double engine[7];
    double R=287.05, gm_gas=1.4;
    int flagatm;

    double temp_0;
    temp_0= temp0 + 273.15;
    temp_h=temp_0 - 0.0065*h;
    press_h=press0*(pow((temp_h/(temp_0)),5.2561));
    rho_h=press_h/(R*(temp_h));
    vsuono_h=sqrt(gm_gas*R*temp_h);
    mach = V/vsuono_h;
    if(mach > 0.6){
        flagatm = -1;
        printf(ROSSO "ERRORE -4: Mach= %.2lf superiore al Mach di Drag Divergence (M_DD=0.6)\n" RESET,mach);
    }else{
        flagatm = 0;
    }
    return flagatm;
}

int AtmosphereChoice(){
    extern double press0, temp0, rho0, vsuono0;    // Valori atmosfera a quota 0 
    extern double press_h ,temp_h ,rho_h, vsuono_h, mach;    // Valori atmosfera a quota h scelta
    extern double engine[7];
    extern double CI[3];   // Condizioni iniziali di velocità, quota e rampa {V,h,gamma}
    double R=287.05, gm_gas=1.4;
    int flagatm;

    printf("INPUT ATMOSFERA E CONDIZIONI DI VOLO\n");
    printf("\nLa simulazione fa riferimento al modello atmosferico ISA, avente i segeunti valori per quota h=0m (sea level):\n\n");
    printf("\tPressione: \t\tP = %lf Pa\n",press0);
    printf("\tTemperatura: \t\tT = %lf C\n",temp0);
    printf("\tDensita': \t\trho = %lf kg/m^3\n",rho0);
    printf("\tVelocita' del Suono: \ta = %lf m/s\n",vsuono0);
    
    int input = 0;
    printf("\nSe non si desidera procedere con i suddetti parametri e' possibile modificarli, reinserendoli manualmente o scegliendo una quota differente.\n");
    printf("\nPremere:\n");
    printf("\t[1] se si desidera procedere con i valori iniziali precedentemente indicati\n"); // mantiene i valori iniziali di default
    printf("\t[2] se si desidera modificare i valori iniziali a h=0\n"); //modifica i valori iniziali di default
    printf("\t[3] se di desidera immettere i valori manualmente a una quota specifica (richiesta in seguito)\n");// immette i valori manualmente a una data quota senza richiederla poi
    int f = scanf("%d", &input);


    if(f!=1||input!=1&&input!=2&&input!=3){
        printf("Errore input, inserire 1,2 o 3\n");
        system("pause");
        flagatm = -101;
        return flagatm;
    }

    if(input==1){
        printf("Inserire la quota di volo in m [0-4116]\t\t");
        scanf("%lf",&CI[1]);
        if(CI[1]>4116){
            printf(GIALLO "WARNING: Quota superiore a quota di tangenza, ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-102;
            return flagatm;
        }
        if(CI[1]<0){
            printf(GIALLO "WARNING: Quota troppo bassa, ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-102;
            return flagatm;
        }
        printf("Inserire la velocita' di volo in m/s [30-75]\t");
        scanf("%lf", &CI[0]);
        if(CI[0]>75){
            printf(GIALLO "WARNING: Velocita' superiore alla V_NE (146 KIAS-75 m/s), ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-105;
            return flagatm;
        }
        if(CI[0]<30){
            printf(GIALLO "WARNING: Velocita' inferiore alla velocita' minima (58 KIAS-30 m/s), ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-105;
            return flagatm;
            }
        printf("Inserire l'angolo di rampa gamma in gradi:\t");
        scanf("%lf",&CI[2]);
        flagatm=atmospherecalc(CI[1],CI[0]);
    }

    if (input ==2){
        char input_ut;
        printf("Si vuole modificare la temperatura T0? [y/n] ");
        scanf(" %c",&input_ut);
        if(input_ut!='y' && input_ut!='n'){
            printf("Inserire y o n! ");
            scanf(" %c",&input_ut);
        }
        if(input_ut=='y'){
            printf("Inserire temperatura in C\t");
            scanf("%lf",&temp0);
        }
        getchar();
        printf("Si vuole modificare la pressione p0? [y/n] ");
        scanf(" %c",&input_ut);
        if(input_ut!='y' && input_ut!='n'){
            printf("Inserire y o n! ");
            scanf(" %c",&input_ut);
        }
        if(input_ut=='y'){
            printf("Inserire pressione in Pa\t");
            scanf("%lf",&press0);
        }
        rho0=press0/(R*(temp0+273.15));
        vsuono0=sqrt(gm_gas*R*(temp0+273.15));
        printf("\nNuove condizioni a quota 0m: \n\n");
        printf("\tPressione: \t\tP = %lf Pa\n",press0);
        printf("\tTemperatura: \t\tT = %lf C\n",temp0);
        printf("\tDensita': \t\trho = %lf kg/m^3\n",rho0);
        printf("\nPremere invio per continuare");
        getchar();
        getchar();
        flagatm=-1000;
    }

    if(input==3){
        printf("Inserire la temperatura in C\t");
        scanf("%lf",&temp_h);
        temp_h=temp_h+273.15;
        printf("Inserire la pressione in Pa\t");
        scanf("%lf",&press_h);
        if(press_h<0){
            printf(GIALLO "WARNING: Pressione minore di 0 Pa, ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-103;
            return flagatm;
        }
        printf("Inserire la quota di volo in m [0-4116]\t\t");
        scanf("%lf",&CI[1]);
        if(CI[1]>4116){
            printf(GIALLO "WARNING: Quota superiore a quota di tangenza, ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-102;
            return flagatm;
        }
        if(CI[1]<0){
            printf(GIALLO "WARNING: Quota troppo bassa, ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-102;
            return flagatm;
        }
        printf("Inserire la velocita' di volo in m/s [30-75]\t");
        scanf("%lf",&CI[0]);
        if(CI[0]>75){
            printf(GIALLO "WARNING: Velocita' superiore alla V_NE (146 KIAS-75 m/s), ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-105;
            return flagatm;
        }
        if(CI[0]<30){
            printf(GIALLO "WARNING: Velocita' inferiore alla velocita' minima (58 KIAS-30 m/s), ripetere immissione dati\nPremere invio per procedere" RESET);
            getchar();
            getchar();
            flagatm=-105;
            return flagatm;
        }
        printf("Inserire l'angolo di rampa gamma in gradi:\t");
        scanf("%lf",&CI[2]);
        rho_h=press_h/(R*temp_h);
        vsuono_h=sqrt(gm_gas*R*temp_h);
        mach=CI[0]/vsuono_h;
        flagatm=0;
        if(mach>0.6){
        printf(ROSSO "ERRORE -10: Mach=%.2f superiore al Mach di Drag Divergence (M_DD=0.6)\n" RESET,mach);
        flagatm=-1;
        }
    }
    if(flagatm==-1){
        flagatm=-104;
        printf("Ripetere immissione dati\nPremere invio per procedere");
        getchar();
        getchar();
    }
    if(flagatm==0){
        printf("\nRiepilogo Condizioni di Volo\n");
        printf("Temperatura: \t\t%f [C]\n",temp_h-273.15);
        printf("Pressione: \t\t%f [Pa]\n",press_h);
        printf("Densita': \t\t%f [kg/m^3]\n",rho_h);
        printf("Quota: \t\t\t%f [m]\n",CI[1]);
        printf("Velocita': \t\t%f [m/s]\n",CI[0]);
        printf("Mach: \t\t\t%f\n",mach);
        printf("Angolo di rampa: \t%f [deg]\n",CI[2]);
    }
    return flagatm;       
}
#endif //SIMULATORE_ATMOSPHERE_H
