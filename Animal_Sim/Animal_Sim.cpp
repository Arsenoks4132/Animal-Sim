#include <iostream>
#include <windows.h>
#include <random>
#include <fstream>
#include <string>

using namespace std;

#define DBLUE 1
#define DGREEN 2
#define DLBLUE 3
#define DRED 4
#define DPINK 5
#define DYELLOW 6
#define DEFAULT 7
#define GRAY 8
#define BLUE 9
#define GREEN 10
#define LBLUE 11
#define RED 12
#define PINK 13
#define YELLOW 14
#define WHITE 15


int preds_born = 0;
int preds_die = 0;
int preds_stopped = 0;

int herbs_born = 0;
int herbs_die = 0;
int herbs_eaten = 0;
int herbs_super_eaten = 0;

int grass_generated = 0;
int grass_destroyed = 0;
int grass_eaten = 0;

int cataclism_count = 0;


void print(string text, int color = DEFAULT) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	cout << text;
	SetConsoleTextAttribute(hConsole, DEFAULT);
}

void println(string text, int color = DEFAULT) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	cout << text << endl;
	SetConsoleTextAttribute(hConsole, DEFAULT);
}

void printToConsole(int generations, int size) {
	size += 2;
	ifstream wo("logs.txt");
	string buff;
	system("cls");
	string text = "";
	for (int j = 0; j <= size + 1; j++) {
		getline(wo, buff);
		text += buff;
		text.push_back('\n');
	}
	cout << text;
	Sleep(3000);
	for (int i = 0; i < generations; i++) {
		system("cls");
		string text = "";
		for (int j = 0; j <= size + 1; j++) {
			getline(wo, buff);
			text += buff;
			text.push_back('\n');
		}
		if (i == generations - 1) {
			getline(wo, buff);
			text += buff;
			text.push_back('\n');
			text.push_back('\n');
			cout << text;
			system("pause");
			return;
		}
		cout << text;
		Sleep(500);
	}
	system("cls");
}

int ichek(int min, int max, string x = "") {
	int res;
	bool b, t = 0;
	if (x.length() == 0) {
		t = 1;
	}
	while (1) {
		if (t) {
			cin >> x;
		}
		b = 1;
		if ((x[0] == '-') || (x[0] >= '0' && x[0] <= '9')) {
			for (int i = 1; i < x.length(); ++i) {
				if (not((x[i] >= '0' && x[i] <= '9'))) {
					b = 0;
					break;
				}
			}
			try {
				if (b) {
					res = stoi(x);
					if (res >= min && res <= max) {
						return res;
					}
					else {
						cout << "Число не попадает в необходимый диапазон, повторите ввод: " << endl;
						cin.clear();
						cin.ignore();
						continue;
					}
				}
			}
			catch (out_of_range const& e) {
				cout << endl << "Число слишком большое" << endl;
				cin.clear();
				cin.ignore();
				continue;
			}
		}
		cout << "Неверный формат данных, введите целое число: " << endl;
		cin.clear();
		cin.ignore();
	}
}

struct entity {
	bool alive = 0;
	int age = 0;
	int hung = 0;
};

vector<vector<entity>> generate(vector<vector<entity>> one, vector<vector<entity>> two, int cnt, int hung) {
	random_device rd;
	mt19937 gen(rd());
	int size = one.size();
	int x;
	int y;
	while (cnt > 0) {
		x = gen() % size;
		y = gen() % size;
		if (!(one[y][x].alive) && !(two[y][x].alive)) {
			one[y][x].alive = 1;
			one[y][x].hung = hung;
			--cnt;
		}
	}
	return one;
}

vector<vector<char>> grass(vector<vector<char>> field, vector<vector<entity>> preds, int rec) {
	random_device rd;
	mt19937 gen(rd());
	int size = field.size();

	int super = rec / 100 * (gen() % 10);
	int hard = rec / 100 * (gen() % 10);

	int attempts = rec * 2;
	int attempts_s = super * 2;
	int attempts_h = hard * 2;

	int x;
	int y;
	while (rec > 0 && attempts != 0) {
		x = gen() % size;
		y = gen() % size;
		if (field[y][x] == ' ' && !(preds[y][x].alive)) {
			field[y][x] = '.';
			--rec;
			++grass_generated;
		}
		else {
			--attempts;
		}
	}
	while (super > 0 && attempts_s != 0) {
		x = gen() % size;
		y = gen() % size;
		if (!(preds[y][x].alive)) {
			field[y][x] = '*';
			--super;
			++grass_generated;
		}
		else {
			--attempts_s;
		}
	}

	while (hard > 0 && attempts_h != 0) {
		x = gen() % size;
		y = gen() % size;
		if (field[y][x] == ' ' && !(preds[y][x].alive)) {
			field[y][x] = '#';
			--hard;
			++grass_generated;
		}
		else {
			--attempts_h;
		}
	}
	return field;
}

