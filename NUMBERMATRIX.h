#ifndef NUMBERMATRIX_INCLUDED
#define NUMBERMATRIX_INCLUDED

typedef struct Numbermatrix {
    int *Feld; //Codiert in binären Positionen. Feld[i] gibt Positionen der Zahl i+1 an.
    int *ZahlenReihe; //Reihe[i] enthält die dort vorhandenen Zahlen codiert als 0000000010 falls 1 vorhanden ist also als 2^1. 2^0 gibt an ob die Reihe voll ist. Analog sind die anderen Arrays codiert.
    int *ZahlenSpalte;
    int *ZahlenQuadrat;
    int *FelderReihe; //Binäre Positionen wie Feld
    int *FelderSpalte;
    int *FelderQuadrat;
    int size;
    int blockWidth;
    int blockHeight;
} Numbermatrix;

int getQuadrat(int x, int y);
int getPosQuadrat(int x, int y);
void addNumber(Numbermatrix *n, int x, int y, int number);
void removeNumber(Numbermatrix *n, int x, int y, int number);
void refreshPositions(Numbermatrix *n);
void printMatrix(const Numbermatrix *n);
int initMatrix(Numbermatrix *m, int size);
void destroyMatrix(Numbermatrix *m);
void printBinary(const int *m);
void printMatrixInfo(const Numbermatrix *m, int info);
#endif