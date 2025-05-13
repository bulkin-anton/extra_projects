#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum Point_pos {LEFT,  RIGHT};
enum {ARG_ROTATE = 0};

struct Point{
    double x, y;
    struct Point *left;
    struct Point *right;
};

typedef struct Point Point;

Point*
rotate(Point *arg){
    double newx  = cos(ARG_ROTATE) * arg -> x - sin(ARG_ROTATE) * arg -> y;
    double newy  = sin(ARG_ROTATE) * arg -> x + cos(ARG_ROTATE) * arg -> y;
    arg -> x = newx;
    arg -> y = newy;
    return arg;
}

struct Segment{
    Point *left;
    Point *right;
};

typedef struct Segment Segment;

struct Node{
    Segment *seg;
    struct Node *left;
    struct Node *right;
    int height;
};

typedef struct Node node;

int
height(node* ptr){
	return ptr ? ptr -> height : 0;
}

int
bfactor(node* ptr){
	return height(ptr -> right) - height(ptr -> left);
}

void
fixheight(node* ptr){
	int hl = height(ptr -> left);
	int hr = height(ptr -> right);
	ptr -> height = (hl > hr ? hl : hr) + 1;
}

int
cmp_node(Point *l, Point *r, Segment *b){
    if ((l == b -> left) && (r == b -> right)){
        return 0;
    }
    if (l == b -> left){
        if (r -> y < b -> right -> y){
            return -1;
        }
        return 1;
    } else if (l -> y < b -> left -> y){
        return -1;
    }
    return 1;
}

node*
rotate_right(node* ptr){
	node* f = ptr -> left;
	ptr -> left = f -> right;
	f -> right = ptr;
	fixheight(ptr);
	fixheight(f);
	return f;
}

node*
rotate_left(node* ptr){
	node* f = ptr -> right;
	ptr -> right = f -> left;
	f -> left = ptr;
	fixheight(ptr);
	fixheight(f);
	return f;
}

node*
balance(node* ptr){
	fixheight(ptr);
	if(bfactor(ptr) == 2){
		if(bfactor(ptr -> right) < 0)
			ptr -> right = rotate_right(ptr -> right);
		return rotate_left(ptr);
	}
	if(bfactor(ptr) == -2){
		if(bfactor(ptr -> left) > 0)
			ptr -> left = rotate_left(ptr -> left);
		return rotate_right(ptr);
	}
	return ptr;
}

node*
insert(node* ptr, Point *l, Point *r){
	if(!ptr){
        node *s = malloc(sizeof(node));
        s -> seg = malloc(sizeof(Segment));
        s -> seg -> left = l;
        s -> seg -> right = r;
        s -> height = 0;
        s -> left = NULL;
        s -> right = NULL;
        return s;
    }
	if(cmp_node(l, r, ptr -> seg) == -1)
		ptr -> left = insert(ptr -> left, l, r);
	else
		ptr -> right = insert(ptr -> right, l, r);
	return balance(ptr);
}

node*
findmin(node* ptr){
	return ptr -> left ? findmin(ptr -> left) : ptr;
}

node*
removemin(node* ptr){
	if(ptr -> left == 0)
		return ptr -> right;
	ptr -> left = removemin(ptr -> left);
	return balance(ptr);
}

node*
remove_node(node* ptr, Point *l, Point *r){
	if(!ptr) return 0;
	if(cmp_node(l, r, ptr -> seg) == -1)
		ptr -> left = remove_node(ptr -> left, l, r);
	else if(cmp_node(l, r, ptr -> seg) == 1)
		ptr -> right = remove_node(ptr -> right, l, r);
    else {
		node* q = ptr -> left;
		node* f = ptr -> right;
        free(ptr -> seg);
		free(ptr);
		if(!f) return q;
		node* min = findmin(f);
		min -> right = removemin(f);
		min -> left = q;
		return balance(min);
	}
    return balance(ptr);
}

void
delete_tree(node *ptr){
    if (ptr == NULL) return;
    delete_tree(ptr -> left);
    delete_tree(ptr -> right);
    free(ptr -> seg);
    free(ptr);
}

Segment*
min_node(node* head){
    if (head -> left == NULL){
      return head -> seg;
    } else{
       return min_node(head -> left);
    }
}

Segment*
above(node *head, Point *l, Point *r){
    if (head == NULL){
        return NULL;
    } else if (cmp_node(l, r, head -> seg) == 0){
        return min_node(head);
    } else if (cmp_node(l, r, head -> seg) == -1){
        return above(head -> left, l, r);
    } else{
        return above(head -> right, l, r);
    }
}