string show(vector<vector<entity>> preds, vector<vector<entity>> herbs, vector<vector<char>> vect) {
	int msize = vect.size();
	string s = " " + string(msize * 2 + 1, '_') + "\n";
	for (int i = 0; i < msize; ++i) {
		s += "|";
		for (int j = 0; j < msize; ++j) {
			if (preds[i][j].alive) {
				s += " P";
			}
			else if (herbs[i][j].alive) {
				s += " H";
			}
			else if (vect[i][j] != ' ') {
				s += " " + string(1, vect[i][j]);
			}
			else {
				s += "  ";
			}
		}
		s += " |\n";
	}
	s += " " + string(msize * 2 + 1, '-');
	return s + "\n\n";
}

vector<vector<entity>> go(vector<vector<entity>> one, vector<vector<entity>> two, vector<vector<char>>& three, bool ispred) {
	random_device rd;
	mt19937 gen(rd());
	int msize = one.size();
	vector<vector<entity>> newOne(msize, vector<entity>(msize, entity()));
	int x;
	int y;
	for (int i = 0; i < msize; ++i) {
		for (int j = 0; j < msize; ++j) {
			if (one[i][j].alive) {
				int attempts = 10;
				while (attempts != 0) {
					x = gen() % 3 - 1;
					y = gen() % 3 - 1;
					if (i == 0) {
						y = abs(y);
					}
					if (j == 0) {
						x = abs(x);
					}
					if (i == msize - 1) {
						y = -abs(y);
					}
					if (j == msize - 1) {
						x = -abs(x);
					}
					if (!newOne[i + y][j + x].alive && !one[i + y][j + x].alive && !two[i + y][j + x].alive) {
						if (ispred && three[i + y][j + x] == '#') {
							three[i + y][j + x] = ' ';
							++preds_stopped;
							break;
						}
						newOne[i + y][j + x] = one[i][j];
						if (ispred && three[i + y][j + x] != ' ') {
							three[i + y][j + x] = ' ';
							++grass_destroyed;
						}
						break;
					}
					--attempts;
				}
			}
		}
	}
	return newOne;
}

void die(vector<vector<entity>>& one, float maxAge, float needHung, bool ispred) {
	random_device rd;
	mt19937 gen(rd());
	int msize = one.size();
	for (int i = 0; i < msize; ++i) {
		for (int j = 0; j < msize; ++j) {
			if (one[i][j].alive) {
				int chance = (one[i][j].age / maxAge) * 100 - sqrt(maxAge);
				int rand = gen() % 100;
				if (rand <= chance && (one[i][j].hung < needHung)) {
					one[i][j] = entity();
					if (ispred) {
						++preds_die;
					}
					else
					{
						++herbs_die;
					}
				}
			}
		}
	}
}

void hunt(vector<vector<entity>>& preds, vector<vector<entity>>& herbs) {
	int msize = preds.size();
	for (int y = 0; y < msize; ++y) {
		for (int x = 0; x < msize; ++x) {
			if (preds[y][x].alive) {
				vector<vector<int>> coord;
				int y_st = y - 1, y_ed = y + 1;
				int x_st = x - 1, x_ed = x + 1;

				if (y == 0) {
					y_st = y;
				}
				if (x == 0) {
					x_st = x;
				}
				if (y == msize - 1) {
					y_ed = y;
				}
				if (x == msize - 1) {
					x_ed = x;
				}
				for (int i = y_st; i <= y_ed; ++i) {
					for (int j = x_st; j <= x_ed; ++j) {
						if (herbs[i][j].alive) {
							coord.push_back({ i,j });
						}
					}
				}
				if (coord.size() != 0) {
					random_device rd;
					mt19937 gen(rd());
					int ch = gen() % coord.size();
					herbs[coord[ch][0]][coord[ch][1]] = entity();
					++preds[y][x].hung;
					++herbs_eaten;
				}
			}
		}
	}
}

