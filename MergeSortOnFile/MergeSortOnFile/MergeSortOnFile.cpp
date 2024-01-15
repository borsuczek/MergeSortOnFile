// projekt1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <cstdio>

#define MAX_SET_SIZE 15
#define DOUBLE_BYTES 8
#define SET_BYTES (MAX_SET_SIZE*DOUBLE_BYTES)
#define BUFOR_BYTES 720
#define BUFOR_SIZE 6



struct Set {
    double numbers[MAX_SET_SIZE];
    double sum = 0;

    void sumNumbers() {
        sum = 0;
        for (int i = 0; i < MAX_SET_SIZE; i++) {
            sum += numbers[i];
        }
    }
};



struct InOutTape {
    Set bufor[BUFOR_SIZE];
    int actual = 0;
    int file_place = 0;
    std::string file_name = "file.txt";
    bool eof = false;
    int amount = 0;
    bool end = false;
    bool show_phase = false;
    int reading = 0;

    void read() {
        std::fstream file(file_name);
        file.seekg(file_place);
        actual = 0;
        reading++;
        for (int i = 0; i < BUFOR_SIZE; i++) {
            for (int j = 0; j < MAX_SET_SIZE; j++) {
                file >> bufor[i].numbers[j];
                if (file.eof()) {
                    end = true;
                    amount = i;
                    if (i == 0)
                        eof = true;
                    file.close();
                    return;
                }

                if (show_phase)
                    std::cout << bufor[i].numbers[j] << ' ';
            }
            bufor[i].sumNumbers();
            if (show_phase)
                std::cout << ", suma liczb ze zbioru: " << bufor[i].sum << std::endl;
        }
        file_place = file.tellg();
        file.close();
    }

    Set getRecord() {
        if (eof == true) {
            Set empty;
            empty.sum = nan("");
            return empty;
        }
        actual++;
        if (end) {
            amount--;
            if (amount == 0)
                eof = true;
        }
        if (actual == BUFOR_SIZE) {
            Set end_record = bufor[actual - 1];
            read();
            return end_record;
        }
        return bufor[actual - 1];
    }
};


struct Tape {
    Set bufor[BUFOR_SIZE];
    int actual = 0;
    int tape_file_place = 0;
    std::string file_name;
    int writting = 0;

    void add(Set record) {
        bufor[actual] = record;
        actual++;
        if (actual == BUFOR_SIZE) {
            emptyBufor();
        }
    }

    void emptyBufor() {
        if (actual > 0) {
            std::fstream tape_file(file_name);
            tape_file.seekg(tape_file_place);

            writting++;
            for (int i = 0; i < BUFOR_SIZE; i++) {
                if (actual > 0) {
                    for (int j = 0; j < MAX_SET_SIZE; j++) {
                        tape_file << bufor[i].numbers[j] << ' ';
                    }
                    if (i != BUFOR_SIZE - 1 && actual != 1)
                        tape_file << std::endl;
                }
                actual--;
            }

            tape_file_place = tape_file.tellg();
            tape_file.close();
            actual = 0;
        }
    }
};

void addToTapes(bool& first_record, Tape& tape1, Tape& tape2, Set record, double last_sum, bool& is_tape1) {
    if (first_record) {
        first_record = false;
        tape1.add(record);
    }
    else {
        if (record.sum < last_sum)
            is_tape1 = !is_tape1;
        if (is_tape1 == true)
            tape1.add(record);
        else
            tape2.add(record);
    }
}

void addRecordToTape(int& written_record_tape, Set& left_record, Set tape1_record, Set tape2_record, Tape& tape3, bool& finish) {
    int tape = 1;
    if (left_record.sum <= tape1_record.sum) {
        if (tape1_record.sum <= tape2_record.sum)
            tape = 1;
        else
            tape = 2;
    }
    else if (left_record.sum <= tape2_record.sum) {
        finish = false;
        tape = 2;
    }
    else {
        finish = false;
        if (tape1_record.sum <= tape2_record.sum)
            tape = 1;
        else
            tape = 2;
    }
    if (tape == 1) {
        tape3.add(tape1_record);
        left_record = tape1_record;
    }
    else {
        tape3.add(tape2_record);
        left_record = tape2_record;
        if (written_record_tape == 1) {
            written_record_tape = 2;
        }
        else {
            written_record_tape = 1;
        }
    }
}

