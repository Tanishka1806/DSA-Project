
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX 200

// Stack for numbers
double numStack[MAX];
int numTop = -1;

// Stack for operators
char opStack[MAX];
int opTop = -1;

void pushNum(double value) {
    if (numTop < MAX - 1)
        numStack[++numTop] = value;
}

double popNum() {
    if (numTop < 0) {
        printf("Error: Invalid expression!\n");
        exit(1);
    }
    return numStack[numTop--];
}

void pushOp(char op) {
    if (opTop < MAX - 1)
        opStack[++opTop] = op;
}

char popOp() {
    if (opTop < 0) {
        printf("Error: Invalid expression!\n");
        exit(1);
    }
    return opStack[opTop--];
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double calculate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                printf("Error: Division by zero!\n");
                exit(1);
            }
            return a / b;
        case '^': return pow(a, b);
        default:
            printf("Error: Unknown operator!\n");
            exit(1);
    }
}

void evaluateTop() {
    char op = popOp();
    double b = popNum();
    double a = popNum();
    pushNum(calculate(a, b, op));
}

int evaluate(char expr[]) {
    int i = 0;
    numTop = -1;
    opTop = -1;

    while (expr[i] != '\0') {
        if (isspace((unsigned char)expr[i])) {
            i++;
        }
        else if (isdigit((unsigned char)expr[i]) ||
                 expr[i] == '.') {
            char *end;
            double value = strtod(&expr[i], &end);

            if (end == &expr[i]) {
                printf("Error: Invalid number!\n");
                return 0;
            }

            pushNum(value);
            i = (int)(end - expr);
        }
        else if (expr[i] == '(') {
            pushOp(expr[i]);
            i++;
        }
        else if (expr[i] == ')') {
            while (opTop >= 0 && opStack[opTop] != '(')
                evaluateTop();

            if (opTop < 0) {
                printf("Error: Mismatched brackets!\n");
                return 0;
            }

            popOp();
            i++;
        }
        else if (strchr("+-*/^", expr[i]) != NULL) {
            char current = expr[i];

            // Handle unary minus, e.g. -5 or 3 * -2
            if (current == '-' &&
                (i == 0 || expr[i - 1] == '(' ||
                 strchr("+-*/^", expr[i - 1]) != NULL)) {
                pushNum(0);
            }

            while (opTop >= 0 &&
                   opStack[opTop] != '(' &&
                   (precedence(opStack[opTop]) >
                    precedence(current) ||
                    (precedence(opStack[opTop]) ==
                     precedence(current) && current != '^'))) {
                evaluateTop();
            }

            pushOp(current);
            i++;
        }
        else {
            printf("Error: Invalid character '%c'!\n", expr[i]);
            return 0;
        }
    }

    while (opTop >= 0) {
        if (opStack[opTop] == '(') {
            printf("Error: Mismatched brackets!\n");
            return 0;
        }
        evaluateTop();
    }

    if (numTop != 0) {
        printf("Error: Invalid expression!\n");
        return 0;
    }

    printf("Result = %.10g\n", numStack[numTop]);
    return 1;
}

int main() {
    char expression[MAX];

    printf("====================================\n");
    printf(" EXPRESSION EVALUATOR & CALCULATOR\n");
    printf("          Using Stack in C\n");
    printf("====================================\n");
    printf("Operators: +  -  *  /  ^ and brackets\n");
    printf("Type exit to close the calculator.\n");

    while (1) {
        printf("\nEnter expression: ");

        if (fgets(expression, sizeof(expression), stdin) == NULL)
            break;

        expression[strcspn(expression, "\n")] = '\0';

        if (strcmp(expression, "exit") == 0)
            break;

        if (strlen(expression) == 0) {
            printf("Please enter an expression.\n");
            continue;
        }

        evaluate(expression);
    }

    printf("\nCalculator closed.\n");
    return 0;
}

