#include <iostream>
#include <windows.h>
#include <random>
#include <fstream>
#include <string>

using namespace std;

int predsBorn = 0;
int predsDead = 0;
int predsStopped = 0;

int herbsBorn = 0;
int herbsDead = 0;
int herbsEaten = 0;
int herbsSuperEaten = 0;

int grassGenerated = 0;
int grassDestroyed = 0;
int grassEaten = 0;

int cataclysmCount = 0;

//#define DBLUE 1
//#define DGREEN 2
//#define DLBLUE 3
//#define DRED 4
//#define DPINK 5
//#define DYELLOW 6
//#define DEFAULT 7
//#define GRAY 8
//#define BLUE 9
//#define GREEN 10
//#define LBLUE 11
//#define RED 12
//#define PINK 13
//#define YELLOW 14
//#define WHITE 15
//
//void print(string text, int color = DEFAULT) {
//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(hConsole, color);
//	cout << text;
//	SetConsoleTextAttribute(hConsole, DEFAULT);
//}
//
//void println(string text, int color = DEFAULT) {
//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(hConsole, color);
//	cout << text << endl;
//	SetConsoleTextAttribute(hConsole, DEFAULT);
//}

int intInput(int mn = INT_MIN, int mx = INT_MAX, const string& message = "") {
	int digits_c, var;
	string s;
	bool isnum;
	do {
		digits_c = 0;
		isnum = true;
		cout << message;
		getline(cin, s);
		if ('0' <= s[0] && s[0] <= '9') {
			digits_c++;
		}
		else if (!(s[0] == '+' || s[0] == '-')) {
			cout << "Вы ввели не целое число, а что-то другое! Повторите ввод: ";
			isnum = false;
			continue;
		}

		for (int i = 1; i < s.length(); i++) {
			if (s[i] >= '0' && s[i] <= '9') {
				digits_c++;
			}
			else {
				cout << "Вы ввели не целое число, а что-то другое! Повторите ввод: ";
				isnum = false;
				break;
			}
		}
		if (!isnum) continue;

		if (digits_c == 0) {
			cout << "Вы ввели не целое число, а что-то другое! Повторите ввод: ";
			isnum = false;
			continue;
		}
		try {
			var = stoi(s);
		}
		catch (out_of_range const& e) {
			cout << "Вы ввели слишком большое число для типа int! Повторите ввод: ";
			isnum = false;
			continue;
		}
		if (var < mn || var > mx) cout << "Вы должны ввести число в отрезке ["<< mn << "; "<< mx << "]. Повторите ввод: ";
	} while (!isnum || var > mx || var < mn);
	return var;
}

struct animal {
	bool alive = 0;
	int age = 0;
	int food = 0;
};

vector<vector<animal>> generate(vector<vector<animal>> firstSpecies, vector<vector<animal>> secondSpecies, int count, int food) {
	random_device rd;
	mt19937 gen(rd());
	int size = firstSpecies.size();
	int x;
	int y;
	while (count > 0) {
		x = gen() % size;
		y = gen() % size;
		if (!(firstSpecies[y][x].alive) && !(secondSpecies[y][x].alive)) {
			firstSpecies[y][x].alive = 1;
			firstSpecies[y][x].food = food;
			--count;
		}
	}
	return firstSpecies;
}

vector<vector<char>> grassGrowth(vector<vector<char>> grass, vector<vector<animal>> predators, int rec) {
	random_device rd;
	mt19937 gen(rd());
	int size = grass.size();

	int super = rec * (1 + (gen() % 20)) / 70;
	int hard = rec * (1 + (gen() % 20)) / 70;

	int attempts = rec * 2;
	int attempts_s = super * 2;
	int attempts_h = hard * 2;

	int x;
	int y;
	while (rec > 0 && attempts != 0) {
		x = gen() % size;
		y = gen() % size;
		if (grass[y][x] == ' ' && !(predators[y][x].alive)) {
			grass[y][x] = '^';
			--rec;
			++grassGenerated;
		}
		else {
			--attempts;
		}
	}
	while (super > 0 && attempts_s != 0) {
		x = gen() % size;
		y = gen() % size;
		if (!(predators[y][x].alive)) {
			grass[y][x] = '$';
			--super;
			++grassGenerated;
		}
		else {
			--attempts_s;
		}
	}

	while (hard > 0 && attempts_h != 0) {
		x = gen() % size;
		y = gen() % size;
		if (grass[y][x] == ' ' && !(predators[y][x].alive)) {
			grass[y][x] = '&';
			--hard;
			++grassGenerated;
		}
		else {
			--attempts_h;
		}
	}
	return grass;
}

