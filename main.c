#include <stdio.h>
#include "FloatVar.h"

int main(int argc, char* argv[])
{
    printf("Hello variables:\n");
    struct FloatVar *x, *y, *f;
    x = FloatVar_constructor(x);
    y = FloatVar_constructor(y);
    f = FloatVar_constructor(f);
    
    (*x->setupLeaf)(x);
    (*y->setupLeaf)(y);
    (*f->setupInter2)(f, FloatVar_mul, x, y);
    
    (*x->setVal)( x, 10.0);
    (*y->setVal)( y, 10.0);
    printf("x = %.2f, y = %.2f, f = %.2f\n",(*x->func)(x),(*y->func)(y),(*f->func)(f));
    
    return 0;
}
