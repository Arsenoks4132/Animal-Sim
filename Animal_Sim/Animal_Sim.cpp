#include <iostream>
#include <windows.h>
#include <random>
#include <fstream>
#include <string>

using namespace std;

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


vector<vector<entity>> generate(int size, int cnt, int hung) {
    random_device rd;
    mt19937 gen(rd());
    int x;
    int y;
    entity ent;
    vector<vector<entity>> field(size, vector<entity>(size, ent));
    while (cnt > 0) {
        x = gen() % size;
        y = gen() % size;
        if (! field[y][x].alive) {
            field[y][x].alive = 1;
            field[y][x].hung = hung;
            --cnt;
        }
    }
    return field;
}


vector<vector<char>> grass(vector<vector<char>> field, vector<vector<entity>> preds, int rec) {
    random_device rd;
    mt19937 gen(rd());
    int size = field.size();
    int super = rec / 100 * (gen() % 10);
    int hard = rec / 100 * (gen() % 10);
    int x;
    int y;
    while (rec > 0) {
        x = gen() % size;
        y = gen() % size;
        if (field[y][x] == ' ' && !(preds[y][x].alive)) {
            field[y][x] = '.';
            --rec;
        }
    }
    while (super > 0) {
        x = gen() % size;
        y = gen() % size;
        if (field[y][x] == ' ' && !(preds[y][x].alive)) {
            field[y][x] = '.';
            --super;
        }
    }
    return field;
}


string show(vector<vector<char>> vect) {
    string s = "";
    int msize = vect.size();
    for (int i = 0; i < msize; ++i) {
        for (int j = 0; j < msize; ++j) {
            s += " ";
            s += string(1, vect[i][j]);
        }
        s += "\n";
    }
    return s + "\n";
}

void animals(int size = 50, int dur = 30, int pred_cnt = 100, int pred_age = 15, int pred_start = 3, int pred_end = 10,
    int pred_born = 65, int pred_hung = 5, int herb_cnt = 100, int herb_age = 15, int herb_start = 2,
    int herb_end = 8, int herb_born = 85, int herb_hung = 5, int grass_rec = 100, int storm_chanse = 1) {
    random_device rd;
    mt19937 gen(rd());

    vector<vector<entity>> preds = generate(size, pred_cnt, pred_hung / 2);
    vector<vector<entity>> herbs = generate(size, herb_cnt, herb_hung / 2);

    vector<vector<char>> field(size, vector<char>(size, ' '));
    field = grass(field, preds, grass_rec);

    ofstream logs("logs.txt");
    logs << "Начальные условия:\n";
    logs << show(field);

    for (int yr = 1; yr <= dur; ++yr) {
        for (int mnth = 1; mnth <= 12; ++mnth) {
            
        }
    }







    logs.close();
}






int main() {
    setlocale(LC_ALL, "Russian");
    animals();

   /* cout << "Введите режим определение начальных параметров (0 - автоматические параметры, 1 - ручная установка): ";
    int mode = ichek(0, 1);
    if (mode == 0) {
        animals();
    }
    else {
        int m = 10000;

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
        cout << "Введите шанс рождаемости хищников (от 0 до 100%): ";
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
        cout << "Введите шанс рождаемость травоядных (от 0 до 100%): ";
        int herb_born = ichek(0, 100);
        cout << "Введите количество травы, необходимой травоядному: ";
        int herb_hung = ichek(0, m);
        cout << "Введите количество восстанавливаемой в год травы: ";
        int grass_rec = ichek(0, size * size);
        cout << "Введите шанс на природный катаклизм в месяц (от 0 до 100%): ";
        int storm_chanse = (0, 100);
        animals(size, dur, pred_cnt, pred_age, pred_start, pred_end, pred_born, pred_hung, herb_cnt, herb_age,
            herb_start, herb_end, herb_born, herb_hung, grass_rec, storm_chanse);
    }*/
    return 0;
}