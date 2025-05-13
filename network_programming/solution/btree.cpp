#include <cstring>
#include <iostream>

struct key_type{
    key_type();
    key_type(const char *, const char *);
    ~key_type();
    key_type(const key_type&);
    key_type& operator = (const key_type&);
    char *key1;
    char *key2;
};

key_type::key_type(){
    key1 = 0;
    key2 = 0;
}

key_type::key_type(const char *a, const char *b){
    key1 = new char[strlen(a) + 1];
    strcpy(key1, a);
    if (b){
        key2 = new char[strlen(b) + 1];
        strcpy(key2, b);
    } else key2 = 0;
}

key_type::~key_type(){
    delete []key1;
    delete []key2;
}
key_type::key_type(const key_type &x){
    key1 = new char[strlen(x.key1) + 1];
    strcpy(key1, x.key1);
    if (x.key2){
        key2 = new char[strlen(x.key2) + 1];
        strcpy(key2, x.key2);
    } else key2 = 0;
}

key_type&
key_type::operator = (const key_type &x){
    if (this == &x){
        return *this;
    }
    delete []key1;
    delete []key2;
    key1 = new char[strlen(x.key1) + 1];
    strcpy(key1, x.key1);
    if (x.key2){
        key2 = new char[strlen(x.key2) + 1];
        strcpy(key2, x.key2);
    } else key2 = 0;
    return *this;
}

struct node{
    node();
    node(key_type, const char*);
    ~node();
    key_type key;
    node *left;
    node *right;
    char *val;
};

node::node(key_type a, const char *c){
    key = a;
    val = new char[strlen(c) + 1];
    strcpy(val, c);
    left = 0;
    right = 0;
}

node::~node(){
    delete left;
    delete right;
    delete []val;
}

int
compare(key_type a, key_type b){
    if (strcmp(a.key1, b.key1) > 0){
        return 1;
    } else if (strcmp(a.key1, b.key1) == 0){
        if (a.key2){
            if (b.key2){
                if (strcmp(a.key2, b.key2) == 0){
                    return 0;
                } else if (strcmp(a.key2, b.key2) > 0){
                    return 1;
                } else return -1;
            }
            else return 1;
        } else if (b.key2) return -1;
        else return 0;
    }
    return -1;
}

node*
insert(node* ptr, key_type a, const char* c){
	if(!ptr) return new node(a, c);
	if(compare(a, ptr -> key) < 0)
		ptr -> left = insert(ptr -> left, a, c);
	else
		ptr -> right = insert(ptr -> right, a, c);
	return ptr;
}

node*
find(node *ptr, key_type a){
    if (ptr == 0){
        return 0;
    }
    if (compare(a, ptr -> key) < 0){
		return find(ptr -> left, a);
    } else if (compare(a, ptr -> key) > 0){
		return find(ptr -> right, a);
    }
    return ptr;
}

int main(){
    node *tree = insert(0, key_type("a", 0), "1");
    tree = insert(tree, key_type("b", 0), "2");
    tree = insert(tree, key_type("c", 0), "3");
    tree = insert(tree, key_type("d", "f"), "4");
    std::cout << find(tree, key_type("c", 0)) -> val << std::endl;
    delete tree;
}
