#include <stdio.h>
#include <stdbool.h>

#ifndef WIERSZE
#define WIERSZE 8
#endif

#ifndef KOLUMNY
#define KOLUMNY 8
#endif

#ifndef DLUGOSC
#define DLUGOSC 5
#endif

// Pomocnicza deklaracja makro MAX_DIAG jako min(WIERSZE, KOLUMNY), czyli maksymalna długość dowolnej przekątnej w macierzy.
#if WIERSZE > KOLUMNY
#define MAX_DIAG KOLUMNY
#else
#define MAX_DIAG WIERSZE
#endif

// Zmienna globalna przedstawiająca planszę gry jako macierz wymiaru WIERSZE x KOLUMNY.
// Przyjęta przeze mnie reprezentacja danych to odpowiednie wartości int: 0 dla pola pustego oraz 1 i 2 dla odpowiednia gracza 1 i 2.
// Przyjąłem także konwencję, by sama zmienna globalna była używana tylko w main(), pozostałe funkcje otrzymują PLANSZA jako argument
int PLANSZA[WIERSZE][KOLUMNY];
// Ta zmienna globalna pomaga śledzić jak wysoka jest kolumna pionów w danej kolumnie planszy.
int WYSOKOSCI[KOLUMNY];

// Talblica brudnopis dla sprawdzania, czy dany wiersz zawiera palindrom.
int wiersz_b[KOLUMNY];
// Talblica brudnopis dla sprawdzania, czy dana kolumna zawiera palindrom.
int kolumna_b[WIERSZE];
// Talblica brudnopis dla sprawdzania, czy dana przekątna zawiera palindrom.
int przekatna_b[MAX_DIAG];

// Sprawdza, czy dane pole jest w zakresie macierzy przedstawiającej planszę, wydzelona dla czytelności
bool dobre_pole(int w, int k)
{
    return (w < WIERSZE && w >= 0) && (k < KOLUMNY && k >= 0);
}

// Sprawdza, czy zawiera palindrom danej długości iterując po kolejnych "podtablicach" długości dokładnie DLUGOSC
// i wewnątrz każdej "podtablicy" iterując jednocześnie od końca i początku potwierdza, czy znaki są takie same oraz niezerowe.
bool zawiera_palindrom(int *ciag, int n)
{
    if (n < DLUGOSC)
        return false;

    int ii = 0;
    bool zawiera = false;

    while (ii <= n - DLUGOSC)
    {
        int shift = 0;
        while ((ciag[ii + shift] == ciag[ii + DLUGOSC - 1 - shift] && shift <= DLUGOSC / 2) && ciag[ii + shift] != 0)
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

// Sprawdza, czy jakiś wiersz na planszy zawiera palindrom.
bool sprawdz_poziomo(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;

    for (int w = 0; w < WIERSZE; w++)
    {

        for (int k = 0; k < KOLUMNY; k++)
        {
            wiersz_b[k] = plansza[w][k];
        }

        if (zawiera_palindrom(wiersz_b, KOLUMNY))
            jest = true;
    }

    return jest;
}

// Sprawdza, czy jakaś kolumna na planszy zawiera palindrom.
bool sprawdz_pionowo(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;

    for (int k = 0; k < KOLUMNY; k++)
    {

        for (int w = 0; w < WIERSZE; w++)
        {
            kolumna_b[k] = plansza[w][k];
        }

        if (zawiera_palindrom(kolumna_b, WIERSZE))
            jest = true;
    }

    return jest;
}

// Sprawdza, czy jakakolwiek diagonala zawiera palindrom.
// Najpierw sprawdza przekątne pod główną diagonalą włącznie, potem przekątne nad nią.
bool sprawdz_diagonalnie(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;

    for (int w = 0; w < WIERSZE; w++)
    {
        int dw = w, dk = 0, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna_b[dlugosc_przek] = plansza[dw][dk];
            dw++;
            dk++;
            dlugosc_przek++;
        }

        if (zawiera_palindrom(przekatna_b, dlugosc_przek))
            jest = true;
    }

    for (int k = 1; k < KOLUMNY; k++)
    {
        int dw = 0, dk = k, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna_b[dlugosc_przek] = plansza[dw][dk];
            dw++;
            dk++;
        }

        if (zawiera_palindrom(przekatna_b, dlugosc_przek))
            jest = true;
    }

    return jest;
}

