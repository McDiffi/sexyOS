/*
* Tutaj znajdują się zrobione przeze mnie testy dla poszczególnych funkcjonalności.
* Trzeba pamiętać, że jeśli plik jest usuwany to w pamięci dysku zostają dane. Znikają
  tylko zajęte bloki z wektora bitowego
* test statycznego obiektu należy uruchomić 2 razy
 */

#include "FileManager.h"
#include "MemoryManager.h"
#include "pipe.h"
#include <iomanip>
#include <iostream>
#include "Planista.h"
#include "Procesy.h"
#include "Interpreter.h"

using namespace std;

bool messages = true;
bool detailedMessages = true;
bool operationStatus = true;

const std::string procName = "process";

void displayFunctionOutcome(const int &outcome) {
	if (operationStatus) {
		if (outcome == FILE_ERROR_NONE) { std::cout << "Operacja powiodla sie!\n"; }
		else if (outcome == FILE_ERROR_EMPTY_NAME)		{ std::cout << "Pusta nazwa!\n"; }
		else if (outcome == FILE_ERROR_NAME_TOO_LONG)	{ std::cout << "Nazwa za dluga!\n"; }
		else if (outcome == FILE_ERROR_NAME_USED)		{ std::cout << "Nazwa zajeta!\n"; }
		else if (outcome == FILE_ERROR_NO_INODES_LEFT)	{ std::cout << "Osiagnieto limit plikow!\n"; }
		else if (outcome == FILE_ERROR_DATA_TOO_BIG)	{ std::cout << "Dane za duze!\n"; }
		else if (outcome == FILE_ERROR_NOT_FOUND)		{ std::cout << "Nie znaleziono pliku!\n"; }
		else if (outcome == FILE_ERROR_NOT_OPENED)		{ std::cout << "Plik nie jest otwarty!\n"; }
		else if (outcome == FILE_ERROR_OPENED)			{ std::cout << "Plik jest otwarty!\n"; }
		else if (outcome == FILE_ERROR_SYNC)			{ std::cout << "Semafor opuszczony!\n"; }
		else if (outcome == FILE_ERROR_NOT_R_MODE)		{ std::cout << "Plik nie jest do odczytu!\n"; }
		else if (outcome == FILE_ERROR_NOT_W_MODE)		{ std::cout << "Plik nie jest do zapisu!\n"; }
		else if (outcome == FILE_SYNC_WAITING)			{ std::cout << "Zasob zajety!\n"; }
		else { std::cout << "Nie obsluzony blad: " << outcome << "\n"; }
		std::cout << '\n';
	}
}


//System test
void test7() {
	MemoryManager mm;
	mm.memoryInit();
	Planista p;
	Pipeline pipy(nullptr);
	proc_tree tree(&mm, &p, &pipy);
	pipy.tree = &tree;
	FileManager fm(&p, &tree);
	Interpreter intr(&fm,&mm,&tree,&pipy);

	fm.set_messages(messages);
	fm.set_detailed_messages(detailedMessages);

	tree.fork(new PCB("proc1", 1), "program1.txt", 128);

	while(intr.execute_line("proc1")){}
}

int main() {
	const vector<string>desc{
		"Test systemu"
	};

	const auto run = [](void fun()) {
		system("cls");
		fun();
		cout << '\n';
		system("pause");
		system("cls");
	};

	system("cls");
	const vector<void(*)()>functions{ &test7 };

	while (true) {
		//Elementy stałe
		cout << "Messages: " << (messages ? "Enabled" : "Disabled") << '\n';
		cout << "Detailed Messages: " << (detailedMessages ? "Enabled" : "Disabled") << "\n";
		cout << "Operation Status: " << (operationStatus ? "Enabled" : "Disabled") << "\n\n";

		FileManager::display_file_system_params();
		cout << "\n\n";

		cout << setfill('0') << setw(2) << -3 << ". " << "Enable/Disable Operation Status" << '\n';
		cout << setfill('0') << setw(2) << -2 << ". " << "Enable/Disable Detailed Messages" << '\n';
		cout << setfill('0') << setw(2) << -1 << ". " << "Enable/Disable Messages" << '\n';
		cout << setfill('0') << setw(2) << 0 << ". " << "Exit" << "\n\n";

		//Wyświetla numery i opisy poszczególnych testów
		for (unsigned int i = 0; i < desc.size(); i++) {
			cout << setfill('0') << setw(2) << i + 1 << ". " << desc[i] << '\n';
		}
		cout << "\nPodaj numer(-3 do " << functions.size() << "): ";

		int testNum;
		cin >> testNum;

		//Włączanie/wyłączanie statusu operacji
		if (testNum == -3) {
			if (operationStatus) {
				std::cout << "\nOperation status disabled\n\n";
				system("pause");
				operationStatus = false;
			}
			else {
				std::cout << "\nOperation status enabled\n\n";
				system("pause");
				operationStatus = true;
			}
		}

		//Włączanie/wyłączanie szczegółowych wiadomości
		if (testNum == -2) {
			if (detailedMessages) {
				std::cout << "\nDetailed messages disabled\n\n";
				system("pause");
				detailedMessages = false;
			}
			else {
				std::cout << "\nDetailed messages enabled\n\n";
				system("pause");
				detailedMessages = true;
			}
		}

		//Włączanie/wyłączanie wiadomości
		if (testNum == -1) {
			if (messages) {
				std::cout << "\nMessages disabled\n\n";
				system("pause");
				messages = false;
			}
			else {
				std::cout << "\nMessages enabled\n\n";
				system("pause");
				messages = true;
			}
		}

		//Działa jeśli wybrano poprawny numer testu
		else if (static_cast<size_t>(testNum) >= 1 && static_cast<size_t>(testNum) <= functions.size()) {
			run(functions[testNum - 1]);
		}

		//Wyjście
		else if (testNum == 0) { break; }

		system("cls");
	}
}
