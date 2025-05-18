#include "../libs/mongoose/mongoose.h"
// Подключение библиотеки Mongoose для реализации встроенного HTTP-сервера
#include "../include/input/input.h"
// Подключение собственной функции read_file() для чтения файлов
#include "../include/output/output.h"
// Подключение собственных функций вывода
#include "../include/constants.h"
// Подключение заголовка с определениями различных констант
#include "../include/cache/cache.h"
// Подключение функций для кеширования файлов
#include <stdlib.h>
#include <signal.h>
#include <unistd.h> // Работает только на Linux

// Определение кодов ошибок для обработки различных ситуаций
enum {
  ERR_OK                  =  0,
  // Ошибки нет, всё прошло успешно
  ERR_MISSING_ENV         = -1,
  // Не установлены переменные окружения LOGIN_USER или LOGIN_PASS
  ERR_INVALID_CREDENTIALS = -2,
  // Неверное имя пользователя или пароль
  ERR_FILE_NOT_FOUND      = -3
  // Не удалось открыть нужный файл
};

// Определение глобальной переменной для хранения номера сигнала
volatile sig_atomic_t signal_status = 0;

// Объявление массива для хранения указателей на данные
// кешируемых файлов (имя файла и его содержимое)
CachedFile cache[SENT_FILES_NUMBER];

// Хендлер для обработки сигнала
void signal_handler(int signal) {
  signal_status = signal;
}

// Вспомогательная функция заполнения вариадических аргументов
void fill_va(int* va, size_t size_va, int number) {
  size_t arg = 0, mul = 1;
  while (arg < size_va) {
    va[arg] = number;
    va[arg + 1] = number * mul;

    ++mul;
    arg += 2;
  }
}

// Функция обработки входящих HTTP-запросов
static int process_request(struct mg_connection *c,
                           struct mg_http_message *hm) {
  
  int status_code          = HTTP_STATUS_CODE_INTERNAL_SERVER_ERROR;
  // Код состояния по умолчанию — внутренняя ошибка сервера
  const char *ctype        = "";
  // Тип содержимого (Content-Type) будет выбран позже
  char *response           = NULL;
  // Ответ, который будет отправлен клиенту
  char *formatted_response = NULL;
  // Отформатированный ответ решения, который будет отправлен клиенту
  int error_code           = ERR_FILE_NOT_FOUND;
  // Ошибка по умолчанию — файл не найден
  
  // Проверка: если пришёл POST-запрос на /login
  if (!mg_strcmp(hm->uri, mg_str(URI_LOGIN)) &&
      !mg_strcasecmp(hm->method, mg_str("POST"))) {

    const char *expected_user = getenv("LOGIN_USER");
    // Получение ожидаемого имени из переменной окружения
    const char *expected_pass = getenv("LOGIN_PASS");
    // Получение ожидаемого пароля из переменной окружения
    
    if (!expected_user || !expected_pass)
      
      error_code = ERR_MISSING_ENV;
      // Если переменные окружения не заданы — ошибка
    
    else {

      char username[100], password[100];
      // Буферы для хранения введённого имени пользователя и пароля
      mg_http_get_var(&hm->body, "username", username, sizeof(username));
      mg_http_get_var(&hm->body, "password", password, sizeof(password));
      // Извлечение значений из тела POST-запроса
      
      // Проверка введённых данных
      if (!strcmp(username, expected_user) &&
          !strcmp(password, expected_pass)) {
        
        response = cache_file(cache, SENT_FILES_NUMBER,
                              PATH_SUCCESS_HTML, read_file);
        // Если всё верно — читаем HTML с приветствием
        status_code = HTTP_STATUS_CODE_OK;
        error_code  = ERR_OK;

      }

      else {

        response = cache_file(cache, SENT_FILES_NUMBER,
                              PATH_ERROR_HTML, read_file);
        // Иначе — HTML с ошибкой
        status_code = HTTP_STATUS_CODE_UNAUTHORIZED;
        error_code  = ERR_INVALID_CREDENTIALS;
      
      }

      // Если файл успешно прочитан —
      // подготавливаем оставшийся ответ
      if (response)
        ctype = CONTENT_TYPE_HTML;
      else {

        // Иначе — ответ по умолчанию
        status_code = HTTP_STATUS_CODE_INTERNAL_SERVER_ERROR;
        ctype       = "";
        error_code  = ERR_FILE_NOT_FOUND;
      
      }
    }
  }
  // Проверка: если пришёл POST-запрос на /solution
  else if (!mg_strcmp(hm->uri, mg_str(URI_SOLUTION)) &&
           !mg_strcasecmp(hm->method, mg_str("POST"))) {
    
    // Читаем неформатированный (сырой) файл решения
    response = cache_file(cache, SENT_FILES_NUMBER,
                          PATH_SOLUTION_HTML, read_file);
    
    if (response) {
      
      char number[BYTES_PER_NUMBER];
      // Буфер для хранения числа
      mg_http_get_var(&hm->body, "number", number, sizeof(number));
      // Извлечение значения из тела POST-запроса
      
      int va[NUMBER_COUNT];
      // Вариадические аргументы для
      // форматирования файла решения
      size_t size_va = sizeof(va) / sizeof(va[0]);
      int numeric_number = atoi(number);
      // Интерпретирование целочисленного
      // значения в байтовой строке number
      
      // Заполнение вариадических аргументов
      fill_va(va, size_va, numeric_number);

      formatted_response = format_response(response, va);
      // Формируем отформатированный файл решения
      
      if (formatted_response) {
        status_code = HTTP_STATUS_CODE_OK;
        ctype       = CONTENT_TYPE_HTML;
        error_code  = ERR_OK;
      }
    }
  }
  // Если запрашивается файл стилей
  else if (!mg_strcmp(hm->uri, mg_str(URI_STYLES_CSS))) {
    
    response = cache_file(cache, SENT_FILES_NUMBER,
                          PATH_CSS_STYLES, read_file); // Читаем файл CSS
    if (response) {
      status_code = HTTP_STATUS_CODE_OK;
      ctype       = CONTENT_TYPE_CSS;
      error_code  = ERR_OK;
    }
  }
  // Если запрашивается что-либо другое — возвращаем login.html
  else {

    response = cache_file(cache, SENT_FILES_NUMBER,
                          PATH_LOGIN_HTML, read_file);
    // Загружаем страницу логина
    if (response) {
      status_code = HTTP_STATUS_CODE_OK;
      ctype       = CONTENT_TYPE_HTML;
      error_code  = ERR_OK;
    }
  }
  
  // Если не было ошибок сервера — отправляем ответ клиенту
  if (error_code == ERR_INVALID_CREDENTIALS ||
      error_code == ERR_OK)
    mg_http_reply(c, status_code, ctype, "%s",
                  formatted_response ? formatted_response : response);
  else
    mg_http_reply(c, status_code, "", "");
    // В случае внутренней ошибки сервера —
    // пустой ответ с кодом 500

  free(formatted_response);
  // Освобождение выделенной памяти
  // под отформатированный файл решения
  return error_code;
  // Возврат кода ошибки
}

