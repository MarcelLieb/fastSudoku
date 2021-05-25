#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "NUMBERMATRIX.h"

int getQuadrat(int x, int y);
int getPosQuadrat(int x, int y);
int istEinzigeSingle(int Feld, int Wert);
int istEinzige(Numbermatrix *m, int x, int y, int Wert);
int istEinzigeMoeglichkeit(Numbermatrix *m, int x, int y, int Wert);
int openSudoku (char * file, Numbermatrix *m);
//int istGeloest(Numbermatrix *m);
int nextNumber(int x, int Zahlen);
int nextSpace(Numbermatrix *m, int level);
void BranchAndBound (Numbermatrix *m, int level);

int main(int argc, char *argv[])
{
    Numbermatrix m;
    int x, y, i, count;
    double start, end;

    if(argc >= 3) {
        fprintf(stderr, "Too many Arguments");
        exit(EXIT_FAILURE);
    }

    count = 69;
    if(initMatrix(&m, 9) < 0) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    if(argc == 2  && openSudoku(argv[1], &m) != 0) {
        printf("Error opening Sudoku\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Branch and Bound:\n");
    start = clock()  / CLOCKS_PER_SEC * 1000;
    BranchAndBound(&m, nextSpace(&m, 0));
    end = clock() * 1000 / CLOCKS_PER_SEC;
    printf("\nComputationtime in ms: %lf\n\n", end - start);

    printf("Optimized Branch and Bound:\n");
    start = clock() * 1000 / CLOCKS_PER_SEC;
    while(count) {
        count = 0;
        for (x = 0; x < m.size; x++) {
            for (y = 0; y < m.size; y++) {
                if(!((m.FelderReihe[y] >> x) & 1)) {
                    for (i = nextNumber(0, m.ZahlenReihe[y]); i < 10; i = nextNumber(i, m.ZahlenReihe[y])) {
                        if(istEinzigeMoeglichkeit(&m, x, y, i)) {
                            addNumber(&m, x, y, i);
                            //printf("Number found:\tx: %i\ty: %i\tNumber: %i\n", x + 1, y + 1, i);
                            count++;
                            break;
                        }
                    }
                }
            }
        }
        //printf("Count: %i\n", count);
    }
    BranchAndBound(&m, nextSpace(&m, 0));
    end = clock() * 1000 / CLOCKS_PER_SEC;
    printf("\nComputationtime in ms: %lf\n\n", end - start);
    return 0;
}

int istEinzigeSingle(int Feld, int Wert)
{
    return !((Feld >> Wert) & 1);
}

int istEinzige(Numbermatrix *m, int x, int y, int Wert)
{
    return istEinzigeSingle(m->ZahlenReihe[y], Wert) && istEinzigeSingle(m->ZahlenSpalte[x], Wert) && istEinzigeSingle(m->ZahlenQuadrat[getQuadrat(x, y)], Wert);
}

int istEinzigeMoeglichkeitReihe(Numbermatrix *m, int x, int y, int Wert)
{
    int i;
    for(i = 0; i < m->size; i++) {
        if(!((m->FelderReihe[y] >> i) & 1) && i != x && istEinzige(m, i, y, Wert)) {
            return 0;
        }
    }
    return 1;
}

int istEinzigeMoeglichkeitSpalte(Numbermatrix *m, int x, int y, int Wert)
{
    int i;
    for(i = 0; i < m->size; i++) {
        if(!((m->FelderSpalte[x] >> i) & 1) && i != y && istEinzige(m, x, i, Wert)) {
            return 0;
        }
    }
    return 1;
}

int istEinzigeMoeglichkeitQuadrat(Numbermatrix *m, int q, int p, int Wert)
{
    int i;
    for(i = 0; i < m->size; i++) {
        if(!((m->FelderQuadrat[q] >> i) & 1) && i != p && istEinzige(m, getPosQuadrat(i, q), getQuadrat(i, q), Wert)) {
            return 0;
        }
    }
    return 1;
}

int istEinzigeMoeglichkeit(Numbermatrix *m, int x, int y, int Wert)
{
    if(!istEinzige(m, x, y, Wert)) return 0;
    return istEinzigeMoeglichkeitReihe(m, x, y, Wert) || istEinzigeMoeglichkeitSpalte(m, x, y, Wert) || istEinzigeMoeglichkeitQuadrat(m, getQuadrat(x, y), getPosQuadrat(x, y), Wert);
}

/*
int istGeloest(Numbermatrix *m)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if(!(m->ZahlenReihe[i] & 1)) return 0;
    }
}
*/

int openSudoku (char * file, Numbermatrix *m)
{
    FILE *f;
    char c;
    int x, y;
    x = 0;
    y = 0;
    f = fopen(file, "r");
    if(f == NULL) {
        fprintf(stderr, "Error opening file\n");
        return -2;
    }
    c = fgetc(f);
    while(c != EOF) {
        if(c >= '1' && c <= '9') {
            if(istEinzige(m, x, y, c - '0')) {
                addNumber(m, x, y, c - '0');
                x++;
            }
            else {
                printf("Kein echtes Sudoku\n");
                x++;
            }
        }
        if(c == 'x' || c == '0' || c == '#' || c == '*') {
            x++;
        }
        if (c == '\n') {
            y++;
            x = 0;
        }
        c = fgetc(f);
    }
    if(!feof(f)) {
        fprintf(stderr ,"Error reading file");
        fclose(f);
        return -2;
    }
    fclose(f);
    return 0;
}

int nextNumber(int x, int Zahlen) 
{
    x++;
    while((Zahlen >> x) & 1) {
        x++;
    }
    return x;
}

int nextSpace(Numbermatrix *m, int level)
{
    for(level ; level < 81; level++) {
        if(!((m->FelderSpalte[level % m->size] >> level / m->size) & 1)) return level;
    }
    return 81;
}

void BranchAndBound (Numbermatrix *m, int level)
{
    if(level == m->size * m->size) {
        //printMatrix(m);
    }
    else {
        int i, x, y;
        x = level % m->size;
        y = level / m->size;
        for (i = nextNumber(0, m->ZahlenReihe[y]); i < m->size + 1; i = nextNumber(i, m->ZahlenReihe[y])) {
            if(istEinzige(m, x, y, i)) {
                addNumber(m, x, y, i);
                BranchAndBound(m, nextSpace(m, level));
                removeNumber(m, x, y, i);
            }
        }
    }
}
