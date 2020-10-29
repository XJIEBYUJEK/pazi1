#include "tommath.h"
#include "pointCurveStructures.h"
#include "parametrs.h"

void initPoint(struct Point *point){
    mp_init(&point->X);
    mp_init(&point->Y);
    mp_init(&point->Z);
}

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

void initCurve(struct Curve *curve){
    mp_init(&curve->X);
    mp_init(&curve->Y);
    mp_init(&curve->Z);
    mp_init(&curve->x);
    mp_init(&curve->y);
    mp_init(&curve->theta);
    mp_init(&curve->a);
    mp_init(&curve->e);
    mp_init(&curve->d);
    mp_init(&curve->p);
    mp_init(&curve->q);
}

void curveCreate (struct Curve *curve)
{
    mp_int temp;
    mp_init(&temp);

    mp_read_radix(&curve->theta, theta_, 10);
    mp_read_radix(&curve->a, a_, 16);
    mp_read_radix(&curve->x, x_, 16);
    mp_read_radix(&curve->y, y_, 16);
    mp_read_radix(&curve->p, p_, 16);
    mp_read_radix(&curve->q, q_, 16);

    // e = (-(3 * theta^2 + 4 * a)) / 16
    mp_sqrmod(&curve->theta, &curve->p, &curve->e);                              // e = theta ^2
    mp_set_i32(&temp, 3);                                                      // temp = 3
    mp_mulmod(&curve->e, &temp, &curve->p, &curve->e);                             // e = 3 * theta^2
    mp_set_i32(&temp, 4);                                                      // temp = 4
    mp_mulmod(&temp, &curve->a, &curve->p, &temp);                               // temp = 4 * a
    mp_addmod(&temp, &curve->e, &curve->p, &curve->e);                           // e = 3 * theta^2 + 4 * a
    mp_read_radix(&temp, "-1", 10);                                                    // temp = -1
    mp_mulmod (&temp, &curve->e, &curve->p, &curve->e);                               // e = -(3 * theta^2 + 4 * a)
    mp_set_i32(&temp, 16);                                                    // temp = 16
    mp_invmod(&temp, &curve->p, &temp);                                          // temp = 1/16
    mp_mulmod(&curve->e, &temp, &curve->p, &curve->e);                           // e = (-(3 * theta^2 + 4 * a)) / 16

    // d = 3 * theta / 4
    mp_set_i32(&temp, 4);
    mp_invmod(&temp, &curve->p, &temp);
    mp_mulmod(&curve->theta, &temp, &curve->p, &curve->d);
    mp_set_i32(&temp, 3);
    mp_mulmod(&curve->d, &temp, &curve->p, &curve->d);

    // X = 2 * (x - theta)
    mp_submod(&curve->x, &curve->theta, &curve->p, &curve->X);
    mp_addmod(&curve->X, &curve->X, &curve->p, &curve->X);

    // Y = (2 * x + theta) * (x - theta)^2 - y^2
    mp_addmod(&curve->x, &curve->x, &curve->p, &curve->Y);                        // Y = 2 * x
    mp_addmod(&curve->Y, &curve->theta, &curve->p, &curve->Y);                       // Y = 2 * x + theta
    mp_submod(&curve->x, &curve->theta, &curve->p, &temp);                   // temp = x - theta
    mp_sqrmod(&temp, &curve->p, &temp);                                     // temp = (x - theta)^2
    mp_addmod(&curve->Y, &temp, &curve->p, &curve->Y);                      // Y = (2 * x + theta) * (x - theta)^2
    mp_sqrmod(&curve->y, &curve->p, &temp);                         // temp = y^2
    mp_submod(&curve->Y, &temp, &curve->p, &curve->Y);             // Y = (2 * x + theta) * (x - theta)^2 - y^2

    // Z = y
    mp_copy(&curve->y, &curve->Z);
    mp_mod(&curve->Z, &curve->p, &curve->Z);

    mp_clear(&temp);

}
