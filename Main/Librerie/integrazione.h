#ifndef INTEGRAZIONE_H_INCLUDED
#define INTEGRAZIONE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define RESET "\033[0m"
#define GIALLO "\033[0;33m"
#define ROSSO "\033[0;31m"

extern double alpha[126];
extern double general_data[30];
extern double vett_stato[15];
extern double engine[7];
extern double g;
extern double rho0;

extern int file_count;
extern char file_count_char[20];
extern double CI[3];

int integrazione(double dt, double tMax){
	//Apri file dei comandi

	char estensione_file[10] = ".txt";
    char nome_file_comandi[100] = "../main/Input/commandHistory";
    sprintf(file_count_char, "%d", file_count);
    strcat(nome_file_comandi, file_count_char);
    strcat(nome_file_comandi, estensione_file);

	FILE *fileCH, *fileRE;
    fileCH = fopen(nome_file_comandi, "r");
	if(fileCH==NULL){
		printf(ROSSO "\nERRORE -13: Errore apertura del file commandHistory%d.txt per la lettura" RESET, file_count);
		exit(-13);
	}

    char nome_file_stato[100] = "../Main/Output/simOutput";
    sprintf(file_count_char, "%d", file_count);
    strcat(nome_file_stato, file_count_char);
    strcat(nome_file_stato, estensione_file);
	
    fileRE = fopen(nome_file_stato, "w");

	if(fileCH==NULL){
		printf(ROSSO "\nERRORE -14: Errore creazione del file simOutput%d.txt per la scrittura" RESET, file_count);
		exit(-14);
	}

	double mass_trim=general_data[0];
	vett_stato[12]=mass_trim;

	double wingSpan=general_data[1], refChord=general_data[3], superficieAlare=general_data[2];

	double RPMmin=engine[2], RPMmax=engine[3], dRPM=RPMmax-RPMmin, RPM;

	double transmissionEta=engine[5];

	double specFuelConsumption=engine[6];

	double Cx1,Cy1,Cz1,cl1,cm1,cn1;
	double X,Y,Z,l,m,n;
	double u1,v1,w1,p1,q1,r1,fi1,theta1,psi1,h1,x1,y1;
	double m_fuel;
	double Cx_ss,Cx_alpha,Cx_deltae,Cy_ss,Cy_alpha,Cy_deltaa,Cy_deltar,Cz_ss,Cz_alpha,Cz_deltae,cl_alpha,cl_deltaa,cl_deltar,cm_alpha,cm_deltae,cn_alpha,cn_deltaa,cn_deltar;
	double deltae, deltaa, deltar, throttle, Cx_deltaa, Cx_deltar, Cy_deltae, Cz_deltaa, Cz_deltar, cl_deltae, cm_deltaa, cm_deltar, cn_deltae, cl_ss, cm_ss, cn_ss;

	double rdot,pdot;
	double manetta;
	double T0=288,t_quota, P0=101.325,p_quota,rho_quotah;
	double R=287.05;
	double Cx_beta ,Cy_beta ,Cz_beta ,cl_beta ,cm_beta ,cn_beta;
	double T, enginePower;
	double Cxp_alpha,Cxq_alpha,Cxr_alpha,Cyp_alpha,Cyq_alpha,Cyr_alpha,Czp_alpha,Czq_alpha,Czr_alpha,ClP_alpha,ClQ_alpha,ClR_alpha,CmP_alpha,CmQ_alpha,CmR_alpha,CnP_alpha,CnQ_alpha,CnR_alpha;
    double mass;

	double time;

	double alpha_now, V, beta;

	double pAD, qAD, rAD;

	double du, dv, dw, dp, dq, dr, dfi, dtheta, dpsi, dh, dx, dy, dm;

	double frazComb;

	double jx = general_data[13];
	double jy = general_data[14];
	double jz = general_data[15];
    double jxz = general_data[16];

	int timeStep, stepMax=tMax/dt;

	//condizioni iniziali ratei angolari
	pdot=0; // usati nel calcolo del residuo di r e p
    rdot=0;

    frazComb=general_data[29];
    m_fuel=frazComb*mass_trim;

	//Lettura prima riga command history
	fscanf(fileCH, "%lf, %lf, %lf, %lf, %lf", &time, &deltae, &deltaa, &deltar, &throttle);


	//Stampa su file risultati la condizione di trim (t=0)
	fprintf(fileRE,"%lf",time);
		for(int i=0; i<15; i++){
			fprintf(fileRE, ", %lf", vett_stato[i]);
		}
		fprintf(fileRE, "\n");

	int printCount=0;
	double percentComplete;
	double nextTime=0;
	
	timeStep=1;

	//Integra equazioni del moto
	while(timeStep<=stepMax){
		time=time+dt;
		


		//Condizioni iniziali per il passo attuale:
		u1=vett_stato[0];
		v1=vett_stato[1];
		w1=vett_stato[2];
		p1=vett_stato[3];
		q1=vett_stato[4];
		r1=vett_stato[5];
		fi1=vett_stato[6]*(M_PI/180); //radianti
		theta1=vett_stato[7]*(M_PI/180); //radianti
		psi1=vett_stato[8]*(M_PI/180); //radianti
		h1=vett_stato[9];
		x1=vett_stato[10];
		y1=vett_stato[11];
		mass=vett_stato[12];


		V=sqrt(u1*u1+v1*v1+w1*w1);

		//Calcolo dati atmosfera
		//atmospherecalc(h1, V);
		//t_quota=temp_h;
		//rho_quotah=rho_h;
		t_quota            = T0 - 0.0065*h1;
        rho_quotah         = 1.225 * pow(((t_quota)/(T0)),4.25);

		alpha_now=atan2(w1,u1)*180/M_PI; //gradi
		beta=atan2(v1,u1)*180/M_PI;   //gradi
		
		//Warning incidenza
		int flag_alfa = 0;
		if(alpha_now<=alpha[0]){
			flag_alfa = -1;
			// printf("\n WARNING: Angolo d'attacco minore di alpha_min, imposto alpha = %d.\n", alpha[0]);
			alpha_now=alpha[0];
		}
		if(alpha_now>=alpha[125]){
			flag_alfa = 1;
			// printf("\n WARNING: Angolo d'attacco maggiore di alpha_max, imposto alpha = %d.\n", alpha[125]);
			alpha_now=alpha[125];
		}

		//Calcolo coefficienti aerodinamici
		//interpola per il calcolo dei valori intermedi:
		Cx_ss=interpola(alpha,Cx,alpha_now);
		Cy_ss=interpola(alpha,Cy,alpha_now);
		Cz_ss=interpola(alpha,Cz,alpha_now);
		cl_ss=interpola(alpha,Cl,alpha_now);
		cm_ss=interpola(alpha,Cm,alpha_now);
		cn_ss=interpola(alpha,Cn,alpha_now);

		Cx_alpha=interpola(alpha,Cxa,alpha_now);
		Cx_deltae=interpola(alpha,CXde,alpha_now);
		Cx_deltaa=0;
		Cx_deltar=0;

		Cy_alpha=interpola(alpha,Cya,alpha_now);
		Cy_deltaa=interpola(alpha,CYda,alpha_now);
		Cy_deltar=interpola(alpha,CYdr,alpha_now);
		Cy_deltae=0;

		Cz_alpha=interpola(alpha,Czalpha,alpha_now);
		Cz_deltae=interpola(alpha,CZde,alpha_now);
		Cz_deltaa=0;
		Cz_deltar=0;

		cl_alpha=interpola(alpha,ClA,alpha_now);
		cl_deltaa=interpola(alpha,Clda,alpha_now);
		cl_deltar=interpola(alpha,Cldr,alpha_now);
		cl_deltae=0;

		cm_alpha=interpola(alpha,CmA,alpha_now);
		cm_deltae=interpola(alpha,Cmde,alpha_now);
		cm_deltaa=0;
		cm_deltar=0;

		cn_alpha=interpola(alpha,CnA,alpha_now);
		cn_deltaa=interpola(alpha,Cnda,alpha_now);
		cn_deltar=interpola(alpha,Cndr,alpha_now);
		cn_deltae=0;

		Cx_beta = interpola(alpha,Cxb,alpha_now);
		Cy_beta = interpola(alpha,Cyb,alpha_now);
		Cz_beta = interpola(alpha,Czb,alpha_now);

		cl_beta = interpola(alpha,ClB,alpha_now);
		cm_beta = interpola(alpha,CmB,alpha_now);
		cn_beta = interpola(alpha,CnB,alpha_now);

		Cxp_alpha = interpola(alpha,Cxp,alpha_now);
		Cxq_alpha= interpola(alpha,Cxq,alpha_now);
		Cxr_alpha= interpola(alpha,Cxr,alpha_now);

		Cyp_alpha= interpola(alpha,Cyp,alpha_now);
		Cyq_alpha= interpola(alpha,Cyq,alpha_now);
		Cyr_alpha= interpola(alpha,Cyr,alpha_now);

		Czp_alpha= interpola(alpha,Czp,alpha_now);
		Czq_alpha= interpola(alpha,Czq,alpha_now);
		Czr_alpha= interpola(alpha,Czr,alpha_now);

		ClP_alpha= interpola(alpha,ClP,alpha_now);
		ClQ_alpha= interpola(alpha,ClQ,alpha_now);
		ClR_alpha= interpola(alpha,ClR,alpha_now);

		CmP_alpha= interpola(alpha,CmP,alpha_now);
		CmQ_alpha= interpola(alpha,CmQ,alpha_now);
		CmR_alpha= interpola(alpha,CmR,alpha_now);

		CnP_alpha= interpola(alpha,CnP,alpha_now);
		CnQ_alpha= interpola(alpha,CnQ,alpha_now);
		CnR_alpha= interpola(alpha,CnR,alpha_now);

        //Conversione angoli in radianti
		alpha_now=alpha_now*M_PI/180;
		beta=beta*M_PI/180;
		deltae=deltae*M_PI/180;
		deltaa=deltaa*M_PI/180;
		deltar=deltar*M_PI/180;

		//Adimensionalizzazione ratei angolari
		pAD=p1*wingSpan/(2*V);
		qAD=q1*refChord/(2*V);
		rAD=r1*wingSpan/(2*V);

		//Calcolo coefficienti totali
		Cx1=Cx_ss + Cx_alpha*alpha_now + Cx_beta*beta + Cxp_alpha*pAD + Cxq_alpha*qAD + Cxr_alpha*rAD + Cx_deltaa*deltaa + Cx_deltae*deltae + Cx_deltar*deltar;
		Cy1=Cy_ss + Cy_alpha*alpha_now + Cy_beta*beta + Cyp_alpha*pAD + Cyq_alpha*qAD + Cyr_alpha*rAD + Cy_deltaa*deltaa + Cy_deltae*deltae + Cy_deltar*deltar;
		Cz1=Cz_ss + Cz_alpha*alpha_now + Cz_beta*beta + Czp_alpha*pAD + Czq_alpha*qAD + Czr_alpha*rAD + Cz_deltaa*deltaa + Cz_deltae*deltae + Cz_deltar*deltar;
		cl1=cl_ss + cl_alpha*alpha_now + cl_beta*beta + ClP_alpha*pAD + ClQ_alpha*qAD + ClR_alpha*rAD + cl_deltaa*deltaa + cl_deltae*deltae + cl_deltar*deltar;
		cm1=cm_ss + cm_alpha*alpha_now + cm_beta*beta + CmP_alpha*pAD + CmQ_alpha*qAD + CmR_alpha*rAD + cm_deltaa*deltaa + cm_deltae*deltae + cm_deltar*deltar;
		cn1=cn_ss + cn_alpha*alpha_now + cn_beta*beta + CnP_alpha*pAD + CnQ_alpha*qAD + CnR_alpha*rAD + cn_deltaa*deltaa + cn_deltae*deltae + cn_deltar*deltar;

		//Calcolo forze aerodinamiche
		X=(0.5*rho_quotah*superficieAlare*V*V)*Cx1;
		Y=(0.5*rho_quotah*superficieAlare*V*V)*Cy1;
		Z=(0.5*rho_quotah*superficieAlare*V*V)*Cz1;
		l=(0.5*rho_quotah*superficieAlare*V*V)*cl1*wingSpan;
		m=(0.5*rho_quotah*superficieAlare*V*V)*cm1*refChord;
		n=(0.5*rho_quotah*superficieAlare*V*V)*cn1*wingSpan;

        //Calcolo spinta e coppia dell'elica e potenza
		RPM=RPMmin+throttle*dRPM;
		propel(V, RPM, rho_quotah);
		T=propThrust;
		enginePower=transmissionEta*(RPM*M_PI/30)*propTorque;

		double enginePower_max_h = engine[0]* pow(rho_quotah/(rho0),engine[1])*1000; //[W]
		int flag_enginePower = 0;
		if (enginePower > enginePower_max_h){
			flag_enginePower = -1;
			// printf("WARNING: Potenza massima superata; Potenza limitata alla potenza massima\n");//Messaggio di warning
		}

		//Calcolo residui
		du=(r1*v1-q1*w1) - g*sin(theta1) + X/mass + T/mass; //
		dv=(p1*w1-r1*u1) + g*sin(fi1)*cos(theta1) + Y/mass;
		dw=(q1*u1-p1*v1) + g*cos(fi1)*cos(theta1) + Z/mass;
		dp=-(jz-jy)*q1*r1/jx + (p1*q1+rdot)*jxz/jx + l/jx;
		dq=-(jx-jz)*p1*r1/jy - (p1*p1-r1*r1)*jxz/jy + m/jy; // 
		dr=-(jy-jx)*p1*q1/jz - (q1*r1-pdot)*jxz/jz + n/jz;
		dfi=p1 + q1*sin(fi1)*tan(theta1) + r1*cos(fi1)*tan(theta1);
		dtheta=q1*cos(fi1)-r1*sin(fi1); //
		dpsi=q1*sin(fi1)/cos(theta1)+r1*cos(fi1)/cos(theta1);
		dh=-(-u1*sin(theta1)+v1*cos(theta1)*sin(fi1)+w1*cos(theta1)*cos(fi1));

		dx=u1*cos(psi1)*cos(theta1)+v1*(cos(psi1)*sin(theta1)*sin(fi1)-sin(psi1)*cos(theta1))+w1*(cos(psi1)*sin(theta1)*cos(fi1)+sin(psi1)*sin(fi1));
		dy=u1*sin(psi1)*cos(theta1)-v1*(sin(psi1)*sin(theta1)*sin(fi1)-cos(psi1)*cos(fi1))+w1*(sin(psi1)*sin(theta1)*cos(fi1)-cos(psi1)*sin(fi1));

		dm=-enginePower*specFuelConsumption;

		//Stampa a video progressi simulazione

		nextTime=time+dt;
		percentComplete=(nextTime)/tMax*100;

		if (printCount != 0){
			printf("\033[14A");
			// printf("\033[0;0f");
		}

		printf("\033[?25l"); // Nasconde il cursore
		int barWidth = 50;
		printf("\n[");
		int pos = barWidth * percentComplete / 100;
		for (int i = 0; i < barWidth; ++i) {
			if (i < pos) printf("=");
			else if (i == pos) printf(">");
			else printf(" ");
		}
		printf("] %.0lf%%\n",percentComplete);
		// fflush(stdout);
		
		printf("\n(Tempo di simulazione %.1lf s/%.1lf s)\nV [m/s]\t\t|VS [fpm]\t|Quota [m]\n",timeStep*0.01,tMax);
		printf("%.1lf\t\t|%.1lf\t\t|%.1lf\n",V,dh*60/0.3048,h1);
		printf("Phi [deg]\t|Theta [deg]\t|Psi [deg]\n");
		printf("%.2lf\t\t|%.2lf\t\t|%.2lf\n",fi1*180/M_PI,theta1*180/M_PI,psi1*180/M_PI);
		printf("p [rad/s]\t|q [rad/s]\t|r [rad/s]\n");
		printf("%.3lf\t\t|%.3lf\t\t|%.3lf\n",p1,q1,r1);
		printf("Eq [deg]\t|Ail [deg]\t|Rud [deg]\n");
		printf("%.2lf\t\t|%.2lf\t\t|%.2lf\n",deltae*180/M_PI,deltaa*180/M_PI,deltar*180/M_PI);
		printf("Man [%%]\t\t|T [N]\t\t|P [kW]\n");	
		printf("%.2lf\t\t|%.1lf\t\t|%.1lf\n",throttle*100,T,enginePower/1000);

		if(flag_alfa == -1){
			printf(GIALLO "WARNING: Angolo d'attacco minore di alpha_min, imposto alpha = %d." RESET, alpha[0]);
		}
		if(flag_alfa == 1){
			printf(GIALLO "WARNING: Angolo d'attacco maggiore di alpha_max, imposto alpha = %d." RESET, alpha[125]);
		}
		if(flag_enginePower == -1){
			printf(GIALLO "WARNING: Potenza massima superata; Potenza limitata alla potenza massima" RESET);
		}

		printCount++;

		//Calcolo nuovi stati all'istante successivo con Eulero esplicito
		vett_stato[0]=u1+du*dt;
		vett_stato[1]=v1+dv*dt;
		vett_stato[2]=w1+dw*dt;
		vett_stato[3]=p1+dp*dt;
		vett_stato[4]=q1+dq*dt;
		vett_stato[5]=r1+dr*dt;
		vett_stato[6]=(fi1+dfi*dt)*(180/M_PI);
		vett_stato[7]=(theta1+dtheta*dt)*(180/M_PI);
		vett_stato[8]=(psi1+dpsi*dt)*(180/M_PI);
		vett_stato[9]=h1+dh*dt;
		vett_stato[10]=x1+dx*dt;
		vett_stato[11]=y1+dy*dt;
		vett_stato[12]=mass+dm*dt;
		vett_stato[13] = propThrust;
		vett_stato[14] = enginePower;


		//Calcolo accelerazioni angolari con metodo FDM
		pdot=(vett_stato[3]-p1)/dt;
		rdot=(vett_stato[5]-r1)/dt;
		
		
		if (vett_stato[12] <= (mass_trim-m_fuel)){
			printf(ROSSO "\nERRORE -7: Carburante terminato.\n" RESET);
            // printf("\nPremere invio per continuare");
            // getchar();
            // getchar();
			vett_stat_trim(trim_angles, vett_stato, CI);
			printf("\033[?25h");
			fclose(fileCH);
            fclose(fileRE);
			return -7;
		}
		if (vett_stato[9] <=0){
			printf(ROSSO "\nERRORE -8: Raggiunta quota nulla.\n" RESET);
            // printf("\nPremere invio per continuare");
            // getchar();
            // getchar();
			vett_stat_trim(trim_angles, vett_stato, CI);
			printf("\033[?25h");
			fclose(fileCH);
            fclose(fileRE);
			return -8;

		}
        if (vett_stato[9] >=4116){
			printf(ROSSO "\nERRORE -9: Raggiunta quota di tangenza (4116 m).\n" RESET);
			// printf("\nPremere invio per continuare");
            // getchar();
            // getchar();
			vett_stat_trim(trim_angles, vett_stato, CI);
			printf("\033[?25h");
			fclose(fileCH);
            fclose(fileRE);
			return -9;

		}
		if (sqrt(vett_stato[0]*vett_stato[0] + vett_stato[1]*vett_stato[1] + vett_stato[2]*vett_stato[2])<=30){
            printf(ROSSO "\nERRORE -10: Raggiunta la velocita' di stallo (30 m/s).\n" RESET);
            // printf("\nPremere invio per continuare");
            // getchar();
            // getchar();
			vett_stat_trim(trim_angles, vett_stato, CI);
			printf("\033[?25h");
			fclose(fileCH);
            fclose(fileRE);
			return -10;
		}
		if (sqrt(vett_stato[0]*vett_stato[0] + vett_stato[1]*vett_stato[1] + vett_stato[2]*vett_stato[2])>=75){
			printf(ROSSO "ERRORE -11: Superata la velocita' massima (75 m/s).\n" RESET);
			// printf("\n Premere invio per continuare");
            // getchar();
            // getchar();
			vett_stat_trim(trim_angles, vett_stato, CI);
			printf("\033[?25h");
			fclose(fileCH);
            fclose(fileRE);
			return -11;
		}
		
		
		
		//Stampa su file i risultati:
		fprintf(fileRE,"%lf",time);
		for(int i=0; i<15; i++){
			fprintf(fileRE, ", %lf", vett_stato[i]);
		}
		fprintf(fileRE, "\n");

		//Lettura prossima riga command history
		fscanf(fileCH, "%lf, %lf, %lf, %lf, %lf", &time, &deltae, &deltaa, &deltar, &throttle);
        
		timeStep++;

}

vett_stat_trim(trim_angles, vett_stato, CI);

printf("\033[?25h"); // Mostra il cursore
printf("\nIntegrazione terminata, risultati salvati correttamente!\n");
fclose(fileCH);
fclose(fileRE);
}


#endif

