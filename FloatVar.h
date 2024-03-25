//FloatVar.h
//Author: Junfu Cheng
//University of Florida
#ifndef FLOATVAR_H
#define FLOATVAR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct FloatVar
{
    void (*setupLeaf)( struct FloatVar *this);
    void (*setupInter1)( struct FloatVar *this, float (*func)( struct FloatVar *), struct FloatVar *x); 
    void (*setupInter2)( struct FloatVar *this, float (*func)( struct FloatVar *), struct FloatVar *x,  struct FloatVar *y); 
    void (*setVal)( struct FloatVar *this, float val);
    void (*deconstructor)( struct FloatVar *this);
    
    float (*func)( struct FloatVar *this); 
    float (*locGrad)(struct FloatVar *this, struct FloatVar *par); 
    
    bool syn;
    
    float val;
    float grad;
    bool leaf;
    
    struct FloatVar *x;
    struct FloatVar *y;   
};

struct FloatVar* FloatVar_constructor(struct FloatVar *this);
void FloatVar_deconstructor(struct FloatVar *this);
void FloatVar_setupLeaf( struct FloatVar * this);
void FloatVar_setupInter1( struct FloatVar *this,  float (*func)( struct FloatVar *), struct FloatVar *x);
void FloatVar_setupInter2( struct FloatVar *this,  float (*func)( struct FloatVar *), struct FloatVar *x, struct FloatVar *y);
void FloatVar_setVal( struct FloatVar *this, float val);
float FloatVar_locGrad (struct FloatVar * this, struct FloatVar *par);
float FloatVar_add(struct FloatVar *this);
float FloatVar_mul(struct FloatVar *this);
float FloatVar_relu(struct FloatVar *this);
float FloatVar_leaf(struct FloatVar *this);

struct FloatVar* FloatVar_constructor(struct FloatVar *this)
{
    this = (struct FloatVar *)malloc(sizeof(struct FloatVar));
    this->setupLeaf = &FloatVar_setupLeaf;
    this->setupInter1 = &FloatVar_setupInter1;
    this->setupInter2 = &FloatVar_setupInter2;
    this->setVal = &FloatVar_setVal;
    this->deconstructor = &FloatVar_deconstructor;
    
    this->locGrad = &FloatVar_locGrad;
    
    this->syn = false;
    return this;
}

void FloatVar_deconstructor(struct FloatVar *this)
{
    free(this);
}

void FloatVar_setupLeaf( struct FloatVar * this)
{
    this->func = &FloatVar_leaf;
    
    this->syn = false;
    
    this->leaf = true;
}

void FloatVar_setupInter1( struct FloatVar *this,  float (*func)( struct FloatVar *), struct FloatVar *x)
{    
    this->func = func;
    
    this->syn = false;
    
    this->leaf = false;
    
    this->x = x;
}

void FloatVar_setupInter2( struct FloatVar *this,  float (*func)( struct FloatVar *), struct FloatVar *x, struct FloatVar *y)
{
    this->func = func; 
    
    this->syn = false;
    
    this->leaf = false;
    
    this->x = x;
    this->y = y;
}

void FloatVar_setVal( struct FloatVar *this, float val)
{
    this->val = val;
}

float FloatVar_locGrad (struct FloatVar * this, struct FloatVar *par)
{
    if( this->leaf == true)
    {
        if( par == this )
        {
            return 1.0;
        }
        if( par != this )
        {
            return 0.0;
        }
    }
    if( this->leaf != true)
    {
        if( this->func == &FloatVar_add)
        {
            this->grad = (*this->x->locGrad)(this->x, par) 
            + (*this->y->locGrad)(this->y, par);
            return this->grad;
        }
        if( this->func == &FloatVar_mul)
        {
            this->grad = (*this->x->locGrad)(this->x, par) * this->y->val
            + this->x->val * (*this->y->locGrad)(this->y, par);
            return this->grad;
        }
        if( this->func == &FloatVar_relu)
        {
            if(this->x->val >= 0)
            {
                this->grad = 1.0 * this->x->locGrad(this->x, par);
                return this->grad;
            }
            if(this->x->val < 0)
            {
                this->grad = 0.0 * this->x->locGrad(this->x, par);
                return this->grad;
            }
        }
    }
    printf("Error: from FloatVar_locGrad\n");
    return 0;
}

float FloatVar_add(struct FloatVar *this)
{
    this->val = (*this->x->func)(this->x) + (*this->y->func)(this->y);
    return this->val;
}

float FloatVar_mul(struct FloatVar *this)
{
    this->val = (*this->x->func)(this->x) * (*this->y->func)(this->y);
    return this->val;
}

float FloatVar_relu(struct FloatVar *this)
{
    if( this->x->val >= 0)
    {
        this->val = (*this->x->func)(this->x);
        return this->val;
    }
    else
    {
        this->val = 0.0 * (*this->x->func)(this->x);
        return this->val;
    }
}

float FloatVar_leaf(struct FloatVar *this)
{
    return this->val;
}

#endif