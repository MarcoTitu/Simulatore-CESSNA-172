#ifndef READ_H
#define READ_H

void readEngine();
void readPropeller();
void readDba();



void readEngine()
{
    //Variabili esterne da manipolare
    extern double engine[7];

    FILE *file;
    char filename[] = "engine.txt";
    char ch;
    int i=0;

    //Apertura file
    file = fopen(filename, "r");

    //Controllo su esistenza file
    if (file == NULL) {
        printf("Errore nell'apertura del file engine.txt\n");
        system("pause");
        exit(1);
    }


    //Lettura file
    while ((ch = fgetc(file)) != EOF) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%lf", &engine[i]);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {
            ch = fgetc(file);
        }


    }

    //Chiusura file
    fclose(file);
}




void readPropeller()
{
    //Variabili esterne da manipolare
    extern double propeller_data[11], CSI[60], RD[60], CH[60], BA[60];

    FILE *file;
    char filename[] = "propeller.txt";
    char ch;
    int i=0;

    //Apertura file
    file = fopen(filename, "r");

    //Controllo su esistenza file
    if (file == NULL) {
        printf("Errore nell'apertura del file propeller.txt\n");
        system("pause");
        exit(1);
    }


    //Lettura file
    while (i < 12) {                                 //lettura dati singoli
        ch = fgetc(file);

            if (ch != '*') {
               fseek(file, -1, SEEK_CUR);
               fscanf(file, "%lf", &propeller_data[i]);
               i++;
            }

            while ((ch != '\n') && (ch != EOF)) {
                ch = fgetc(file);
            }
        }



    i = 0;

    while ((ch = fgetc(file)) != EOF && i < 61) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%lf %lf %lf %lf", &CSI[i], &RD[i], &CH[i], &BA[i]);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {
            ch = fgetc(file);
        }

    }


    //Chiusura file
    fclose(file);

}


void readDba()
{
    //Variabili esterne da manipolare
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


    FILE *file;
    char filename[] = "dba.txt";
    char ch;
    int i=0;

    //Apertura file
    file = fopen(filename, "r");

    //Controllo su esistenza file
    if (file == NULL) {
        printf("Errore nell'apertura del file dba.txt\n");
        system("pause");
        exit(1);
    }


    //Lettura file
        while (i < 31) {                                 //lettura dati singoli
            ch = fgetc(file);

                if (ch != '*') {
                   fseek(file, -1, SEEK_CUR);
                   fscanf(file, "%lf", &general_data[i]);
                   i++;
                }

                while ((ch != '\n') && (ch != EOF)) {
                    ch = fgetc(file);
                }
            }



    //lettura matrice SS
    i = 0;

    while ((ch = fgetc(file)) != EOF && i < 127) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%lf %lf %lf %lf %lf %lf %lf", &alpha[i], &Cx[i], &Cy[i], &Cz[i], &Cl[i], &Cm[i], &Cn[i]);
            //printf("\n\n %lf %lf %lf %lf %lf %lf %lf i=%d", alpha[i], Cx[i], Cy[i], Cz[i], Cl[i], Cm[i], Cn[i], i);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {

            ch = fgetc(file);
        }

    }



    //lettura X force derivative
    i = 0;

    while ((ch = fgetc(file)) != EOF && i < 127) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf %lf", &Cxa[i], &Cxap[i], &Cxu[i], &Cxq[i], &Cxb[i], &Cxp[i], &Cxr[i]);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {
            ch = fgetc(file);
        }

    }


    //lettura Y force derivative
    i = 0;

    while ((ch = fgetc(file)) != EOF && i < 127) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf", &Cyb[i], &Cybp[i], &Cyp[i], &Cyr[i], &Cya[i], &Cyq[i]);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {
            ch = fgetc(file);
        }

    }

    //lettura Z force derivative
    i = 0;

    while ((ch = fgetc(file)) != EOF && i < 127) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf %lf", &Czalpha[i], &Czap[i], &Czu[i], &Czq[i], &Czb[i], &Czp[i], &Czr[i]);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {
            ch = fgetc(file);
        }

    }


    //lettura rolling moment derivatives
    i = 0;

    while ((ch = fgetc(file)) != EOF && i < 127) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf", &ClB[i], &ClBP[i], &ClP[i], &ClR[i], &ClA[i], &ClQ[i]);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {
            ch = fgetc(file);
        }

    }

    //lettura pitching moment derivatives
    i = 0;

    while ((ch = fgetc(file)) != EOF && i < 127) {

        if (ch != '*') {
            fseek(file, -1, SEEK_CUR);
            fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf %lf", &CmA[i], &CmAP[i], &CmU[i], &CmQ[i], &CmB[i], &CmP[i], &CmR[i]);
            i++;
        }


        //Spostamento alla riga successiva ignornado il resto della riga attuale
        while ((ch != '\n') && (ch != EOF)) {
            ch = fgetc(file);
        }

    }


    //Yawing moment derivatives
    i = 0;

        while ((ch = fgetc(file)) != EOF && i < 127) {

            if (ch != '*') {
                fseek(file, -1, SEEK_CUR);
                fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf", &CnB[i], &CnBP[i], &CnP[i], &CnR[i], &CnA[i], &CnQ[i]);
                i++;
            }


            //Spostamento alla riga successiva ignornado il resto della riga attuale
            while ((ch != '\n') && (ch != EOF)) {
                ch = fgetc(file);
            }

        }


    //Control force derivatives
    i = 0;

        while ((ch = fgetc(file)) != EOF && i < 127) {

            if (ch != '*') {
                fseek(file, -1, SEEK_CUR);
                fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf", &CXde[i], &CXdle[i], &CZde[i], &CZdle[i], &CYda[i], &CYdr[i]);
                i++;
            }


            //Spostamento alla riga successiva ignornado il resto della riga attuale
            while ((ch != '\n') && (ch != EOF)) {
                ch = fgetc(file);
            }

        }

    //Control moment derivatives
    i = 0;

        while ((ch = fgetc(file)) != EOF && i < 127) {

            if (ch != '*') {
                fseek(file, -1, SEEK_CUR);
                fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf", &Clda[i], &Cldr[i], &Cmde[i], &Cmdle[i], &Cnda[i], &Cndr[i]);
                i++;
            }


            //Spostamento alla riga successiva ignornado il resto della riga attuale
            while ((ch != '\n') && (ch != EOF)) {
                ch = fgetc(file);
            }

        }



    //rotary derivatives
    i = 0;

        while ((ch = fgetc(file)) != EOF && i < 127) {

            if (ch != '*') {
                fseek(file, -1, SEEK_CUR);
                fscanf(file, "%*lf %lf %lf %lf %lf %lf %lf", &CXom[i], &CYom[i], &CZom[i], &Clom[i], &Cmom[i], &Cnom[i]);
                i++;
            }


            //Spostamento alla riga successiva ignornado il resto della riga attuale
            while ((ch != '\n') && (ch != EOF)) {
                ch = fgetc(file);
            }

        }

    //Chiusura file
    fclose(file);

}


#endif
