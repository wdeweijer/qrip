#ifndef RIP_H_INCLUDED
#define RIP_H_INCLUDED

#include <gmp.h>

typedef struct list_node {
    mpz_t data;
    struct list_node *down;
} list_node;

list_node *glob_t;

void error(char *);

void list_init();
list_node *new_list_node(list_node *);
void delete_top();
void delete_list();
void list_atleast_n_error(int);
void list_push(unsigned long);

void op_pop();
void op_dupl();
void op_swap();
void op_inc();
void op_dec();
void op_add();
void op_sub();
void op_mul();
void op_div();
void op_eq();
void op_greater();
void op_less();
void op_numout();
void op_charout();
void op_charin();
void op_r_lower();
void op_r_upper();
void dump();


#endif // RIP_H_INCLUDED
