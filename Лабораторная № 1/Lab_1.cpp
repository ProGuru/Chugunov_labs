#include <iostream>
#include <math.h> // для функции pow
const int SIZE_OF_LONG = 32;  // число типа unsigned long состоит из 32 разрядов в двоичной системе
const int SIZE_EXP_OF_FLOAT = 8; // количество битов, выделенныx для порядка числа типа float
const int SIZE_MANTISSA_OF_FLOAT = 23; // количество битов, выделенныx для мантиссы числа типа float
const int SIZE_OF_FLOAT = SIZE_EXP_OF_FLOAT + SIZE_MANTISSA_OF_FLOAT + 1;

using namespace std;

char *unsignedLongNumberSave;
char *unsignedFloatNumberSave;

void printBinary(char* binaryCode, int sizeToPrint) {
    for (int i = sizeToPrint; i != 0 ; i--) {
        cout << binaryCode[i - 1];
    }
    cout << endl;
}

void convertToBinary(char *binaryCode, unsigned long unsignedLongNumber, int numberOfBit, int startPoint) {
    for (int i = startPoint; i < (numberOfBit + startPoint); i++) {
        binaryCode[i] = unsignedLongNumber % 2 == 1 ? '1' : '0';
        unsignedLongNumber = unsignedLongNumber >> 1; // побитовый сдвиг вправо
    }
}

void zeroing(char *binaryCode, int numberOfBit) { // обнуляем элементы массива
    for (int i = 0; i < numberOfBit; i++) { 
        binaryCode[i] = '0';
    }
}

void convertFractionalToBinary(char *binaryCode, double fractionalNumber, int numberOfBit) {
    for (int i = 1; i < numberOfBit; i++) {
        
        double exponent2 = pow(2, -i);
        if ((fractionalNumber >= exponent2) && (fractionalNumber != 0)) {
            binaryCode[numberOfBit - i] = '1';
            fractionalNumber -= exponent2;
        }
    }
}

void convertIntToBinary(char *binaryCode, unsigned long unsignedLongNumber, int numberOfBit) {
    for (int i = 1; i <= numberOfBit; i++) {
        
        unsigned long exponent2 = pow(2, numberOfBit - i);
        if ((unsignedLongNumber >= exponent2) && (unsignedLongNumber != 0)) {
            binaryCode[numberOfBit - i] = '1';
            unsignedLongNumber -= exponent2;
        }
    }
}

int findExponent(char *binaryCode, int numberOfBit) {
    for (int i = numberOfBit; i > 0; i--) { 
        if ((binaryCode[i - 1]) == '1') {
            return (numberOfBit - i + 1) * (-1);
        }
    }
    return 0;
}

void mergeBinaryArray(char *mainBinaryCode, char *addBinaryCode, int wide, int offset) {
    for (int i = 1; i <= wide; i++) { 
        mainBinaryCode[SIZE_MANTISSA_OF_FLOAT - i] = addBinaryCode[(SIZE_MANTISSA_OF_FLOAT * 2) - i + offset];
    }
}

void mergeIntAndFloat(char *binaryCode, char *intPart, int sizeIntPart, char *floatPart, int sizeFloatPart, int offset) {
    for (int i = 0; i <= offset; i++){
        binaryCode[SIZE_MANTISSA_OF_FLOAT - i + 1] = intPart[offset - i];
    }
    for (int i = 0; i < (SIZE_MANTISSA_OF_FLOAT - offset); i++){
        binaryCode[SIZE_MANTISSA_OF_FLOAT - offset - i] = floatPart[sizeFloatPart - i - 1];
    }
}

char *convertLongToBinary() {
    cout << "Введите число типа Unsigned long:";
    unsigned long unsignedLongNumber;
    cin >> unsignedLongNumber;
    char *binaryCodeLong = new char[SIZE_OF_LONG]; // выделяем память под массив символов для числа Long
    zeroing(binaryCodeLong, SIZE_OF_LONG);
    convertToBinary(binaryCodeLong, unsignedLongNumber, SIZE_OF_LONG, 0);
    cout << "Число в формате unsigned long в двоичной системе будет выглядеть так: ";
    unsignedLongNumberSave = binaryCodeLong;
    return binaryCodeLong;
}

