#ifndef CONSTANTS_H
#define CONSTANTS_H

// Базовые пути
#define DIR_TEMPLATES "./templates/"
#define DIR_CSS       "./css/"
#define DIR_LOGS      "./server/logs/"

// Количество HTML-шаблонов
#define TEMPLATES_NUMBER 4

// Количество CSS файлов
#define CSS_NUMBER 1

// Общее количество файлов, отправляемых клиенту
#define SENT_FILES_NUMBER TEMPLATES_NUMBER + CSS_NUMBER

// Пути к HTML-шаблонам
#define PATH_LOGIN_HTML    DIR_TEMPLATES "login.html"
#define PATH_SUCCESS_HTML  DIR_TEMPLATES "success.html"
#define PATH_ERROR_HTML    DIR_TEMPLATES "error.html"
#define PATH_SOLUTION_HTML DIR_TEMPLATES "solution.html"

// Путь к CSS
#define PATH_CSS_STYLES DIR_CSS "styles.css"

// Путь к файлу логов
#define PATH_LOG_FILE DIR_LOGS "log.log"

// Content-Type заголовки
#define CONTENT_TYPE_HTML "Content-Type: text/html; charset=utf-8\r\n"
#define CONTENT_TYPE_CSS  "Content-Type: text/css; charset=utf-8\r\n"

// Коды состояния HTTP
#define HTTP_STATUS_CODE_OK 200
#define HTTP_STATUS_CODE_UNAUTHORIZED 401
#define HTTP_STATUS_CODE_INTERNAL_SERVER_ERROR 500

// Константы необходимые для форматирования файла решения
// (жесткая связь с solution.html)
#define BYTES_PER_NUMBER 10
#define NUMBER_COUNT 18
#define CONVERSION_SPECIFICATION_LENGTH 2 // < BYTES_PER_NUMBER

#endif

