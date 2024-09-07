#include <stdio.h>
#include <stdlib.h>

#define MAX_DEP 100
typedef int dtype;

struct Stack {
	dtype data[MAX_DEP];
	int top;
};

void initStack(struct Stack *stack) {
	stack->top = -1;
}

int isEmpty(struct Stack *stack) {
	return stack->top == -1;
}

int isFull(struct Stack *stack) {
	return stack->top == MAX_DEP - 1;
}

void push(struct Stack *stack, dtype value) {
	if (isFull(stack)) {
		printf(" Stack is Full \n");
		return;
	}
	stack->data[++(stack->top)] = value;
	printf(" value %d has been pushed \n", value);
}

void pop(struct Stack *stack) {
    if (isEmpty(stack)) {
		printf(" Stack is empty, no way to pop \n");
		return;
    }
	(stack->top) --;
}

int top(struct Stack *stack) {
	if (isEmpty(stack)) {
		printf(" Stack is empty, top return 0 \n");
		return 0;
    }
    return stack->data[(stack->top)];
}

int main() {
    struct Stack stack;
    initStack(&stack);

    push(&stack, 10);
    push(&stack, 20);
    push(&stack, 30);

    printf("栈顶元素: %d\n", top(&stack));
	pop(&stack);
    printf("出栈元素: %d\n", top(&stack));
	pop(&stack);
    printf("出栈元素: %d\n", top(&stack));
	pop(&stack);
    printf("栈顶元素: %d\n", top(&stack));
	pop(&stack);

    return 0;
}