char *convertFloatToBinary() {
    cout << "Введите число типа float:";
    float floatNumber;
    cin >> floatNumber;
    char *binaryCodeFloat = new char[SIZE_OF_FLOAT];
    zeroing(binaryCodeFloat, SIZE_OF_FLOAT);
    binaryCodeFloat[SIZE_OF_FLOAT - 1] = floatNumber > 0 ? '0' : '1'; // определяем знак
    if (floatNumber == 0) { // особый случай когда число = 0
        convertToBinary(binaryCodeFloat, -128, SIZE_EXP_OF_FLOAT, SIZE_MANTISSA_OF_FLOAT);
        return binaryCodeFloat;
    }
    floatNumber = abs(floatNumber);
    long intPart = (long)floatNumber; // целая часть
    char intPartBinary[SIZE_MANTISSA_OF_FLOAT]; // целая часть в двоичном представлении
    zeroing(intPartBinary, SIZE_MANTISSA_OF_FLOAT);
    double floatPart = floatNumber - intPart; // дробная часть. Необходимо использовать тип double для увеличения количества разрядов мантиссы
    char floatPartBinary[SIZE_MANTISSA_OF_FLOAT * 2]; // целая часть в двоичном представлении
    zeroing(floatPartBinary, SIZE_MANTISSA_OF_FLOAT * 2);
    int offset;
    if ((floatPart != 0) && (intPart == 0)) {
        convertFractionalToBinary(floatPartBinary, floatPart, SIZE_MANTISSA_OF_FLOAT * 2);
        offset = findExponent(floatPartBinary, SIZE_MANTISSA_OF_FLOAT * 2);
        mergeBinaryArray(binaryCodeFloat, floatPartBinary, SIZE_MANTISSA_OF_FLOAT, offset); // записываем в матрицу мантиссу
        convertToBinary(binaryCodeFloat, 127 + offset, SIZE_EXP_OF_FLOAT, SIZE_MANTISSA_OF_FLOAT); // записываем в матрицу показатель
        unsignedFloatNumberSave = binaryCodeFloat;
        return binaryCodeFloat;
    }
    // далее целая часть не равна 0
    convertIntToBinary(intPartBinary, intPart, SIZE_MANTISSA_OF_FLOAT);
    convertFractionalToBinary(floatPartBinary, floatPart, SIZE_MANTISSA_OF_FLOAT * 2);
    offset = SIZE_MANTISSA_OF_FLOAT + findExponent(intPartBinary, SIZE_MANTISSA_OF_FLOAT) + 1;
    mergeIntAndFloat(binaryCodeFloat, intPartBinary, SIZE_MANTISSA_OF_FLOAT, floatPartBinary, SIZE_MANTISSA_OF_FLOAT * 2, offset);
    convertToBinary(binaryCodeFloat, 127 + offset - 1, SIZE_EXP_OF_FLOAT, SIZE_MANTISSA_OF_FLOAT); // записываем в матрицу показатель
 
    cout << "Число в формате float в двоичной системе будет выглядеть так: ";
    unsignedFloatNumberSave = binaryCodeFloat;
    return binaryCodeFloat;
}

void swapBinary(char * binaryCode, int highBit, int couple) {
    char swapLeft[2];
    char swapRight[2];
    for (int j = 0; j < couple; j++) {
        for (int i = 0; i <  2; i++) {
            swapLeft[i] = binaryCode[highBit - 1 + i + j*2];
            swapRight[i] = binaryCode[highBit - 2 - i - j*2];
        }

        for (int i = 0; i < 2; i++) {
            binaryCode[highBit - 1 + i + j*2] = swapRight[1 - i];
            binaryCode[highBit - 2 - i - j*2] = swapLeft[1 - i];
        }
    }
}

int main()
{
    printBinary(convertLongToBinary(), SIZE_OF_LONG);
    printBinary(convertFloatToBinary(), SIZE_OF_FLOAT);

    cout << endl << "Задание: Поменять местами значения рядом стоящих бит в парах" << endl;
    cout << "Введите номер старшего разряда левой пары: ";
    int highBit;
    cin >> highBit;
    cout << "Введите количество пар: ";
    int couple;
    cin >> couple;
    if ((highBit <= (couple * 2)) || (SIZE_OF_LONG < (couple * 2 + highBit))) {
        cout << "Введено недопустимое количество пар либо введён номер несуществующего старшего разряда левой пары";
        return 0;
    }
    swapBinary(unsignedLongNumberSave, highBit, couple);
    swapBinary(unsignedFloatNumberSave, highBit, couple);
    cout << "Результат для числа типа unsigned long: ";
    printBinary(unsignedLongNumberSave, SIZE_OF_LONG);
    cout << "Результат для числа типа float: ";
    printBinary(unsignedFloatNumberSave, SIZE_OF_FLOAT);
    
    return 0;
}