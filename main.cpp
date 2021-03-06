// Подключение необходимых библиотек
#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// Проверка ввода 
vector<int> smartCheckOfInput(size_t numberOfArguments,char minNumber,char maxNumber) {
	string str;
	vector<int> ls;
	while (1) {
		// Считывание данных в str
		getline(cin, str);
		// Поиск запрещенных символов в str
		auto it = find_if(str.begin(), str.end(), [minNumber,maxNumber](const char& ch) {
			if (ch <= maxNumber and ch >= minNumber or ch == ' ' or ch == 9) return false;
			else return true;
		});
		// Если найдены запрещенные символы - вернуть поток ввода в рабочее состояние и повторить ввод
		if (it != str.end() or cin.eof()) {
			cout << "Недопустимые символы. Повторите ввод" << endl;
			cin.clear();
			continue;
		}
		// Если не найдены запрещенные символы - последовательно извлекать числа из строки и добавлять в вектор значений
		auto beg = str.begin();
		auto end = beg;
		while (beg != str.end()) {
			while (beg != str.end() and (*beg == ' ' or *beg == 9)) ++beg;
			end = beg;
			while (end != str.end() and (*end >= '0' and *end <= '9')) ++end;
			if (beg == end) break;
			int var = atoi(string(beg, end).c_str());
			ls.push_back(var);
			beg = end;
		}
		// Если количество извлеченных чисел совпадает с количество требуемых значений - выходим из цикла ввода
		if (ls.size() == numberOfArguments*numberOfArguments) break;
		// Иначе очищаем вектор значений и повторяем ввод
		else {
			cout << "Введено неверное количество аргументов. Повторите ввод" << endl;
			ls.clear();
		}
	}
	// возвращаем в main готовый вектор с значениями
	return ls;
}
// Заполнение матрицы значениями вектора
bool fillMatrixFromVector(int** matrix, size_t size, const vector<int> &vec) {
	// Заполняем матрицу элементами из вектора значений
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			// Если элемент вектора значений не попадает в отрезок [1,100] - выйти и повторить ввод
			if (vec[i*size + j] < 1 or vec[i*size + j] > 100) {
				return false;
			}
			// Если элемент попадает в отрезок - записать в матрицу
			matrix[i][j] = vec[i*size + j];
		}
	}
	// При успешном заполнении матрицы - выйти и продолжить выполнение программы
	return true;
}
// Заполнение матрицы случайными значениями
void fillMatrixWithRandomNumbers(int** matrix, size_t size) {
	// Генерация случайных чисел при каждом запуске программы
	srand(time(NULL));
	// Заполнение матрицы числами из отрезка [1,100]
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			matrix[i][j] = rand() % 100 + 1;
		}
	}
}
// Вывод матрицы в консоль
void showMatrix(int** matrix, size_t size) {
	// Вывод матрицы в консоль
	cout << "Исходная матрица" << endl;
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			cout<<matrix[i][j]<<" | ";
		}
		cout << endl;
	}
}

int main()
{
	setlocale(0, "rus");

	// Организация проверки на ввод размера матрицы
	cout << "Введите размер матрицы от 2 до 5 включительно: ";
	size_t size;
	while(1){
		size = smartCheckOfInput(1, '2', '5')[0];
		if (size >= 2 and size <= 5) {
			break;
		}
		cout << "Неверный размер. Повторите ввод" << endl;
	}

	// Организация проверки на ввод способа заполнения матрицы
	cout << "Выберите способ ввода(0 - с клавиатуры 1 - случайные числа): ";
	int choice;
	while (1) {
		choice = smartCheckOfInput(1, '0', '1')[0];
		if (choice == 0 or choice == 1) {
			break;
		}
		cout << "Неверный выбор. Повторите ввод" << endl;
	}

	// Выделение памяти по матрицу
	int** matrix = new int*[size];
	for (size_t i = 0; i < size; i++) {
		matrix[i] = new int[size];
	}

	// Выделение памяти под массив результатов
	int* sumOfElements = new int[size*2];

	// Организация проверки на ввод элементов матрицы
	vector<int> values;
	if (choice == 0) {
		// Ввод через стандартный поток ввода
		cout << "Введите элементы матрицы через пробел" << endl;
		while (1) {
			values = smartCheckOfInput(size, '0', '9');
			if (fillMatrixFromVector(matrix, size, values)) {
				break;
			}
			cout << "Элементы не удовлетворяют отрезку [1,100]. Повторите ввод" << endl;
		}
	}
	else {
		// Заполнение случайными числами в отрезке [1,100]
		cout << "Матрица заполнена случайными числами" << endl;
		fillMatrixWithRandomNumbers(matrix, size);
	}

	// Вывод матрицы в консоль
	cout << endl;
	showMatrix(matrix, size);

	// Подсчет суммы элементов каждой строки и запись в первую половину sumOfElements
	int rowSum = 0;
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			rowSum += matrix[i][j];
		}
		sumOfElements[i] = rowSum;
		rowSum = 0;
	}

	// Подсчет суммы элементов каждого столбца и запись во вторую половину sumOfElements
	int columnSum = 0;
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			columnSum += matrix[j][i];
		}
		sumOfElements[size + i] = columnSum;
		columnSum = 0;
	}

	// Сортировка первой половины по убыванию
	sort(sumOfElements, sumOfElements + size, greater<int>());

	// Сортировка второй половины по возрастанию
	sort(sumOfElements + size, sumOfElements + 2 * size);

	// Вывод массива sumOfElements в консоль
	cout << endl << "Результат" << endl;
	for (size_t i = 0; i < 2 * size; i++) {
		cout << sumOfElements[i] << " | ";
	}

	// Освобождение памяти под массив sumOfElements
	delete[]sumOfElements;

	// Освобождение памяти под матрицу matrix
	for (size_t i = 0; i < size; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;

	cout << endl << endl;
	system("pause");
	return 0;
}