// Sprawdza, czy jakakolwiek przeciwprzekątna zawiera palindrom.
// Najpierw sprawdza przekątne nad główną przeciwprzekątną włącznie, potem przekątne pod nią.
bool sprawdz_przeciwdiagonalnie(int plansza[WIERSZE][KOLUMNY])
{
    bool jest = false;

    for (int w = 0; w < WIERSZE; w++)
    {
        int dw = w, dk = 0, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna_b[dlugosc_przek] = plansza[dw][dk];
            dw--;
            dk++;
            dlugosc_przek++;
        }

        jest = jest || zawiera_palindrom(przekatna_b, dlugosc_przek);
    }

    for (int k = 1; k < KOLUMNY; k++)
    {
        int dw = WIERSZE - 1, dk = k, dlugosc_przek = 0;

        while (dobre_pole(dw, dk))
        {
            przekatna_b[dlugosc_przek] = plansza[dw][dk];
            dw--;
            dk++;
            dlugosc_przek++;
        }

        jest = jest || zawiera_palindrom(przekatna_b, dlugosc_przek);
    }

    return jest;
}

// Funkcja agreguje wyniki wszystkich możliwości wystąpienia palindrom
// i mówi, czy wystąpił on gdziekolwiek, jeśli tak, oznacza t, że gracz stawiający ostatni ruch wygrał
bool wygrana(int plansza[WIERSZE][KOLUMNY])
{
    bool pz = sprawdz_poziomo(plansza);
    bool pn = sprawdz_pionowo(plansza);
    bool dg = sprawdz_diagonalnie(plansza);
    bool ad = sprawdz_przeciwdiagonalnie(plansza);

    return pz || pn || dg || ad;
}

// Wypisuje planszę
void wypisz_plansze(int plansza[WIERSZE][KOLUMNY])
{
    for (int i = 0; i < WIERSZE; i++)
    {
        for (int j = 0; j < KOLUMNY; j++)
        {
            switch (plansza[i][j])
            {
            case 0:
                printf(" -");
                break;
            case 1:
                printf(" 1");
                break;
            case 2:
                printf(" 2");
                break;
            }
        }
        printf("\n");
    }

    for (int i = 0; i < KOLUMNY; i++)
    {
        printf(" %c", 'a' + i);
    }
    printf("\n");
}

// Dla czytelnośći, konwertuje char na odpowiedni indeks typu int
int char_to_index(char input)
{
    return input - 'a';
}

// Wykonuje ruch, czyli stawia pion danego gracza w danej kolumnie na planszy
void ruch(int kol_indeks, int player, int plansza[WIERSZE][KOLUMNY])
{
    plansza[WIERSZE - WYSOKOSCI[kol_indeks] - 1][kol_indeks] = player;
    WYSOKOSCI[kol_indeks]++;
}

// Głównie dla czytelności, zwraca następnego gracza
int nowa_tura(int gracz)
{
    return gracz == 1 ? 2 : 1;
}

// Pyta gracza o ruch (kolumnę) oraz zwraca odpowiadający jej indeks, lub -1 jeśli gracz zdecydowal przerwać grę
// Przez czytanie znaku nowej linii jako char, funkcja czeka aż przyjęty znak będzie inny niż newline lub end of file
int zapytaj_o_ruch(int gracz)
{
    printf("%d:\n", gracz);
    char kol = '\n';

    while (kol == '\n' || kol == EOF)
    {
        kol = (char)getchar();
    }

    if (kol == '.')
        return -1;
    else
        return char_to_index(kol);
}

// Funkcja zarządza przebiegiem rozgrywki, czyli pętli wypisywanie planszy, pytanie o ruch, stawianie go oraz sprawdzanie wygranej
// dopóki któryś z graczy nie przerwał gry lub nie została zatrzymana.
void petla_gry()
{
    int gracz = 1;
    bool zatrzymanie = false;

    while (!zatrzymanie)
    {
        wypisz_plansze(PLANSZA);

        int kol = zapytaj_o_ruch(gracz);

        if (kol == -1)
        {
            zatrzymanie = true;
        }
        else
        {
            ruch(kol, gracz, PLANSZA);
            if (wygrana(PLANSZA))
            {
                wypisz_plansze(PLANSZA);
                printf("%d!\n", gracz);
                zatrzymanie = true;
            }
        }

        gracz = nowa_tura(gracz);
    }
}

int main(void)
{
    petla_gry();

    return 0;
}