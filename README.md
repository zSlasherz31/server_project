# Название проекта
Простое UI-веб-приложение на языке Си с использованием библиотеки Mongoose.

## Использование
```
git clone https://github.com/zSlasherz31/server_project.git
cd server_project
echo "LOGIN_USER=admin LOGIN_PASS=1234 ./server/server.out" > run.sh
make run
```

Опционально можно установить Sass через пакетный менеджер вашей ОС для
более удобной работы с CSS (готовый CSS уже скомпилирован для Вас).

## Make цели
Собрать проект: ```make```

| Цель | Описание |
| --- | ---- |
| run | Запуск проекта |
| clean | Очистка проекта |
| rebuild | Пересобрать проект |

