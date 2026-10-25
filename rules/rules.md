# Правила проекта

Обязательны для всего кода, который мы пишем в этом репозитории (пакеты `ksivi_*`).
Сторонние пакеты (`robolaunch_*`, `sllidar_ros2`, `rf2o_laser_odometry`) не переформатируем
и не переименовываем — они вендорные.

Стиль — **Google C++ Style Guide**, версия языка — **C++17**.

| Документ | О чём |
|---|---|
| [`cpp-style.md`](cpp-style.md) | Имена, файлы, объявления, форматирование, проверка |
| [`testing.md`](testing.md) | Как пишем и запускаем тесты (GoogleTest) |

## Источник истины — конфиги, а не документ

| Что | Конфиг |
|---|---|
| Форматирование | [`.clang-format`](../.clang-format) (`BasedOnStyle: Google`) |
| Имена | [`.clang-tidy`](../.clang-tidy) (`readability-identifier-naming`) |

`rules/*.md` описывают эти конфиги человеческим языком. Если правило нужно поменять —
меняем конфиг **и** документ, иначе `clang-format`/`clang-tidy` начнут спорить с правилами.

## Базовый принцип

**Один стиль для кода и тестов.** Класс — `CamelCase` и в коде, и в `TEST(...)`. Функция и
тест-кейс — `CamelCase`. Переменная — `snake_case`.

## Автоматическая проверка

```bash
# форматирование (из корня репозитория)
clang-format --dry-run --Werror \
  ksivi_mecanum_control/include/**/*.hpp \
  ksivi_mecanum_control/src/*.cpp \
  ksivi_mecanum_control/test/*.cpp

# исправить
clang-format -i \
  ksivi_mecanum_control/include/**/*.hpp \
  ksivi_mecanum_control/src/*.cpp \
  ksivi_mecanum_control/test/*.cpp

# имена + остальные проверки
clang-tidy -p build/ksivi_mecanum_control \
  ksivi_mecanum_control/src/mecanum_inverse_kinematics.cpp
```
