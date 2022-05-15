#include <iostream>
#include <list>
using namespace std;

char *longToMachineBinary(long num) {
    //метод перевода long в машинное представление
    char *result = new char[32];//32 бита на число
    result[0] = num >= 0 ? '0' : '1';//знаковый бит
    int i = 31;
    while (num != 0) {
        //перевод в двоичную систему счисления
        result[i--] = num % 2 == 0 ? '0' : '1';
        num /= 2;
    }
    for (; i > 0; --i) {
        //заполнение оставшихся битов нулями
        result[i] = '0';
    }
    return result;
}

int getOrder (char *binaryFloat) {
    //метод получения порядка вещественного числа
    int result;
    for (int i = 0; i < 25; ++i) {
        if (binaryFloat[i] == '.') {
            //количество значащих бит перед точкой будет результатом
            result = i - 1;
            break;
        }
    }
    return result + 127;
}

char *orderToBinary(int order) {
    //метод перевода порядка в двоичную запись
    char *result = new char[8];//8 бит на порядок у float
    int i = 7;
    while (order != 0) {
        char rest = order % 2 == 0 ? '0' : '1';
        result[i--] = rest;
        order /= 2;
    }
    for (; i >= 0; --i) {
        result[i] = '0';
    }
    return result;
}

char *floatToBinary(float num) {
    //метод перевода float в двоичную систему счисления
    char *result = new char[25];//24 знака на биты и один на плавающую точку
    if (num < 0) num = abs(num);//если число отрицательное, получаем его модуль
    int intPart = (int)num;//целая часть
    float floatPart = num - (float)intPart;//дробная часть
    auto *intPartBinary = new list<char>();
    while (intPart != 0) {
        char rest = intPart % 2 == 0 ? '0' : '1';
        intPartBinary->push_front(rest);
        intPart /= 2;
    }
    int i = 0;
    for (auto it = intPartBinary->begin(); it != intPartBinary->end(); ++it) {
        result[i++] = *it;
    }
    result[i++] = '.';//целая часть закончилась, дальше идёт дробная
    for (; i < 25; ++i) {
        char rest = ((floatPart * 2) < 1) ? '0' : '1';
        result[i] = rest;
        floatPart *= 2;
        if (floatPart > 1) floatPart /= 10;
    }
    return result;
}

char *getMantissa(char *binary) {
    //метод получения мантиссы из двоичного представления вещественного числа
    char *result = new char[23];//23 бита на мантиссу (float)
    int counter = 0;
    for (int i = 1; i < 25; ++i) {
        //не учитываем первый бит и точку
        if (binary[i] == '.') continue;
        result[counter++] = binary[i];
    }
    return result;
}

char *floatToMachineBinary(float num) {
    //метод получения машинного представления числа float
    char *result = new char[32];//32 бита на число
    result[0] = num >= 0 ? '0' : '1';//знаковый бит
    char *binary = floatToBinary(num);//float в двоичной системе
    char *order = orderToBinary(getOrder(binary));//порядок
    for (int i = 1; i < 9; ++i) {//запись порядка
        result[i] = order[i - 1];
    }
    char *mantissa = getMantissa(binary);//мантисса
    for (int i = 9; i < 32; ++i) {//запись мантиссы
        result[i] = mantissa[i - 9];
    }
    return result;
}

void printLong(char *array) {
    //метод печати представления long
    cout << "S: " << array[0] << " | Number: ";
    for (int i = 1; i < 32; ++i) {
        cout << array[i];
    }
    cout << endl;
}

void printFloat(char *array) {
    //метод печати представления float
    cout << "S: " << array[0] << " | E: ";
    for (int i = 1; i < 9; ++i) {
        cout << array[i];
    }
    cout << " | M: ";
    for (int i = 9; i < 32; ++i) {
        cout << array[i];
    }
    cout << endl;
}

void printCharArray(char *array, int length) {
    //метод печати массива символов
    for (int i = 0; i < length; ++i) {
        cout << array[i];
    }
    cout << endl;
}

bool validate(int length, int firstGroup, int secondGroup, int groupSize) {
    //метод, проверяющий, возможно ли в данном условии произвести замену групп бит
    if (firstGroup >= secondGroup) return false;//первая группа старше или равна второй
    if (firstGroup + groupSize > secondGroup) return false;//первая группа заходит границей на вторую
    if (secondGroup + groupSize >= length) return false;//вторая группа выходит за пределы массива
    if (groupSize == 0) return false;//нулевой размер группы
    if (firstGroup >= length || secondGroup >= length) return false;//индексы за пределами массива
    return true;
}

void swapBitGroup(char *array, int length, int firstGroup, int secondGroup, int groupSize) {
    int firstIndex = length - firstGroup - 1;//индекс младшего бита первой группы в массиве
    int secondIndex = length - secondGroup - 1;//индекс младшего бита второй группы в массиве
    char temp;//промежуточная переменная символа
    for (int i = 0; i < groupSize; ++i) {
        temp = array[firstIndex - i];
        array[firstIndex - i] = array[secondIndex - i];
        array[secondIndex - i] = temp;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    long first;
    float second;

    cout << "Введите число (long): ";
    cin >> first;
    printLong(longToMachineBinary(first));

    cout << endl << "Введите число (float): ";
    cin >> second;
    printFloat(floatToMachineBinary(second));

    int choice, firstGroup, secondGroup, groupSize;
    cout << "Выберите число для преобразования (0 - long, 1 - float): ";
    cin >> choice;
    cout << "Введите через пробел индексы младших разрядов первой и второй группы и длину группы бит: ";
    cin >> firstGroup >> secondGroup >> groupSize;

    char *array;
    int length = 32;
    if (choice == 0) array = longToMachineBinary(first);
    else array = floatToMachineBinary(second);
    if (validate(length, firstGroup, secondGroup, groupSize)) {
        cout << "Число до обработки: ";
        printCharArray(array, length);
        swapBitGroup(array, length, firstGroup, secondGroup, groupSize);
        cout << "Число после обработки: ";
        printCharArray(array, length);
    } else {
        cout << "Ошибка! Недопустимые данные!";
    }

    return 0;
}