bool merge(std::string file_name, int& reading, int& writting) {
    std::ofstream file(file_name);
    InOutTape tape1;
    tape1.file_name = "tape1.txt";
    InOutTape tape2;
    tape2.file_name = "tape2.txt";
    Tape tape3;
    tape3.file_name = file_name;
    bool first_record = true;
    int written_record_tape = 1;
    Set left_record;
    Set tape1_record;
    Set tape2_record;
    bool finish = true;
    tape1.read();
    tape2.read();

    while (true) {
        if (first_record) {
            tape1_record = tape1.getRecord();
            tape2_record = tape2.getRecord();
        }
        else if (written_record_tape == 1) {
            if (tape1_record.sum == tape1_record.sum)
                tape1_record = tape1.getRecord();
            else
                tape2_record = tape2.getRecord();
        }
        else {
            if (tape2_record.sum == tape2_record.sum)
                tape2_record = tape2.getRecord();
            else
                tape1_record = tape1.getRecord();
        }
        if (tape1_record.sum != tape1_record.sum && tape2_record.sum != tape2_record.sum)
            break;
        if (tape1_record.sum != tape1_record.sum && tape2_record.sum == tape2_record.sum) {
            if (first_record) {
                first_record = false;
                written_record_tape = 2;
                left_record = tape2_record;
            }
            if (left_record.sum > tape2_record.sum)
                finish = false;
            tape3.add(tape2_record);
            left_record = tape2_record;
        }
        else if (tape2_record.sum != tape2_record.sum && tape1_record.sum == tape1_record.sum) {
            if (first_record) {
                first_record = false;
                written_record_tape = 1;
                left_record = tape1_record;
            }
            if (left_record.sum > tape1_record.sum)
                finish = false;
            tape3.add(tape1_record);
            left_record = tape1_record;
        }
        else {
            if (first_record) {
                left_record = tape1_record;
                addRecordToTape(written_record_tape, left_record, tape1_record, tape2_record, tape3, finish);
                first_record = false;
                finish = true;
            }
            else {
                if (written_record_tape == 1) {
                    addRecordToTape(written_record_tape, left_record, tape1_record, tape2_record, tape3, finish);
                }
                else {
                    addRecordToTape(written_record_tape, left_record, tape2_record, tape1_record, tape3, finish);
                }
            }
        }
    }
    tape3.emptyBufor();

    writting += tape3.writting;
    reading += tape1.reading;
    reading += tape2.reading;

    return finish;
}

void sortScheme21(std::string file_name, bool show_phases) {
    bool first_phase = true;
    const  char* tape1_file = "tape1.txt";
    const char* tape2_file = "tape2.txt";
    int phase = 0;
    int writting = 0;
    int reading = 0;
    while (true) {
        std::ofstream file1(tape1_file);
        std::ofstream file2(tape2_file);
        InOutTape tape3;
        tape3.file_name = file_name;
        Tape tape1;
        tape1.file_name = tape1_file;
        Tape tape2;
        tape2.file_name = tape2_file;
        bool eof = 0;
        bool first_record = true;
        bool is_tape1 = true;
        Set last_record;

        if (first_phase) {
            std::cout << "Plik poczatkowy:" << std::endl;
            tape3.show_phase = true;
        }
        else {
            if (show_phases)
                std::cout << "Po " << phase << " fazie:" << std::endl;
            tape3.show_phase = show_phases;
        }
        tape3.read();

        eof = tape3.eof;
        while (!eof) {
            Set record = tape3.getRecord();
            addToTapes(first_record, tape1, tape2, record, last_record.sum, is_tape1);
            last_record = record;
            eof = tape3.eof;
        }
        tape1.emptyBufor();
        tape2.emptyBufor();

        writting += tape1.writting;
        writting += tape2.writting;

        reading += tape3.reading;

        first_phase = false;
        phase++;

        bool finish = merge(file_name, reading, writting);

        if (finish)
            break;
    }
    std::cout << "Posortowany plik:" << std::endl;
    InOutTape tape3;
    tape3.file_name = file_name;
    tape3.show_phase = true;
    tape3.read();
    while (!tape3.eof)
        Set record = tape3.getRecord();
    std::cout << "Liczba faz: " << phase << std::endl;
    std::cout << "Liczba zapisow: " << writting << std::endl;
    std::cout << "Liczba odczytow: " << reading << std::endl;
    remove(tape1_file);
    remove(tape2_file);

}


