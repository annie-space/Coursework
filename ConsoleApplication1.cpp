#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <conio.h>
#include <list>
#include <Windows.h>
#include <limits>
using namespace std;

struct Airport {
    int id;
    char name[20];
    char brand[10];
    int expensesFlight;
    int passengers;
};
struct my_list {
    Airport data;
    struct my_list* prev;
    struct my_list* next;
};

const int MAXSTUDENTSONPAGE = 15;

struct Airport read_data(FILE* f);
struct my_list* add_first(struct my_list* beg, struct Airport d);
struct my_list* add_last(struct my_list* beg, struct Airport d);
struct my_list* create_my_list(struct my_list* beg, FILE* f);
struct my_list* delete_at_id(struct my_list* beg, int deleteId);
void update_flight(struct my_list* beg, int updateId);
void sort_my_list(struct my_list* beg, int criteria);
void save_my_list(struct my_list* beg, const char* filename);
struct Airport create_by_file(FILE* f);
void search_my_list(struct my_list* beg, int criteria);
void calculate_cost(struct my_list* beg);

//------СКРОЛЛИНГ------//
void ShowTablePart(struct my_list* beg);
int CountStudents(struct my_list* beg);
void ShowPartStudents(struct my_list* beg, int page);



int main() {
    struct my_list* beg = NULL;
    system("title Курсовая");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE* f;
    int c, i = 1;
    int choice;
    char filename[15];

    cout << "Вы хотети задать имя файла? Да - 1. Нет - нажмите любую клавишу." << endl;
    cin >> choice;
    system("cls");//очистка консоли 
    
    if (choice == 1) {
        cout << "Введите имя файла: ";
        cin >> filename;
        f = fopen(filename, "rb+");
        if (!f) {
            f = fopen(filename, "wb+");
            if (f == nullptr) {
                cout << "Ошибка открытия файла";
                exit(1);
            }
        }

    }
    else {
        strcpy(filename, "file.dat");
        f = fopen(filename, "rb+");
        if (!f) {
            f = fopen(filename, "wb+");
            if (f == nullptr) {
                cout << "Ошибка открытия файла";
                exit(1);
            }
        }

    }

    struct Airport d;
    fseek(f, 0, SEEK_END);
    long v = ftell(f) / sizeof(struct Airport);
   
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    string print_main[12] = { "Меню:","1. Создание таблицы","2. Просмотр рейсов","3. Добавить рейс","4. Удалить рейс",
                            "5. Корректировка рейсов","6. Сортировка","7. Сохранение таблицы","8. Загрузка данных",
                            "9. Поиск по...","10. Обработка данных","11. ВЫХОД"
    };



    int int_print_main = 12;
    while (true) {
        do {
            for (int j = 0; j < int_print_main; j++) {
                if (j == i) {
                    SetConsoleTextAttribute(hConsole, 10);
                    cout << " -> " << print_main[j] << endl;
                }
                else
                    cout << "    " << print_main[j] << endl;
                SetConsoleTextAttribute(hConsole, 15);
            }
            c = _getch();
            if (c == 115 || c == 80) i++;
            if (c == 119 || c == 72) i--;
            if (i < 1) i = 1;
            if (i > int_print_main - 1) i = int_print_main - 1;
            system("cls");
        } while (c != 13);

        //system("cls");//очистка консоли 
        /*cout << "1. Создание таблицы" << endl << "2. Просмотр рейсов" << endl << "3. Добавить рейс" << endl << "4. Удалить рейс" << endl << "5. Корректировка рейсов" << endl << "6. Сортировка" << endl << "7. Сохранение таблицы" << endl << "8. Загрузка данных" << endl << "9. Поиск по..." << endl << "10. Обработка данных" << endl << "11. ВЫХОД" << endl;
        cin >> c;*/
        switch (i) {
        case 1:
            d = read_data(f);
            beg = add_first(beg, d);
            break;
        case 2:
            ShowTablePart(beg);

            break;
        case 3:
            beg = create_my_list(beg, f);
            break;

        case 4:
            int deleteId;
            cout << "Введите id для удаления: ";
            cin >> deleteId;
            beg = delete_at_id(beg, deleteId);
            break;

        case 5:
            int updateId;
            cout << "Введите id для корректировки: ";
            cin >> updateId;
            update_flight(beg, updateId);
            break;

        case 6:
            int criteria;
            cout << "Выберите пункт для сортировки:" << endl << "1) По номеру." << endl << "2) По названию рейса." << endl << "3) По названию марки" << endl << "4) По стоимости." << endl << "5) По количеству пассажиров." << endl;
            cin >> criteria;
            sort_my_list(beg, criteria);
            break;

        case 7:
            save_my_list(beg, filename);
            break;

        case 8:
            rewind(f);
            for (v; v > 0; v--) {
                d = create_by_file(f);
                beg = add_first(beg, d);
            }
            puts("Список создан");
            break;

        case 9:
            cout << "Выберите пункт для поиска:" << endl << "1) По номеру." << endl << "2) По названию рейса." << endl << "3) По названию марки" << endl << "4) По стоимости." << endl << "5) По количеству пассажиров." << endl;
            cin >> criteria;
            system("cls");
            search_my_list(beg, criteria);
            break;
        case 10:

            calculate_cost(beg);
            break;


        case 11:
            fclose(f); return 0;
        default: break;


        }
    }
}
/////-----------------------ЗАПИСЬ ДАННЫХ-----------------------/////
struct Airport read_data(FILE* f) {
    struct Airport air;
    fseek(f, 0, SEEK_END);
    int num = ftell(f) / sizeof(struct Airport) + 1;
    bool validInput = false;

