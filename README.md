# int2023_t
Лабаратоная работа ИТМО, Основы программирования(ОП), 1 семестр

## Задача

  Реализовать пользовательский тип для целого знакового числа фиксированной длины **int2023_t**.
  Размер получившегося типа не должен превышать 253 байтa.

  Для вышеуказанного типа требуется реализовать следующий набор функций и операторов

     1. Конвертация из типа int32_t
     2. Конвертация из строки
     3. Сложение
     4. Вычитание
     5. Умножение
     6. Деление
     7. Вывод числа в поток (не обязательно в 10й системе счисления)

## Тесты
Проект содержит [тесты](https://github.com/MikhailLebedItmo/int2023_t/blob/main/tests/number_test.cpp), написанные с помощью фрэймворка GoogleTests. Также есть
workflow в Github Actions запускающий их при каждом push.

## Примечание
 - Переполнение - Undefined Behavior

## Ограничения
 - Запрещается использование стандартных контейнеров (std::vector, std::list и тд)
 - Запрещается использование std::bitset