// Основная функция-обработчик событий сервера
static void main_fun(struct mg_connection* c,
                                       int ev,
                                     void* ev_data) {
  
  // Если произошло событие HTTP-запроса
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message* hm = (struct mg_http_message*)ev_data;
    // Преобразуем данные события к нужному типу
    
    output_to_file(PATH_LOG_FILE, "Результат обработки: %d\n",
                   process_request(c, hm));
    // Передаём запрос в обработчик и выполняем
    // логирование результата обработки в файл
  }
}

// Точка входа в программу
int main() {
  const char *server_address = SERVER_ADDRESS;
  // Адрес, по которому будет запущен сервер

  struct mg_mgr mgr; // Контекст менеджера соединений
  
  // Назначаем сигналы, на которые будет реагировать хендлер
  signal(SIGINT, signal_handler); 
  // Сигнал SIGINT, когда пользователь набирает специальный
  // символ INTR (с помощью сочетания клавиш Ctrl-c в терминале)
  signal(SIGTERM, signal_handler);
  // Сигнал SIGTERM, который по умолчанию отправляет команда kill 
  
  init_cache(cache, SENT_FILES_NUMBER);
  // Инициализация кеша для HTML-шаблонов

  mg_mgr_init(&mgr); // Инициализация менеджера
  mg_http_listen(&mgr, server_address, main_fun, NULL);
  // Запуск сервера на указанном адресе и обработчиком событий
  
  output_to_stdout("Сервер запущен на %s\n\n", server_address);
  // Вывод сообщения в консоль
  
  MG_INFO(("\nPID процесса: %d\n", getpid()));
  // Вывод PID процесса в стандартный поток вывода

  while (signal_status == 0) mg_mgr_poll(&mgr, 1000);
  // Основной цикл обработки событий с интервалом 1000 мс
  
  mg_mgr_free(&mgr);
  clear_cache(cache, SENT_FILES_NUMBER);
  // Очистка ресурсов перед завершением
  MG_INFO(("\nВыход по сигналу %d", signal_status));
  // Выводим в стандартный поток вывода номер сигнала,
  // который был отправлен процессу (SIGINT или SIGTERM)

  return 0;
}

