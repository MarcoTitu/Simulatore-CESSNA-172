#ifndef PROPEL_H
#define PROPEL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//NELLA SEGUENTE FUNIONE SI CALCOLANO LE PRESTAZIONI DELL'ELICA SFRUTTANDO LA TEORIA DELL'ELEMENTO DI PALA DI GLAUERT

void propel(double V, double RPM, double rho)
{
    extern double propThrust, propTorque;
	extern double propeller_data[11], CSI[60], RD[60], CH[60], BA[60];

    int n_max=49;
    double chord[n_max];
    double rstep[n_max];
    double BA1[n_max];
    double r1[n_max];
    for(int i=0;i<n_max;i++) //salvataggio dati propeller
    {
        int k = 11 + i;
        chord[i]=CH[k];
        rstep[i]=RD[k];
        BA1[i]=BA[k];
        r1[i]=CSI[k];
    }
    double diam=propeller_data[0]; //diametro elica
    double Raggio=diam/2.0; //raggio elica &&
    double n=RPM/60.0; //round-per-second
    double omega=n*2.0*M_PI; //veloci� angolare [rad/s]
    double rho1=rho; //velocit� dalla function.atmosfera
    double Vel=V; //da verificare

    double cl_0=propeller_data[6];
    double cl_alpha=propeller_data[5];
    double cd_0=propeller_data[10];
    double cd_alpha=propeller_data[9];
    double cd_alpha2=propeller_data[8];

    double alpha1;
    double a2[n_max],b2[n_max];
    double th, phi1, eff, DtDr,DqDr,cl,cd,CT,CQ, tem1,tem2;
    double a, anew;
    double b, bnew;

    int j=0;
    int finished=0;

    double rad;
    double Vlocal,V0,V2;
    double thrust=0.0; //inizializzazione vettore spinta
    double torque=0.0; //inizializzazione vettore coppia


    for(j=1; j<n_max; j++){

            rad=r1[j];
            th=BA1[j]/180.0*M_PI; //angolo di svergolamento in radianti

            a=0.1; //inizializzazione axial inflow factor
            b=0.01; //inizializzazione angular inflow (swirl) factor
            finished=0; //inizializzione flag

            int sum=1; //inizializzione variabile di supporto

            while (finished==0){

                    V0=Vel*(1+a); //componente assiale del flusso aumentata tramite l'axial inflow factor
                    V2=omega*rad*(1-b); //componente angolare del flusso all'incirca uguale alla velocit� angolare
                    phi1=atan2(V0,V2); //angolo tra le due componenti del flusso V0 e V2

                    alpha1=th-phi1; //angolo di attacco relativo alla j-esima sezione della pala

                    cl=cl_0+cl_alpha*alpha1; //CL coefficiente di portanza
                    cd=cd_0+cd_alpha*alpha1+cd_alpha2*alpha1*alpha1; // CD coefficiente di resistenza CD = CD0+CD1*CL+CD2*CL^2

                    Vlocal=sqrt(V0*V0+V2*V2); // velocit� locale del flusso
                    CT = cl*cos(phi1)-cd*sin(phi1); //CT coefficiente di spinta adimensionale
                    DtDr=0.5*rho1*Vlocal*Vlocal*2.0*chord[j]*CT; //contributo di spinta della j-esima sezione
                    CQ = cd*cos(phi1)+cl*sin(phi1); //CQ coefficiente di coppia adimensionale
                    DqDr=0.5*rho1*Vlocal*Vlocal*2.0*chord[j]*rad*CQ; //contributo di coppia della j-esima sezione

                    tem1=DtDr/(4.0*M_PI*rad*rho1*Vel*Vel*(1+a)); //fattore correttivo del coefficiente "a"
                    tem2=DqDr/(4.0*M_PI*rad*rad*rad*rho1*Vel*(1+a)*omega); //fattore correttivo del coefficiente "b"
                    anew=0.5*(a+tem1); //nuovo valore coefficiente "a"
                    bnew=0.5*(b+tem2); //nuovo valore coefficiente "b"

                    //processo iterativo per arrivare a convergenza
                    if (fabs(anew-a)<1/100000){
                     if (fabs(bnew-b)<1/100000){
                        finished=1;
                     }
                    }
                    a=anew; //definizione valore finale coefficiente "a"
                    b=bnew; //definizione valore finale coefficiente "b"
                    sum=sum+1;
                    if (sum>500){
                        finished=1;
                    }
            }
            a2[j-1]=a; //definizione valore finale coefficiente "a" per la j-esima stazione
            b2[j-1]=b; //definizione valore finale coefficiente "b" per la j-esima stazione

            thrust=thrust+DtDr*(rstep[j]-rstep[j-1]); //sommatoria dei contributi di spinta dalla stazione 1 alla stazione j
            torque=torque+DqDr*(rstep[j]-rstep[j-1]); //sommatoria dei contributi di coppia dalla stazione 1 alla stazione j
    }
//



    propThrust=thrust;
    propTorque=torque;
    double t=thrust/(rho1*n*n*diam*diam*diam*diam); //coefficiente di spinta adimensionale
    double q=torque/(rho1*n*n*diam*diam*diam*diam*diam); //coefficiente di coppia adimensionale

    double J=Vel/(n*diam);//rapporto di avanzamento

    if (t<0){ //caso di spinta negativa
        eff=0.0; //efficienza elica
    }
    else{
        eff=t/q*J/(2.0*M_PI); //efficienza elica
    }
    
    // FILE *fpt;
    // if(wr==1)
    // {fpt = fopen("../Main/Output/output_TEP.txt", "w+"); //file per salvataggio dati di output della libreria
    // if(fpt==NULL)
    // {
    //     printf("ERRORE -14: Impossibile apertura file output_TEP.txt per la scrittura\n");exit(-14);
    // }
    // fprintf(fpt,"Thrust %lf [N]\nTorque %lf [Nm]\nCt %lf\nCq %lf\nEfficiency %lf\n",output.thrust,output.torque,output.ct,output.cq,output.eff);}
    // return output;
}


#endif