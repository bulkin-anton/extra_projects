#include <stdio.h>
//using only for calculating tha square root of x
#include <math.h>


//подинтегральная функция из условия
double
function(double x){
    return 1 / ((25 * x * x + 1) * sqrt(x));
}


//реализация метода трапеций(метод из условия)
//подинтегральная функция func(x), на отрезке [a, b], число отрезков разбиения n
double
integral_trap(double (*func)(double), double a, double b, int n){
    double h, result;
    //вычисляем шаг
    h = (b - a) / n;
    result = (func(a) + func(b)) / 2;
    for (int i = 1; i < n; ++i){
        result += func(a + i * h);
    }
    return result * h;
}

//реализация метода средних прямоугольников
//подинтегральная функция func(x), на отрезке [a, b], число отрезков разбиения n
double
integral_rectan(double (*func)(double), double a, double b, int n){
    double h, result = 0, x = a;
    //вычисляем шаг
    h = (b - a) / n;
    for (int i = 1; i <= n; i++){
        x = a + (i - 0.5) * h;
        result += func(x);
    }
    return result * h;
}

int
main(void){
    // результат метода трапеций
    double res1,
    // результат метода сердних прямоугольников
    res2,
    // точки в 2х случаях:
    a[2] = {1, 0},
    b[2] = {2, 1},
    //приближенный результат аналитических решений
    ist_res[2] = {0.01683288970140462, 0.9672379973052818};
    //кол-во отрезков в разбиении
    int n_num[] = {16, 32, 64};
    /*
     * Output format:
     * (далее: I_t - результат метода трапеций,
     *         I_p - результат метода средних прямоугольников,
     *         I_a - приближенное значение аналитического значения,
     *         proc = I_(p || t) / I_a * 100 % - процент, который
     * составляет результат вычислений от примерного значения интеграла на 100%
     * )
     * (
     *     вывод всех приближенных значений интегралов в формате %lf с 20тью знаками после запятой
     *     вывод
     * )
     * (
     *     метод_вычисления:
     *         (T) - если использован метод трапеций
     *         (P) - если использован метод средних прямоугольников
     * )
     *  кол-во_отрезков_в_разбиении: (метод_вычисления) |I_a - I| proc
    */
    for (int k = 0; k < 2; k++){
        printf("The result for %d interval: [%lf, %lf]:\n", k + 1, a[k], b[k]);
        for (int i = 0; i < (sizeof(n_num) / sizeof(int)); ++i){
            res1 = integral_trap(function, a[k], b[k], n_num[i]);
            printf("%*d: (T) %.*lf %.*lf %.*lf%%\n", 4, n_num[i], 20,
            res1, 20, res1 - ist_res[k], 10, (res1 / ist_res[k] * 100));
            res2 = integral_rectan(function, a[k], b[k], n_num[i]);
            printf("%*d: (P) %.*lf %.*lf %.*lf%%\n", 4, n_num[i], 20,
            res2, 20, (ist_res[k] - res2), 10, (res2 / ist_res[k] * 100));
        }
    }
    return 0;
}
