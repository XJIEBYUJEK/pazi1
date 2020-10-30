#include <stdio.h>
#include <stdlib.h>
#include "tommath.h"
#include "parametrs.h"
#include "pointCurveStructures.h"



int main()
{
    int flag;
    char stroka[150] = "";
    struct Point point, negative, Q, neutral, P, point2;
    initPoint(&point);initPoint(&negative);initPoint(&Q);initPoint(&neutral);initPoint(&P);initPoint(&point2);
    struct Curve jacob;
    initCurve(&jacob);
    curveCreate(&jacob);
    basePoint(&P, &jacob);
    printf("\nКоординаты точки P: ");
    printProjectivePoint(&P);
    if (pointOnCurve(&P, &jacob)) {
        printf("\nТочка P лежит на кривой");
    }
    else{
        printf("\nТочка P не лежит на кривой");
    }
    mp_int k1, k2, k3;
    mp_init_multi(&k1, &k2, &k3, NULL);
    mp_rand(&k1, 5);
    mp_to_radix(&k1, stroka, 150, NULL, 10);
    printf("\nСлучайное значение k = %s", stroka);
    binaryMethod(&Q, &P, &jacob, &k1);
    printf("\nКоординаты точки Q: ");
    printProjectivePoint(&Q);

    //test 1

    printf("\nТест №1, проверим лежит ли Q на кривой:");
    if (pointOnCurve(&Q, &jacob)) {
        printf("\nТочка Q лежит на кривой");
    }
    else{
        printf("\nТочка Q не лежит на кривой");
    }

    //test 2

    printf("\n\nТест №2, проверим что q * P = O:");
    createPoint(&neutral, "0", "1", "1"); //Нейтральный элемент
    binaryMethod(&point, &P, &jacob, &jacob.q); // point = q * P
    if (pointsEquality(&point, &neutral, &jacob)) {
        printf("\nq * P = O, условие выполнено");
    }
    else{
        printf("\nq * P не равен нейтральному элементу");
    }

    //test 3

    printf("\n\nТест №3.1, проверим что (q+1) * P = P:");
    mp_int temp;
    mp_init(&temp);
    mp_set_i32(&temp, 1);
    mp_add(&temp, &jacob.q, &temp);
    binaryMethod(&point, &P, &jacob, &temp); //point = (q+1) * P
    if (pointsEquality(&point, &P, &jacob)) {
        printf("\n(q+1) * P = P, условие выполнено");
    }
    else{
        printf("\n(q+1) * P не равен P");
    }
    printf("\nТест №3.2, проверим что (q-1) * P = -P:");
    negativePoint(&negative, &P); // -P
    mp_set_i32(&temp, 1);
    mp_sub(&jacob.q, &temp, &temp);
    binaryMethod(&point, &P, &jacob, &temp); //point = (q-1) * P
    if (pointsEquality(&point, &negative, &jacob)) {
        printf("\n(q-1) * P = -P, условие выполнено");
    }
    else{
        printf("\n(q-1) * P не равен -P");
    }

    //test4

    printf("\n\nТест №4, проверим что k1 * P + k2 * P = (k1+ k2) * P :");
    printf("\nСлучайное значение k1 = %s", stroka);
    stroka[150] = "";
    mp_rand(&k2, 5);
    mp_to_radix(&k2, stroka, 150, NULL, 10);
    printf("\nСлучайное значение k2 = %s", stroka);
    binaryMethod(&point, &P, &jacob, &k1); // point = k1*P
    binaryMethod(&point2, &P, &jacob, &k2); // point2 = k2*P
    pointsAddition(&point, &point, &point2, &jacob); // point = k1*P + k2*P
    mp_add(&k1, &k2, &k3); //k3 = k1+k2
    stroka[150] = "";
    mp_to_radix(&k3, stroka, 150, NULL, 10);
    printf("\nk1+k2                 = %s", stroka);
    binaryMethod(&point2, &P, &jacob, &k3); // point2 = (k2+k1)*P
    if (pointsEquality(&point, &point2, &jacob)) {
        printf("\nk1 * P + k2 * P = (k1+ k2) * P, условие выполнено");
    }
    else{
        printf("\nУсловие не выполнилось :(");
    }



    mp_clear_multi(&k1, &k2, &k3, NULL);
    clearPoint(&point);clearPoint(&negative);clearPoint(&Q);clearPoint(&neutral);clearPoint(&P);clearPoint(&point2);
    clearCurve(&jacob);



}


