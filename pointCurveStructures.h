#include "tommath.h"

struct Point
{
    mp_int X, Y, Z;
};
// инициализация точки
void initPoint(struct Point *point);
// Создание точки с произовльными координатами заданными в 16-ой и 10-ой системе счисления
void customPointInit16 (struct Point *point, char *X, char *Y, char *Z);
void customPointInit10 (struct Point *point, char *X, char *Y, char *Z);
// Вывод точки в проективных и афинных координатах соответственно
void printProjectivePoint(const struct Point *point);
void printAffinePoint(const struct Point *point);