#include <stdio.h>
#include "FloatVar.h"

#include <math.h>

void adam(struct FloatVar *x,struct FloatVar *y,struct FloatVar *f,float lr,float* x_i,float* y_i, int iteration, 
float beta1,float beta2,float lambda,bool amsgrad,bool maximize);

int main(int argc, char* argv[])
{
    printf("Hello variables:\n");
    struct FloatVar *x, *y, *z1, *z2, *z3, *z4, *f;
    float lr = 1e-1, *x_i, *y_i;
    
    float beta1 = 1-1e-1, beta2 = 1-1e-3, lambda = 0;
    bool amsgrad = false, maximize = false;
    int iteraiton = atof(argv[1]);
    
    x = FloatVar_constructor();
    y = FloatVar_constructor();
    z1 = FloatVar_constructor();
    z2 = FloatVar_constructor();
    z3 = FloatVar_constructor();
    z4 = FloatVar_constructor();
    f = FloatVar_constructor();
    x_i = (float *)malloc(sizeof(float));
    y_i = (float *)malloc(sizeof(float));
    
    (*x->setupLeaf)(x);
    (*y->setupLeaf)(y);
    (*z1->setupInter2)(z1, FloatVar_mul, x, x);
    (*z2->setupInter2)(z2, FloatVar_mul, y, y);
    (*z3->setupInter2)(z3, FloatVar_mul, x, x);
    (*z4->setupInter2)(z4, FloatVar_add, z1, z2);
    (*f->setupInter2)(f, FloatVar_mul, z3, z4);
    
    
    
    *x_i = 200.0;
    *y_i = 100.0;
    (*x->setVal)( x, *x_i);
    (*y->setVal)( y, *y_i);
    (*f->func)(f);
    printf("x = %.2f, y = %.2f, f = %.2f\n",(*x->func)(x),(*y->func)(y),(*f->func)(f));
    //printf("f'_x = %.2f, f'_y = %.2f\n\n",(*f->locGrad)(f, x),(*f->locGrad)(f, y));
    
    adam(x,y,f,lr,x_i,y_i, iteraiton,
    beta1 , beta2, lambda, amsgrad , maximize);
    
    (*x->deconstructor)(x);
    (*y->deconstructor)(y);
    (*z1->deconstructor)(z1);
    (*z2->deconstructor)(z2);
    (*f->deconstructor)(f);
    
    return 0;
}

void adam(struct FloatVar *x,struct FloatVar *y,struct FloatVar *f,float lr,float* x_i,float* y_i, int iteration,
float beta1,float beta2,float lambda,bool amsgrad,bool maximize)
{
    
    
    float x_grad, y_grad, xStep, yStep;
    
    
    float m_x = 0, m_y = 0;
    float v_x = 0, v_y = 0;
    float m_x_cov, m_y_cov ;
    float v_x_cov, v_y_cov ;
    float v_x_cov_max = 0, v_y_cov_max = 0;
    float eps = 1e-8;
    for(int i = 1; i <= iteration; i ++)
    {
        (*f->func)(f);
        x_grad = (*f->locGrad)(f, x);
        y_grad = (*f->locGrad)(f, y);
        
        //Adaptive Moment Optimization algorithm
        if(maximize == true)
        {
            x_grad = - x_grad;
            y_grad = - y_grad;
        }
        if(lambda != 0)
        {
            x_grad = x_grad + lambda * *x_i;
            y_grad = y_grad + lambda * *y_i;
        }
        m_x = beta1 * m_x + ( 1 - beta1 ) * x_grad;
        m_y = beta1 * m_y + ( 1 - beta1 ) * y_grad;
        
        //printf("m_x = %.4f, m_y = %.4f\n\n", m_x, m_y);
        
        v_x = beta2 * v_x + ( 1 - beta2 ) * x_grad * x_grad;
        v_y = beta2 * v_y + ( 1 - beta2 ) * y_grad * y_grad;
        
        //printf("v_x = %.4f, v_y = %.4f\n\n", v_x, v_y);
        
        m_x_cov = m_x / ( 1 - pow(beta1,i));
        m_y_cov = m_y / ( 1 - pow(beta1,i));
        
        //printf("m_x_cov = %.4f, m_y_cov = %.4f\n\n", m_x_cov, m_y_cov);
        
        v_x_cov = v_x / ( 1 - pow(beta2,i));
        v_y_cov = v_y / ( 1 - pow(beta2,i));
        
        if(amsgrad == true)
        {
            if(v_x_cov_max < v_x_cov )
            {
                v_x_cov_max = v_x_cov_max;
            }
            if(v_y_cov_max < v_y_cov )
            {
                v_y_cov_max = v_y_cov_max;
            }
            xStep = - lr * m_x_cov / (pow(v_x_cov_max, 0.5) + eps);
            yStep = - lr * m_y_cov / (pow(v_y_cov_max, 0.5) + eps);
        }
        else
        {
            xStep = - lr * m_x_cov / (pow(v_x_cov, 0.5) + eps);
            yStep = - lr * m_y_cov / (pow(v_y_cov, 0.5) + eps);
        }
        

        //printf("xStep = %.4f, yStep = %.4f\n\n", xStep, yStep);
        
        
        
        *x_i = *x_i + xStep;
        *y_i = *y_i + yStep;
        
        
        (*x->setVal)( x, *x_i);
        (*y->setVal)( y, *y_i);
        printf("x = %.4f, y = %.4f, f = %.4f\n",(*x->func)(x),(*y->func)(y),(*f->func)(f));
        //printf("f'_x = %.2f, f'_y = %.2f\n\n",(*f->locGrad)(f, x),(*f->locGrad)(f, y));
    }
}

