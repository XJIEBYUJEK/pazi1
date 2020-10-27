#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "tommath.h"


int main(void)
{
    mp_int number1, number2, number3;
    mp_init(&number1);
    char stroka[150];

    mp_set_i32(&number1, 1231243254);
    mp_expt_d(&number1, 2, &number1);



    mp_to_radix(&number1, stroka, 150, NULL, 10);
    printf("\n%s", stroka);

    mp_clear_multi(&number1, &number2,&number3, NULL);
    return EXIT_SUCCESS;
}


