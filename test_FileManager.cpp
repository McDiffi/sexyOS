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

void displayFunctionOutcome(const int &outcome) {
	if (operationStatus) {
		if(outcome == FILE_ERROR_NONE) { std::cout << "Operacja powiodla sie!\n"; }
		else if (outcome == FILE_ERROR_EMPTY_NAME) { std::cout << "Pusta nazwa!\n"; }
		else if (outcome == FILE_ERROR_NAME_TOO_LONG) { std::cout << "Nazwa za dluga!\n"; }
		else if (outcome == FILE_ERROR_NAME_USED) { std::cout << "Nazwa zajeta!\n"; }
		else if (outcome == FILE_ERROR_NO_INODES_LEFT) { std::cout << "Osiagnieto limit plikow!\n"; }
		else if (outcome == FILE_ERROR_DATA_TOO_BIG) { std::cout << "Dane za duze!\n"; }
		else if (outcome == FILE_ERROR_NOT_FOUND) { std::cout << "Nie znaleziono pliku!\n"; }
		else if (outcome == FILE_ERROR_NOT_OPENED) { std::cout << "Plik nie jest otwarty!\n"; }
		else if (outcome == FILE_ERROR_NOT_R_MODE) { std::cout << "Plik nie jest do odczytu!\n"; }
		else if (outcome == FILE_ERROR_NOT_W_MODE) { std::cout << "Plik nie jest do zapisu!\n"; }
		else { std::cout << "Nie obsluzony blad: " << outcome << "\n"; }
		std::cout << '\n';
	}
}

//File create, delete
void test1() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	displayFunctionOutcome(FM.file_create("plik1", string("TekstTekst Tekst./*+$łążśyłóżźówa")));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_create("plik2", string("TekstTekst Tekst./*+$łążśyłóżźówa")));
	displayFunctionOutcome(FM.file_write("plik2", string("END")));

	cout << "--- Zawartosc Dysku (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	displayFunctionOutcome(FM.display_file_info("plik1"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_close("plik1"));
	displayFunctionOutcome(FM.file_delete("plik1"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	cout << "--- Katalog Glowny ---\n";
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

	displayFunctionOutcome(FM.file_close("plik2"));
	displayFunctionOutcome(FM.file_delete("plik2"));

	displayFunctionOutcome(FM.file_create("plik2", string("TekstTekst Tekst./*+$łążśyłóżźówaaaaaaaaaaaaa")));
	displayFunctionOutcome(FM.file_create("plik4", string("STARTWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWaWWWWWWWWWWWWWWWWWwwwwwwwwwwwwwwwwWWWWWWWWWWWWaWWWWWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWWWWWWasdjaskdjsdhasjdhasjkhasdasdasdjhas12234567890sdfghjkldasdsdfghjkl;trfdeysgygysdgas675as6d5ty1ehgshda.adhghgdEND")));
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

	cout << "--- Zawartosc Dysku (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';
	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	cout << "--- Parametry ---\n";
	FileManager::display_file_system_params();
	cout << '\n';

	cout << "--- Katalog Glowny ---\n";
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

	displayFunctionOutcome(FM.file_create("plik4", string("Plik4Plik4Plik4Plik4Plik4Plik4Plik4Plik4")));

	displayFunctionOutcome(FM.file_create("plik5", string("Plik5Plik5Plik5Plik5")));

	cout << "--- Zawartosc Dysku (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_close("plik2"));
	displayFunctionOutcome(FM.file_delete("plik2"));

	displayFunctionOutcome(FM.file_close("plik4"));
	displayFunctionOutcome(FM.file_delete("plik4"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_create("plik6", string("Plik6Plik6Plik6Plik6Plik6Plik6Plik6")));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	cout << "--- Zawartosc Dysku (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	displayFunctionOutcome(FM.file_write("plik5", "Plik5Plik5Plik5Plik5Plik5Plik5Plik5Plik5Plik5Plik5"));

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_write("plik3", "Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3Plik3"));

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

	cout << "--- Katalog Glowny ---\n";
	FM.display_root_directory();
	cout << '\n';

	displayFunctionOutcome(FM.file_rename("plik1", "Awesome"));
	displayFunctionOutcome(FM.file_rename("plik2", "Super"));
	displayFunctionOutcome(FM.file_rename("plik3", "Fantastic"));

	displayFunctionOutcome(FM.display_file_info("plik1"));

	displayFunctionOutcome(FM.display_file_info("Awesome"));

	cout << "--- Katalog Glowny ---\n";
	FM.display_root_directory();
	cout << '\n';
}

//File Open/Close
void test5() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	displayFunctionOutcome(FM.file_create("plik1", string("Tekst")));

	displayFunctionOutcome(FM.file_open("plik1", OPEN_R_MODE));

	displayFunctionOutcome(FM.file_write("plik1", "Wpis"));

	displayFunctionOutcome(FM.file_close("plik1"));

	displayFunctionOutcome(FM.file_create("plik10"));
	displayFunctionOutcome(FM.file_create("plik11"));
	displayFunctionOutcome(FM.file_create("plik12"));
	displayFunctionOutcome(FM.file_create("plik13"));
	displayFunctionOutcome(FM.file_create("plik14"));
	displayFunctionOutcome(FM.file_create("plik15"));
	displayFunctionOutcome(FM.file_create("plik16"));
	displayFunctionOutcome(FM.file_create("plik17"));
	displayFunctionOutcome(FM.file_create("plik18"));
	displayFunctionOutcome(FM.file_create("plik19"));
	displayFunctionOutcome(FM.file_create("plik20"));

	displayFunctionOutcome(FM.file_close_all());

	std::string result;
	FM.file_read_all("plik1", result);
	std::cout << result << '\n';
}

