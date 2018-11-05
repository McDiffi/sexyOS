/*
* Tutaj znajdują się zrobione przeze mnie testy dla poszczególnych funkcjonalności
* Trzeba pamiętać, że jeśli plik jest usuwany to w pamięci dysku zostaje, znika
  tylko z wektora bitowego i tablicy z powiązaniami
* test statycznego obiektu należy uruchomić 2 razy
 */

#include "FileManager.h"
#include <iomanip>
#include <iostream>

using namespace std;

bool messages = true;
bool detailedMessages = true;

void displayFunctionOutcome(const bool &outcome) {
	std::cout << (outcome ? "Operacja powiodła się." : "Operacja nie powiodła się.") << '\n';
}

//File Create, Delete, Truncate
void test1() {
	FileManager FM;
	FM.Messages(messages);
	FM.DetailedMessages(detailedMessages);

	displayFunctionOutcome(FM.FileCreate("plik1.txt", string("TekstTekst Tekst./*+$łążśyłóżźówa")));
	cout << '\n';

	//cout << "--- Disk Content (bit) ---\n";
	//FM.DisplayDiskContentBinary();
	cout << "--- Disk Content (char) ---\n";
	FM.DisplayDiskContentChar();
	cout << '\n';

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("plik2.txt", string("TekstTekst Tekst./*+$łążśyłóżźówa")));
	cout << '\n';

	displayFunctionOutcome(FM.DisplayFileInfo("plik1.txt"));
	cout << '\n';

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	displayFunctionOutcome(FM.FileDelete("plik1.txt"));
	cout << '\n';

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	cout << "--- Struktura Katalogów ---\n";
	FM.DisplayDirectoryStructure();
	cout << '\n';
}

