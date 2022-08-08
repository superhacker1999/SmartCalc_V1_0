#include "s21_calc.h"

int get_calculated(char* array, double* result) {
    int calc_result = 0;
    t_node* head_stack = NULL;
    t_node* head_output = NULL;
    for (int i = 0; array[i]; i++) {  // чтобы избежать ошибок SIN != sin
       array[i] = tolower(array[i]);
    }
    if (get_brackets(array) || parse_array(array, &head_stack, &head_output)) {
        destroy_nodes(&head_output);  // ошибка ввода
        destroy_nodes(&head_stack);
        calc_result = 1;
    } else {
        // printf("%lf", get_result(&head_output, &head_stack));
        if (get_result(&head_output, &head_stack, result) == 1) {
            calc_result = 1;
        }
        destroy_nodes(&head_stack);
        destroy_nodes(&head_output);
    }
    return calc_result;
}

// функция парсинга,
// возвращает 0 - ок, 1 - ошибка ввода
int parse_array(char* array, t_node** head_stack, t_node **head_output) {
    int parse_res = 0;
    if (array) {
        int i = 0, operation = 0;
        char* ptmp = malloc(sizeof(char) * 256);
        double operand = 0;
        while (array[i]) {
            memset(ptmp, '\0', 256);
            if (array[i] == ' ') {
                i++;
                continue;
            } else if (array[i] >= '0' && array[i] <= '9') {
                if (get_variable(array, ptmp, &i, &operand) > 1) {  // too many dots
                    parse_res = 1;
                    break;
                }
                i--;
            } else if (array[i] >= 'a' && array[i] <= 'z') {
                if (get_function(array, &i, ptmp, &operation) == ERROR) {
                    parse_res = 1;
                    break;
                }
            } else if (write_operation(array[i], &operation)) {
                parse_res = 1;
                break;
            }
            i++;
            filter_element(operand, operation, head_stack, head_output);
            operand = 0;
            operation = 0;
        }
        clear_stack(head_stack, head_output);
        free(ptmp);
    }
    return parse_res;
}

int get_function(char *array, int *i, char *ptmp, int *operation) {
    copy_func(array, i, ptmp);
    *operation = is_func(ptmp);
    *i = *i - 1;
    return *operation;
}

int get_variable(char *array, char *ptmp, int *i, double *operand) {
    int dot_counter = 0;
    copy_num(array, i, &dot_counter, ptmp);
    *operand = atof(ptmp);
    return dot_counter;
}

int write_operation(char sym, int *operation) {
    int error = 0;
    if (sym == '+') {
        *operation = SUM;
    } else if (sym == '-') {
        *operation = SUB;
    } else if (sym == '*') {
        *operation = MULT;
    } else if (sym == '/') {
        *operation = DIV;
    } else if (sym == '^') {
        *operation = POW;
    } else if (sym == '(') {
        *operation = OPEN;
    } else if (sym == ')') {
        *operation = CLOSE;
    } else {
        error = 1;
    }
    return error;
}

void clear_stack(t_node **head_stack, t_node **head_output) {
    if (*head_stack != NULL) {
        while (*head_stack != NULL) {
            if (*head_output == NULL) {
                *head_output = push(*head_output, (*head_stack)->operand, (*head_stack)->operation);
            } else {
               push_back(*head_output, (*head_stack)->operand, (*head_stack)->operation);
            }
            pop_stack(head_stack);
        }
    }
}


void filter_element(double operand, int operation, t_node **head_stack, t_node **head_output) {
    if (operation == NUM) {  // если число или переменная
        add_variable_to_output(head_output, operand, operation);
    } else {  // если операция
        if (*head_stack == NULL) {  // если стек пустой
            *head_stack = push(*head_stack, operand, operation);
        } else {
            if (!process_brackets(head_stack, head_output, operand, operation)) {
                process_precedence(head_stack, head_output, operand, operation);
            }
        }
    }
}

int process_brackets(t_node **head_stack, t_node **head_output, double operand, int operation) {
    int is_bracket = 0;
    if (operation == OPEN) {  // откр скобка
        *head_stack = push(*head_stack, operand, operation);
        is_bracket = 1;
    } else if (operation == CLOSE) {  // закр скобка
        while (*head_stack != NULL && (*head_stack)->operation != 19) {
            if (*head_output == NULL) {
                *head_output = push(*head_output, (*head_stack)->operand, (*head_stack)->operation);
            } else {
                push_back(*head_output, (*head_stack)->operand, (*head_stack)->operation);
            }
            pop_stack(head_stack);
        }
        pop_stack(head_stack);
        is_bracket = 1;
    }
    return is_bracket;
}

