#include "tommath.h"
#include "pointCurveStructures.h"
#include "parametrs.h"

void initPoint(struct Point *point){
    mp_init(&point->X);
    mp_init(&point->Y);
    mp_init(&point->Z);
}
void clearPoint(struct Point *point){
    mp_clear_multi(&point->X, &point->Y, &point->Z, NULL);
}
void customPointInit (struct Point *point, char *X, char *Y, char *Z)
{
    mp_read_radix(&point->X, X, 10);
    mp_read_radix(&point->Y, Y, 10);
    mp_read_radix(&point->Z, Z,10);
}
void printProjectivePoint(const struct Point *point){
    char stroka1[150] = "";
    char stroka2[150] = "";
    char stroka3[150] = "";
    mp_to_radix(&point->X, stroka1, 150, NULL, 10);
    printf("\nX = %s", stroka1);
    mp_to_radix(&point->Y, stroka2, 150, NULL, 10);
    printf("\nY = %s", stroka2);
    mp_to_radix(&point->Z, stroka3, 150, NULL, 10);
    printf("\nZ = %s", stroka3);
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
    mp_init(&curve->m);
}
void clearCurve(struct Curve *curve){
    mp_clear_multi(&curve->X, &curve->Y, &curve->Z, &curve->x, &curve->y, &curve->theta, &curve->a, &curve->e, &curve->d, &curve->p, &curve->q, curve->m, NULL);
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
    mp_read_radix(&curve->m, m_, 16);

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
    mp_addmod(&curve->x, &curve->x, &curve->p, &curve->Y);         // Y = 2 * x
    mp_addmod(&curve->Y, &curve->theta, &curve->p, &curve->Y);     // Y = 2 * x + theta
    mp_submod(&curve->x, &curve->theta, &curve->p, &temp);         // temp = x - theta
    mp_sqrmod(&temp, &curve->p, &temp);                            // temp = (x - theta)^2
    mp_mulmod(&curve->Y, &temp, &curve->p, &curve->Y);             // Y = (2 * x + theta) * (x - theta)^2
    mp_sqrmod(&curve->y, &curve->p, &temp);                        // temp = y^2
    mp_submod(&curve->Y, &temp, &curve->p, &curve->Y);             // Y = (2 * x + theta) * (x - theta)^2 - y^2

    // Z = y
    mp_copy(&curve->y, &curve->Z);
    mp_mod(&curve->Z, &curve->p, &curve->Z);

    mp_clear(&temp);

}
void pointsAddition (struct Point *result, const struct Point *point1, const struct Point *point2, const struct Curve *curve)
{
    mp_int  T1, T2, T3, T4, T5, T6, T7, T8;
    mp_init(&T1);mp_init(&T2);mp_init(&T3);mp_init(&T4);mp_init(&T5);mp_init(&T6);mp_init(&T7);mp_init(&T8);

    mp_copy(&point1->X, &T1);                       //    T1 = X1
    mp_copy(&point1->Y, &T2);                       //    T2 = Y1
    mp_copy(&point1->Z, &T3);                       //    T3 = Z1
    mp_copy(&point2->X, &T4);                       //    T4 = X2
    mp_copy(&point2->Y, &T5);                       //    T5 = Y2
    mp_copy(&point2->Z, &T6);                       //    T6 = Z2
    mp_mulmod(&T1, &T3, &curve->p, &T7);                        //    T7 = T1*T3 = X1 * Z1
    mp_addmod(&T2, &T7, &curve->p, &T7);                        //    T7 = T2+T7 = X1 * Z1 + Y1
    mp_mulmod(&T4, &T6, &curve->p, &T8);                        //    T8 = T4*T6 = X2 * Z2
    mp_addmod(&T5, &T8, &curve->p, &T8);                        //    T8 = T5+T8 = X2 * Z2 + Y2
    mp_mulmod(&T2, &T5, &curve->p, &T2);                        //    T2 = T2*T5 = Y1 * Y2
    mp_mulmod(&T7, &T8, &curve->p, &T7);                        //    T7 = T7*T8 = (X1 * Z1 + Y1) * (X2 * Z2 + Y2)
    mp_submod(&T7, &T2, &curve->p, &T7);                      //    T7 = T7-T2
    mp_mulmod(&T1, &T4, &curve->p, &T5);                        //    T5 = T1*T4
    mp_addmod(&T1, &T3, &curve->p, &T1);                       //    T1 = T1+T3
    mp_mulmod(&T3, &T6, &curve->p, &T8);                         //    T8 = T3*T6
    mp_addmod(&T4, &T6, &curve->p, &T4);                        //    T4 = T4+T6
    mp_mulmod(&T5, &T8, &curve->p, &T6);                        //    T6 = T5*T8
    mp_submod(&T7, &T6, &curve->p, &T7);                        //    T7 = T7-T6
    mp_mulmod(&T1, &T4, &curve->p, &T1);                        //    T1 = T1*T4
    mp_submod(&T1, &T5, &curve->p, &T1);                         //    T1 = T1-T5
    mp_submod(&T1, &T8, &curve->p, &T1);                        //    T1 = T1-T8
    mp_sqrmod(&T1, &curve->p, &T3);                        //    T3 = T1^2
    mp_addmod(&T6, &T6, &curve->p, &T6);                        //    T6 = 2*T6
    mp_submod(&T3, &T6, &curve->p, &T3);                        //    T3 = T3-T6
    mp_mulmod(&curve->e, &T6, &curve->p, &T4);   // T4 = 2 * e * X1 * X2 * Z1 * Z2
    mp_mulmod(&T3, &T4, &curve->p, &T3);// T3 = 2 * e * X1 * X2 * Z1 * Z
    mp_mulmod(&curve->d, &T6, &curve->p, &T4);                        //    T4 = d*T6
    mp_submod(&T2, &T4, &curve->p, &T2);                        //    T2 = T2+T4
    mp_sqrmod(&T8, &curve->p, &T4);                        //    T4 = T8^2
    mp_sqrmod(&T5, &curve->p, &T8);                        //    T8 = T5^2
    mp_mulmod(&T8, &curve->e, &curve->p, &T8);
    mp_addmod(&T8, &T4, &curve->p, &T5);                        //    T5 = T4+T8
    mp_mulmod(&T2, &T5, &curve->p, &T2);                         //    T2 = T2*T5
    mp_addmod(&T2, &T3, &curve->p, &T2);                        //    T2 = T2+T3
    mp_submod(&T4, &T8, &curve->p, &T5);                         //    T5 = T4-T8
    mp_copy(&T7, &result->X);                        //    X3 = T7
    mp_copy(&T2, &result->Y);                        //    Y3 = T2
    mp_copy(&T5, &result->Z);                        //    Z3 = T5



    mp_clear_multi(&T1, &T2, &T3, &T4, &T5, &T6, &T7, &T8, NULL);
}
void affineCoordinatesConversion (struct Point *result, const struct Point *point, const struct Curve *curve)
{
    mp_int x, y;
    mp_init_multi(&x, &y, NULL);

    // x
    mp_invmod(&point->Z, &curve->p, &x);
    mp_mulmod(&x, &point->X, &curve->p, &x);

    // y
    mp_invmod(&point->Z, &curve->p, &y);
    mp_sqrmod(&y, &curve->p, &y);
    mp_mulmod(&y, &point->Y, &curve->p, &y);

    mp_copy(&x, &result->X);
    mp_copy(&y, &result->Y);
    mp_clear_multi(&x, &y, NULL);
}
void printAffinePoint(const struct Point *point, const  struct Curve *curve){
    struct Point temp;
    initPoint(&temp);
    affineCoordinatesConversion(&temp, point, curve);
    char stroka[150] = "";
    mp_to_radix(&temp.X, stroka, 150, NULL, 10);
    printf("\nx = %s", stroka);
    stroka[150] = "";
    mp_to_radix(&temp.Y, stroka, 150, NULL, 10);
    printf("\ny = %s", stroka);
    stroka[150] = "";
}
void negativePoint (struct Point *result, const struct Point *point){
    mp_neg(&point->X, &result->X);
    mp_copy(&point->Y, &result->Y);
    mp_copy(&point->Z, &result->Z);
}
void basePoint (struct Point *result, const struct Curve *curve){
    mp_copy(&curve->X, &result->X);
    mp_copy(&curve->Y, &result->Y);
    mp_copy(&curve->Z, &result->Z);
}
_Bool pointsEquality(const struct Point *point1, const struct Point *point2, const struct Curve *curve){

    struct Point temp, temp2;
    initPoint(&temp); initPoint(&temp2);

    affineCoordinatesConversion(&temp, point1, curve);
    affineCoordinatesConversion(&temp2, point2, curve);

    return  (mp_cmp(&temp.X, &temp2.X) == MP_EQ) && (mp_cmp(&temp.Y, &temp2.Y) == MP_EQ) ;
    clearPoint(&temp); clearPoint(&temp2);
}
_Bool pointOnCurve (const struct Curve *curve, const struct Point *point)
{
    int err;
    _Bool result;
    mp_int temp1, temp2, temp3;
    mp_init_multi(&temp1, &temp2, &temp3, NULL);
    //Y^2 = e * X^4 - 2 * d * X^2 * Z^2 + Z^4
    mp_set_i32(&temp1, 4);                                    // temp1 = 4
    mp_exptmod(&point->X, &temp1, &curve->p, &temp2);                   // temp2 = X^4
    mp_mulmod(&temp2, &curve->e, &curve->p, &temp2);                    // temp2 = e * X^4
    mp_exptmod(&point->Z, &temp1, &curve->p, &temp3);                   // temp3 = Z^4
    mp_addmod(&temp2, &temp3, &curve->p, &temp2);                       // temp2 = e * X^4 + Z^4
    mp_mulmod(&point->Z, &point->Z, &curve->p, &temp3);                 // temp3 = Z^2
    mp_addmod(&temp3, &temp3, &curve->p, &temp3);                       // temp3 = 2 * Z^2
    mp_mulmod(&temp3, &curve->d, &curve->p, &temp3);                    // temp3 = 2 * d * Z^2
    mp_mulmod(&point->X, &point->X, &curve->p, &temp1);                 // temp1 = X^2
    mp_mulmod(&temp3, &temp1, &curve->p, &temp3);                       // temp3 = 2 * d * Z^2 * X^2
    mp_submod(&temp2, &temp3, &curve->p, &temp2);                       // temp2 = e * X^4 + Z^4 - 2 * d * Z^2 * X^2
    mp_mulmod(&point->Y, &point->Y, &curve->p, &temp3);                 // temp3 = Y^2
    mp_submod(&temp2, &temp3, &curve->p, &temp2);                       // temp2 = e * X^4 + Z^4 - 2 * d * Z^2 * X^2 - Y^2
    mp_zero(&temp1);                                             // temp1 = 0
    if (mp_cmp(&temp1, &temp2) == MP_EQ){
        result = 1;
    }
    else{
        result = 0;
    }

    mp_clear_multi(&temp1, &temp2, &temp3, NULL);

    return result;
}
void montgomeryLadder (struct Point *result, const struct Point *point, const struct Curve *curve, const mp_int k){

    int bits = mp_count_bits(&k);

    struct Point R,Q;
    initPoint(&R); initPoint(&Q);

    mp_copy(&point->X, &R.X);                       // R = P
    mp_copy(&point->Y, &R.Y);
    mp_copy(&point->Z, &R.Z);

    customPointInit(&Q, "0", "1", "1");  // Q = O
    int flag;
    for (int i = bits - 1; i >= 0; i--)
    {
        if ( mp_get_bit(&k, i) )
        {
            pointsAddition (&Q, &Q, &R, curve);     // Q = Q + R
            pointsAddition (&R, &R, &R, curve);     // R = 2 * R
        }
        else
        {
            pointsAddition (&R, &R, &Q, curve);     // R = Q + R
            pointsAddition (&Q, &Q, &Q, curve);     // Q = 2 * Q
        }
    }
    mp_copy(&Q.X, &result->X);
    mp_copy(&Q.Y, &result->Y);
    mp_copy(&Q.Z, &result->Z);

    clearPoint(&Q); clearPoint(&R);
}
void binaryMethod(struct Point *result, const struct Point *P, const struct Curve *curve, const mp_int *k) { // Лекции, слайд 53
    struct Point Q;
    initPoint(&Q);
    customPointInit(&Q, "0", "1", "1");  // Q = O
    int bits = mp_count_bits(k);

    for (int i = bits - 1; i >= 0; --i) {
        pointsAddition (&Q, &Q, &Q, curve);
        if (mp_get_bit(k, i))
            pointsAddition (&Q, &Q, P, curve);
    }
    mp_copy(&Q.X, &result->X);
    mp_copy(&Q.Y, &result->Y);
    mp_copy(&Q.Z, &result->Z);

}