/*
Поле шахматной доски определяется парой натуральных чисел,
каждое из которых не превосходит восьми: первое число — номер вертикали
(при счете слева направо), второе — номер горизонтали (при счете снизу
вверх). Даны натуральные числа a, b, c, d, e, f, каждое из которых не
превосходит восьми. На поле (a, b) расположена белая фигура, на поле (c, d)
— черная. Определить, может ли белая фигура пойти на поле (e, f), не попав
при этом под удар черной фигуры. Рассмотреть следующие варианты
сочетаний белой и черной фигур:

а) ладья и ладья; з) ферзь и слон; п) слон и конь;
б) ладья и ферзь; и) конь и конь; р) слон и ладья;
в) ладья и конь; к) конь и ладья; с) король и слон;
г) ладья и слон; л) конь и ферзь; т) король и ферзь;
д) ферзь и ферзь; м) конь и слон; у) король и конь;
е) ферзь и ладья; н) слон и слон; ф) король и ладья;
ж) ферзь и конь; о) слон и ферзь;
*/

#include <iostream>

#define VIEW = true

using namespace std;

// codes

const int NONE = 0x0;
const int WHITE = 0x1;
const int BLACK = 0x2;
const int WHITE_ZONE = 0x3;
const int BLACK_ZONE = 0x4;
const int INTER_ZONE = 0x5;

char ch_c(int code) {
	switch (code)
	{
	case NONE:
		return 'O';
	case WHITE:
		return 'W';
	case BLACK:
		return 'B';
	case WHITE_ZONE:
		return '+';
	case BLACK_ZONE:
		return '-';
	case INTER_ZONE:
		return 'Z';
	default:
		return 'O';
	}
}

// chess table
const int S = 8;
auto table = new int[S * S];

int ix(int i, int j) {
	return i * S + j;
}

void mv_t(void (func)(int i, int j)) {
	for (int i = 0; i < S * S; i++)
		func(i / S, i % S);
}

void cl_t() {
	mv_t([](int i, int j) {table[ix(i, j)] = NONE; });
}

void v_t() {
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++)
		{
			cout << ch_c(table[ix(i, j)]) << ' ';
		}
		cout << endl;
	}
}

int cx, cy;
bool cc;

void sp(int x, int y, bool c /*true - white; false - black*/)
{
	cx = x;
	cy = y;
	cc = c;

	if (c)
		table[ix(y, x)] = WHITE;
	else
		table[ix(y, x)] = BLACK;
}

void sz(int i, int j, bool c) {
	auto v = table[ix(i, j)];
	if ((v == WHITE_ZONE && !c) || (v == BLACK_ZONE && c))
		table[ix(i, j)] = INTER_ZONE;
	else
		table[ix(i, j)] = c ? WHITE_ZONE : BLACK_ZONE;
}

// pieces
void king(int x, int y, bool c) {
	sp(x, y, c);
	mv_t([](int i, int j) {
		if (cx != j || cy != i)
		{
			if ((cx == j || cx == j + 1 || cx == j - 1) &&
				(cy == i || cy == i + 1 || cy == i - 1))
				sz(i, j, cc);
		}
		});
}

void rook(int x, int y, bool c) {
	sp(x, y, c);
	mv_t([](int i, int j) {
		if (cx != j || cy != i)
		{
			if (cx == j || cy == i)
				sz(i, j, cc);
		}
		});
}

void bishop(int x, int y, bool c) {
	sp(x, y, c);
	mv_t([](int i, int j) {
		if (cx != j || cy != i)
		{
			if (cx - j == cy - i ||
				cx - j == -(cy - i))
				sz(i, j, cc);
		}
		});
}

void queen(int x, int y, bool c) {
	sp(x, y, c);
	mv_t([](int i, int j) {
		if (cx != j || cy != i)
		{
			if ((cx == j || cx == j + 1 || cx == j - 1) &&
				(cy == i || cy == i + 1 || cy == i - 1) ||
				cx == j || cy == i ||
				cx - j == cy - i ||
				cx - j == -(cy - i))
				sz(i, j, cc);
		}
		});
}

void knight(int x, int y, bool c) {
	sp(x, y, c);
	mv_t([](int i, int j) {
		if (cx != j || cy != i)
		{
			if (cx == j + 2 && (cy == i + 1 || cy == i - 1) ||
				cx == j - 2 && (cy == i + 1 || cy == i - 1) ||
				cy == i + 2 && (cx == j + 1 || cx == j - 1) ||
				cy == i - 2 && (cx == j + 1 || cx == j - 1))
				sz(i, j, cc);
		}
		});
}

void sw_ch(int i, int x, int y, bool c) {
	switch (i)
	{
	case 0:
		rook(x, y, c);
		break;
	case 1:
		queen(x, y, c);
		break;
	case 2:
		knight(x, y, c);
		break;
	case 3:
		bishop(x, y, c);
		break;
	case 4:
		king(x, y, c);
		break;
	default:
		break;
	}
}

// input

int gi(char name) {
	cout << endl;
	cout << name << ": ";

	int input;
	cin >> input;

	return input;
}

bool ch(int n) {
	return n < 0 || n > S;
}

int ep() {
	cout << "INCORRECT INPUT. END PROGRAM!";
	return -1;
}

void res(int wx, int wy, int bx, int by, int nx, int ny) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++)
		{
			cl_t();
			sw_ch(i, wx, wy, true);
			sw_ch(j, bx, by, false);
			cout << i * 5 + j + 1 << ": " << ((table[ix(ny, nx)] == WHITE_ZONE ? 1 : 0) ? 1 : 0) << endl;
#if defined(VIEW)
			v_t();
#endif
		}
	}
}


int main()
{
	cout << "check my chess plz >_<";

	auto a = gi('a');
	if (!cin || ch(a)) return ep();

	auto b = gi('b');
	if (!cin || ch(b)) return ep();

	auto c = gi('c');
	if (!cin || ch(c)) return ep();

	auto d = gi('d');
	if (!cin || ch(d)) return ep();

	auto e = gi('e');
	if (!cin || ch(e)) return ep();

	auto f = gi('f');
	if (!cin || ch(f)) return ep();

	res(a, b, c, d, e, f);
}
