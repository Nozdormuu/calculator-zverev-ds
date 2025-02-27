#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK_SIZE 65536
extern int flag_float;

typedef union {
    long int_val;
    double float_val;
} Number;

typedef struct {
    Number data[STACK_SIZE];
    int top;
} NumStack;

typedef struct {
    char data[STACK_SIZE];
    int top;
} OpStack;
void init_num(NumStack* s);
int is_empty_num(NumStack* s);
void push_num(NumStack* s, Number n);
Number pop_num(NumStack* s);

void init_op(OpStack* s);
int is_empty_op(OpStack* s);
void push_op(OpStack* s, char op);
char pop_op(OpStack* s);
char peek_op(OpStack* s);
int priority(char op);
Number apply_operation(Number a, Number b, char op);
int validate_input(const char* s);
Number evaluate(const char* s);
