#include "MathFunctions.h"

double mysqrt(double x)
{
    if (x <= 0)
    {
        return 0;
    }

    double result;
    double delta;
    result = x;
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        if (result <= 0)
        {
            result = 0.1;
        }
        delta = x - (result * result);
        result = result + 0.5 * delta / result;
    }
    return result;
}
