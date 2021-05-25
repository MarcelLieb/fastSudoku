#include "NUMBERMATRIX.h"

#include <stdio.h>
#include <stdlib.h>


//Kann auch umgekehrt verwendet werden mit y als Quadrat und x als Position
int getQuadrat(int x, int y)
{
    return x/3 + 3*(y/3);
}

int getPosQuadrat(int x, int y)
{
    return x % 3 + 3 * (y % 3);
}

void addNumber(Numbermatrix *n, int x, int y, int number)
{
    
    if((n->FelderReihe[y] >> x) & 1) {
        fprintf(stderr, "Number already at x:%i, y:%i\n", x, y);
        return ;
    }

    int q = getQuadrat(x, y);

    n->Feld[(number - 1) * n->size + y] |= 1 << x;

    n->ZahlenReihe[y] |= 1 << number;
    //if(n->ZahlenReihe[y] == 1022) (n->ZahlenReihe[y])++;

    n->ZahlenSpalte[x] |= 1 << number;
    //if(n->ZahlenSpalte[x] == 1022) (n->ZahlenSpalte[x])++;

    n->ZahlenQuadrat[q] |= 1 << number;
    //if(n->ZahlenQuadrat[q] == 1022) (n->ZahlenQuadrat[q])++;
    n->FelderReihe[y] |= 1 << x;
    n->FelderSpalte[x] |= 1 << y;
    n->FelderQuadrat[q] |= 1 << getPosQuadrat(x, y);
    //printf("Number added: x:%i, y:%i, n:%i\n", x, y, number);
}

void removeNumber(Numbermatrix *n, int x, int y, int number)
{
    int q = getQuadrat(x, y);

    n->Feld[(number - 1) * n->size + y] ^= 1 << x;

    //if(n->ZahlenReihe[y] == 1023) (n->ZahlenReihe[y])--;
    n->ZahlenReihe[y] ^= 1 << number;

    //if(n->ZahlenSpalte[x] == 1023) (n->ZahlenSpalte[x])--;
    n->ZahlenSpalte[x] ^= 1 << number;

    //if(n->ZahlenQuadrat[q] == 1023) (n->ZahlenQuadrat[q])--;
    n->ZahlenQuadrat[q] ^= 1 << number;

    n->FelderReihe[y] ^= 1 << x;
    n->FelderSpalte[x] ^= 1 << y;
    n->FelderQuadrat[q] ^= 1 << getPosQuadrat(x, y);
}

/*
void refreshPositions(Numbermatrix *n)
{
    int i,j;
    for (i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            n->FelderReihe[i] |= n->Feld[j][i];
        }
    }
    for (i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            n->FelderSpalte[i] |= ((n->FelderReihe[j] >> i) & 1) << j;
        }
        
    }
    for (i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            n->FelderQuadrat[getQuadrat(j, i)] |= ((n->FelderReihe[i] >> j) & 1) << getPosQuadrat(j, i);
        }
    }
}
*/

void printMatrix(const Numbermatrix *n)
{
    int i,j,k;
    char Matrix[n->size][n->size];
    for (i = 0; i < n->size; i++) {
        for (j = 0; j < n->size; j++) {
            for (k = 0; k < n->size; k++) {
                if((n->Feld[k * n->size + j] >> i) & 1) {
                    Matrix[i][j] = k + '1';
                    break;
                }
            }
            if(k >= n->size) Matrix[i][j] = ' ';
        }
    }
    for (i = 0; i < n->size; i++) {
        for (j = 0; j < n->size; j++) {
            printf("%c ", Matrix[j][i]);
            if (j % 3 == 2 && j != 8) {
                printf("| ");
            }
        }
        printf("\n");
        if (i % 3 == 2 && i !=8) {
            for (j = 1; j < 22; j++) {
                printf("-");
            }
            printf("\n");
        }
    }
    printf("\n");
}

int initMatrix(Numbermatrix *m, int size)
{
    m->FelderQuadrat = calloc(size, sizeof(int));
    if(m->FelderQuadrat == NULL) return -1;
    m->FelderReihe = calloc(size, sizeof(int));
    if(m->FelderReihe == NULL) {
        destroyMatrix(m);
        return -1;
    }
    m->FelderSpalte = calloc(size, sizeof(int));
    if (m->FelderSpalte == NULL) {
        destroyMatrix(m);
        return -1;
    }
    m->ZahlenQuadrat = calloc(size, sizeof(int));
    if (m->ZahlenQuadrat == NULL) {
        destroyMatrix(m);
        return -1;
    }
    m->ZahlenReihe = calloc(size, sizeof(int));
    if (m->ZahlenReihe == NULL) {
        destroyMatrix(m);
        return -1;
    }
    m->ZahlenSpalte = calloc(size, sizeof(int));
    if(m->ZahlenSpalte == NULL) {
        destroyMatrix(m);
        return -1;
    }
    m->Feld = calloc(size * size, sizeof(int));
    if(m->Feld == NULL) {
        destroyMatrix(m);
        return -1;
    }
    m->size = size;
    return 0;
    /*
    int i,j;
    for (i = 0; i < 9; i++) {
        m->FelderQuadrat[i] = 0;
        m->FelderReihe[i] = 0;
        m->FelderSpalte[i] = 0;
        m->ZahlenQuadrat[i] = 0;
        m->ZahlenReihe[i] = 0;
        m->ZahlenSpalte[i] = 0;
        //printf("%i init: ", i);
        for (j = 0; j < 9; j++) {
            m->Feld[i][j] = 0;
            //printf("%i, %i; ", i, j);
        }
        //printf("\n");
    }
    */
}

