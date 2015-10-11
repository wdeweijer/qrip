#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rip.h"

/*
Prime:
21W[D1W[1sSDD4RDD4r5rqms]P1EI[DO9io]i1]

Still to implement fully:
    does prime.rip work?
    n, g, F<bla>[code], <bla>

Ideas:
    decrease mallocs?
*/


char *prog;
void **jump_table;

void error(char *e) {
    printf("\n%s\n",e);
    exit(1);
}

// Clutch
char *build_jt_h(char *p, char *b) {
    char c;
    char *t;
    while(c=*(t=p++)) {
        if(c=='I' || c=='W') {
            while(isspace(*p)) p++;
            if(*p!='[') error("I or W not followed by code block. ");
            p=build_jt_h(p+1,t);
            if(!p) error("Brackets don't match. Wrong '['. ");
            *(t-prog+jump_table) = p-1;
        } else if(c==']') {
            *(t-prog+jump_table) = b;
            return p;
        } else if(c=='[') {
            error("Code block without I or W. ");
        } else if(isdigit(c)) {
            //printf(":%c\n",c);
            mpz_t *constant = malloc(sizeof(*constant));
            mpz_init_set_ui(*constant, c-'0');
            *(t-prog+jump_table) = constant;
        }
    }
    return NULL;
}

// jump on '[', ']'
void build_jump_table() {
    jump_table = malloc(strlen(prog)*sizeof(*jump_table));
    if(build_jt_h(prog,0)) error("Brackets don't match. Wrong ']'. ");
}

// DOES NOT FREE MPZ_T NUMBERS!!!
void delete_jt() {
    free(jump_table);
}

void interpret() {
    char *p = prog;
    while(*p) {
        /*printf(":%c:\t",*p);
        dump();
        printf("\n");*/
        switch(*p) {
        case 'P': op_pop();     break;
        case 'S': op_swap();    break;
        case 'D': op_dupl();    break;
        case 'i': op_inc();     break;
        case 'd': op_dec();     break;
        case 'r': op_r_lower(); break;
        case 'R': op_r_upper(); break;
        case 'a': op_add();     break;
        case 's': op_sub();     break;
        case 'm': op_mul();     break;
        case 'q': op_div();     break;
        case 'G': op_greater(); break;
        case 'L': op_less();    break;
        case 'E': op_eq();      break;
        case 'I':
        case 'W':
            if(!mpz_sgn(glob_t->data)) {
                p = jump_table[p-prog];
            }
            op_pop();
            break;
        case ']':
            if(*(char*)jump_table[p-prog]=='W') {
                p = jump_table[p-prog];
                continue;
            }
            break;
        case 'o': op_charout(); break;
        case 'O': op_numout();  break;
        case 'g': op_charin();  break;
        case '$': dump();       break;
        default:
            if(isspace(*p)) while(isspace(*p)) p++;
            if(isdigit(*p)) {
                list_node *temp = malloc(sizeof(*temp));
                mpz_init_set(temp->data,*(mpz_t*)jump_table[p-prog]);
                temp->down = glob_t;
                glob_t = temp;
            }
        }
        p++;
    }
}

int main(int argc, char **argv) {
    if(argc!=2) {
        printf("Pass a rip program.\n");
        return 0;
    }
    prog = argv[1];
    //prog = "21W[D1W[1sSDD4RDD4r5rqms]1EI[DO9io]i1]";
    //prog = "09s0LO9io$";

    build_jump_table();

    list_init();

    interpret();

    delete_list();

    delete_jt();

    exit(0);
}
