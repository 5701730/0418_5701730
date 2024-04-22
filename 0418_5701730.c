#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> ///bool때문에 넣음

#define MAX_EXPRESSION_LENGTH 100

typedef int element;
typedef struct Stacktype {
    element* data;
    int capacity;
    int top;
} StackType;

void init(StackType* sptr, int ofs) {// 스택 초기화 함수

    sptr->data = (element*)malloc(sizeof(StackType) * ofs);
    sptr->top = -1;
    sptr->capacity = ofs;
}

int is_full(StackType* sptr) {// 스택이 가득 찼는지 확인하는 함수
    if (sptr->top == sptr->capacity - 1) {
        sptr->capacity *= 2;
        sptr->data = (element*)realloc(sptr->data, sptr->capacity * sizeof(element));
    }
    return 0;
}

int is_empty(StackType* sptr) {// 스택이 비어있는지 확인하는 함수
    return (sptr->top == -1);
}

void push(StackType* sptr, element item) {// 스택에 요소를 추가하는 함수
    if (is_full(sptr)) {
        fprintf(stderr, "Stack is full\n");
        return;
    }
    sptr->data[++sptr->top] = item;
}

element pop(StackType* sptr) {// 스택에서 요소를 제거하고 반환하는 함수
    if (is_empty(sptr)) {
        fprintf(stderr, "stack is empty\n");
        return -1;
    }
    return sptr->data[sptr->top--];
}

element peek(StackType* sptr) {// 스택의 top 요소를 반환하는 함수
    if (is_empty(sptr)) {
        fprintf(stderr, "stack is empty\n");
        return -1;
    }
    return sptr->data[sptr->top];
}

bool is_operator(char c) {//연산자 확인함수
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool is_operand(char c) {//피연산자 확인함수
    return (c >= '0' && c <= '9');
}

int prec(char op) {//연산자들 우선순위 반환하느 함수
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

void infix_to_postfix(char* infix, char* postfix) {//중위식->후위식
    StackType s;
    init(&s, 10);
    int i = 0, j = 0;
    while (infix[i] != '\0') {
        if (is_operand(infix[i]))
            postfix[j++] = infix[i];
        else if (infix[i] == '(')
            push(&s, infix[i]);
        else if (infix[i] == ')') {
            while (!is_empty(&s) && peek(&s) != '(')
                postfix[j++] = pop(&s);
            pop(&s); 
        }
        else if (is_operator(infix[i])) {
            while (!is_empty(&s) && prec(infix[i]) <= prec(peek(&s)))
                postfix[j++] = pop(&s);
            push(&s, infix[i]);
        }
        i++;
    }
    while (!is_empty(&s))
        postfix[j++] = pop(&s);
    postfix[j] = '\0'; 
}

int eval(char* expression) {
    StackType s;
    init(&s, 10);
    int i = 0;
    while (expression[i] != '\0') {
        if (is_operand(expression[i]))
            push(&s, expression[i] - '0');
        else if (is_operator(expression[i])) {
            int operand2 = pop(&s);
            int operand1 = pop(&s);
            switch (expression[i]) {
            case '+':
                push(&s, operand1 + operand2);
                break;
            case '-':
                push(&s, operand1 - operand2);
                break;
            case '*':
                push(&s, operand1 * operand2);
                break;
            case '/':
                push(&s, operand1 / operand2);
                break;
            }
        }
        i++;
    }
    return pop(&s);
}

int main(int argc, char* argv[]) {
    char infixExpression[MAX_EXPRESSION_LENGTH];
    char postfixExpression[MAX_EXPRESSION_LENGTH];

    while (1) {
        printf("다음과 같은 메뉴로 동작하는 프로그램입니다:\n");
        printf("1. 중위식을 입력 받음\n");
        printf("2. 중위식을 후위식으로 변환\n");
        printf("3. 후위식을 계산\n");
        printf("4. 종료\n");
        printf("메뉴를 선택하세요:");

        int choice;
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("중위식을 입력하세요: ");
            getchar(); //입력 버퍼를 비우기 위해서 getchar(); 호출
            fgets(infixExpression, sizeof(infixExpression), stdin);
            printf("입력된 중위식 : %s\n", infixExpression);
            break;
        case 2:
            infix_to_postfix(infixExpression, postfixExpression);
            printf("후위식 : %s\n", postfixExpression);
            break;
        case 3:
            printf("결과: %d\n", eval(postfixExpression));
            break;
        case 4:
            printf("프로그램을 종료합니다.\n");
            exit(0);
        default:
            printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
            break;
        }
    }
    return 0;
}