void eat(vector<vector<entity>>& herbs, vector<vector<char>>& grass) {
	int msize = herbs.size();
	for (int y = 0; y < msize; ++y) {
		for (int x = 0; x < msize; ++x) {
			if (herbs[y][x].alive) {
				vector<vector<int>> super;
				vector<vector<int>> norm;
				int y_st = y - 1, y_ed = y + 1;
				int x_st = x - 1, x_ed = x + 1;

				if (y == 0) {
					y_st = y;
				}
				if (x == 0) {
					x_st = x;
				}
				if (y == msize - 1) {
					y_ed = y;
				}
				if (x == msize - 1) {
					x_ed = x;
				}
				for (int i = y_st; i <= y_ed; ++i) {
					for (int j = x_st; j <= x_ed; ++j) {
						if (grass[i][j] == '*') {
							super.push_back({ i,j });
						}
						if (grass[i][j] == '.') {
							norm.push_back({ i,j });
						}
					}
				}
				if (super.size() != 0) {
					random_device rd;
					mt19937 gen(rd());
					int ch = gen() % super.size();
					grass[super[ch][0]][super[ch][1]] = ' ';
					herbs[y][x].hung += 3;
					++herbs_super_eaten;
				}
				else if (norm.size() != 0) {
					random_device rd;
					mt19937 gen(rd());
					int ch = gen() % norm.size();
					grass[norm[ch][0]][norm[ch][1]] = ' ';
					++herbs[y][x].hung;
					++grass_eaten;
				}
			}
		}
	}
}

void born(vector<vector<entity>>& one, vector<vector<entity>>& two, vector<vector<char>>& three, bool ispred,
	int mh, int minAge, int maxAge, int chance) {
	random_device rd;
	mt19937 gen(rd());
	int msize = one.size();
	for (int y = 0; y < msize; ++y) {
		for (int x = 0; x < msize; ++x) {
			if (one[y][x].alive && minAge <= one[y][x].age && one[y][x].age <= maxAge) {
				vector<vector<int>> coord;
				bool para = false;
				int cnt = 0;
				int y_st = y - 1, y_ed = y + 1;
				int x_st = x - 1, x_ed = x + 1;

				if (y == 0) {
					y_st = y;
				}
				if (x == 0) {
					x_st = x;
				}
				if (y == msize - 1) {
					y_ed = y;
				}
				if (x == msize - 1) {
					x_ed = x;
				}
				for (int i = y_st; i <= y_ed; ++i) {
					for (int j = x_st; j <= x_ed; ++j) {
						if (!two[i][j].alive && !one[i][j].alive) {
							coord.push_back({ i,j });
						}
						else {
							if (one[i][j].alive && minAge <= one[i][j].age && one[i][j].age <= maxAge && !(y == i && x == j)) {
								para = true;
								++cnt;
							}
							else if (one[i][j].alive || two[i][j].alive) {
								++cnt;
							}
						}
					}
				}
				int rand = gen() % 100;
				if (coord.size() != 0 && para && cnt == 2 && rand <= chance) {
					int ch = gen() % coord.size();
					one[coord[ch][0]][coord[ch][1]].alive = true;
					one[coord[ch][0]][coord[ch][1]].hung = mh;
					if (ispred) {
						three[coord[ch][0]][coord[ch][1]] = ' ';
						++preds_born;
					}
					else {
						++herbs_born;
					}
				}
			}
		}
	}
}

void plusage(vector<vector<entity>>& one) {
	for (int i = 0; i < one.size(); ++i) {
		for (int j = 0; j < one.size(); ++j) {
			if (one[i][j].alive) {
				++one[i][j].age;
				one[i][j].hung /= 2;
			}
		}
	}
}

void cataclism(vector<vector<entity>>& preds, vector<vector<entity>>& herbs, vector<vector<char>>& grass, int isdeath) {
	random_device rd;
	mt19937 gen(rd());
	int chanse = gen() % 100;
	if (isdeath > chanse) {
		int part = gen() % 4;
		int size = preds.size();
		int x_st = 0;
		int y_st = 0;
		int x_ed = size;
		int y_ed = size;
		if (part == 0) {
			x_ed /= 2;
			y_ed /= 2;
		}
		else if (part == 1) {
			x_st = size / 2;
			y_ed /= 2;
		}
		else if (part == 2) {
			y_st = size / 2;
			x_ed /= 2;
		}
		else {
			x_st = size / 2;
			y_st = size / 2;
		}
		for (int i = y_st; i < y_ed; ++i) {
			for (int j = x_st; j < x_ed; ++j) {

				if (grass[i][j] != ' ') {
					grass[i][j] = ' ';
					++grass_destroyed;
				}
				if (preds[i][j].alive) {
					preds[i][j] = entity();
					++preds_die;
				}
				if (herbs[i][j].alive) {
					herbs[i][j] = entity();
					++herbs_die;
				}

			}
		}
		++cataclism_count;
	}
}