//Directories
void test2() {
	FileManager FM;
	FM.Messages(messages);
	FM.DetailedMessages(detailedMessages);

	displayFunctionOutcome(FM.DirectoryCreate("Podkatalog0"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryCreate("Podkatalog0"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryCreate("Podkatalog1"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryCreate("Podkatalog2"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryCreate("Podkatalog3"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryDown("Katalog"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryDown("Podkatalog2"));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("Plik1.txt", "Dane Tekstowe"));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("Plik2.txt", "Dane Tekstowe"));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("Plik3.txt", "Dane Tekstowe"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryUp());
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryUp());
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryDown("Podkatalog2"));
	displayFunctionOutcome(FM.DirectoryCreate("Dir1"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryDown("Dir1"));
	displayFunctionOutcome(FM.DirectoryCreate("Dir2"));
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryDown("Dir2"));
	displayFunctionOutcome(FM.FileCreate("Plikt.txt", "Dane Tekstowe"));
	cout << '\n';

	FM.DirectoryRoot();
	displayFunctionOutcome(FM.FileCreate("Plik1.txt", "Dane Tekstowe"));
	displayFunctionOutcome(FM.FileCreate("Plik2.txt", "Dane Tekstowe Bardzo Długie i Nudne, Żeby Tylko Były"));
	cout << '\n';

	displayFunctionOutcome(FM.DisplayFileInfo("Plik2.txt"));
	cout << '\n';

	displayFunctionOutcome(FM.FileDelete("NoName.txt"));
	cout << '\n';

	displayFunctionOutcome(FM.DisplayDirectoryInfo("Podkatalog2"));
	cout << '\n';

	cout << "--- Struktura Katalogów ---\n";
	FM.DisplayDirectoryStructure();
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryRename("Podkatalog2", "Qatalog"));
	cout << '\n';

	cout << "--- Struktura Katalogów ---\n";
	FM.DisplayDirectoryStructure();
	cout << '\n';

	displayFunctionOutcome(FM.DirectoryDelete("Podkatalog2"));
	cout << '\n';

	cout << "--- Struktura Katalogów ---\n";
	FM.DisplayDirectoryStructure();
}

//Space fill and file delete
void test3() {
	FileManager FM;
	FM.Messages(messages);
	FM.DetailedMessages(detailedMessages);

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';
	displayFunctionOutcome(FM.FileCreate("plik1.txt", string("TekstTekst Tekst./*+$łążśyłóżźówa")));
	displayFunctionOutcome(FM.FileCreate("plik2.txt", string("Blablablablablablablablabla")));
	displayFunctionOutcome(FM.FileCreate("plik3.txt", string("Sratatatatatata")));
	displayFunctionOutcome(FM.FileDelete("plik2.txt"));
	displayFunctionOutcome(FM.FileCreate("plik2.txt", string("TekstTekst Tekst./*+$łążśyłóżźówaaaaaaaaaaaaa")));
	displayFunctionOutcome(FM.FileCreate("plik4.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik5.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik6.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik7.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik8.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik9.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik10.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik11.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik12.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik13.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik14.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik15.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik16.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));
	displayFunctionOutcome(FM.FileCreate("plik17.txt", string("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW")));

	cout << '\n';
	cout << "--- Disk Content (char) ---\n";
	FM.DisplayDiskContentChar();
	cout << '\n';
	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	cout << "--- Params ---\n";
	FM.DisplayFileSystemParams();
	cout << '\n';

	cout << "--- Struktura Katalogów ---\n";
	FM.DisplayDirectoryStructure();
}

//Allocation test
void test4() {
	//Test przygotowany dla bloków o rozmiarze 32 bajt
	FileManager FM;
	FM.Messages(messages);
	FM.DetailedMessages(detailedMessages);

	displayFunctionOutcome(FM.FileCreate("plik1.txt", string("Plik1Plik1Plik1Plik1Plik1Plik1")));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("plik2.txt", string("Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2Plik2")));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("plik3.txt", string("Plik3Plik3")));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("plik4.txt", string("Plik4Plik4Plik4Plik4Plik4Plik4Plik4")));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("plik5.txt", string("Plik5Plik5Plik5Plik5")));
	cout << '\n';

	cout << "--- Disk Content (char) ---\n";
	FM.DisplayDiskContentChar();
	cout << '\n';

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	displayFunctionOutcome(FM.FileDelete("plik2.txt"));
	cout << '\n';

	displayFunctionOutcome(FM.FileDelete("plik4.txt"));
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("plik6.txt", string("Plik6Plik6Plik6Plik6Plik6Plik6Plik6")));
	cout << '\n';

	cout << "--- Disk Content (char) ---\n";
	FM.DisplayDiskContentChar();
	cout << '\n';

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	displayFunctionOutcome(FM.FileSaveData("plik5.txt", "Plik5Plik5Plik5Plik5Plik5Plik5"));
	cout << '\n';
	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	displayFunctionOutcome(FM.FileSaveData("plik3.txt", "Plik3Plik3Plik3Plik3Plik3Plik3"));
	cout << '\n';

	displayFunctionOutcome(FM.DisplayFileInfo("plik3.txt"));
	cout << '\n';

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
	cout << '\n';

	displayFunctionOutcome(FM.FileCreate("plik7.txt", string("Plik1Plik1Plik1Plik1")));
	cout << '\n';

	displayFunctionOutcome(FM.FileSaveData("plik1.txt", "Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1Plik1"));
	cout << '\n';

	displayFunctionOutcome(FM.DisplayFileInfo("plik1.txt"));
	cout << '\n';

	cout << "--- Bit Vector ---\n";
	FM.DisplayBitVector();
}