void destroyMatrix(Numbermatrix *m)
{
    free(m->Feld);
    free(m->FelderQuadrat);
    free(m->FelderReihe);
    free(m->FelderSpalte);
    free(m->ZahlenQuadrat);
    free(m->ZahlenReihe);
    free(m->ZahlenSpalte);
}

void printBinary(const int *m)
{
    int i, j;
    /*
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 10; j++) {
            printf("%i", (m[i] >> j) & 1);
        }
        printf("\n");
    }
    */
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            printf("%i ", (m[i] >> j) & 1);
            if (j % 3 == 2 && j != 8) {
                printf("| ");
            }
        }
        printf("\n");
        if (i % 3 == 2 && i !=8) {
            for (j = 1; j < 22; j++) {
                printf("-");
            }
            printf("\n");
        }
    }
    printf("\n");
}

void printMatrixInfo(const Numbermatrix *m, int info)
{
    int i, j;
    if (info == 1) {
    printf("FelderReihe: \n");
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if((m->FelderReihe[i] >> j) & 1) printf("+ ");
            else printf("  ");
            //printf("%i ", (m->FelderReihe[i] >> j) & 1);
            if (j % 3 == 2 && j != 8) {
                printf("| ");
            }
        }
        printf("\n");
        if (i % 3 == 2 && i !=8) {
            for (j = 1; j < 22; j++) {
                printf("-");
            }
            printf("\n");
        }
    }
    printf("\n");

    printf("FelderSpalte: \n");
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            //printf("%i ", (m->FelderSpalte[j] >> i) & 1);
            if((m->FelderSpalte[j] >> i) & 1) printf("+ ");
            else printf("  ");
            if (j % 3 == 2 && j != 8) {
                printf("| ");
            }
        }
        printf("\n");
        if (i % 3 == 2 && i !=8) {
            for (j = 1; j < 22; j++) {
                printf("-");
            }
            printf("\n");
        }
    }
    printf("\n");

    printf("FelderQuadrat: \n");
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            //printf("%i ", (m->FelderQuadrat[getQuadrat(j, i)] >> getPosQuadrat(j, i)) & 1);
            if((m->FelderQuadrat[getQuadrat(j, i)] >> getPosQuadrat(j, i)) & 1) printf("+ ");
            else printf("  ");
            if (j % 3 == 2 && j != 8) {
                printf("| ");
            }
        }
        printf("\n");
        if (i % 3 == 2 && i !=8) {
            for (j = 1; j < 22; j++) {
                printf("-");
            }
            printf("\n");
        }
    }
    printf("\n");
    }
    else if (info == 2){
        printf("ZahlenReihen:\t");
        for(i = 1; i < 10; i++) {
            printf("\t%i:", i);
        }
        printf("\n");
        for(i = 0; i < 9; i++) {
            printf("Reihe %i: Voll: %i\t", i + 1, m->ZahlenReihe[i] & 1);
            for(j = 1; j < 10; j++) {
                printf("%i\t", (m->ZahlenReihe[i] >> j) & 1);
            }
            printf("\n");
        }
        printf("\n");

        printf("ZahlenSpalten:\t");
        for(i = 1; i < 10; i++) {
            printf("\t%i:", i);
        }
        printf("\n");
        for(i = 0; i < 9; i++) {
            printf("Spalte %i: Voll: %i\t", i + 1, m->ZahlenSpalte[i] & 1);
            for(j = 1; j < 10; j++) {
                printf("%i\t", (m->ZahlenSpalte[i] >> j) & 1);
            }
            printf("\n");
        }
        printf("\n");

        printf("ZahlenQuadrat:\t");
        for(i = 1; i < 10; i++) {
            printf("\t%i:", i);
        }
        printf("\n");
        for(i = 0; i < 9; i++) {
            printf("Quadrat %i: Voll: %i\t", i + 1, m->ZahlenQuadrat[i] & 1);
            for(j = 1; j < 10; j++) {
                printf("%i\t", (m->ZahlenQuadrat[i] >> j) & 1);
            }
            printf("\n");
        }
        printf("\n");
    }
    else {
        printMatrixInfo(m, 1);
        printMatrixInfo(m, 2);
    }
}