void process_precedence(t_node **head_stack, t_node **head_output, double operand, int operation) {
    // приоритетность операции выше, чем приоритетность опреации на вершине стека
    if (calc_precedence(operation) > calc_precedence((*head_stack)->operation)) {
        *head_stack = push(*head_stack, operand, operation);
        // приоритетность меньше
    } else if (calc_precedence(operation) < calc_precedence((*head_stack)->operation)) {
        while (*head_stack != NULL &&
        calc_precedence(operation) < calc_precedence((*head_stack)->operation)) {
            if (*head_output == NULL) {
                *head_output = push(*head_output, (*head_stack)->operand, (*head_stack)->operation);
            } else {
                push_back(*head_output, (*head_stack)->operand, (*head_stack)->operation);
            }
            pop_stack(head_stack);
        }
        *head_stack = push(*head_stack, operand, operation);
    } else {  // одинаковая приоритетность
        if (operation == MOD) {  // взятие остатка кладем в стек
            *head_stack = push(*head_stack, operand, operation);
        } else {
            if (*head_output == NULL) {
                *head_output = push(*head_output, operand, (*head_stack)->operation);
            } else {
                push_back(*head_output, operand, (*head_stack)->operation);
            }
            pop_stack(head_stack);
            *head_stack = push(*head_stack, operand, operation);
        }
    }
}

void add_variable_to_output(t_node **head_output, double operand, int operation) {
        if (*head_output == NULL) {
            *head_output = push(*head_output, operand, operation);
        } else {
            push_back(*head_output, operand, operation);
        }
}

// 0 - ok, 1 - цепи закончились
// дергает данные из цепочки и чистит текущую структуру
// новая голова - next
int pop_stack(t_node** head) {
    int pop_res = 0;
    t_node* prev = NULL;
    if (*head != NULL) {  // если эта цепь существует
        prev = *head;  // запоминаем текущую цепь (голову)
        *head = prev->next;  // новая голова теперь следующая цепь
        free(prev);  // очищаем текущую цепь
    } else {
        pop_res = 1;
    }
    return pop_res;
}


int calc_precedence(int operation) {
    int prec = 0;
    if (operation >= 2 && operation <= 10) {
        prec = 4;
    } else if (operation == 11) {
        prec = 3;
    } else if (operation >= 12 && operation <= 14) {
        prec = 2;
    } else if (operation >= 15 && operation <= 16) {
        prec = 1;
    }
    return prec;
}

t_node* push(t_node* head, double operand, int operation) {
    t_node* tmp = (t_node*)malloc(sizeof(t_node));  // выделяем память под новую цепь
    tmp->operand = operand;  // инициализируем ее
    tmp->operation = operation;
    if (head == NULL) {
        head = tmp;
        tmp->next = NULL;
    } else {
        tmp->next = head;
        head = tmp;
    }
    return head;
}

t_node* get_last_node(t_node *head) {
    if (head == NULL) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return head;
}

void push_back(t_node* head, double operand, int operation) {
    t_node *last = get_last_node(head);
    t_node *tmp = (t_node*) malloc(sizeof(t_node));
    tmp->operand = operand;
    tmp->operation = operation;
    tmp->next = NULL;
    last->next = tmp;
}

// 0 - ok, 1 - цепи закончились
// дергает данные из цепочки и чистит текущую структуру
// новая голова - next
int pop(t_node** head, double* operand, int* operation) {
    int pop_res = 0;
    t_node* prev = NULL;
    if (*head != NULL) {  // если эта цепь существует
        prev = *head;  // запоминаем текущую цепь (голову)
        *operand = prev->operand;  // вытаскиваем значения цепи
        *operation = prev->operation;
        *head = prev->next;  // новая голова теперь следующая цепь
        free(prev);  // очищаем текущую цепь
    } else {
        pop_res = 1;
    }
    return pop_res;
}

// отчистка всех цепей в случае некорректного ввода
void destroy_nodes(t_node** head) {
    while (*head != NULL) {
        t_node* prev = NULL;
        prev = *head;  // запоминаем текущую цепь (голову)
        *head = prev->next;  // новая голова теперь следующая цепь
        free(prev);  // очищаем текущую цепь
    }
}

// копирует число в отдельную строку
void copy_num(const char* src_array, int* begin, int* dot_counter, char* num) {
    if (src_array) {
        int i = 0;
        while ((src_array[*begin] >= '0' && src_array[*begin] <= '9')
        || src_array[*begin] == '.') {
            if (src_array[*begin] == '.') *dot_counter += 1;
            num[i] = src_array[*begin];
            *begin += 1;
            i++;
        }
    }
}