vector<vector<animal>> movement(vector<vector<animal>> firstSpecies, vector<vector<animal>> secondSpecies, vector<vector<char>>& grass, bool isPredator) {
	random_device rd;
	mt19937 gen(rd());
	int len = firstSpecies.size();
	vector<vector<animal>> newOne(len, vector<animal>(len, animal()));
	int x;
	int y;
	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			if (firstSpecies[i][j].alive) {
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
					if (i == len - 1) {
						y = -abs(y);
					}
					if (j == len - 1) {
						x = -abs(x);
					}
					if (!newOne[i + y][j + x].alive && !firstSpecies[i + y][j + x].alive && !secondSpecies[i + y][j + x].alive) {
						if (isPredator && grass[i + y][j + x] == '&') {
							grass[i + y][j + x] = ' ';
							++predsStopped;
							break;
						}
						newOne[i + y][j + x] = firstSpecies[i][j];
						if (isPredator && grass[i + y][j + x] != ' ') {
							grass[i + y][j + x] = ' ';
							++grassDestroyed;
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

void death(vector<vector<animal>>& firstSpecies, float maxAge, float needFood, bool isPredator) {
	random_device rd;
	mt19937 gen(rd());
	int len = firstSpecies.size();
	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			if (firstSpecies[i][j].alive) {
				int chance = ((maxAge-firstSpecies[i][j].age) / maxAge) * 100 * (firstSpecies[i][j].food / needFood) * 100 + sqrt(maxAge*2);
				int rand = gen() % 100;
				if (rand > chance) {
					firstSpecies[i][j] = animal();
					if (isPredator) {
						++predsDead;
					}
					else
					{
						++herbsDead;
					}
				}
			}
		}
	}
}

void predatorsEat(vector<vector<animal>>& predators, vector<vector<animal>>& herbivores) {
	int len = predators.size();
	for (int y = 0; y < len; ++y) {
		for (int x = 0; x < len; ++x) {
			if (predators[y][x].alive) {
				vector<vector<int>> coord;
				int y_st = y - 1, y_ed = y + 1;
				int x_st = x - 1, x_ed = x + 1;

				if (y == 0) {
					y_st = y;
				}
				if (x == 0) {
					x_st = x;
				}
				if (y == len - 1) {
					y_ed = y;
				}
				if (x == len - 1) {
					x_ed = x;
				}
				for (int i = y_st; i <= y_ed; ++i) {
					for (int j = x_st; j <= x_ed; ++j) {
						if (herbivores[i][j].alive) {
							coord.push_back({ i,j });
						}
					}
				}
				if (coord.size() != 0) {
					random_device rd;
					mt19937 gen(rd());
					int ch = gen() % coord.size();
					herbivores[coord[ch][0]][coord[ch][1]] = animal();
					++predators[y][x].food;
					++herbsEaten;
				}
			}
		}
	}
}

void herbivoresEat(vector<vector<animal>>& herbivores, vector<vector<char>>& grass) {
	int len = herbivores.size();
	for (int y = 0; y < len; ++y) {
		for (int x = 0; x < len; ++x) {
			if (herbivores[y][x].alive) {
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
				if (y == len - 1) {
					y_ed = y;
				}
				if (x == len - 1) {
					x_ed = x;
				}
				for (int i = y_st; i <= y_ed; ++i) {
					for (int j = x_st; j <= x_ed; ++j) {
						if (grass[i][j] == '$') {
							super.push_back({ i,j });
						}
						if (grass[i][j] == '^') {
							norm.push_back({ i,j });
						}
					}
				}
				if (super.size() != 0) {
					random_device rd;
					mt19937 gen(rd());
					int ch = gen() % super.size();
					grass[super[ch][0]][super[ch][1]] = ' ';
					herbivores[y][x].food += 3;
					++herbsSuperEaten;
				}
				else if (norm.size() != 0) {
					random_device rd;
					mt19937 gen(rd());
					int ch = gen() % norm.size();
					grass[norm[ch][0]][norm[ch][1]] = ' ';
					++herbivores[y][x].food;
					++grassEaten;
				}
			}
		}
	}
}

void mating(vector<vector<animal>>& firstSpecies, vector<vector<animal>>& secondSpecies, vector<vector<char>>& grass, bool isPredator,
	int mh, int minAge, int maxAge, int chance) {
	random_device rd;
	mt19937 gen(rd());
	int len = firstSpecies.size();
	for (int y = 0; y < len; ++y) {
		for (int x = 0; x < len; ++x) {
			if (firstSpecies[y][x].alive && minAge <= firstSpecies[y][x].age && firstSpecies[y][x].age <= maxAge) {
				vector<vector<int>> coord;
				bool pair = false;
				int count = 0;
				int y_st = y - 1, y_ed = y + 1;
				int x_st = x - 1, x_ed = x + 1;

				if (y == 0) {
					y_st = y;
				}
				if (x == 0) {
					x_st = x;
				}
				if (y == len - 1) {
					y_ed = y;
				}
				if (x == len - 1) {
					x_ed = x;
				}
				for (int i = y_st; i <= y_ed; ++i) {
					for (int j = x_st; j <= x_ed; ++j) {
						if (!secondSpecies[i][j].alive && !firstSpecies[i][j].alive) {
							coord.push_back({ i,j });
						}
						else {
							if (firstSpecies[i][j].alive && minAge <= firstSpecies[i][j].age && firstSpecies[i][j].age <= maxAge && !(y == i && x == j)) {
								pair = true;
								++count;
							}
							else if (firstSpecies[i][j].alive || secondSpecies[i][j].alive) {
								++count;
							}
						}
					}
				}
				int rand = gen() % 100;
				if (coord.size() != 0 && pair && count == 2 && rand <= chance) {
					int ch = gen() % coord.size();
					firstSpecies[coord[ch][0]][coord[ch][1]].alive = true;
					firstSpecies[coord[ch][0]][coord[ch][1]].food = mh;
					if (isPredator) {
						grass[coord[ch][0]][coord[ch][1]] = ' ';
						++predsBorn;
					}
					else {
						++herbsBorn;
					}
				}
			}
		}
	}
}

void addAges(vector<vector<animal>>& firstSpecies) {
	for (int i = 0; i < firstSpecies.size(); ++i) {
		for (int j = 0; j < firstSpecies.size(); ++j) {
			if (firstSpecies[i][j].alive) {
				++firstSpecies[i][j].age;
				firstSpecies[i][j].food /= 2;
			}
		}
	}
}

bool cataclysm(vector<vector<animal>>& predators, vector<vector<animal>>& herbivores, vector<vector<char>>& grass, int isdeath) {
	random_device rd;
	mt19937 gen(rd());
	int chanse = gen() % 100;
	if (isdeath > chanse) {
		int len = predators.size();
		for (int i = 0; i < len; ++i) {
			for (int j = 0; j < len; ++j) {
				bool isDead = gen()%2;
				if (isDead) {
					if (grass[i][j] != ' ') {
						grass[i][j] = ' ';
						++grassDestroyed;
					}
					else if (predators[i][j].alive) {
						predators[i][j] = animal();
						++predsDead;
					}
					else if (herbivores[i][j].alive) {
						herbivores[i][j] = animal();
						++herbsDead;
					}
				}
			}
		}
		++cataclysmCount;
		return true;
	}
	return false;
}

bool checkTheEnd(vector<vector<animal>>& predators, vector<vector<animal>>& herbivores) {
	for (int i = 0; i < predators.size(); ++i) {
		for (int j = 0; j < predators.size(); ++j) {
			if (predators[i][j].alive || herbivores[i][j].alive) {
				return false;
			}
		}
	}
	return true;
}

string printWorld(vector<vector<animal>> predators, vector<vector<animal>> herbivores, vector<vector<char>> grass) {
	int len = grass.size();
	string s = " " + string(len * 2 + 1, '_') + "\n";
	for (int i = 0; i < len; ++i) {
		s += "|";
		for (int j = 0; j < len; ++j) {
			if (predators[i][j].alive) {
				s += " Х";
			}
			else if (herbivores[i][j].alive) {
				s += " Т";
			}
			else if (grass[i][j] != ' ') {
				s += " " + string(1, grass[i][j]);
			}
			else {
				s += "  ";
			}
		}
		s += " |\n";
	}
	s += " " + string(len * 2 + 1, '-');
	return s + "\n\n";
}

string printStats() {
	string s = "Статистика\n";
	s += "Хищников родилось - " + to_string(predsBorn) + '\n';
	s += "Хищников умерло - " + to_string(predsDead) + '\n';
	s += '\n';
	s += "Травоядных родилось - " + to_string(herbsBorn) + '\n';
	s += "Травоядных умерло - " + to_string(herbsBorn) + '\n';
	s += '\n';
	s += "Травы сгенерировано - " + to_string(grassGenerated) + '\n';
	s += "Травы уничтожено - " + to_string(grassDestroyed) + '\n';
	s += '\n';
	s += "Травоядных съедено - " + to_string(herbsEaten) + '\n';
	s += "Травы съедено - " + to_string(grassEaten) + '\n';
	s += "Супер-травы съедено - " + to_string(herbsSuperEaten) + '\n';
	s += '\n';
	s += "Хищников остановлено - " + to_string(predsStopped) + '\n';
	s += '\n';
	s += "Количество катаклизмов - " + to_string(cataclysmCount) + '\n';

	return s;
}

string printParametres(int worldSize, int generations, int countOfPreds, int predMaxAge, int predMaturity, int predAging,
	int predBornChance, int predNeedFood, int countOfHerbs, int herbMaxAge, int herbMaturity,
	int herbAging, int herbBornChance, int herbNeedFood, int grassRecovery, int cataclysmChance, string season) {
	string s = "Установленные параметры:\n";
	s += "Размеры квадратного поля: " + to_string(worldSize) + "x" + to_string(worldSize) + " клеток\n";
	s += "Количество лет симуляции: " + to_string(generations) + '\n';
	s += "Среднее количество травы: " + to_string(grassRecovery) + '\n';
	s += "Шанс катаклизма: " + to_string(cataclysmChance) + '\n';
	s += "Исходное время года: " + season + '\n';

	s += "\nХищники:\n";
	s += "Начальное кол-во хищников: " + to_string(countOfPreds) + '\n';
	s += "Максимальный возраст хищников: " + to_string(predMaxAge) + '\n';
	s += "Возраст начала и конца репродукции хищников: с " + to_string(predMaturity) + " до " + to_string(predAging) + '\n';
	s += "Шанс рождения хищника: " + to_string(predBornChance) + '\n';
	s += "Необходимое насыщение для хищника: " + to_string(predNeedFood) + '\n';

	s += "\nТравоядные:\n";
	s += "Начальное кол-во травоядных: " + to_string(countOfHerbs) + '\n';
	s += "Максимальный возраст травоядных: " + to_string(herbMaxAge) + '\n';
	s += "Возраст начала и конца репродукции травоядных: с " + to_string(herbMaturity) + " до " + to_string(herbAging) + '\n';
	s += "Шанс рождения травоядного: " + to_string(herbBornChance) + '\n';
	s += "Необходимое насыщение для травоядного: " + to_string(herbNeedFood) + '\n';
	return s;
}

void printToConsole(int generations, int size) {
	size += 2;
	ifstream wo("outputFile.txt");
	string buff;
	system("cls");
	string text = "";
	for (int i = 0; i < 20; i++) {
		getline(wo, buff);
	}
	for (int j = 0; j <= size + 1; j++) {
		getline(wo, buff);
		text += buff;
		text.push_back('\n');
	}
	cout << text;
	Sleep(3000);
	for (int i = 0; i < generations; i++) {
		system("cls");
		text = "";
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
		}
		else {
			cout << text;
			Sleep(250);
		}
	}
	system("cls");
	text = "";
	while (getline(wo, buff)) {
		text += buff;
		text.push_back('\n');
	}
	cout << text;
	system("pause");
}

void simulate(int worldSize = 30, int generations = 40, int countOfPreds = 50, int predMaxAge = 18, int predMaturity = 3, int predAging = 14,
	int predBornChance = 60, int predNeedFood = 4, int countOfHerbs = 150, int herbMaxAge = 14, int herbMaturity = 2,
	int herbAging = 12, int herbBornChance = 90, int herbNeedFood = 6, int grassRecovery = 125, int cataclysmChance = 1, int curSeason = 0) {
	const string seasons[4] = { "Весна", "Лето", "Осень", "Зима" };

	string parametres = printParametres(worldSize,generations,countOfPreds,predMaxAge,predMaturity,predAging,predBornChance,predNeedFood,countOfHerbs, herbMaxAge, herbMaturity, herbAging, herbBornChance, herbNeedFood, grassRecovery, cataclysmChance, seasons[curSeason]);
	cout << parametres << endl;
	cout << "\nДождитесь просчета симуляции...\n";
	random_device rd;
	mt19937 gen(rd());

	vector<vector<animal>> predators(worldSize, vector<animal>(worldSize, animal()));
	vector<vector<animal>> herbivores(worldSize, vector<animal>(worldSize, animal()));

	predators = generate(predators, herbivores, countOfPreds, predNeedFood);
	herbivores = generate(herbivores, predators, countOfHerbs, herbNeedFood);

	vector<vector<char>> grass(worldSize, vector<char>(worldSize, ' '));
	grass = grassGrowth(grass, predators, grassRecovery*3);

	ofstream outputFile("outputFile.txt");
	outputFile << parametres;
	outputFile << "Начальные условия:\n";
	outputFile << printWorld(predators, herbivores, grass);

	int defGrassRecovery = grassRecovery;
	int defPredBornChance = predBornChance;
	int defHerbBornChance = herbBornChance;
	--curSeason;
	for (int year = 1; year <= generations; ++year) {
		for (int month = 1; month <= 12; ++month) {
			if ((month - 1) % 3 == 0) {
				++curSeason;
				if (curSeason == 4) {
					curSeason = 0;
				}
				switch (curSeason) {
				case 0: // Весна
					grassRecovery = defGrassRecovery * 2;
					predBornChance = defPredBornChance - 20;
					herbBornChance = defHerbBornChance + 10;
				case 1: // Лето
					grassRecovery = defGrassRecovery;
					predBornChance = defPredBornChance;
					herbBornChance = defHerbBornChance - 10;
				case 2: // Осень
					grassRecovery = (defGrassRecovery * 4) / 5;
					predBornChance = defPredBornChance + 10;
					herbBornChance = defHerbBornChance;
				case 3: // Зима
					grassRecovery = defGrassRecovery / 5;
					predBornChance = defPredBornChance / 2;
					herbBornChance = defHerbBornChance / 2;
				}
			}
			predatorsEat(predators, herbivores);
			herbivoresEat(herbivores, grass);


			death(herbivores, herbMaxAge, herbNeedFood, false);

			predators = movement(predators, herbivores, grass, true);
			herbivores = movement(herbivores, predators, grass, false);

			mating(predators, herbivores, grass, true, predNeedFood / 5, predMaturity, predAging, predBornChance);
			mating(herbivores, predators, grass, false, herbNeedFood / 5, herbMaturity, herbAging, herbBornChance);

			grass = grassGrowth(grass, predators, grassRecovery);

			int isCataclysm = cataclysm(predators, herbivores, grass, cataclysmChance);

			outputFile << "Год: " << year << ", Месяц: " << month << ", Время года: " << seasons[curSeason] << '\n';
			outputFile << printWorld(predators, herbivores, grass);
			if (checkTheEnd(predators, herbivores)) {
				outputFile << "Все популяции существ вымерли, конец симуляции\n";
				outputFile << printStats();
				outputFile.close();
				system("pause");
				system("clr");
				printToConsole((year - 1) * 12 + month, worldSize);
				return;
			}
		}
		addAges(predators);
		addAges(herbivores);
		death(predators, predMaxAge, predNeedFood, true);
	}
	outputFile << "Симуляция завершена\n";
	outputFile << printStats();
	outputFile.close();
	system("pause");
	system("clr");
	printToConsole(generations * 12, worldSize);
}

int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Программа симуляции жизни популяции хищников и популяции травоядных.\n";
	cout << "Если хотите запустить симуляцию с параметрами по умолчанию - введите 0\n";
	cout << "Если хотите установить параметры вручную - введите 1\n";
	cout << "Введите режим определение начальных параметров: ";
	int choice = intInput(0, 1);
	cout << endl;
	if (choice == 0) {
		simulate();
	}
	else {
		cout << "Введите ширину квадратного поля: ";
		int worldSize = intInput(1, 100);
		int maxAges = 1000;
		cout << "Введите количество лет симуляции: ";
		int generations = intInput(1, maxAges);
		cout << "Введите начальное количество травоядных: ";
		int countOfHerbs = intInput(0, worldSize * worldSize);
		cout << "Введите максимальный возраст травоядных: ";
		int herbMaxAge = intInput(1, maxAges);
		cout << "Введите минимальный репродуктивный возраст травоядных: ";
		int herbMaturity = intInput(0, maxAges - 1);
		cout << "Введите максимальный репродуктивный возраст травоядных: ";
		int herbAging = intInput(herbMaturity, maxAges);
		cout << "Введите шанс рождаемость травоядных (от 0% до 100%): ";
		int herbBornChance = intInput(0, 100);
		cout << "Введите количество травы, необходимой травоядному: ";
		int herbNeedFood = intInput(0, 100);
		cout << "Введите начальное количество хищников: ";
		int countOfPreds = intInput(0, worldSize * worldSize - countOfHerbs);
		cout << "Введите максимальный возраст хищников: ";
		int predMaxAge = intInput(1, maxAges);
		cout << "Введите минимальный репродуктивный возраст хищников: ";
		int predMaturity = intInput(0, maxAges-1);
		cout << "Введите максимальный репродуктивный возраст хищников: ";
		int predAging = intInput(predMaturity, maxAges);
		cout << "Введите шанс рождаемости хищников (от 0% до 100%): ";
		int predBornChance = intInput(0, 100);
		cout << "Введите количество травоядных, необходимых хищнику: ";
		int predNeedFood = intInput(0, 100);
		cout << "Введите количество восстанавливаемой в год травы: ";
		int grassRecovery = intInput(0, worldSize * worldSize);
		cout << "Введите шанс на природный катаклизм в месяц (от 0 до 100%): ";
		int cataclysmChance = intInput(0, 100);
		cout << "Введите начальное время года (весна - 0, лето - 1, осень - 2, зима - 3): ";
		int season = intInput(0, 3);
		simulate(worldSize, generations, countOfPreds, predMaxAge, predMaturity, predAging, predBornChance, predNeedFood, countOfHerbs, herbMaxAge,
			herbMaturity, herbAging, herbBornChance, herbNeedFood, grassRecovery, cataclysmChance, season);
	}
	return 0;
}