    while (!validInput) {
        cout << "ID: ";
        if (cin >> air.id) {
            validInput = true;
        }
        else {
            cout << "Ошибка ввода! Пожалуйста введите ID." << endl;
            cin.clear();
            while (cin.get() != '\n') {
                continue;
            }
        }
    }

    validInput = false;
    while (!validInput) {
        cout << "Название рейса: ";
        if (cin >> air.name) {
            validInput = true;
        }
        else {
            cout << "Ошибка ввода! Пожалуйста введите название." << endl;
            cin.clear();
            while (cin.get() != '\n') {
                continue;
            }
        }
    }

    validInput = false;
    while (!validInput) {
        cout << "Марка самолета: ";
        if (cin >> air.brand) {
            validInput = true;
        }
        else {
            cout << "Ошибка ввода! Пожалуйста введите марку самолета." << endl;
            cin.clear();
            while (cin.get() != '\n') {
                continue;
            }
        }
    }

    validInput = false;
    while (!validInput) {
        cout << "Расходы: ";
        if (cin >> air.expensesFlight) {
            validInput = true;
        }
        else {
            cout << "Ошибка ввода! Пожалуйста введите расходы." << endl;
            cin.clear();
            while (cin.get() != '\n') {
                continue;
            }
        }
    }

    validInput = false;
    while (!validInput) {
        cout << "Количество пассажиров: ";
        if (cin >> air.passengers) {
            validInput = true;
        }
        else {
            cout << "Ошибка ввода! Пожалуйста введите количество пассажиров." << endl;
            cin.clear();
            while (cin.get() != '\n') {
                continue;
            }
        }
    }

    fwrite(&air, sizeof(struct Airport), 1, f);
    return air;
}

struct my_list* add_first(struct my_list* beg, struct Airport d)
{
    struct my_list* newNode;
    newNode = new my_list;
    newNode->data = d;
    newNode->next = beg;
    newNode->prev = nullptr;
    if (beg != nullptr) {
        beg->prev = nullptr;
    }
    beg = newNode;
    return beg;
}

struct my_list* add_last(struct my_list* beg, struct Airport d)
{
    struct my_list* newNode, * end;
    newNode = new my_list;
    newNode->data = d;
    newNode->next = nullptr;
    if (beg == nullptr) {
        beg = newNode;
        newNode->prev = nullptr;
    }
    else {
        end = beg;
        while (end->next != nullptr) {
            end = end->next;
        }
        end->next = newNode;
        newNode->prev = end;
    }
    return beg;
}


/////-----------------------СОЗДАНИЕ ТАБЛИЦЫ-----------------------/////
struct my_list* create_my_list(struct my_list* beg, FILE* f) {
    int fl = 0;
    struct Airport d;
    do {
        if (beg == NULL) { //если список пуст, то создать список
            d = read_data(f);
            beg = add_first(beg, d);
        }
        else { // иначе, добавить новый элемент в конец списка
            d = read_data(f);
            add_last(beg, d);
        }

        printf("Если хотите добавить - 1(выход - 0) -> ");
        scanf("%d", &fl);
    } while (fl);
    return beg;
}

/////-----------------------УДАЛЕНИЕ-----------------------/////
struct my_list* delete_at_id(struct my_list* beg, int deleteId) {

    if (beg == nullptr) {
        cout << "Список пуст" << endl;
        return beg;
    }

    struct my_list* current = beg;

    // Ищем элемент с указанным id
    while (current != nullptr) {
        if (current->data.id == deleteId) {
            // Нашли элемент с указанным id

            struct my_list* prev = current->prev;
            struct my_list* next = current->next;

            if (prev != nullptr) {
                prev->next = next;
            }
            else {
                beg = next;
            }

            if (next != nullptr) {
                next->prev = prev;
            }

            break;
        }

        current = current->next;
    }

    return beg;
}

