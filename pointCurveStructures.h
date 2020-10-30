#include "tommath.h"

struct Point{
    mp_int X, Y, Z;
};

struct Curve{
    mp_int theta, a, e, d, x, y, p, q, X, Y, Z;
};
// Инициализация точки
void initPoint(struct Point *point);
// Удаление точки
void clearPoint(struct Point *point);
// Создание точки с произовльными координатами заданными в 16-ой и 10-ой системе счисления
void customPointInit16 (struct Point *point, char *X, char *Y, char *Z);
void customPointInit10 (struct Point *point, char *X, char *Y, char *Z);
// Вывод точки в проективных и афинных координатах соответственно
void printProjectivePoint(const struct Point *point);
void printAffinePoint(const struct Point *point);
// Инициализация кривой
void initCurve(struct Curve *curve);
// Удаление кривой
void clearCurve(struct Curve *curve);
// Создание кривой
void curveCreate (struct Curve *curve);
// Сложение двух точек
void pointsAddition (struct Point *result, const struct Point *point1, const struct Point *point2, const struct Curve *curve);
// Перевод из проективных в афинные координаты
void affineCoordinatesConversion (struct Point *result, const struct Point *point, const struct Curve *curve);
// Обратная точка
void negativePoint (struct Point *result, const struct Point *point);
// Порождающая точка
void basePoint (struct Point *result, const struct Curve *curve);
// Проверка лежит ли точка на кривой
_Bool pointOnCurve (const struct Curve *curve, const struct Point *point);