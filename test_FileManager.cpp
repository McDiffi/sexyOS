/*
* Tutaj znajdują się zrobione przeze mnie testy dla poszczególnych funkcjonalności.
* Trzeba pamiętać, że jeśli plik jest usuwany to w pamięci dysku zostają dane. Znikają
  tylko zajęte bloki z wektora bitowego
* test statycznego obiektu należy uruchomić 2 razy
 */

#include "FileManager.h"
#include <iomanip>
#include <iostream>

using namespace std;

bool messages = true;
bool detailedMessages = true;
bool operationStatus = true;

void displayFunctionOutcome(const bool &outcome) {
	if (operationStatus) { std::cout << (outcome ? "Operacja powiodła się." : "Operacja nie powiodła się.") << "\n\n"; }
}

//File Create, Delete
void test1() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	displayFunctionOutcome(FM.file_create("plik1", string("TekstTekst Tekst./*+$łążśyłóżźówa")));

	cout << "--- Disk Content (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_create("plik2", string("TekstTekst Tekst./*+$łążśyłóżźówa")));

	displayFunctionOutcome(FM.display_file_info("plik1"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_delete("plik1"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	cout << "--- Katalog Główny ---\n";
	FM.display_root_directory();
	cout << '\n';
}

//Space fill and file delete
void test2() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';
	displayFunctionOutcome(FM.file_create("plik1", string("TekstTekst Tekst./*+$łążśyłóżźówa")));
	displayFunctionOutcome(FM.file_create("plik2", string("Blablablablablablablablabla")));
	displayFunctionOutcome(FM.file_create("plik3", string("Sratatatatatata")));
	displayFunctionOutcome(FM.file_delete("plik2"));
	displayFunctionOutcome(FM.file_create("plik2", string("TekstTekst Tekst./*+$łążśyłóżźówaaaaaaaaaaaaa")));
	displayFunctionOutcome(FM.file_create("plik4", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWaWWWWWWWWWWWWWWWWWwwwwwwwwwwwwwwwwWWWWWWWWWWWWaWWWWWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.file_create("plik5", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWm")));
	displayFunctionOutcome(FM.file_create("plik6", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWl")));
	displayFunctionOutcome(FM.file_create("plik7", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWk")));
	displayFunctionOutcome(FM.file_create("plik8", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWj")));
	displayFunctionOutcome(FM.file_create("plik9", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWi")));
	displayFunctionOutcome(FM.file_create("plik10", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWh")));
	displayFunctionOutcome(FM.file_create("plik11", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWg")));
	displayFunctionOutcome(FM.file_create("plik12", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWf")));
	displayFunctionOutcome(FM.file_create("plik13", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWe")));
	displayFunctionOutcome(FM.file_create("plik14", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWd")));
	displayFunctionOutcome(FM.file_create("plik15", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWc")));
	displayFunctionOutcome(FM.file_create("plik16", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWb")));
	displayFunctionOutcome(FM.file_create("plik17", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWa")));

	cout << '\n';
	displayFunctionOutcome(FM.display_file_info("plik4"));
	cout << '\n';

	cout << "--- Disk Content (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';
	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	cout << "--- Params ---\n";
	FM.display_file_system_params();
	cout << '\n';

	cout << "--- Katalog Główny ---\n";
	FM.display_root_directory();
}

//Allocation test
void test3() {

	//Test przygotowany dla bloków o rozmiarze 32 bajt
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	displayFunctionOutcome(FM.file_create("plik1", string("Plik1Plik1Plik1Plik1Plik1Plik1")));

	displayFunctionOutcome(FM.file_create("plik2", string("Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2")));

	displayFunctionOutcome(FM.file_create("plik3", string("Plik3Plik3")));

	displayFunctionOutcome(FM.file_create("plik4", string("Plik4Plik4Plik4Plik4Plik4Plik4Plik4")));

	displayFunctionOutcome(FM.file_create("plik5", string("Plik5Plik5Plik5Plik5")));

	cout << "--- Disk Content (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_delete("plik2"));

	displayFunctionOutcome(FM.file_delete("plik4"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_create("plik6", string("Plik6Plik6Plik6Plik6Plik6Plik6Plik6")));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	cout << "--- Disk Content (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	displayFunctionOutcome(FM.file_write("plik5", "Plik5Plik5Plik5Plik5Plik5Plik5"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_write("plik3", "Plik3Plik3Plik3Plik3Plik3Plik3"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.display_file_info("plik3"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_create("plik7", string("Plik1Plik1Plik1Plik1")));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_write("plik1", "Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.display_file_info("plik1"));
}

//Rename test
void test4() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	displayFunctionOutcome(FM.file_create("plik1", string("Tekst")));
	displayFunctionOutcome(FM.file_create("plik2", string("Tekst")));
	displayFunctionOutcome(FM.file_create("plik3", string("Tekst")));

	displayFunctionOutcome(FM.display_file_info("plik1"));

	cout << "--- Katalog Główny ---\n";
	FM.display_root_directory();
	cout << '\n';

	displayFunctionOutcome(FM.file_rename("plik1", "Awesome"));
	displayFunctionOutcome(FM.file_rename("plik2", "Super"));
	displayFunctionOutcome(FM.file_rename("plik3", "Fantastic"));

	displayFunctionOutcome(FM.display_file_info("plik1"));

	displayFunctionOutcome(FM.display_file_info("Awesome"));

	cout << "--- Katalog Główny ---\n";
	FM.display_root_directory();
	cout << '\n';
}

//Static class object test
void test5() {
	std::cout << "Za drugim wywołaniem testu w pamięci powinny być już zapisane dane oraz nazwa pliku będzie już zajęta.\n";
	fileManager.set_messages(messages);
	fileManager.set_detailed_messages(detailedMessages);

	cout << "--- Disk Content (char) ---\n";
	fileManager.display_disk_content_char();
	cout << '\n';

	displayFunctionOutcome(fileManager.file_create("Plik1", "DataSrata"));

	cout << "--- Disk Content (char) ---\n";
	fileManager.display_disk_content_char();
	cout << '\n';
}

//File Open/Close
void test6() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	displayFunctionOutcome(FM.file_create("plik1", string("Tekst")));

	displayFunctionOutcome(FM.file_open("plik1"));

	displayFunctionOutcome(FM.file_close("plik1"));
}

int main() {
	const vector<string>desc{ "File Create, Delete", "Space fill and file delete", "Allocation test", "Rename test", "Static FileManager object test","File Open/Close" };

	const auto run = [](void fun()) {
		system("cls");
		fun();
		cout << '\n';
		system("pause");
		system("cls");
	};

	system("chcp 1250");
	system("cls");
	const vector<void(*)()>functions{ &test1,&test2,&test3,&test4,&test5, &test6 };

	while (true) {
		//Elementy stałe
		cout << "Messages: " << (messages ? "Enabled" : "Disabled") << '\n';
		cout << "Detailed Messages: " << (detailedMessages ? "Enabled" : "Disabled") << "\n";
		cout << "Operation Status: " << (operationStatus ? "Enabled" : "Disabled") << "\n\n";

		fileManager.display_file_system_params();
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
			if (messages) {
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
