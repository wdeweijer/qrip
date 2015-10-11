#include <stdio.h>
#include <stdlib.h>

#include "rip.h"

mpz_t const_0, const_1;

/* DONT FORGET TO CHECK FOR NULL!!! */
void list_init() {
    mpz_init_set_ui(const_0,0);
    mpz_init_set_ui(const_1,1);
    glob_t = NULL;
}

list_node *new_list_node(list_node *down) {
    list_node *res = malloc(sizeof(*res));
    //mpz_init_set(res->data, data);
    mpz_init(res->data);
    res->down = down;
    return res;
}

void delete_top() {
    if(glob_t!=NULL) {
        list_node *temp = glob_t;
        glob_t = glob_t->down;
        mpz_clear(temp->data);
        free(temp);
    }
}

void delete_list() {
    while(glob_t!=NULL) {   // remove duplicate check?
        delete_top();
    }
}

void list_atleast_n_error(int n) {
    list_node *mover = glob_t;
    while(--n>0)
        if((mover = mover->down)==NULL)
            error("Not enough elements on stack. ");
}

void list_push(unsigned long data) {
    list_node *res = malloc(sizeof(*res));
    mpz_init_set_ui(res->data, data);
    res->down = glob_t;
    glob_t = res;
}



void op_pop() {
    delete_top();
}

void op_dupl() {
    list_node *temp = glob_t;
    glob_t = new_list_node(glob_t);
    mpz_set(glob_t->data, temp->data);
}

void op_swap() {
    list_atleast_n_error(2);
    list_node *temp = glob_t->down;
    glob_t->down = glob_t->down->down;
    temp->down = glob_t;
    glob_t = temp;
}

void op_inc() {
    mpz_add_ui(glob_t->data, glob_t->data, 1);
}

void op_dec() {
    mpz_sub_ui(glob_t->data, glob_t->data, 1);
}

void op_add() {
    list_atleast_n_error(2);
    mpz_add(glob_t->down->data,glob_t->down->data,glob_t->data);
    delete_top();
}

void op_sub() {
    list_atleast_n_error(2);
    mpz_sub(glob_t->down->data,glob_t->down->data,glob_t->data);
    delete_top();
}

void op_mul() {
    list_atleast_n_error(2);
    mpz_mul(glob_t->down->data,glob_t->down->data,glob_t->data);
    delete_top();
}

void op_div() {
    list_atleast_n_error(2);
    mpz_tdiv_q(glob_t->down->data,glob_t->down->data,glob_t->data);
    delete_top();
}

void op_eq() {
    list_atleast_n_error(2);
    int r = mpz_cmp(glob_t->data,glob_t->down->data);
    delete_top();
    r ? mpz_set(glob_t->data,const_0) : mpz_set(glob_t->data,const_1);
}

void op_greater() {
    list_atleast_n_error(2);
    int r = mpz_cmp(glob_t->data,glob_t->down->data);
    delete_top();
    r==-1 ? mpz_set(glob_t->data,const_1) : mpz_set(glob_t->data,const_0);
}

void op_less() {
    list_atleast_n_error(2);
    int r = mpz_cmp(glob_t->data,glob_t->down->data);
    delete_top();
    r==1 ? mpz_set(glob_t->data,const_1) : mpz_set(glob_t->data,const_0);
}

void op_numout() {
    mpz_out_str(stdout,10,glob_t->data);
    op_pop();
}

void op_charout() {
    unsigned long v = mpz_get_ui(glob_t->data);
    printf("%c",(char)(v & 0xFF));
    op_pop();
}

void op_charin() {

}

/* Doesn't work on 0R and 1R! */
void op_r_lower() {
    int v = (int)mpz_get_ui(glob_t->data);
    op_pop();

    if(v==0 || v==1) return;
    if(v==2) op_swap();

    list_atleast_n_error(v);

    list_node *mover = glob_t;
    int i;
    for(i=1; i<v; i++) {
        mover = mover->down;
    }

    list_node *temp = glob_t->down;
    glob_t->down = mover->down;
    mover->down = glob_t;

    glob_t = temp;
}

/* Doesn't work on 0R and 1R! */
void op_r_upper() {
    int v = (int)mpz_get_ui(glob_t->data);
    op_pop();

    if(v==0 || v==1) return;
    if(v==2) op_swap();

    list_atleast_n_error(v);

    list_node *mover = glob_t;
    int i;
    for(i=2; i<v; i++) {
        mover = mover->down;
    }

    list_node *temp = mover->down;
    mover->down = mover->down->down;
    temp->down = glob_t;

    glob_t = temp;
}

void dump() {
    if(glob_t==NULL) {
        printf("empty");
        return;
    }
    list_node *mover = glob_t;
    do {
        mpz_out_str(stdout,10,mover->data);
        printf(", ");
    } while((mover = mover->down)!=NULL);
}
