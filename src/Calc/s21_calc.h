#ifndef SRC_CALC_S21_CALC_H_
#define SRC_CALC_S21_CALC_H_

    #ifdef __cplusplus
        extern "C" {
    #endif
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct t_node {
    double operand;     // число (операнд)
    int operation;  // операция (оператор)
    struct t_node *next;   // указатель на следующую структуру
} t_node;


// В порядке приоритета
#define NUM 0
#define DOT 1
#define COS 2
#define SIN 3
#define TAN 4
#define ACOS 5
#define ASIN 6
#define ATAN 7
#define SQRT 8
#define LN 9
#define LOG 10
#define POW 11
#define MULT 12
#define DIV 13
#define MOD 14
#define SUM 15
#define SUB 16
#define ERROR 18
#define OPEN 19
#define CLOSE 20

int get_calculated(char* array, double* result);
t_node* push(t_node* head, double operand, int operation);
void push_back(t_node* head, double operand, int operation);
t_node* get_last_node(t_node *head);
int pop(t_node** head, double* operand, int* operation);
int parse_array(char* array, t_node** head_stack, t_node **head_output);
void copy_num(const char* src_array, int* begin, int* dot_counter, char* num);
void copy_func(const char* src_array, int* begin, char* func);
int is_func(char* array);
void destroy_nodes(t_node** head);
int get_brackets(char* array);
void filter_element(double operand, int operation, t_node **head_stack, t_node **head_output);
int pop_stack(t_node** head);
int calc_precedence(int operation);
int write_operation(char sym, int *operation);
void clear_stack(t_node **head_stack, t_node **head_output);
int get_variable(char *array, char *ptmp, int *i, double *operand);
int get_function(char *array, int *i, char *ptmp, int *operation);
void add_variable_to_output(t_node **head_output, double operand, int operation);
int process_brackets(t_node **head_stack, t_node **head_output, double operand, int operation);
void process_precedence(t_node **head_stack, t_node **head_output, double operand, int operation);
int get_res(t_node** head_output, t_node** head_stack);
int get_result(t_node** head_output, t_node** head_stack, double* operand);

    #ifdef __cplusplus
        }
    #endif
#endif  // SRC_CALC_S21_CALC_H_