//Rename test
void test5() {
	FileManager FM;
	FM.Messages(messages);
	FM.DetailedMessages(detailedMessages);

	displayFunctionOutcome(FM.FileCreate("plik1.txt", string("Tekst")));
	displayFunctionOutcome(FM.FileCreate("plik2.txt", string("Tekst")));
	displayFunctionOutcome(FM.FileCreate("plik3.txt", string("Tekst")));
	cout << '\n';
	displayFunctionOutcome(FM.DisplayFileInfo("plik1.txt"));
	cout << '\n';
	cout << "--- Struktura Katalogów ---\n";
	FM.DisplayDirectoryStructure();
	cout << '\n';

	displayFunctionOutcome(FM.FileRename("plik1.txt", "Awesome.txt"));
	displayFunctionOutcome(FM.FileRename("plik2.txt", "Super.txt"));
	displayFunctionOutcome(FM.FileRename("plik3.txt", "Fantastic.txt"));
	cout << '\n';
	displayFunctionOutcome(FM.DisplayFileInfo("plik1.txt"));
	cout << '\n';
	displayFunctionOutcome(FM.DisplayFileInfo("Awesome.txt"));
	cout << '\n';
	cout << "--- Struktura Katalogów ---\n";
	FM.DisplayDirectoryStructure();
	cout << '\n';
}

//Static class object test
void test6() {
	//Za drugim wywołaniem testu w pamięci powinny być już zapisane dane oraz nazwa pliku będzie już zajęta.
	fileManager.Messages(messages);
	fileManager.DetailedMessages(detailedMessages);

	cout << "--- Disk Content (char) ---\n";
	fileManager.DisplayDiskContentChar();
	cout << '\n';

	displayFunctionOutcome(fileManager.FileCreate("Plik1", "DataSrata"));
	cout << '\n';

	cout << "--- Disk Content (char) ---\n";
	fileManager.DisplayDiskContentChar();
	cout << '\n';
}

//Serializer test
void test7() {
	std::cout << "--- Int To String ---\n";
	std::cout << 2735 << " --> " << Serializer::IntToString(2735) << '\n'; //Ciąg znaków zaczynający się na 'ą'

	std::cout << "--- Int To String ---\n";
	std::cout << -2735 << " --> " << Serializer::IntToString(-2735) << '\n';

	std::cout << "--- String to Int ---\n";
	std::cout << "\"˙˙˙˙˙˙˙˙˙˙\" --> " << Serializer::StringToInt("˙˙˙˙˙˙˙˙˙˙") << '\n'; //Liczba 2550

	//Test czy konwersja + konwersja zwrotna zachowują poprawne dane
	const unsigned int number = 2590;
	std::cout << "--- Two-way check ---\n";
	std::cout << "Given number: " << number << '\n';
	std::cout << "Converted to string: " << Serializer::IntToString(number) << '\n';
	std::cout << "String converted to int: " << Serializer::StringToInt(Serializer::IntToString(number)) << '\n';
}

int main() {
	const vector<string>desc{ "File Create, Delete, Truncate", "Directories", "Space fill and file delete", "Allocation test", "Rename test", "Static class object test", "Serializer test" };

	const auto run = [](void fun()) {
		system("cls");
		fun();
		cout << '\n';
		system("pause");
		system("cls");
	};

	system("chcp 1250");
	system("cls");
	const vector<void(*)()>functions{ &test1,&test2,&test3,&test4,&test5, &test6, &test7 };

	while (true) {
		//Elementy stałe
		cout << "Messages: " << (messages ? "Enabled" : "Disabled") << '\n';
		cout << "Detailed messages: " << (detailedMessages ? "Enabled" : "Disabled") << "\n\n";

		fileManager.DisplayFileSystemParams();
		cout << "\n\n";

		cout << setfill('0') << setw(2) << -2 << ". " << "Enable/Disable Detailed Messages" << '\n';
		cout << setfill('0') << setw(2) << -1 << ". " << "Enable/Disable Messages" << '\n';
		cout << setfill('0') << setw(2) << 0 << ". " << "Exit" << "\n\n";

		//Wyświetla numery i opisy poszczególnych testów
		for (unsigned int i = 0; i < desc.size(); i++) {
			cout << setfill('0') << setw(2) << i + 1 << ". " << desc[i] << '\n';
		}
		cout << "\nPodaj numer(-2 do " << functions.size() << "): ";

		int testNum;
		cin >> testNum;

		//Włączanie/wyłączanie wiadomości
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