void createRandom(int records_amount, double min, double max) {
    std::ofstream file("file.txt");
    for (int i = 0; i < records_amount; i++) {
        for (int j = 0; j < MAX_SET_SIZE; j++) {
            double tmp = (rand() / (double)RAND_MAX) * (max - min) + min;
            file << tmp << " ";

        }
        if (i != records_amount - 1)
            file << std::endl;
    }
    file.close();

}

void random() {
    bool ok = false;
    int records_amount = 0;
    double min, max;
    while (!ok) {
        if (records_amount == 0) {
            std::cout << "Podaj ile rekordow chcesz utworzyc" << std::endl;
            std::cin >> records_amount;
            if (records_amount <= 0)
                std::cout << "Ilosc rekordow musi byc wieksza od 0" << std::endl;
        }
        else {
            std::cout << "Podaj zakres losowanych liczb" << std::endl;
            std::cin >> min >> max;
            if (min > max) {
                std::cout << "Pierwsza liczba musi by� wieksza od drugiej" << std::endl;
            }
            else
                ok = true;
        }
    }
    createRandom(records_amount, min, max);
}

std::string readFromFile() {
    bool ok = false;
    std::string file_name;
    while (!ok) {
        std::cout << "Podaj nazwe pliku ktory chcesz posortowac" << std::endl;
        std::cin >> file_name;
        std::ifstream file(file_name);
        if (!file.good())
            std::cout << "Plik nie istnieje" << std::endl;
        else {
            file.close();
            ok = true;
        }
    }
    return file_name;
}

void readFromConsole(std::string file_name) {
    std::cout << "Wpisz rekordy (na koncu nalezy wstawic s�owo koniec w nowej linii)" << std::endl;
    std::string line;
    std::ofstream file(file_name);
    while (true) {
        std::getline(std::cin, line);
        if (line != "koniec")
            file << line << std::endl;
        else
            break;
    }
}

void menu(std::string& file_name, bool& show_phases) {
    bool ok = false;
    std::string number;
    while (!ok) {
        std::cout << "Wybierz tryb" << std::endl << "1 - generuj randomowe liczby" << std::endl;
        std::cout << "2 - wczytaj liczby z pliku" << std::endl << "3 - wczytaj liczby z konsoli" << std::endl;
        std::cin >> number;
        if (number != "1" && number != "2" && number != "3") {
            std::cout << "Wybrany tryb nie istnieje" << std::endl;
        }
        else
            ok = true;
    }
    if (number == "1")
        random();
    else if (number == "2")
        file_name = readFromFile();
    else
        readFromConsole(file_name);
    std::cout << "Czy chcesz wyswietlac plik po kazdej fazie sortowania? [tak/nie]" << std::endl;
    ok = false;
    while (!ok) {
        std::string show;
        std::cin >> show;
        if (show != "tak" && show != "nie")
            std::cout << "Niepoprawna odpowiedz" << std::endl;
        else {
            if (show == "tak")
                show_phases = true;
            else
                show_phases = false;
            ok = true;
        }
    }
}

int main()
{
    srand(time(NULL));
    std::string file_name = "file.txt";
    bool show_phases;
    menu(file_name, show_phases);
    sortScheme21(file_name, show_phases);

}