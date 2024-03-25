#include <stdio.h>
#include "FloatVar.h"

int main(int argc, char* argv[])
{
    printf("Hello variables:\n");
    struct FloatVar *x, *y, *z, *f;
    float lr = 0.001, xStep, yStep;
    x = FloatVar_constructor(x);
    y = FloatVar_constructor(y);
    z = FloatVar_constructor(z);
    f = FloatVar_constructor(f);
    
    (*x->setupLeaf)(x);
    (*y->setupLeaf)(y);
    (*z->setupInter2)(z, FloatVar_mul, x, y);
    (*f->setupInter2)(f, FloatVar_mul, z, z);
    
    (*x->setVal)( x, 10.0);
    (*y->setVal)( y, 10.0);
    printf("x = %.2f, y = %.2f, f = %.2f\n",(*x->func)(x),(*y->func)(y),(*f->func)(f));
    //printf("f'_x = %.2f, f'_y = %.2f\n\n",(*f->locGrad)(f, x),(*f->locGrad)(f, y));
    
    for(int i = 0; i < 1000; i ++)
    {
        xStep =  -(*f->locGrad)(f, x) * lr;
        yStep =  -(*f->locGrad)(f, y) * lr;
        //printf("xStep = %.2f, yStep = %.2f\n\n", xStep, yStep);
        (*x->setVal)( x, x->val + xStep);
        (*y->setVal)( y, y->val + yStep);
        printf("x = %.2f, y = %.2f, f = %.2f\n",(*x->func)(x),(*y->func)(y),(*f->func)(f));
        //printf("f'_x = %.2f, f'_y = %.2f\n\n",(*f->locGrad)(f, x),(*f->locGrad)(f, y));
    }
    
    (*x->deconstructor)(x);
    (*y->deconstructor)(y);
    (*z->deconstructor)(z);
    (*f->deconstructor)(f);
    
    return 0;
}
