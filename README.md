# calculator-zverev-ds

Программа на языке C для вычисления арифметических выражений с целыми числами, операциями `+`, `-`, `*`, `/` и скобками. Использует алгоритм **Shunting Yard** для преобразования в постфиксную запись и последующего вычисления.

## 📋 Особенности
- Поддерживаемые операции: `+`, `-`, `*`, `/`.
- Обработка скобок `()` для изменения приоритета операций.
- Считывает до `EOF`
- If you delite bolshoe chiclo na malenkoe - to vi debil i vse govno
- Vse probeli are allowed
- moshno use --float to float arithmetic no vvod only int

## 🚀 Установка и запуск
1. **Скомпилируйте программу**:
   ```bash
   gcc  main.c -o calculator.exe
   
2. **Запустите программу**:
   ```bash
   ./calculator.exe
   
3. **Введите выражение**:
   - Введите выражение и нажмите `Ctrl+D`  для завершения ввода.
   - Пример: `(5 - 3) * 4 + 10 / 2`

4. **Результат**:
   - Программа выведет результат или `0` при ошибке (например, деление на ноль).

## 📚 Зависимости
- Компилятор C (например, `gcc` или `clang`).
- Стандартные библиотеки: `<stdio.h>`, `<stdlib.h>`, `<string.h>

## 🔧 Детали реализации

### Структуры данных
- **Стек (`Stack`)**:
  - Управление операторами при преобразовании в постфиксную форму.
  - Используется для вычисления результата из постфиксной записи.


### Алгоритм Shunting Yard

```C

while (есть токены) {
  if (токен — число) → добавить в выходную очередь;
  else if (токен — оператор) {
    while (верх стека — оператор с >= приоритетом) {
      вытолкнуть оператор из стека в выходную очередь;
    }
    добавить токен в стек;
  }
  else if (токен — '(') → добавить в стек;
  else if (токен — ')') {
    вытолкнуть все операторы из стека до '(';
    удалить '(' из стека;
  }
}
```
###  Вычисление постфиксной записи

```C

int calculate(Queue* postfix) {
  Stack stack;
  init(&stack);

  while (postfix->size > 0) {
    char* token = popQ(postfix);
    if (is_number(token)) {
      push(&stack, token);
    } else {
      int b = toInt(pop(&stack));
      int a = toInt(pop(&stack));
      switch (token[0]) {
        case '+': push(&stack, intToStr(a + b)); break;
        case '-': push(&stack, intToStr(a - b)); break;
        case '*': push(&stack, intToStr(a * b)); break;
        case '/': 
          if (b == 0) return 0; // Ошибка деления на ноль
          push(&stack, intToStr(a / b)); 
          break;
      }
    }
  }
  return toInt(pop(&stack));
}
```
### 📖 Примеры
```
Пример 1: Приоритет операций
Ввод: 5 + 3 * 2   
Результат: 11

Пример 2: Ошибка деления на ноль
Ввод: 10 / (5 - 5)  
Результат: 0
```


### ⚠️ Ограничения
1. **Только целые числа**: `5 / 2 = 2` (дробная часть отбрасывается).
2. **Нет валидации ввода**: `2 + + 3` → неопределенное поведение.
3. **Переполнение целых чисел**: `999999999 * 999999999` → результат некорректен.
