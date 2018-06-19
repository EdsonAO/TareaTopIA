#include "BackProp.h"

int main()
{
    BackProp A(0.0001,0.5);
    // Input Size, Output Size, Capas, Función x Capa
    A.Start(2,1,{3,6},{1,1});
    A.Train({{1,1,1},{1,-1,1},{1,1,-1},{1,-1,-1}},{{0},{1},{1},{0}}); 
    A.Test({1,1,1});
    A.Test({1,-1,1});
    A.Test({1,1,-1});
    A.Test({1,-1,-1});

    return 0;
}