bool checkTheEnd(vector<vector<entity>>& preds, vector<vector<entity>>& herbs) {
	for (int i = 0; i < preds.size(); ++i) {
		for (int j = 0; j < preds.size(); ++j) {
			if (preds[i][j].alive || herbs[i][j].alive) {
				return false;
			}
		}
	}
	return true;
}

string printStats() {
	string s = "Статистика\n";
	s += "Хищников родилось - " + to_string(preds_born) + '\n';
	s += "Хищников умерло - " + to_string(preds_die) + '\n';
	s += '\n';
	s += "Травоядных родилось - " + to_string(herbs_born) + '\n';
	s += "Травоядных умерло - " + to_string(herbs_born) + '\n';
	s += '\n';
	s += "Травы сгенерировано - " + to_string(grass_generated) + '\n';
	s += "Травы уничтожено - " + to_string(grass_destroyed) + '\n';
	s += '\n';
	s += "Травоядных съедено - " + to_string(herbs_eaten) + '\n';
	s += "Травы съедено - " + to_string(grass_eaten) + '\n';
	s += "Супер-травы съедено - " + to_string(herbs_super_eaten) + '\n';
	s += '\n';
	s += "Хищников остановлено - " + to_string(preds_stopped) + '\n';
	s += '\n';
	s += "Количество катаклизмов - " + to_string(cataclism_count) + '\n';

	return s;
}

void animals(int size = 10, int dur = 40, int pred_cnt = 5, int pred_age = 18, int pred_start = 3, int pred_end = 10,
	int pred_born = 65, int pred_hung = 5, int herb_cnt = 20, int herb_age = 12, int herb_start = 2,
	int herb_end = 9, int herb_born = 100, int herb_hung = 5, int grass_rec = 100, int storm_chanse = 50, int curseason = 0) {
	const string season[4] = { "Весна", "Лето", "Осень", "Зима" };
	cout << "Установленные параметры:\n";
	cout << "Размер квадратного поля: " << size * size << endl;
	cout << "Количество лет симуляции: " << dur << endl;
	cout << "Среднее количество травы: " << grass_rec << endl;
	cout << "Шанс катаклизма: " << storm_chanse << endl;
	cout << "Исходное время года: " << season[curseason] << endl;

	cout << "\nХищники:\n";
	cout << "Начальное кол-во хищников: " << pred_cnt << endl;
	cout << "Максимальный возраст хищников: " << pred_age << endl;
	cout << "Возраст начала и конца репродукции хищников: с " << pred_start << " до " << pred_end << endl;
	cout << "Шанс рождения хищника: " << pred_born << "%" << endl;
	cout << "Необходимое насыщение для хищника: " << pred_hung << endl;

	cout << "\nТравоядные:\n";
	cout << "Начальное кол-во травоядных: " << herb_cnt << endl;
	cout << "Максимальный возраст травоядных: " << herb_age << endl;
	cout << "Возраст начала и конца репродукции травоядных: с " << herb_start << " до " << herb_end << endl;
	cout << "Шанс рождения травоядного: " << herb_born << "%" << endl;
	cout << "Необходимое насыщение для трявоядного: " << herb_hung << endl;

	random_device rd;
	mt19937 gen(rd());

	vector<vector<entity>> preds(size, vector<entity>(size, entity()));
	vector<vector<entity>> herbs(size, vector<entity>(size, entity()));

	preds = generate(preds, herbs, pred_cnt, pred_hung);
	herbs = generate(herbs, preds, herb_cnt, herb_hung);

	vector<vector<char>> field(size, vector<char>(size, ' '));
	field = grass(field, preds, grass_rec);

	ofstream logs("logs.txt");
	logs << "Начальные условия:\n";
	logs << show(preds, herbs, field);

	int def_grass_rec = grass_rec;
	int def_pred_born = pred_born;
	int def_herb_born = herb_born;
	--curseason;
	for (int yr = 1; yr <= dur; ++yr) {
		for (int mnth = 1; mnth <= 12; ++mnth) {
			if ((mnth - 1) % 3 == 0) {
				++curseason;
				if (curseason == 4) {
					curseason = 0;
				}
				switch (curseason) {
				case 0: // Весна
					grass_rec = def_grass_rec;
					pred_born = def_pred_born + 5;
					herb_born = def_herb_born + 5;
				case 1: // Лето
					grass_rec = (def_grass_rec * 4) / 5;
					pred_born = def_pred_born - 10;
					herb_born = def_herb_born - 10;
				case 2: // Осень
					grass_rec = (def_grass_rec * 9) / 10;
					pred_born = def_pred_born;
					herb_born = def_herb_born;
				case 3: // Зима
					grass_rec = (def_grass_rec * 2) / 10;
					pred_born = def_pred_born / 2;
					herb_born = def_herb_born / 2;
				}
			}
			hunt(preds, herbs);
			eat(herbs, field);


			die(herbs, herb_age, herb_hung, false);

			preds = go(preds, herbs, field, true);
			herbs = go(herbs, preds, field, false);

			born(preds, herbs, field, true, pred_hung / 2, pred_start, pred_end, pred_born);
			born(herbs, preds, field, false, herb_hung / 2, herb_start, herb_end, herb_born);

			field = grass(field, preds, grass_rec);

			cataclism(preds, herbs, field, storm_chanse);

			logs << "Год: " << yr << ", Месяц: " << mnth << ", Время года: " << season[curseason] << '\n';
			logs << show(preds, herbs, field);
			if (checkTheEnd(preds, herbs)) {
				logs << "Все популяции существ вымерли, конец симуляции\n";
				logs << printStats();
				logs.close();
				system("pause");
				system("clr");
				printToConsole((yr - 1) * 12 + mnth, size);
				return;
			}
		}
		plusage(preds);
		plusage(herbs);
		die(preds, pred_age, pred_hung, true);
	}
	logs << "Симуляция завершена\n";
	logs << printStats();
	logs.close();
	system("pause");
	system("clr");
	printToConsole(dur * 12, size);
}