Segment*
under(node *head, Point *l, Point *r){
    if (head == NULL){
        return NULL;
    } else if (cmp_node(l, r, head -> seg) == 0){
        if (head -> left) return head -> left -> seg;
        else return NULL;
    } else if (cmp_node(l, r, head -> seg) == -1){
        return under(head -> left, l, r);
    } else{
        Segment *ans = under(head -> right, l, r);
        if (ans) return ans;
        else return head -> seg;
    }
}

int
compare(const void *a, const void *b){
    if ((**(const Point**)a).x > (**(const Point**)b).x){
        return 1;
    } else {
        return -1;
    }
}

int
intersect(Segment *otr1, Segment *otr2){
    if (otr1 == otr2){
        return 0;
    }
    double x1 = otr1 -> left -> x;
    double x2 = otr1 -> right -> x;
    double x3 = otr2 -> left -> x;
    double x4 = otr2 -> right -> x;
    double y1 = otr1 -> left -> y;
    double y2 = otr1 -> right -> y;
    double y3 = otr2 -> left -> y;
    double y4 = otr2 -> right -> y;
    double v1 = ((x4-x3)*(y1-y3)) - ((y4-y3)*(x1-x3));
    double v2 = ((x4-x3)*(y2-y3)) - ((y4-y3)*(x2-x3));
    double v3 = ((x2-x1)*(y3-y1)) - ((y2-y1)*(x3-x1));
    double v4 = ((x2-x1)*(y4-y1)) - ((y2-y1)*(x4-x1));
    return ((v1 * v2 < 0) && (v3 * v4 < 0));
}

int
left_point(Point *l, Point *r, node *tree){
    Segment *otr1, *otr2, *otr;
    otr1 = above(tree, l, r);
    otr2 = under(tree, l, r);
    otr = malloc(sizeof(Segment));
    otr -> left = l;
    otr -> right = r;
    if (otr1){
        if (intersect(otr1, otr)){
            free(otr);
            return 1;
        }
    }
    if (otr2){
        if (intersect(otr2, otr)){
            free(otr);
            return 1;
        }
    }
    free(otr);
    return 0;
}

int
right_point(Point *l, Point *r, node *tree){
    Segment *otr1, *otr2, *otr;
    otr1 = above(tree, l, r);
    otr2 = under(tree, l, r);
    if (otr2 && otr1){
        if (intersect(otr2, otr1)){
            return 1;
        }
    }
    otr = malloc(sizeof(Segment));
    otr -> left = l;
    otr -> right = r;
    if (otr1){
        if (intersect(otr1, otr)){
            free(otr);
            return 1;
        }
    }
    if (otr2){
        if (intersect(otr2, otr)){
            free(otr);
            return 1;
        }
    }
    return 0;
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
    Point **points = malloc(n * sizeof( Point *));
    printf("Введите %d точек в формате \"x y\"\n", n);
    for (int i = 0; i < n; i++){
        points[i] = malloc(sizeof(Point));
        scanf("%lf", &(*points[i]).x);
        scanf("%lf", &(*points[i]).y);
        if (i){
            (*points[i]).left = points[i - 1];
            (*points[i - 1]).right = points[i];
        }
        points[i] = rotate(points[i]);
    }
    (*points[0]).left = points[n - 1];
    (*points[n - 1]).right = points[0];
    qsort(points, n, sizeof(Point*), compare);
    node *tree = NULL;
    int inters = 0;
    for (int i = 0; i < n; i++){
        if ((*points[i]).left -> x > points[i] -> x){
            tree = insert(tree, points[i], (*points[i]).left);
            if (left_point(points[i], (*points[i]).left, tree)){
                inters = 1;
                break;
            }
        } else{
            if (right_point((*points[i]).left, points[i], tree)){
                inters = 1;
                break;
            }
            tree = remove_node(tree, (*points[i]).left, points[i]);
        }
        if ((*points[i]).right -> x > points[i] -> x){
            tree = insert(tree, points[i], (*points[i]).right);
            if (left_point(points[i], (*points[i]).right, tree)){
                inters = 1;
                break;
            }
        } else{
            if (right_point((*points[i]).right, points[i], tree)){
                inters = 1;
                break;
            }
            tree = remove_node(tree, (*points[i]).right, points[i]);
        }
    }
    delete_tree(tree);
    if (inters){
        printf("Многоугольник не является простым\n");
    } else printf("Многоугольник - простой\n");
    for (int i = 0; i < n; i++){
        free(points[i]);
    }
    free(points);
    return 0;
}
