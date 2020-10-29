#include "tommath.h"
#include "pointCurveStructures.h"

void initPoint(struct Point *point){
    mp_init(&point->X);
    mp_init(&point->Y);
    mp_init(&point->Z);
}

/*
 * Инициализация точки с произвольными координатами
 */
void customPointInit16 (struct Point *point, char *X, char *Y, char *Z)
{
    mp_read_radix(&point->X, X, 16);
    mp_read_radix(&point->Y, Y, 16);
    mp_read_radix(&point->Z, Z,16);
}

void customPointInit10 (struct Point *point, char *X, char *Y, char *Z)
{
    mp_read_radix(&point->X, X, 10);
    mp_read_radix(&point->Y, Y, 10);
    mp_read_radix(&point->Z, Z,10);
}

void printProjectivePoint(const struct Point *point){
    char stroka[150] = "";
    mp_to_radix(&point->X, stroka, 150, NULL, 10);
    printf("\nX = %s", stroka);
    stroka[150] = "";
    mp_to_radix(&point->Y, stroka, 150, NULL, 10);
    printf("\nY = %s", stroka);
    stroka[150] = "";
    mp_to_radix(&point->Z, stroka, 150, NULL, 10);
    printf("\nZ = %s", stroka);
}

void printAffinePoint(const struct Point *point){
    char stroka[150] = "";
    mp_to_radix(&point->X, stroka, 150, NULL, 10);
    printf("\nx = %s", stroka);
    stroka[150] = "";
    mp_to_radix(&point->Y, stroka, 150, NULL, 10);
    printf("\ny = %s", stroka);
    stroka[150] = "";
}

