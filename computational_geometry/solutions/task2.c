#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Point{
    double x, y;
};

typedef struct Point Point;

const double Delta = 0.000000000000001;

int
check_point(Point a, Point b, Point p){
    return (((b.x - a.x) * (p.x - a.x) + (b.y - a.y) * (p.y - a.y)) > 0);
}

double
vect_comp(Point a, Point b){
    return abs(a.x * b.y - b.x * a.y);
}

double
otr_dist(Point point, Point a, Point b){
    Point ab;
    Point ap;
    ab.x = b.x - a.x;
    ab.y = b.y - a.y;
    ap.x = point.x - a.x;
    ap.y = point.y - a.y;
    return vect_comp(ab, ap) / sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int
main(void){
    int n;
    printf("Введите количество n вершин у многоугольника\n");
    printf("n = ");
    scanf("%d", &n);
    if (n < 3){
        printf("Не хватает точек для многоугольника\n");
    }
    Point *points = malloc(n * sizeof(Point));
    printf("Введите %d точек в формате \"x y\"\n", n);
    for (int i = 0; i < n; i++){
        scanf("%lf", &(points[i].x));
        scanf("%lf", &(points[i].y));
    }
    printf("Введите точку \"x y\"\n");
    Point point;
    scanf("%lf", &(point.x));
    scanf("%lf", &(point.y));
    Point ans = point;
    double mdist, dist;
    for (int i = 0; i < n; i++){
        if (i == 0){
            mdist = sqrt(pow(points[i].x - point.x, 2) + pow(points[i].y - point.y, 2));
            ans = points[i];
            if (mdist < Delta){
                break;
            }
        } else{
            dist = sqrt(pow(points[i].x - point.x, 2) + pow(points[i].y - point.y, 2));
            if (dist < Delta){
                ans = points[i];
                break;
            } else{
                if (dist < mdist){
                    mdist = dist;
                    ans = points[i];
                }
            }
        }
        Point a = points[i];
        Point b = points[(i + 1) % n];
        dist = otr_dist(point, a, b);
        double len_ah = sqrt(pow(a.x - point.x, 2) + pow(a.y - point.y, 2) - pow(dist, 2));
        if ((len_ah < sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2))) && (check_point(a, b, point))){
            if (dist < mdist){
                mdist = dist;
                ans.x = a.x + (b.x - a.x) *
                (len_ah / sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
                ans.y = a.y + (b.y - a.y) *
                (len_ah / sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
            }
        }
    }
    printf("Проекция точки на многоугольник:\n");
    printf("x = %lf, y = %lf\n", ans.x, ans.y);
    free(points);
    return 0;
}
