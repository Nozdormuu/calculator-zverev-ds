#include <stdio.h>
#include <stdlib.h>
#include<string.h>


 struct stack {
   char** stack;
   int size;
} typedef Stack;


struct queue {
   char** queue;
   int size;
} typedef  Queue;



int intToStr(int num, char* buffer) {
   int i = 0;
   int isNegative = 0;
   if (num < 0) {
      isNegative = 1;
      num = -num;
   }
   do {
      buffer[i] = (char)('0' + (num % 10));
      i++;
      num /= 10;
   } while (num > 0);

   if (isNegative) {
      buffer[i] = '-';
      i++;
   }
   for (int j = 0; j < i / 2; j++) {
      char temp = buffer[j];
      buffer[j] = buffer[i - 1 - j];
      buffer[i - 1 - j] = temp;
   }
   buffer[i] = '\n';
   return i; 
}

int isDigit(char c) {
   return c >= '0' && c <= '9';
}

int len(char* str) {
   int i = 0;
   while (str[i] != '\n') {
      i++;
   }
   return i;
}

char* copy(char* real) {
   int ln = len(real);
   char* copy = (char*)malloc((ln + 1) * sizeof(char));
   for (int i = 0; i < ln; i++) {
      copy[i] = real[i];
   }
   copy[ln] = '\n';
   return copy;
}


void init(Stack* stack) {
   stack->stack = NULL;
   stack->size = 0;
}


void push(Stack* stackk, char* c) {
   char** newStack = (char**)malloc((stackk->size + 1) * sizeof(char*));
   newStack[0] = copy(c);

   for (int i = 0; i < stackk->size; i++) {
      newStack[i + 1] = stackk->stack[i];
   }
   free(stackk->stack);
   stackk->stack = newStack;
   stackk->size += 1;
}

char* pop(Stack* stackk) {
   if (stackk->size == 0) {
      return NULL;
   }
   char* c = stackk->stack[0];
   char** newStack = NULL;
   if (stackk->size - 1 > 0) {
      newStack = (char**)malloc((stackk->size - 1) * sizeof(char*));
      for (int i = 1; i < stackk->size; i++) {
         newStack[i - 1] = stackk->stack[i];
      }
   }
   free(stackk->stack);
   stackk->stack = newStack;
   stackk->size -= 1;
   return c;
}


void print(Stack* stack) {
   for (int i = 0; i < stack->size; i++) {
      printf("%s\n", stack->stack[i]);
   }
}

void initQ(Queue* queue) {
   queue->queue = NULL;
   queue->size = 0;
}

void pushQ(Queue* queue, char* token) {
   char** newQueue = (char**)malloc((queue->size + 1) * sizeof(char*));
   for (int i = 0; i < queue->size; i++) {
      newQueue[i] = queue->queue[i];
   }
   newQueue[queue->size] = copy(token);
   free(queue->queue);
   queue->queue = newQueue;
   queue->size += 1;
}

char* popQ(Queue* queue) {
   if (queue->size == 0) {
      return NULL;
   }   
  char* front = queue->queue[0];
  
   if (queue->size - 1 > 0) {
      char** newQueue = (char**)malloc((queue->size - 1) * sizeof(char*));
      for (int i = 1; i < queue->size; i++) {
         newQueue[i - 1] = queue->queue[i];
      }
      free(queue->queue);
      queue->queue = newQueue;
   }
   else {
      free(queue->queue);
      queue->queue = NULL;
   }
   queue->size -= 1;
   return front;
}

void printQ(Queue* queue) {
   for (int i = 0; i < queue->size; i++) {
      printf("%s ", queue->queue[i]);
   }
   printf("\n");
}

int prior(char* op) {
   if (op[0] == '+' || op[0] == '-') return 1;
   if (op[0] == '*' || op[0] == '/') return 2;
   return 0;
}

int isOp(char* token) {
   if (token == NULL) return 0;
   if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/') {
      return 1;
   }
   return 0;
}

int isLB(char* token) {
   return (token[0] == '(');
}

int isRB(char* token) {
   return (token[0] == ')');
}

int toInt(char* token) {
   int num = 0;
   int i = 0;
   int sign = 1;
   if (token[0] == '-') {
      sign = -1;
      i = 1;
   }
   for (; token[i] != '\n'; i++) {
      num = num * 10 + (token[i] - '0');
   }
   return num * sign;
}


