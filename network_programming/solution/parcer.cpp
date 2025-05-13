#include <iostream>
#include <fstream>
int
main(int argc, char *argv[]){
    std::ifstream in(argv[1]);
    if (in.is_open())
    {
        char x;
        while (in >> x)
        {
            std::cout << x;
            if (x == '{'){

            }
        }
    }
    in.close();
    return 0;
}
