#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "main.h"



int flag_float = 0;

union {
    long int_val;
    double float_val;
} typedef Number;

typedef struct {
    Number data[STACK_SIZE];
    int top;
} NumStack;

typedef struct {
    char data[STACK_SIZE];
    int top;
} OpStack;

void init_num(NumStack *s) { s->top = -1; }
int is_empty_num(NumStack *s) { return s->top == -1; }
void push_num(NumStack *s, Number n) { s->data[++s->top] = n; }
Number pop_num(NumStack *s) { return s->data[s->top--]; }

void init_op(OpStack *s) { s->top = -1; }
int is_empty_op(OpStack *s) { return s->top == -1; }
void push_op(OpStack *s, char op) { s->data[++s->top] = op; }
char pop_op(OpStack *s) { return s->data[s->top--]; }
char peek_op(OpStack *s) { return s->data[s->top]; }

int priority(char op) {
    return op == '+'|| op == '-' ? 1 : op == '*'||  op == '/' ? 2 : 0;
}

Number apply_operation(Number a, Number b, char op) {
    Number r = {0};
    if (flag_float) {
        double x = a.float_val, y = b.float_val;
        switch(op) {
            case '+': r.float_val = x + y; break;
            case '-': r.float_val = x - y; break;
            case '*': r.float_val = x * y; break;
            case '/': 
                if (y > -1e-4 && y < 1e-4) exit(1);
                r.float_val = x / y; break;
        }
    } else {
        long x = a.int_val, y = b.int_val;
        switch(op) {
            case '+': r.int_val = x + y; break;
            case '-': r.int_val = x - y; break;
            case '*': r.int_val = x * y; break;
            case '/': 
                if (!y) exit(1);
                r.int_val = x / y; break;
        }
    }
    return r;
}

int validate_input(const char *s) {
    int bal = 0, last_op = 1;
    for (int i = 0; s[i]; ++i) {
        if (isspace(s[i])) continue;
        
        if (isdigit(s[i])) {
            if (!last_op) return 0;
            while (isdigit(s[i])) ++i;
            --i;
            last_op = 0;
        }
        else if (s[i] == '(') {
            if (!last_op) return 0;
            ++bal;
            last_op = 1;
        }
        else if (s[i] == ')') {
            if (last_op || !bal--) return 0;
            last_op = 0;
        }
        else if (strchr("+-*/", s[i])) {
            
            if (last_op) return 0;
            last_op = 1;
        }
        else return 0;
    }
    return !last_op && !bal;
}

Number evaluate(const char *s) {
    NumStack ns; OpStack os;
    init_num(&ns); init_op(&os);

    for (int i = 0; s[i]; ++i) {
        if (isspace(s[i])) continue;
        if (isdigit(s[i])) {
            Number n = {0};
            long v = 0;
            while (isdigit(s[i])) v = v *10 + (s[i++]-'0');
            --i;
            if (flag_float) n.float_val = v;
            else n.int_val = v;
            push_num(&ns, n);
        }
        else if (s[i] == '(') {
            push_op(&os, '(');
        }
        else if (s[i] == ')') {
            while (peek_op(&os) != '(') {
                char op = pop_op(&os);
                Number b = pop_num(&ns), a = pop_num(&ns);
                push_num(&ns, apply_operation(a, b, op));
            }
            pop_op(&os);
        }
        else {
            while (!is_empty_op(&os) && priority(peek_op(&os)) >= priority(s[i])) {
                char op = pop_op(&os);
                Number b = pop_num(&ns), a = pop_num(&ns);
                push_num(&ns, apply_operation(a, b, op));
            }
            push_op(&os, s[i]);
        }
    }

    while (!is_empty_op(&os)) {
        char op = pop_op(&os);
        Number b = pop_num(&ns), a = pop_num(&ns);
        push_num(&ns, apply_operation(a, b, op));
    }

    return pop_num(&ns);
}

#ifndef GTEST
int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i)
        if (!strcmp(argv[i], "--float")) flag_float = 1;

    char input[STACK_SIZE];
    if (!fgets(input, sizeof(input), stdin)) return 1;
    input[strcspn(input, "\n")] = 0;

    if (!validate_input(input)) return 1;

    Number res = evaluate(input);
    flag_float ? printf("%.4f\n", res.float_val) : printf("%ld\n", res.int_val);

    return 0;
}
#endif