void copy_func(const char* src_array, int* begin, char* func) {
    if (src_array) {
        int i = 0;
        while (src_array[*begin] >= 'a' && src_array[*begin] <= 'z') {
            func[i] = src_array[*begin];
            *begin += 1;
            i++;
        }
    }
}

int is_func(char* array) {
    int func_res = 0;
    if (array) {
        if (!strcmp(array, "sin")) {
            func_res = SIN;
        } else if (!strcmp(array, "cos")) {
            func_res = COS;
        } else if (!strcmp(array, "tan")) {
            func_res = TAN;
        } else if (!strcmp(array, "acos")) {
            func_res = ACOS;
        } else if (!strcmp(array, "asin")) {
            func_res = ASIN;
        } else if (!strcmp(array, "atan")) {
            func_res = ATAN;
        } else if (!strcmp(array, "sqrt")) {
            func_res = SQRT;
        } else if (!strcmp(array, "ln")) {
            func_res = LN;
        } else if (!strcmp(array, "log")) {
            func_res = LOG;
        } else if (!strcmp(array, "mod")) {
            func_res = MOD;
        } else {
            func_res = ERROR;
        }
    }
    return func_res;
}

// returns 1 if count of '(' not equal to ')'
int get_brackets(char* array) {
    int open_counter = 0;
    int close_counter = 0;
    for (int i = 0; i < (int)strlen(array); i++) {
        if (array[i] == '(') open_counter++;
        if (array[i] == ')') close_counter++;
    }
    return (open_counter == close_counter) ? 0 : 1;
}

int get_result(t_node** head_output, t_node** head_stack, double* operand) {
    int operation = 0;
    int one_number = 0;
    int result = 0;
    if ((*head_output)->next == NULL && (*head_output)->operation == NUM) {
        one_number = 1;
    }
    if ((*head_output)->operation != NUM) {
        result = 1;
        destroy_nodes(head_output);
    } else {
        while (1) {
            if ((*head_output)->operation == NUM) {
                pop(head_output, operand, &operation);
                *head_stack = push(*head_stack, *operand, operation);
                if (one_number == 1) {
                    break;
                }
            } else if ((*head_output)->operation > 0) {
                if (get_res(head_output, head_stack) == 1) {
                    break;
                }
            }
        }
        pop(head_output, operand, &operation);
    }
    return result;
}

int get_res(t_node** head_output, t_node** head_stack) {
    int null = 0;
    double res = 0;
    double num1 = 0;
    double num2 = 0;
    int operation = 0;
    if ((*head_output)->operation == COS) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = cos(num1);
    } else if ((*head_output)->operation == SIN) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = sin(num1);
    } else if ((*head_output)->operation == TAN) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = tan(num1);
    } else if ((*head_output)->operation == ACOS) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = acos(num1);
    } else if ((*head_output)->operation == ASIN) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = asin(num1);
    } else if ((*head_output)->operation == ATAN) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = atan(num1);
    } else if ((*head_output)->operation == SQRT) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = sqrt(num1);
    } else if ((*head_output)->operation == LN) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = log(num1);
    } else if ((*head_output)->operation == LOG) {
        pop(head_stack, &num1, &operation);
        pop(head_output, &res, &operation);
        res = log10(num1);
    } else if ((*head_output)->operation == POW) {
        pop(head_stack, &num1, &operation);
        pop(head_stack, &num2, &operation);
        pop(head_output, &res, &operation);
        res = pow(num2, num1);
    } else if ((*head_output)->operation == MULT) {
        pop(head_stack, &num1, &operation);
        pop(head_stack, &num2, &operation);
        pop(head_output, &res, &operation);
        res = num2 * num1;
    } else if ((*head_output)->operation == DIV) {
        pop(head_stack, &num1, &operation);
        pop(head_stack, &num2, &operation);
        pop(head_output, &res, &operation);
        res = num2 / num1;
    } else if ((*head_output)->operation == MOD) {
        pop(head_stack, &num1, &operation);
        pop(head_stack, &num2, &operation);
        pop(head_output, &res, &operation);
        res = fmod(num2, num1);
    } else if ((*head_output)->operation == SUM) {
        pop(head_stack, &num1, &operation);
        pop(head_stack, &num2, &operation);
        pop(head_output, &res, &operation);
        res = num2 + num1;
    } else if ((*head_output)->operation == SUB) {
        pop(head_stack, &num1, &operation);
        pop(head_stack, &num2, &operation);
        pop(head_output, &res, &operation);
        res = num2 - num1;
    }
    if (*head_output == NULL) null = 1;
    *head_output = push(*head_output, res, 0);
    return null;
}
