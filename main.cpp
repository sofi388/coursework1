#include <iostream>

using namespace std;
const int N = 8;

//Переменная результата
int rez = 0;

//Разрядность
const int razr = 11111111;

//Введите вашу малую арифметику
int RulePlusOne[N] = { 1, 3, 0, 6, 2, 4, 7, 5 };

int Plus[N][N];
int PlusP[N][N];
int Mult[N][N];
int MultP[N][N];
int Order[N];
int Count[N];  //Count - кол-во единиц для соотв. числа, для разложения чисел


int Sum(int, int);
void SetOrder()
{
	int t = 0;
	for (int i = 0; i < N; i++)
	{
		Order[i] = t;
		Count[t] = i;
		t = RulePlusOne[t];
	}
}

// Генерация таблицы сложения и переноса по сложению
void SetPlus()
{
	for (int i = 0; i < N; i++)
	{
		Plus[0][i] = Plus[i][0] = i;
		Plus[1][i] = Plus[i][1] = RulePlusOne[i];
		if (RulePlusOne[i] == 0) PlusP[1][i] = PlusP[i][1] = 1;
	}
	for (int i = 2; i < N; i++)
		for (int k = i; k < N; k++)
		{
			PlusP[i][k] = PlusP[k][i] = (Count[i] + Count[k]) / N;
			Plus[i][k] = Plus[k][i] = Order[(Count[i] + Count[k]) % N];
		}
}

// Генерация таблицы умножения и переноса по умножению
void SetMult()
{
	for (int i = 0; i < N; i++)
		Mult[1][i] = Mult[i][1] = i;

	for (int i = 2; i < N; i++)
		for (int k = i; k < N; k++)
		{
			int p = 0;
			int t = i;
			for (int e = 0; e < Count[k] - 1; e++)
			{
				if (PlusP[i][t]) p = Plus[p][1];
				t = Plus[i][t];
			}
			Mult[i][k] = Mult[k][i] = t;
			MultP[i][k] = MultP[k][i] = p;
		}
}