//Sequential Access test
void test6() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);
	std::string result;

	displayFunctionOutcome(FM.file_create("plik1", string("SrataHakunaMatataBatata")));
	std::cout << "Zapisano: " << "SrataHakunaMatataBatata" << '\n';
	displayFunctionOutcome(FM.file_open("plik1", OPEN_RW_MODE));
	FM.file_read("plik1", 5, result);
	std::cout << "Odczyt 5 bajt: " << result << "\n";
	FM.file_read("plik1", 6, result);
	std::cout << "Odczyt 5 bajt: " << result << "\n";
	FM.file_read("plik1", 6, result);
	std::cout << "Odczyt 6 bajt: " << result << "\n";
	FM.file_read("plik1", 6, result);
	std::cout << "Odczyt 6 bajt: " << result << "\n";
	FM.file_read("plik1", 666, result);
	std::cout << "Odczyt 666 bajt: " << "Thank you satan!\n";

	displayFunctionOutcome(FM.file_append("plik1", " The END"));
	displayFunctionOutcome(FM.display_file_info("plik1"));

}

//Write and append test
void test7() {
	FileManager FM;
	FM.set_messages(messages);
	FM.set_detailed_messages(detailedMessages);

	displayFunctionOutcome(FM.file_create("plik1", string("Plik1Plik1Plik1Plik1Plik1Plik1")));

	displayFunctionOutcome(FM.file_create("plik2", string("Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2")));

	displayFunctionOutcome(FM.file_create("plik3", string("Plik3Plik3")));

	cout << "--- Zawartosc Dysku (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.file_append("plik2", "AppendAppendAppendAppendAppendAppendAppendAppendAppendAppend"));

	cout << "--- Zawartosc Dysku (char) ---\n";
	FM.display_disk_content_char();
	cout << '\n';

	cout << "--- Wektor Bitowy ---\n";
	FM.display_bit_vector();
	cout << '\n';

	displayFunctionOutcome(FM.display_file_info("plik2"));
}

int main() {
	const vector<string>desc{ "File Create, Delete", "Space Fill and File Delete", "Allocation", "Rename",
							  "File Open/Close", "Sequential Access", "Write and append test" };

	const auto run = [](void fun()) {
		system("cls");
		fun();
		cout << '\n';
		system("pause");
		system("cls");
	};

	system("cls");
	const vector<void(*)()>functions{ &test1, &test2, &test3, &test4, &test5, &test6, &test7 };

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