void parse(char* input, Queue* outputTokens) {
   int i = 0;
   int start = 0;
   while (input[i] != '\n') {
      if (input[i] == ' ') {
         i++;
         continue;
      }
      
      if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' ||
         input[i] == '(' || input[i] == ')')
      {
         
         if (input[i] == '-' &&
            (i == 0 || input[i - 1] == '(' || input[i - 1] == '+' ||
               input[i - 1] == '-' || input[i - 1] == '*' || input[i - 1] == '/'))
         {
           
            start = i;
            i++;
            while (isDigit(input[i])) {
               i++;
            }
            int length = i - start;
            char* token = (char*)malloc((length + 1) * sizeof(char));
            for (int j = 0; j < length; j++) {
               token[j] = input[start + j];
            }
            token[length] = '\n';
            pushQ(outputTokens, token);
            free(token);
         }
         else {
            
            char op[2];
            op[0] = input[i];
            op[1] = '\n';
            pushQ(outputTokens, op);
            i++;
         }
      }
      else if (isDigit(input[i])) {
         start = i;
         while (isDigit(input[i])) {
            i++;
         }
         int length = i - start;
         char* token = (char*)malloc((length + 1) * sizeof(char));
         for (int j = 0; j < length; j++) {
            token[j] = input[start + j];
         }
         token[length] = '\n';
         pushQ(outputTokens, token);
         free(token);
      }
      else {
         i++;
      }
   }
}


void shuntingYard(Queue* inputTokens, Queue* outputQueue) {
   Stack operatorStack;
   init(&operatorStack);

   char* token = popQ(inputTokens);
   while (token != NULL) {
      if (isOp(token)) {
         while (operatorStack.size > 0) {
            char* topOp = operatorStack.stack[0];
            if (isOp(topOp) &&
               prior(topOp) >= prior(token))
            {
               pushQ(outputQueue, pop(&operatorStack));
            }
            else {
               break;
            }
         }         push(&operatorStack, token);
      }
      else if (isLB(token)) {
         push(&operatorStack, token);
      }
      else if (isRB(token)) {
         while (operatorStack.size > 0) {
            char* topOp = pop(&operatorStack);
            if (isLB(topOp)) {
               free(topOp); 
               topOp = NULL;
               break;
            }
            else {
               pushQ(outputQueue, topOp);
               free(topOp);
            }
         }
      }
      else {
         
         pushQ(outputQueue, token);
      }
      free(token);
      token = popQ(inputTokens);
   }
  
   while (operatorStack.size > 0) {
      char* topOp = pop(&operatorStack);
      if (isLB(topOp)) {
         free(topOp);
         continue;
      }
      pushQ(outputQueue, topOp);
      free(topOp);
   }
}


int calculate(Queue* postfixQueue) {
   Stack calcStack;
   init(&calcStack);

   char* token = popQ(postfixQueue);
   while (token != NULL) {
      if (isOp(token)) {

         char* val2 = pop(&calcStack);
         char* val1 = pop(&calcStack);
         if (val1 == NULL || val2 == NULL) {
            
            if (val1) { 
               free(val1); 
            }
            if (val2) {
               free(val2);
            } 
            free(token);
            return 0;
         }

         int operand1 = toInt(val1);
         int operand2 = toInt(val2);
         free(val1);
         free(val2);

         int result = 0;
         switch (token[0]) {
         case '+': result = operand1 + operand2; break;
         case '-': result = operand1 - operand2; break;
         case '*': result = operand1 * operand2; break;
         case '/':
            if (operand2 == 0) {
               
               free(token);
               return 0;
            }
            result = operand1 / operand2;
            break;
         }
         char buffer[256];
         intToStr(result, buffer);  
         push(&calcStack, buffer);
         
      }
      else {
         
         push(&calcStack, token);
      }
      free(token);
      token = popQ(postfixQueue);
   }
   char* finalVal = pop(&calcStack);
   int res = 0;
   if (finalVal != NULL) {
      res = toInt(finalVal);
      free(finalVal);
   }
   return res;
}


int main() {
    char* input = malloc(2);
    if (input == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    size_t size = 2;
    size_t slen = 0;

    while (1) {
        char buffer[2];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        size_t buffer_len = strlen(buffer); 
        if (slen + buffer_len >= size) {
            size *= 2;
            char* new_input = realloc(input, size);
            if (new_input == NULL) { 
                perror("Failed to reallocate memory");
                free(input);
                return 1;
            }
            input = new_input;
        }

        memcpy(input + slen, buffer, buffer_len);
        slen += buffer_len;
    }

    
    if (slen > 0 && input[slen - 1] == '\n') {
        slen--;
    }
    input[slen] = '\n'; 

    Queue inputTokens;
    initQ(&inputTokens);
    parse(input, &inputTokens);
    
    Queue postfixQueue;
    initQ(&postfixQueue);
    shuntingYard(&inputTokens, &postfixQueue);

    int result = calculate(&postfixQueue);
    printf("%d\n", result);

    while (inputTokens.size > 0) {
        char* tmp = popQ(&inputTokens);
        free(tmp);
    }
    while (postfixQueue.size > 0) {
        char* tmp = popQ(&postfixQueue);
        free(tmp);
    }
    free(input);
    return 0;
}


