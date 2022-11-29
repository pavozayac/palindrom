#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef WIERSZE
#define WIERSZE 8
#endif

#ifndef KOLUMNY
#define KOLUMNY 8
#endif

#ifndef DLUGOSC
#define DLUGOSC 5
#endif

#if WIERSZE > KOLUMNY
#define MAX_DIAG KOLUMNY
#else
#define MAX_DIAG WIERSZE
#endif

int PLANSZA[WIERSZE][KOLUMNY] = {
    {0, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 2, 0, 0, 0},
    {0, 0, 0, 2, 0, 0, 0, 0},
    {1, 0, 2, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 2, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}};

// int PLANSZA[WIERSZE][KOLUMNY] = {
// {0, 1, 0, 0, 0, 0, 0, 0},
// {0, 0, 1, 0, 0, 0, 0, 0},
// {0, 0, 0, 1, 0, 0, 0, 0},
// {0, 0, 0, 0, 1, 0, 0, 0},
// {0, 0, 0, 0, 0, 1, 0, 0},
// {0, 0, 0, 0, 0, 0, 1, 0},
// {0, 0, 0, 0, 0, 0, 0, 0},
// {0, 0, 0, 0, 0, 0, 0, 0}};

// Sprawdza, czy dane pole jest w zakresie macierzy przedstawiającej planszę, wydzelona dla czytelności
bool dobre_pole(int w, int k)
{
    return (w < WIERSZE && w >= 0) && (k < KOLUMNY && k >= 0);
}


// Sprawdza, czy zawiera palindrom danej długości iterując po kolejnych "podtablicach" długości dokładnie DLUGOSC 
// i  iterując jednocześnie od końca i początku potwierdza, czy znaki są takie same oraz niezerowe
bool zawiera_palindrom(int ciag[], int n)
{
    if (n < DLUGOSC)
        return false;

    int ii = 0;
    bool zawiera = false;

    while (ii <= n - DLUGOSC)
    {
        int shift = 0;
        while (ciag[ii + shift] == ciag[ii + DLUGOSC - 1 - shift] && shift <= DLUGOSC / 2 && ciag[ii + shift] != 0)
        {
            shift++;
        }
        if (shift == DLUGOSC / 2 + 1)
        {
            zawiera = true;
        }
        ii++;
    }

    return zawiera;
}

// Sprawdza, czy jakiś wiersz na planszy zawiera palindrom
bool sprawdz_poziomo(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;

    int wiersz[KOLUMNY];

    for (int w = 0; w < WIERSZE; w++)
    {

        for (int k = 0; k < KOLUMNY; k++)
        {
            wiersz[k] = plansza[w][k];
        }

        jest = jest | zawiera_palindrom(wiersz, KOLUMNY);
    }

    return jest;
}

//Sprawdza, czy jakaś kolumna na planszy zawiera palindrom
bool sprawdz_pionowo(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;

    int kolumna[WIERSZE];

    for (int k = 0; k < KOLUMNY; k++)
    {

        for (int w = 0; w < WIERSZE; w++)
        {
            kolumna[k] = plansza[w][k];
        }

        jest = jest || zawiera_palindrom(kolumna, WIERSZE);
    }

    return jest;
}

// Sprawdza, czy jakakolwiek diagonala zawiera palindrom
// Najpierw sprawdza przekątne pod główną diagonalą włącznie, potem przekątne nad nią
bool sprawdz_diagonalnie(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;
    int przekatna[MAX_DIAG];

    for (int w = 0; w < WIERSZE; w++)
    {
        int dw = w, dk = 0, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna[dlugosc_przek] = plansza[dw][dk];
            dw++;
            dk++;
            dlugosc_przek++;
        }

        jest = jest || zawiera_palindrom(przekatna, dlugosc_przek);
    }

    for (int k = 1; k < KOLUMNY; k++)
    {
        int dw = 0, dk = k, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna[dlugosc_przek] = plansza[dw][dk];
            dw++;
            dk++;
        }

        jest = jest || zawiera_palindrom(przekatna, dlugosc_przek);
    }

    return jest;
}

// Sprawdza, czy jakakolwiek przeciwprzekątna zawiera palindrom
// Najpierw sprawdza przekątne nad główną przeciwprzekątną włącznie, potem przekątne pod nią
bool sprawdz_przeciwdiagonalnie(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;
    int przekatna[MAX_DIAG];

    for (int w = 0; w < WIERSZE; w++)
    {
        int dw = w, dk = 0, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna[dlugosc_przek] = plansza[dw][dk];
            dw--;
            dk++;
            dlugosc_przek++;
        }

        jest = jest || zawiera_palindrom(przekatna, dlugosc_przek);
    }

    for (int k = 1; k < KOLUMNY; k++)
    {
        int dw = WIERSZE - 1, dk = k, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna[dlugosc_przek] = plansza[dw][dk];
            dw--;
            dk++;
            dlugosc_przek++;
        }

        jest = jest || zawiera_palindrom(przekatna, dlugosc_przek);
    }

    return jest;
}

// Funkcja agreguje wyniki wszystkich możliwości wystąpienia palindrom
// i mówi, czy wystąpił on gdziekolwiek
bool sprawdz_palindrom()
{
    bool pz = sprawdz_poziomo(PLANSZA);
    bool pn = sprawdz_pionowo(PLANSZA);
    bool dg = sprawdz_diagonalnie(PLANSZA);
    bool ad = sprawdz_przeciwdiagonalnie(PLANSZA);

    return pz || pn || dg || ad;
}

int main(void)
{
    printf("%d", sprawdz_palindrom());

    return 0;

    // 0000012221
}