int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Введите режим определение начальных параметров (0 - автоматические параметры, 1 - ручная установка): ";
	int mode = ichek(0, 1);
	if (mode == 0) {
		animals();
	}
	else {
		int m = 100;

		cout << "Введите ширину поля: ";
		int size = ichek(1, m);
		cout << "Введите количество лет симуляции: ";
		int dur = ichek(1, m);
		cout << "Введите начальное количество хищников: ";
		int pred_cnt = ichek(0, size * size);
		cout << "Введите максимальный возраст хищников: ";
		int pred_age = ichek(1, m);
		cout << "Введите минимальный репродуктивный возраст хищников: ";
		int pred_start = ichek(0, m);
		cout << "Введите максимальный репродуктивный возраст хищников: ";
		int pred_end = ichek(pred_start, m);
		cout << "Введите шанс рождаемости хищников (от 0% до 100%): ";
		int pred_born = ichek(0, 100);
		cout << "Введите количество травоядных, необходимых хищнику: ";
		int pred_hung = ichek(0, m);
		cout << "Введите начальное количество травоядных: ";
		int herb_cnt = ichek(0, size * size - pred_cnt);
		cout << "Введите максимальный возраст травоядных: ";
		int herb_age = ichek(1, m);
		cout << "Введите минимальный репродуктивный возраст травоядных: ";
		int herb_start = ichek(0, m);
		cout << "Введите максимальный репродуктивный возраст травоядных: ";
		int herb_end = ichek(herb_start, m);
		cout << "Введите шанс рождаемость травоядных (от 0% до 100%): ";
		int herb_born = ichek(0, 100);
		cout << "Введите количество травы, необходимой травоядному: ";
		int herb_hung = ichek(0, m);
		cout << "Введите количество восстанавливаемой в год травы: ";
		int grass_rec = ichek(0, size * size);
		cout << "Введите шанс на природный катаклизм в месяц (от 0 до 100%): ";
		int storm_chanse = ichek(0, 100);
		cout << "Введите начальное время года (весна - 0, лето - 1, осень - 2, зима - 3): ";
		int season = ichek(0, 3);
		animals(size, dur, pred_cnt, pred_age, pred_start, pred_end, pred_born, pred_hung, herb_cnt, herb_age,
			herb_start, herb_end, herb_born, herb_hung, grass_rec, storm_chanse, season);
	}
	return 0;
}