void Print()
{
	cout << "Таблицы: \n\nСложение и переполнение по сложению             Умножение и переполнение по умножению\n\n";

	for (int i = 0; i < N; i++)
	{
		cout << i << "| ";
		for (int j = 0; j < N; j++) cout << Plus[i][j] << " ";
		cout << "   " << i << "| ";
		for (int j = 0; j < N; j++) cout << PlusP[i][j] << " ";
		cout << "       " << i << "| ";
		for (int j = 0; j < N; j++) cout << Mult[i][j] << " ";
		cout << "   " << i << "| ";
		for (int j = 0; j < N; j++) cout << MultP[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}
bool Compare(int A, int B)  // > == 1
{
	bool f = 0;
	int n = 1;
	for (int i = 0; i < N - 1; i++)
		n *= 10;
	for (int i = n; i > 0; i /= 10)
	{
		int d = A / i;
		if (Count[A / i] > Count[B / i])
		{
			f = 1;
			break;
		}
		else if (Count[A / i] < Count[B / i])
		{
			f = 0;
			break;
		}
		A = A % i;
		B = B % i;
	}
	return f;
}
bool Num(int one)  // есть ли 8 и 9? => 1
{
	while (one)
		if (((one % 10) == 8) || ((one % 10) == 9)) return 1;
		else	one = one / 10;
	return 0;
}

// Вычитание
int Subtr(int A, int B)
{
	if ((A > 0) && (B < 0)) return Sum(A, -1 * B);
	else 	if ((A < 0) && (B > 0)) return (-1 * Sum(-1 * A, B));
	else 	if ((A < 0) && (B < 0))
	{
		int tmp = A;
		A = abs(B);
		B = abs(tmp);
	}

	int n = 1;
	int t, res = 0;
	bool loan = 0, neg = 0;
	if (Compare(A, B) == 0)
	{
		int tmp = A;
		A = B;
		B = tmp;
		neg = 1;
	}
	while (A || B)
	{
		if (loan)
		{
			t = Count[A % 10] - 1 - Count[B % 10];
			loan = 0;
		}
		else t = Count[A % 10] - Count[B % 10];

		if (t < 0)
		{
			loan = 1;
			t = Order[N + t];
		}
		else t = Order[t];

		res = res + n * t;
		n *= 10;
		A = A / 10;
		B = B / 10;
	}
	if (neg) res *= -1;

	return res;
}

// Сложение
int Sum(int A, int B)
{
	int n = 1;
	int res = 0;
	int over = 0;
	int over1 = 0;
	bool neg = 0;

	if (((A >= 0) && (B >= 0)) || ((A <= 0) && (B <= 0)))
	{
		if ((A < 0) && (B < 0))
		{
			neg = 1;
			A *= -1;
			B *= -1;
		}
		while (A || B)
		{
			int t = Plus[A % 10][B % 10];
			over1 = Plus[PlusP[t][over]][PlusP[A % 10][B % 10]];
			t = Plus[t][over];
			over = over1;
			res = res + n * t;
			n *= 10;
			A = A / 10;
			B = B / 10;
		}
		res = res + n * over;
		if (neg) res *= -1;
	}
	else if (A < 0) res = Subtr(B, -1 * A);
	else if (B < 0) res = Subtr(A, -1 * B);

	return res;
}

// Умножение
int Mul(int A, int B)
{
	int n = 1;
	int res = 0;
	int over = 0;
	bool neg = 0;
	if ((A < 0) && (B < 0)) neg = 0;
	else 	if ((A > 0) && (B < 0)) neg = 1;
	else 	if ((A < 0) && (B > 0)) neg = 1;
	A = abs(A);
	B = abs(B);
	while (B)
	{
		int tmp = 0;
		int ntmp = 1;
		int Atmp = A;
		while (Atmp)
		{
			int t = Mult[Atmp % 10][B % 10] + MultP[Atmp % 10][B % 10] * 10;
			tmp = Sum(tmp, t * ntmp);
			Atmp = Atmp / 10;
			ntmp *= 10;
		}
		B = B / 10;
		res = Sum(res, tmp * n);
		n *= 10;
	}
	if (neg) res *= -1;
	return res;
}

// Деление
int Div(int A, int B)
{
	if ((A == 0) && (B == 0)) return (razr * Order[N - 1] + 1);
	else if (B == 0) return (razr * Order[N - 1] + 2);
	else if (A == 0)
	{
		rez = 0;
		return 0;
	}

	rez = 0;
	int n = 1;
	int res = 0;
	int ost = 0;
	bool one = 0;
	bool neg = 0;
	if ((A < 0) && (B < 0)) neg = 0;
	else 	if ((A > 0) && (B < 0)) neg = 1;
	else 	if ((A < 0) && (B > 0)) { neg = 1; one = 1; }
	A = abs(A);
	B = abs(B);
	A = Subtr(A, B);
	while (A >= 0)
	{
		res = Sum(res, 1);
		rez = A;
		A = Subtr(A, B);
	}
	if (one)
	{
		res = Sum(res, 1);
		rez = abs(A); //модуль
	}
	if (neg) res *= -1;
	return res;
}

int Work(int first, int second, char op) 
{
	int res = 0;
	switch (op)
	{	
	case '+':
		res = Sum(first, second);
		if ((res > razr* Order[N - 1]) || (res < -1 * razr * Order[N - 1])) cout << "Переполнение\n";
		else cout << first << " " << op << " " << second << " = " << res << endl;
		break;
	case '-':
		res = Subtr(first, second);
		if ((res > razr* Order[N - 1]) || (res < -1 * razr * Order[N - 1])) cout << "Переполнение\n";
		else cout << first << " " << op << " " << second << " = " << res << endl;
		break;
	case '*':
		res = Mul(first, second);
		if ((res > razr* Order[N - 1]) || (res < -1 * razr * Order[N - 1])) cout << "Переполнение\n";
		else cout << first << " " << op << " " << second << " = " << res << endl;
		break;
	case '/':
		res = Div(first, second);
		if ((first == 0) && (second == 0)) cout << "Неопределенность. Любое число в промежутке [-222222222;222222222]\n";
		else if ((first != 0) && (second == 0)) cout << "На ноль делить нельзя! (искл. 0/0)\n";
		else if (res == (razr * Order[N - 1] + 1)) cout << first << " " << op << " " << second << " = [-" << razr * Order[N - 1] << ", " << razr * Order[N - 1] << "]\n";
		else if (res == razr * Order[N - 1] + 2) cout << "Неопределенность\n";
		else cout << first << " " << op << " " << second << " = " << res << " mod " << rez << endl;
		break;
	}
	return 0;
};

int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "\nДискретная математика: Курсовая работа\n";
	std::cout << "\nВерещагина Софья\n";
	std::cout << "\nгр. 3630201/90002\n\n";

	SetOrder();
	SetPlus();
	SetMult();
	Print();
	cout << "Пожалуйста, введите два элемента в данном промежутке: [-" << razr * Order[N - 1] << ", " << razr * Order[N - 1] << "] и операцию из перечисленных [+, -, *, /] между ними:\n";
	cout << "+ - сложить \n- - вычесть \n* - умножить \n/ - разделить\n";
	int prog = 1;
	while (prog)
	{
		int first = 0, second = 0;
		char op;
		while (!(cin >> first) || Num(first) || (first > (razr * Order[N - 1])) || (first < (-razr * Order[N - 1])) ||
			!((cin >> op) && ((op == '+') || (op == '-') || (op == '*') || (op == '/'))) ||
			!(cin >> second) || Num(second) || (second > (razr * Order[N - 1])) || (second < (-razr * Order[N - 1])))
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Ошибка. Любое число в промежутке[-" << razr * Order[N - 1] << ", " << razr * Order[N - 1] << "] и операцией [+, -, *, /] между ними:\n";
		}



		int a = Work(first, second, op);



		cout << "Для продолжения нажмите - 1, для выхода - 0\n";
		while (!(cin >> prog) || !((prog == 1) || (prog == 0)))
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Еще раз! Да - 1 или Нет - 0: ";
		}
		cout << endl;
	}
	return 0;
}