/////-----------------------КОРРЕКТИРОВКА-----------------------/////
void update_flight(struct my_list* beg, int updateId) {
    if (beg == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }

    struct my_list* current = beg;

    // Ищем элемент с указанным id
    while (current != nullptr) {
        if (current->data.id == updateId) {
            // Нашли элемент с указанным id

            cout << "Введите новые данные:" << endl;
            cout << "Название рейса: ";
            cin >> current->data.name;
            cout << "Марка самолета: ";
            cin >> current->data.brand;
            cout << "Расходы: ";
            cin >> current->data.expensesFlight;
            cout << "Количество пассажиров: ";
            cin >> current->data.passengers;

            break;
        }

        current = current->next;
    }
}

/////-----------------------СОРТИРОВКА-----------------------/////
void sort_my_list(struct my_list* beg, int criteria) {
    if (beg == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    int size = 0;
    struct my_list* current = beg;

    // Считаем количество элементов в списке
    while (current != nullptr) {
        size++;
        current = current->next;
    }

    // Создаем массив для хранения данных
    struct Airport* arr = new Airport[size];
    current = beg;
    int i = 0;

    // Заполняем массив значениями из списка
    while (current != nullptr) {
        arr[i++] = current->data;
        current = current->next;
    }

    // Сортируем массив по выбранному критерию


    switch (criteria) {
    case 1:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].id > arr[j + 1].id) {
                    struct Airport temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        break;
    case 2:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (strcmp(arr[j].name, arr[j + 1].name) > 0) {
                    struct Airport temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        break;
    case 3:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (strcmp(arr[j].brand, arr[j + 1].brand) > 0) {
                    struct Airport temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        break;
    case 4:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].expensesFlight > arr[j + 1].expensesFlight) {
                    struct Airport temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        break;
    case 5:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].passengers > arr[j + 1].passengers) {
                    struct Airport temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        break;
    default:
        break;
    }

    // Обновляем значения в списке
    current = beg;
    i = 0;
    while (current != nullptr) {
        current->data = arr[i++];
        current = current->next;
    }
    delete[] arr;
}


/////-----------------------СОХРАНЕНИЕ-----------------------/////
void save_my_list(struct my_list* beg, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (f == NULL) {
        cout << "Ошибка открытия файла" << endl;
        exit(1);
    }

    struct my_list* ptr = beg;
    while (ptr != nullptr) {
        fwrite(&(ptr->data), sizeof(struct Airport), 1, f);
        ptr = ptr->next;
    }

    fclose(f);

    FILE* textFile = fopen("text_file.txt", "w");
    if (textFile == NULL) {
        cout << "Ошибка открытия текстового файла" << endl;
        exit(1);
    }

    ptr = beg;
    while (ptr != nullptr) {
        fprintf(textFile, "Id: %d\n", ptr->data.id);
        fprintf(textFile, "Name: %s\n", ptr->data.name);
        fprintf(textFile, "Brand: %s\n", ptr->data.brand);
        fprintf(textFile, "Expenses Flight: %d\n", ptr->data.expensesFlight);
        fprintf(textFile, "Passengers: %d\n\n", ptr->data.passengers);
        ptr = ptr->next;
    }

    fclose(textFile);

    cout << "Таблица успешно сохранена в файл." << endl;
}
/////-----------------------ЗАГРУЗКА ИЗ ФАЙЛА-----------------------/////
struct Airport create_by_file(FILE* f) {
    struct Airport d;
    fread(&d, sizeof(struct Airport), 1, f);
    return d;
}

void print_airport(struct Airport airport) {
    printf("| %5d | %14s | %9s | %7d | %21d |\n", airport.id, airport.name, airport.brand, airport.expensesFlight, airport.passengers);
}

/////-----------------------ПОИСК ПО...-----------------------/////
void search_my_list(struct my_list* beg, int criteria) {
    struct my_list* current = beg;
    int found = 0;
    char searchName[20];
    char searchBrand[6];
    int searchExpensesFlight;
    int searchPassengers;
    //system("cls");

    switch (criteria) {
    case 1:
        cout << "Введите ID рейса для поиска: ";
        cin >> searchExpensesFlight;
        cout << "| Номер | Название рейса |   Марка   | Расходы | Количество пассажиров |\n" << endl;
        while (current != nullptr) {
            if (current->data.id == searchExpensesFlight) {
                print_airport(current->data);
                found = 1;
            }
            current = current->next;
        }
        break;
    case 2:
        cout << "Введите название для поиска: ";
        cin >> searchName;
        cout << "| Номер | Название рейса |   Марка   | Расходы | Количество пассажиров |\n" << endl;
        while (current != nullptr) {
            if (strcmp(current->data.name, searchName) == 0) {
                print_airport(current->data);
                found = 1;
            }
            current = current->next;
        }

        break;
    case 3:
        cout << "Введите бренд для поиска: ";
        cin >> searchBrand;
        cout << "| Номер | Название рейса |   Марка   | Расходы | Количество пассажиров |\n" << endl;
        while (current != nullptr) {
            if (strcmp(current->data.brand, searchBrand) == 0) {
                print_airport(current->data);
                found = 1;
            }
            current = current->next;
        }
        break;
    case 4:
        cout << "Введите расходы на рейс для поиска: ";
        cin >> searchExpensesFlight;
        cout << "| Номер | Название рейса |   Марка   | Расходы | Количество пассажиров |\n" << endl;
        while (current != nullptr) {
            if (current->data.expensesFlight == searchExpensesFlight) {
                print_airport(current->data);
                found = 1;
            }
            current = current->next;
        }
        break;
    case 5:
        cout << "Введите пассажиров для поиска: ";
        cin >> searchPassengers;
        cout << "| Номер | Название рейса |   Марка   | Расходы | Количество пассажиров |\n" << endl;
        while (current != nullptr) {
            if (current->data.passengers == searchPassengers) {
                print_airport(current->data);
                found = 1;
            }
            current = current->next;
        }
        break;
    default:
        cout << "Ошибка ввода!" << endl;
        break;
    }

    if (!found) {
        cout << "Не найдено данных соответствующих указанным критериям." << endl;
    }
}

/////-----------------------КАЛЬКУЛЯТОР-----------------------/////
void calculate_cost(struct my_list* beg) {
    struct my_list* current = beg;
    FILE* res;
    res = fopen("results.txt", "rb+");
    if (!res) {
        res = fopen("results.txt", "wb+");
        if (res == nullptr) {
            cout << "Ошибка открытия файла";
            exit(1);
        }
    }
    double totalCost = 0.0;
    int count = 0;

    cout << "| Номер | Название рейса |   Марка   | Стоимость на одного пассажира |" << endl;
    fprintf(res, "| Номер | Название рейса |   Марка   | Стоимость на одного пассажира |");

    while (current != NULL) {
        struct Airport air = current->data;

        double costPerPassenger = (double)air.expensesFlight / air.passengers;
        printf("| %5d | %14s | %9s | %29f |\n", air.id, air.name, air.brand, costPerPassenger);

        
        fprintf(res, "| %5d | %14s | %9s | %29f |\n", air.id, air.name, air.brand, costPerPassenger);

        totalCost += costPerPassenger;
        count++;

        current = current->next;
    }

    double averageCost = totalCost / count;

    cout << "Средняя стоимость на одного пассажира: " << averageCost << endl;
    fclose(res); // Закрываем файл
}





/////-----------------------СКРОЛЛИНГ-----------------------/////
int CountStudents(struct my_list* beg) {
    int countStudents = 0;

    struct my_list* current = beg;
    while (current != NULL) {
        countStudents++;
        current = current->next;
    }

    return countStudents;
}

void ShowPartStudents(struct my_list* beg, int page) {
    if (beg == NULL) {
        cout << "List is null" << endl;
        return;
    }

    int countStudents = 1;
    system("cls");
    cout << endl << "| Page " << page + 1 << "|" << "ArrowUp, ArrowDown - Table navigation. ESC - Exit" << endl;
    cout << "| Номер | Название рейса |   Марка   | Расходы | Количество пассажиров |\n" << endl;

    struct my_list* current = beg;
    while (current != NULL) {
        if ((countStudents > page * MAXSTUDENTSONPAGE) && (countStudents <= (page + 1) * MAXSTUDENTSONPAGE)) {
            printf("| %5d | %14s | %9s | %7d | %21d |\n", current->data.id, current->data.name, current->data.brand, current->data.expensesFlight, current->data.passengers);
        }

        if (countStudents > (page + 1) * MAXSTUDENTSONPAGE) {
            break;
        }

        countStudents++;
        current = current->next;
    }

    cout << "________________________________________________________________________________" << endl;

    return;
}

void ShowTablePart(struct my_list* beg) {
    if (beg == NULL) {
        cout << "List is null" << endl;
        return;
    }

    int maxStudents = CountStudents(beg);
    cout << maxStudents << endl;

    int key = 0;
    int page = 0;
    int isStop = 0;

    ShowPartStudents(beg, page);

    while (isStop == 0) {
        key = _getch();

        switch (key) {
        case 80: {

            if (page + 1 <= round(maxStudents / MAXSTUDENTSONPAGE)) {
                page++;
                ShowPartStudents(beg, page);
            }
        }
               break;
        case 72: {

            if (page - 1 >= 0) {
                page--;
                ShowPartStudents(beg, page);
            }
        }
               break;
        case 27:
        {
            isStop = 1;
        }
        break;
        }
    }

    return;
}
