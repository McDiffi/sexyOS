/**
	SexyOS
	FileManager.cpp
	Przeznaczenie: Zawiera definicje metod i konstruktor�w dla klas z FileManager.h

	@author Tomasz Kilja�czyk
	@version 02/11/18
*/

#include "FileManager.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

const std::string Serializer::IntToString(unsigned int input) {
	std::string result;
	while (input != 0) {
		if (input % 255 == 0) {
			result.push_back(char(255));
			input -= 255;
		}
		else {
			result.push_back(input % 255);
			input -= input % 255;
		}
	}
	return result;
}
const unsigned Serializer::StringToInt(const std::string& input) {
	unsigned int result = 0;
	for (const char& c : input) {
		result += std::bitset<8>(c).to_ulong();
	}
	return result;
}

using u_int = unsigned int;

//Operator do wy�wietlania czasu z dat�
std::ostream& operator << (std::ostream& os, const tm& time) {
	os << time.tm_hour << ':' << std::setfill('0') << std::setw(2) << time.tm_min << ' ' << std::setfill('0') << std::setw(2) << time.tm_mday << '.'
		<< std::setfill('0') << std::setw(2) << time.tm_mon << '.' << time.tm_year;
	return os;
}
bool operator == (const tm& time1, const tm& time2) {
	if (time1.tm_hour == time2.tm_hour  &&
		time1.tm_isdst == time2.tm_isdst &&
		time1.tm_mday == time2.tm_mday  &&
		time1.tm_min == time2.tm_min   &&
		time1.tm_mon == time2.tm_mon   &&
		time1.tm_sec == time2.tm_sec   &&
		time1.tm_wday == time2.tm_wday  &&
		time1.tm_yday == time2.tm_yday  &&
		time1.tm_year == time2.tm_year) {
		return true;
	}
	else { return false; };
}

bool FileManager::Directory::operator==(const Directory& dir) const
{
	if (/*this->parentDirectory == dir.parentDirectory &&*/
		this->files == dir.files &&
		this->creationTime == dir.creationTime &&
		this->type == dir.type /*&&
		this->creator == dir.creator*/) {
		return true;
	}
	else { return false; }
}



//--------------------------- Dysk --------------------------

FileManager::Disk::Disk() {
	//Zape�nanie naszego dysku zerowymi bajtami (symbolizuje pusty dysk)
	fill(space.begin(), space.end(), NULL);
}

std::shared_ptr<FileManager::Index>& FileManager::IndexBlock::operator[](const size_t& index) {
	return this->block[index];
}

const std::shared_ptr<FileManager::Index>& FileManager::IndexBlock::operator[](const size_t& index) const {
	return this->block[index];
}

void FileManager::Disk::write(const u_int& begin, const u_int& end, const std::string& data) {
	//Indeks kt�ry b�dzie s�u�y� do wskazywania na kom�rki pami�ci
	u_int index = begin;
	//Iterowanie po danych typu string i zapisywanie znak�w na dysku
	for (u_int i = 0; i < data.size() && i <= end - begin; i++) {
		space[index] = data[i];
		index++;
	}
	//Zapisywanie NULL, je�li dane nie wype�ni�y ostatniego bloku
	for (; index <= end; index++) {
		space[index] = NULL;
	}
}

template<typename T>
const T FileManager::Disk::read(const u_int& begin, const u_int& end) const {
	//Dane
	T data;

	//Je�li typ danych to string
	if (typeid(T) == typeid(std::string)) {
		//Odczytaj przestrze� dyskow� od indeksu begin do indeksu end
		for (u_int index = begin; index <= end; index++) {
			//Dodaj znak zapisany na dysku do danych
			data += space[index];
		}
	}

	return data;
}


//----------------------- FileManager -----------------------

FileManager::FileManager() {
	//Przypisanie katalogu g��wnego do obecnego katalogu 
	currentDirectory = DISK.FileSystem.rootDirectory;
}



//-------------------- Podstawowe Metody --------------------

bool FileManager::FileCreate(const std::string& name, const std::string& data) {
	//Rozmiar pliku obliczony na podstawie podanych danych
	const u_int fileSize = CalculateNeededBlocks(data.size())*BLOCK_SIZE;

	if (DISK.FileSystem.InodeTable.size() < INODE_NUMBER_LIMIT) {
		//Je�li plik si� zmie�ci i nazwa nie u�yta
		if (CheckIfEnoughSpace(fileSize) && data.size() <= MAX_FILE_SIZE && CheckIfNameUnused(currentDirectory, name)) {
			//Je�li �cie�ka nie przekracza maksymalnej d�ugo�ci
			if (name.size() + GetCurrentPathLength() < MAX_PATH_LENGTH) {
				std::shared_ptr<File> file = std::make_shared<File>();

				//Zapisywanie daty stworzenia pliku
				file->creationTime = GetCurrentTimeAndDate();

				//Zapisanie danych pliku na dysku
				FileSaveData(file, data);

				//Dodanie pliku do obecnego katalogu
				DISK.FileSystem.InodeTable[GetCurrentPath() + name] = file;
				std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files[name] = GetCurrentPath() + name;

				if (messages) { std::cout << "Stworzono plik o nazwie '" << name << "' w �cie�ce '" << GetCurrentPath() << "'.\n"; }
				return true;
			}
			else { std::cout << "�cie�ka za d�uga!\n"; }
		}


		//Je�li plik si� nie mie�ci
		if (!CheckIfEnoughSpace(fileSize)) {
			std::cout << "Za ma�o miejsca!\n";
		}
		//Je�li plik jest wi�kszy ni� maksymalny dozwolony rozmiar
		if (data.size() > MAX_FILE_SIZE)
		{
			std::cout << "Rozmiar pliku wi�kszy ni� maksymalny dozwolony!\n";
		}
		//Je�li nazwa u�yta
		if (!CheckIfNameUnused(currentDirectory, name)) {
			std::cout << "Nazwa pliku '" << name << "' ju� zaj�ta!\n";
		}
	}
	else {
		std::cout << "Osi�gni�to limit element�w w �cie�ce '" << GetCurrentPath() << "'!\n";
	}

	return false;
}

//!!!!!!!!!! NIEDOKO�CZONE !!!!!!!!!!
//const std::string FileManager::FileOpen(const std::string& name) const {
//	return DISK.read<std::string>(0 * 8, 4 * 8 - 1);
//}
//!!!!!!!!!! NIEDOKO�CZONE !!!!!!!!!!

const std::string FileManager::FileGetData(const std::string& name) {
	//Iterator zwracany podczas przeszukiwania obecnego katalogu za plikiem o podanej nazwie
	const auto fileIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);

	//Je�li znaleziono i-w�ze�
	if (fileIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[fileIterator->second];
		if (inode->type == "FILE") {
			const std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(inode);
			return FileGetData(file);
		}
		else { std::cout << "Plik o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
	}
	else { std::cout << "Plik o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }

	return "";
}

const std::string FileManager::FileGetData(const std::shared_ptr<File>& file) const {
	//Dane
	std::string data = "";
	//Indeks do wczytywania danych z dysku
	size_t indexNumber = 0;
	size_t indexInBlockNumber = 0;
	std::shared_ptr<Index> index = std::make_shared<Index>();

	//Dop�ki nie natrafimy na koniec pliku
	while (index != nullptr && indexNumber) {
		if (indexNumber < BLOCK_DIRECT_INDEX_NUMBER) {
			index = file->directBlocks[indexNumber];
		}
		else if (file->directBlocks[indexNumber] != nullptr) {
			std::cout << "Index number: " << (indexNumber - BLOCK_DIRECT_INDEX_NUMBER) % BLOCK_INDEX_NUMBER << '\n';
			index = (*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER]))[(indexInBlockNumber) % BLOCK_INDEX_NUMBER];
			indexInBlockNumber++;
		}
		else { index = nullptr; }

		//Dodaje do danych fragment pliku pod wskazanym indeksem
		data += DISK.read<std::string>(index->value * BLOCK_SIZE, (index->value + 1)*BLOCK_SIZE - 1);
		//Przypisuje indeksowi kolejny indeks w tablicy FileSystem
		if (indexInBlockNumber % BLOCK_INDEX_NUMBER == 0) { indexNumber++; }
	}
	return data;
}

bool FileManager::FileDelete(const std::string& name) {
	//Iterator zwracany podczas przeszukiwania obecnego katalogu za plikiem o podanej nazwie
	const auto fileIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);

	//Je�li znaleziono i-w�ze�
	if (fileIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[fileIterator->second];
		if (inode->type == "FILE") {
			std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(inode);
			FileDelete(file);
			//Usu� wpis o pliku z obecnego katalogu
			std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.erase(fileIterator);

			if (messages) { std::cout << "Usuni�to plik o nazwie '" << name << "' znajduj�cy si� w �cie�ce '" + GetCurrentPath() + "'.\n"; }

			return true;
		}
		else { std::cout << "Plik o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
	}
	else { std::cout << "Plik o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }

	return false;
}

bool FileManager::FileSaveData(const std::string& name, const std::string& data) {
	if (data.size() <= MAX_FILE_SIZE) {
		//Iterator zwracany podczas przeszukiwania obecnego katalogu za plikiem o podanej nazwie
		const auto fileIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);

		//Je�li znaleziono i-w�ze�
		if (fileIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[fileIterator->second];
			if (inode->type == "FILE") {
				std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(inode);
				if (data.size() <= DISK.FileSystem.freeSpace - file->blocksOccupied*BLOCK_SIZE) {
					FileSaveData(file, data);
					return true;
				}
			}
			else { std::cout << "Plik o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
		}
		else { std::cout << "Plik o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
	}
	else { std::cout << "Podane dane przekraczaj� maksymalny rozmiar pliku!\n"; }
	return false;
}

void FileManager::FileTruncate(std::shared_ptr<File> file, const u_int& neededBlocks) {
	if (neededBlocks != file->blocksOccupied) {
		//Je�li nale�y zmniejszy� plik
		if (neededBlocks < file->blocksOccupied) { FileAllocationDecrease(file, neededBlocks); }
		//Je�li nale�y zwi�kszy� plik
		else if (neededBlocks > file->blocksOccupied) { FileAllocationIncrease(file, neededBlocks); }
	}
}

bool FileManager::DirectoryCreate(const std::string& name) {
	if (!name.empty()) {
		if (*(name.end() - 1) != '/') {
			if (DISK.FileSystem.InodeTable.size() < INODE_NUMBER_LIMIT) {
				//Je�li w katalogu nie istnieje podkatalog o podanej nazwie
				if (std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name) == std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
					//Je�li �cie�ka nie przekracza maksymalnej d�ugo�ci
					if (name.size() + GetCurrentPathLength() < MAX_PATH_LENGTH) {

						//Do iW�z��w w obecnym katalogu dodaj nowy podkatalog
						DISK.FileSystem.InodeTable[GetCurrentPath() + name] = std::make_shared<Directory>();

						std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files[name] = GetCurrentPath() + name;
						std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->creationTime = GetCurrentTimeAndDate();
						if (messages) { std::cout << "Stworzono katalog o nazwie '" << name << "' w �cie�ce '" << GetCurrentPath() << "'.\n"; }

						return true;
					}
					else { std::cout << "�cie�ka za d�uga!\n"; }
				}
				else { std::cout << "Nazwa katalogu '" << name << "' zaj�ta!\n"; }
			}
			else {
				std::cout << "Osi�gni�to limit element�w w �cie�ce '" << GetCurrentPath() << "'!\n";
			}
		}
		else { std::cout << "Podana niepoprawna nazwa!\n"; }
	}
	return false;
}

bool FileManager::DirectoryDelete(const std::string& name) {
	//Iterator zwracany podczas przeszukiwania obecnego katalogu za katalogiem o podanej nazwie
	const auto directoryIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);

	//Je�li znaleziono Inode
	if (directoryIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[directoryIterator->second];
		if (inode->type == "DIRECTORY") {
			//Wywo�aj funkcj� usuwania katalogu wraz z jego zawarto�ci�
			DISK.FileSystem.InodeTable.erase(name);
			std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(inode);
			DirectoryDeleteStructure(directory);

			//Usu� wpis o katalogu z obecnego katalogu
			std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.erase(directoryIterator);
			DISK.FileSystem.InodeTable.erase(GetCurrentPath() + name);
			if (messages) { std::cout << "Usuni�to katalog o nazwie '" << name << "' znajduj�cy si� w �cie�ce '" + GetCurrentPath() + "'.\n"; }

			return true;
		}
		else { std::cout << "Nazwa " << name << " nie wskazuje na katalog!\n"; }
	}
	else { std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }

	return false;
}

bool FileManager::DirectoryChange(const std::string& name) {
	const auto inodeTableIterator = DISK.FileSystem.InodeTable.find(name);

	//Je�li w obecnym katalogu znajduj� si� podkatalogi
	if (inodeTableIterator != DISK.FileSystem.InodeTable.end()) {
		if (inodeTableIterator->second->type == "DIRECTORY") {
			//Przej�cie do katalogu o wskazanej nazwie
			currentDirectory = inodeTableIterator->first;
			std::cout << "Obecna �cie�ka to '" << GetCurrentPath() << "'.\n";
			return true;
		}
		else { std::cout << "Nazwa " << name << " nie wskazuje na katalog!\n"; }
	}
	else { std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
	return false;
}

bool FileManager::DirectoryUp() {
	//Je�li istnieje katalog nadrz�dny
	if (GetCurrentDirectoryParent().size() > size_t(0)) {
		//Przej�cie do katalogu nadrz�dnego
		currentDirectory = GetCurrentDirectoryParent();
		std::cout << "Obecna �cie�ka to '" << GetCurrentPath() << "'.\n";
		return true;
	}
	else { std::cout << "Jeste� w katalogu g��wnym!\n"; }
	return false;
}

bool FileManager::DirectoryDown(const std::string& name) {
	const auto directoryIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);

	//Je�li w obecnym katalogu znajduj� si� podkatalogi
	if (directoryIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[directoryIterator->second];
		if (inode->type == "DIRECTORY") {
			//Przej�cie do katalogu o wskazanej nazwie
			currentDirectory = GetCurrentPath() + directoryIterator->first;
			std::cout << "Obecna �cie�ka to '" << GetCurrentPath() << "'.\n";
			return true;
		}
		else { std::cout << "Nazwa " << name << " nie wskazuje na katalog!\n"; }
	}
	else { std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
	return false;
}

//--------------------- Dodatkowe metody --------------------

bool FileManager::DirectoryRename(const std::string& name, const std::string& changeName) {
	if (!changeName.empty()) {
		if (*(changeName.end() - 1) != '/') {

			//Iterator zwracany podczas przeszukiwania obecnego katalogu za plikiem o podanej nazwie
			const auto directoryIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);

			//Je�li znaleziono i-w�ze�
			if (directoryIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
				const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[directoryIterator->second];
				if (inode->type == "DIRECTORY") {
					if (CheckIfNameUnused(currentDirectory, changeName)) {
						if (changeName.size() + GetCurrentPathLength() < MAX_PATH_LENGTH) {
							const std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(inode);

							//Lokowanie nowego klucza w tablicy hashowej i przypisanie do niego pliku
							DISK.FileSystem.InodeTable[currentDirectory + changeName] = directory;
							std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files[changeName] = GetCurrentPath() + changeName;

							//Usuni�cie starego klucza
							DISK.FileSystem.InodeTable.erase(name);
							std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.erase(directoryIterator);

							if (messages) { std::cout << "Zmieniono nazw� katalogu '" << name << "' na '" << changeName << "'.\n"; }

							return true;
						}
						else { std::cout << "�cie�ka za d�uga!\n"; }
					}
					else {
						std::cout << "Nazwa katalogu '" << changeName << "' ju� zaj�ta!\n";
					}
				}
				else { std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
			}
			else { std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
		}
		else { std::cout << "Podana niepoprawna nowa nazwa!\n"; }
	}
	return false;
}

bool FileManager::FileRename(const std::string& name, const std::string& changeName) {
	if (!changeName.empty()) {
		//Iterator zwracany podczas przeszukiwania obecnego katalogu za plikiem o podanej nazwie
		const auto fileIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);

		//Je�li znaleziono i-w�ze�
		if (fileIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[fileIterator->second];
			if (inode->type == "DIRECTORY") {
				if (CheckIfNameUnused(currentDirectory, changeName)) {
					if (changeName.size() + GetCurrentPathLength() < MAX_PATH_LENGTH) {
						std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(inode);

						//Lokowanie nowego klucza w tablicy hashowej i przypisanie do niego pliku
						DISK.FileSystem.InodeTable[currentDirectory + changeName] = file;
						std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files[changeName] = GetCurrentPath() + changeName;

						//Usuni�cie starego klucza
						DISK.FileSystem.InodeTable.erase(currentDirectory + name);
						std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.erase(fileIterator);

						if (messages) { std::cout << "Zmieniono nazw� pliku '" << name << "' na '" << changeName << "'.\n"; }

						return true;
					}
					else { std::cout << "�cie�ka za d�uga!\n"; }
				}
				else {
					std::cout << "Nazwa pliku '" << changeName << "' ju� zaj�ta!\n";
				}
			}
			else {
				std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n";
			}
		}
		else { std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }
	}
	return false;
}

void FileManager::DirectoryRoot() {
	currentDirectory = DISK.FileSystem.rootDirectory;
	std::cout << "Obecna �cie�ka to '" << GetCurrentPath() << "'.\n";
}

void FileManager::Messages(const bool& onOff) {
	messages = onOff;
}

void FileManager::DetailedMessages(const bool&  onOff) {
	detailedMessages = onOff;
}



//------------------ Metody do wy�wietlania -----------------

void FileManager::DisplayFileSystemParams() const {
	std::cout << "Disk capacity: " << DISK_CAPACITY << " Bytes\n";
	std::cout << "Block size: " << BLOCK_SIZE << " Bytes\n";
	std::cout << "Max file size: " << MAX_FILE_SIZE << " Bytes\n";
	std::cout << "Max indexes in block: " << BLOCK_INDEX_NUMBER << " Indexes\n";
	std::cout << "Max direct indexes in file: " << BLOCK_DIRECT_INDEX_NUMBER << " Indexes\n";
	std::cout << "Max file number: " << INODE_NUMBER_LIMIT << " Files\n";
	std::cout << "Max path length: " << MAX_PATH_LENGTH << " Characters\n";
	std::cout << "Number of files: " << DISK.FileSystem.InodeTable.size() << " Files\n";
}

bool FileManager::DisplayDirectoryInfo(const std::string& name) {
	const auto directoryIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);
	if (directoryIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[directoryIterator->second];
		if (inode->type == "DIRECTORY") {
			const std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(inode);
			std::cout << "Name: " << name << '\n';
			std::cout << "Size: " << CalculateDirectorySize(directory) << " Bytes\n";
			std::cout << "Size on disk: " << CalculateDirectorySizeOnDisk(directory) << " Bytes\n";
			std::cout << "Contains: " << CalculateDirectoryFileNumber(directory) << " Files, " << CalculateDirectoryFolderNumber(directory) << " Folders\n";
			std::cout << "Created: " << directory->creationTime << '\n';

			return true;
		}
		else { std::cout << "Nazwa " << name << " nie wskazuje na katalog!\n"; }
	}
	else { std::cout << "Katalog o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }

	return false;
}

bool FileManager::DisplayFileInfo(const std::string& name) {
	const auto fileIterator = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.find(name);
	if (fileIterator != std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[currentDirectory])->files.end()) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[fileIterator->second];
		if (inode->type == "FILE") {
			const std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(inode);
			std::cout << "Name: " << name << '\n';
			std::cout << "Size: " << file->blocksOccupied*BLOCK_SIZE << " Bytes\n";
			std::cout << "Size on disk: " << file->sizeOnDisk << " Bytes\n";
			std::cout << "Created: " << file->creationTime << '\n';
			std::cout << "Modified: " << file->modificationTime << '\n';
			std::cout << "Saved data: " << FileGetData(file) << '\n';

			return true;
		}
		else { std::cout << "Nazwa " << name << " nie wskazuje na plik!\n"; }
	}
	else { std::cout << "Plik o nazwie '" << name << "' nie znaleziony w �cie�ce '" + GetCurrentPath() + "'!\n"; }

	return false;
}

void FileManager::DisplayDirectoryStructure() {
	const u_int level = 0;
	std::cout << std::string(level + 1, ' ') << DISK.FileSystem.rootDirectory << "\n";
	DisplayDirectory(std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[DISK.FileSystem.rootDirectory]), 0);
}
void FileManager::DisplayDirectory(const std::shared_ptr<Directory>& directory, u_int level) {
	if (directory != nullptr) {
		for (auto i = directory->files.begin(); i != directory->files.end(); ++i) {
			if (DISK.FileSystem.InodeTable[i->second]->type == "FILE") {
				std::cout << std::string(level + 2, ' ') << "- " << i->first << '\n';
			}
		}
		level++;
		for (auto i = directory->files.begin(); i != directory->files.end(); ++i) {
			if (DISK.FileSystem.InodeTable[i->second]->type == "DIRECTORY") {
				std::cout << std::string(level + 1, ' ') << i->first << "/\n";
				DisplayDirectory(std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[i->second]), level);
			}
		}
	}
}

void FileManager::DisplayDiskContentBinary() {
	u_int index = 0;
	for (const char& c : DISK.space) {
		//bitset - tablica bitowa
		std::cout << std::bitset<8>(c) << (index % BLOCK_SIZE == BLOCK_SIZE - 1 ? " , " : "") << (index % 16 == 15 ? " \n" : " ");
		index++;
	}
	std::cout << '\n';
}

void FileManager::DisplayDiskContentChar() {
	u_int index = 0;
	for (const char& c : DISK.space) {
		if (c == ' ') { std::cout << ' '; }
		else if (c >= 0 && c <= 32) std::cout << ".";
		else std::cout << c;
		std::cout << (index % BLOCK_SIZE == BLOCK_SIZE - 1 ? " , " : "") << (index % 32 == 31 ? " \n" : " ");
		index++;
	}
	std::cout << '\n';
}

void FileManager::DisplayBitVector() {
	u_int index = 0;
	for (u_int i = 0; i < DISK.FileSystem.bitVector.size(); i++) {
		if (i % 8 == 0) { std::cout << std::setfill('0') << std::setw(2) << (index / 8) + 1 << ". "; }
		std::cout << DISK.FileSystem.bitVector[i] << (index % 8 == 7 ? "\n" : " ");
		index++;
	}
	std::cout << '\n';
}


//------------------- Metody Sprawdzaj�ce -------------------


//-------------------- Metody Pomocnicze --------------------

void FileManager::FileAddIndexes(const std::shared_ptr<File>& file, const std::vector<u_int>& blocks) const {
	if (file != nullptr) {
		if (blocks.size() != size_t(0) && file->blocksOccupied * BLOCK_SIZE <= MAX_FILE_SIZE) {

			u_int blocksIndex = 0;
			//Wpisanie blok�w do bezpo�redniego bloku indeksowego
			for (size_t i = 0; i < BLOCK_DIRECT_INDEX_NUMBER && i < blocks.size(); i++) {
				if (file->directBlocks[i] == nullptr) {
					file->directBlocks[i] = std::make_shared<Index>(blocks[blocksIndex]);
					blocksIndex++;
				}
			}

			//Wpisanie blok�w do 1-poziomowego bloku indeksowego
			if (blocksIndex < blocks.size()) {
				for (size_t i = BLOCK_DIRECT_INDEX_NUMBER; i < BLOCK_INDEX_NUMBER; i++) {
					if (file->directBlocks[i] == nullptr) {
						file->directBlocks[i] = std::make_shared<IndexBlock>();
					}
					for (size_t j = 0; j < BLOCK_INDEX_NUMBER && blocksIndex < blocks.size(); j++) {
						if ((*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[i]))[j] == nullptr) {
							(*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[i]))[j] = std::make_shared<Index>(blocks[blocksIndex]);
							blocksIndex++;
						}
					}
				}
			}
		}
	}
}

void FileManager::FileAllocationIncrease(std::shared_ptr<File>& file, const u_int& neededBlocks) {
	const u_int increaseBlocksNumber = abs(int(neededBlocks - file->blocksOccupied));
	bool fitsAfterLastIndex = true;
	std::shared_ptr<Index> tempIndex;
	const std::shared_ptr<IndexBlock> tempIndexBlock = std::dynamic_pointer_cast<IndexBlock>(tempIndex);
	u_int lastBlockIndex = 0;

	if (file->blocksOccupied != 0) {
		//Je�li indeksy zapisane s� tylko w bloku ideksowym 
		if (file->blocksOccupied <= BLOCK_INDEX_NUMBER) {
			for (u_int i = BLOCK_INDEX_NUMBER - BLOCK_DIRECT_INDEX_NUMBER; i >= 0 && tempIndex == nullptr; i--) {
				tempIndex = file->directBlocks[i];
				if (i == 0) { break; } //unsigned int po obni�eniu zera przyjmuje warto�� wi�ksz� od zera
			}
		}
		else {
			for (u_int i = file->directBlocks.size() - 1; i > BLOCK_DIRECT_INDEX_NUMBER && tempIndex == nullptr; i--) {
				tempIndex = file->directBlocks[i];
			}
			for (u_int i = 0; i < BLOCK_INDEX_NUMBER && tempIndex != nullptr; i++) {
				lastBlockIndex = tempIndex->value;
				tempIndex = (*tempIndexBlock)[i];
			}
		}

		for (u_int i = lastBlockIndex + 1; i < lastBlockIndex + increaseBlocksNumber + 1; i++) {
			if (DISK.FileSystem.bitVector[i] == BLOCK_OCCUPIED) { fitsAfterLastIndex = false; break; }
			else if (i == increaseBlocksNumber) { break; }
		}
		lastBlockIndex++;
	}
	else {
		fitsAfterLastIndex = false;
	}


	if (fitsAfterLastIndex) {
		std::vector<u_int>blocks;
		for (u_int i = lastBlockIndex; i < lastBlockIndex + increaseBlocksNumber; i++) {
			blocks.push_back(i);
		}
		FileAllocateBlocks(file, blocks);
	}
	else {
		if (file->blocksOccupied > 0) { FileDeallocate(file); };
		FileAllocateBlocks(file, FindUnallocatedBlocks(neededBlocks));
	}
	if (messages) { std::cout << "Zwi�kszono plik do rozmiaru " << file->blocksOccupied*BLOCK_SIZE << " Bajt�w.\n"; }
}

void FileManager::FileAllocationDecrease(const std::shared_ptr<File>& file, const u_int& neededBlocks) {
	const u_int sizeToStart = neededBlocks * BLOCK_SIZE;
	//Zmienna do analizowania, czy ju� mo�na usuwa�/dodawa� bloki do pliku
	u_int currentSize = 0;

	unsigned int indexNumber = 0;
	std::shared_ptr<Index> index = file->directBlocks[indexNumber];

	//Dop�ki indeks na co� wskazuje
	while (index != nullptr) {
		currentSize += BLOCK_SIZE;
		if (indexNumber < BLOCK_DIRECT_INDEX_NUMBER) {
			index = file->directBlocks[indexNumber];
		}
		else if (file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER] != nullptr) {
			index = (*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER]))[indexNumber - BLOCK_DIRECT_INDEX_NUMBER];
		}
		else { index = nullptr; }
		//Spisz kolejny indeks
		indexNumber++;

		//Je�li obecny rozmiar przewy�sza rozmiar potrzebny do rozpocz�cia usuwania
		//zacznij usuwa� bloki
		if (currentSize > sizeToStart && index != nullptr) {
			//Zmniejszenie rozmiaru pliku
			file->blocksOccupied--;
			//Po uci�ciu rozmiar i rozmiar rzeczywisty b�d� takie same
			file->sizeOnDisk = file->blocksOccupied*BLOCK_SIZE;
			//Oznacz obecny indeks jako wolny
			ChangeBitVectorValue(index->value, BLOCK_FREE);
			//Obecny indeks w tablicy FileSystem wskazuje na nic
			if (indexNumber < BLOCK_DIRECT_INDEX_NUMBER) {
				file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER] = nullptr;
			}
			else if (file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER] != nullptr) {
				(*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER]))[indexNumber - BLOCK_DIRECT_INDEX_NUMBER] = nullptr;
			}
		}
	}
	if (file->blocksOccupied == BLOCK_INDEX_NUMBER) {
		file->blocksOccupied--;
		file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER] = nullptr;
	}
	if (messages) { std::cout << "Zmniejszono plik do rozmiaru " << file->blocksOccupied*BLOCK_SIZE << " Bajt�w.\n"; }
}

void FileManager::FileDeallocate(const std::shared_ptr<File>& file) {
	std::vector<u_int>freedBlocks; //Zmienna u�yta do wy�wietlenia komunikatu

	if (file->directBlocks[0] != nullptr) {
		std::shared_ptr<Index> index = std::make_shared<Index>();
		u_int indexNumber = 0;
		while (indexNumber < BLOCK_INDEX_NUMBER && index != nullptr && indexNumber < MAX_FILE_SIZE / BLOCK_SIZE) {
			if (indexNumber < BLOCK_DIRECT_INDEX_NUMBER) {
				index = file->directBlocks[indexNumber];
				file->directBlocks[indexNumber] = nullptr;
			}
			else if (file->directBlocks[indexNumber] != nullptr) {
				index = (*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER]))[indexNumber - BLOCK_DIRECT_INDEX_NUMBER];
				(*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER]))[indexNumber - BLOCK_DIRECT_INDEX_NUMBER] = nullptr;
			}
			else { index = nullptr; }

			if (index != nullptr) {
				freedBlocks.push_back(index->value);
				ChangeBitVectorValue(index->value, BLOCK_FREE);
				indexNumber++;
			}
		}
		file->directBlocks.clear();
	}
	if (detailedMessages) {
		std::sort(freedBlocks.begin(), freedBlocks.end());
		std::cout << "Zwolniono bloki: ";
		for (u_int i = 0; i < freedBlocks.size(); i++) { std::cout << freedBlocks[i] << (i < freedBlocks.size() - 1 ? ", " : ".\n"); }
	}
}

void FileManager::FileAllocateBlocks(const std::shared_ptr<File>& file, const std::vector<u_int>& blocks) {

	for (const auto& i : blocks) {
		ChangeBitVectorValue(i, BLOCK_OCCUPIED);
	}
	file->blocksOccupied += blocks.size();

	FileAddIndexes(file, blocks);

	if (detailedMessages) {
		std::cout << "Zaalokowano bloki: ";
		for (u_int i = 0; i < blocks.size(); i++) { std::cout << blocks[i] << (i < blocks.size() - 1 ? ", " : ".\n"); }
	}
}

void FileManager::DirectoryDeleteStructure(std::shared_ptr<Directory>& directory) {
	//Usuwa wszystkie pliki z katalogu
	for (auto it = directory->files.begin(); it != directory->files.end(); ++it) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[it->second];
		if (inode->type == "FILE") {
			std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(inode);
			FileDelete(file);
			if (messages) { std::cout << "Usuni�to plik o nazwie '" << it->first << "' znajduj�cy si� w �cie�ce '" + GetPath(directory) + "'.\n"; }
		}
	}

	//Usuwa wszystkie katalogi w katalogu
	for (auto it = directory->files.begin(); it != directory->files.end(); ++it) {
		const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[it->second];
		if (inode != nullptr) {
			if (inode->type == "DIRECTORY") {
				//Wywo�anie funkcji na podrz�dnym katalogu
				std::shared_ptr<Directory> dir = std::dynamic_pointer_cast<Directory>(inode);
				DirectoryDeleteStructure(dir);
				DISK.FileSystem.InodeTable.erase(GetPath(directory) + it->first + "/");
				if (messages) { std::cout << "Usuni�to katalog o nazwie '" << it->first << "' znajduj�cy si� w �cie�ce '" << GetPath(directory) << "'.\n"; }
			}
		}
	}
	//Czy�ci list� iW�z��w w katalogu
	directory->files.clear();
}

void FileManager::FileDelete(std::shared_ptr<File>& file) {
	if (file != nullptr) {
		FileDeallocate(file);
		for (auto it = DISK.FileSystem.InodeTable.begin(); it != DISK.FileSystem.InodeTable.end(); ++it) {
			if (it->second == file) { DISK.FileSystem.InodeTable.erase(it); break; }
		}
	}
}

const size_t FileManager::CalculateDirectorySize(const std::shared_ptr<Directory>& directory) {
	if (directory != nullptr) {
		//Rozmiar katalogu
		size_t size = 0;

		//Dodaje rozmiar plik�w w katalogu do rozmiaru katalogu
		for (const auto& element : directory->files) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[element.second];
			if (inode->type == "FILE") {
				size += std::dynamic_pointer_cast<File>(inode)->blocksOccupied*BLOCK_SIZE;
			}
		}
		//Przegl�da katalogi i wywo�uje na nich obecn� funkcj� i dodaje zwr�con� warto�� do rozmiaru
		for (const auto& element : directory->files) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[element.second];
			if (inode->type == "DIRECTORY") {
				size += CalculateDirectorySize(std::dynamic_pointer_cast<Directory>(inode));
			}
		}
		return size;
	}
	else { return 0; }
}

const size_t FileManager::CalculateDirectorySizeOnDisk(const std::shared_ptr<Directory>& directory)
{
	if (directory != nullptr) {
		//Rzeczywisty rozmiar katalogu
		size_t sizeOnDisk = 9;

		//Dodaje rzeczywisty rozmiar plik�w w katalogu do rozmiaru katalogu
		for (const auto& element : directory->files) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[element.second];
			if (inode->type == "FILE") {
				sizeOnDisk += std::dynamic_pointer_cast<File>(inode)->sizeOnDisk;
			}
		}
		//Przegl�da katalogi i wywo�uje na nich obecn� funkcj� i dodaje zwr�con� warto�� do rozmiaru
		for (const auto& element : directory->files) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[element.second];
			if (inode->type == "DIRECTORY") {
				sizeOnDisk += CalculateDirectorySize(std::dynamic_pointer_cast<Directory>(inode));
			}
		}
		return sizeOnDisk;
	}
	else { return 0; }
}

const u_int FileManager::CalculateDirectoryFolderNumber(const std::shared_ptr<Directory>& directory) {
	if (directory != nullptr) {
		//Ilo�� folder�w w danym katalogu
		u_int folderNumber = 0;

		/**
		 Dodaje ilo�� folder�w w tym katalogu do zwracanej zmiennej
		 Przegl�da katalogi i wywo�uje na nich obecn� funkcj� i dodaje zwr�con� warto�� do ilo�ci
		 */
		for (const auto& element : directory->files) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[element.second];
			if (inode->type == "DIRECTORY") {
				folderNumber += CalculateDirectoryFolderNumber(std::dynamic_pointer_cast<Directory>(inode));
				folderNumber += 1;
			}
		}
		return folderNumber;
	}
	else { return 0; }
}

const u_int FileManager::CalculateDirectoryFileNumber(const std::shared_ptr<Directory>& directory) {
	if (directory != nullptr) {
		//Ilo�� folder�w w danym katalogu
		u_int fileNumber = 0;

		/**
		 Dodaje ilo�� plik�w w tym katalogu do zwracanej zmiennej
		 Przegl�da katalogi i wywo�uje na nich obecn� funkcj� i dodaje zwr�con� warto�� do ilo�ci
		 */
		for (const auto& element : directory->files) {
			const std::shared_ptr<Inode> inode = DISK.FileSystem.InodeTable[element.second];
			if (inode->type == "FILE") {
				fileNumber += CalculateDirectoryFileNumber(std::dynamic_pointer_cast<Directory>(inode));
				fileNumber += 1;
			}
		}
		return fileNumber;
	}
	else { return 0; }
}

const std::string FileManager::GetCurrentPath() const {
	return currentDirectory;
}

const std::string FileManager::GetPath(const std::shared_ptr<Directory>& directory) {
	for (auto it = DISK.FileSystem.InodeTable.begin(); it != DISK.FileSystem.InodeTable.end(); ++it) {
		if (it->second->type == "DIRECTORY") {
			if (*std::dynamic_pointer_cast<Directory>(it->second) == *directory) { return it->first; }
		}
	}
	return "";
}

const tm FileManager::GetCurrentTimeAndDate()
{
	time_t tt;
	time(&tt);
	tm timeAndDate = *localtime(&tt);
	timeAndDate.tm_year += 1900;
	timeAndDate.tm_mon += 1;
	return timeAndDate;
}

const size_t FileManager::GetCurrentPathLength() const
{
	std::string tempDir = currentDirectory;
	tempDir.erase(std::remove(tempDir.begin(), tempDir.end(), '/'));
	return tempDir.size();
}

const bool FileManager::CheckIfNameUnused(const std::string& directory, const std::string& name) {
	//Przeszukuje podany katalog za plikiem o tej samej nazwie
	std::shared_ptr<Directory> dir = std::dynamic_pointer_cast<Directory>(DISK.FileSystem.InodeTable[directory]);
	for (auto i = dir->files.begin(); i != dir->files.end(); ++i) {
		//Je�li nazwa ta sama
		if (i->first == name) { return false; }
	}
	return true;
}

const bool FileManager::CheckIfEnoughSpace(const u_int& dataSize) const {
	if (dataSize <= DISK.FileSystem.freeSpace) { return true; }
	return false;
}

const std::string FileManager::GetCurrentDirectoryParent() const {
	std::string result = currentDirectory;
	result.pop_back();
	for (u_int i = result.size() - 1; i >= 0; i--) {
		if (i == 0) { result.pop_back(); break; }
		else if (result[i] == '/') { break; }
		else { result.pop_back(); }
	}
	return result;
}

void FileManager::ChangeBitVectorValue(const u_int& block, const bool& value) {
	//Je�li warto�� zaj�ty to wolne miejsce - BLOCK_SIZE
	if (value == 1) { DISK.FileSystem.freeSpace -= BLOCK_SIZE; }
	//Je�li warto�� wolny to wolne miejsce + BLOCK_SIZE
	else if (value == 0) { DISK.FileSystem.freeSpace += BLOCK_SIZE; }
	//Przypisanie blokowi podanej warto�ci
	DISK.FileSystem.bitVector[block] = value;
}

void FileManager::FileSaveData(std::shared_ptr<File>& file, const std::string& data) {
	file->modificationTime = GetCurrentTimeAndDate();

	//Uzyskuje dane podzielone na fragmenty
	const std::vector<std::string>fileFragments = DataToDataFragments(data);

	//Alokowanie blok�w dla pliku
	FileTruncate(file, fileFragments.size());

	file->sizeOnDisk = data.size();


	//Index pod kt�rym maj� zapisywane by� dane
	u_int indexNumber = 0;
	std::shared_ptr<Index> index = file->directBlocks[indexNumber];

	//Zapisuje wszystkie dane na dysku
	for (const auto& fileFragment : fileFragments) {
		//Zapisuje fragment na dysku
		DISK.write(index->value * BLOCK_SIZE, (index->value + 1) * BLOCK_SIZE - 1, fileFragment);
		//Przypisuje do indeksu numer kolejnego bloku
		indexNumber++;
		if (indexNumber < BLOCK_DIRECT_INDEX_NUMBER) {
			index = file->directBlocks[indexNumber];
		}
		else if (file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER] != nullptr) {
			index = (*std::dynamic_pointer_cast<IndexBlock>(file->directBlocks[BLOCK_DIRECT_INDEX_NUMBER]))[(indexNumber - BLOCK_DIRECT_INDEX_NUMBER) % BLOCK_INDEX_NUMBER];
		}
	}
}

const std::vector<std::string> FileManager::DataToDataFragments(const std::string& data) const {
	//Tablica fragment�w podanych danych
	std::vector<std::string>fileFragments;

	//Przetworzenie ca�ych danych
	for (u_int i = 0; i < CalculateNeededBlocks(data.size()); i++) {
		//Oblicza pocz�tek kolejnej cz�ci fragmentu danych.
		const u_int substrBegin = i * BLOCK_SIZE;
		//Dodaje do tablicy fragment�w kolejny fragment o d�ugo�ci BLOCK_SIZE
		fileFragments.push_back(data.substr(substrBegin, BLOCK_SIZE));
	}
	return fileFragments;
}

const u_int FileManager::CalculateNeededBlocks(const size_t& dataSize) const {
	/*
	Przybli�enie w g�r� rozmiaru pliku przez rozmiar bloku.
	Jest tak, poniewa�, je�li zape�nia chocia� o jeden bajt
	wi�cej przy zaj�tym bloku, to trzeba zaalokowa� wtedy kolejny blok.
	*/
	return int(ceil(double(dataSize) / double(BLOCK_SIZE)));
}

const std::vector<u_int> FileManager::FindUnallocatedBlocksFragmented(u_int blockNumber) {
	//Lista wolnych blok�w
	std::vector<u_int> blockList;

	//Szuka wolnych blok�w
	for (u_int i = 0; i < DISK.FileSystem.bitVector.size(); i++) {
		//Je�li blok wolny
		if (DISK.FileSystem.bitVector[i] == BLOCK_FREE) {
			//Dodaje indeks bloku
			blockList.push_back(i);
			//Potrzeba teraz jeden blok mniej
			blockNumber--;
			//Je�li potrzeba 0 blok�w, przerwij
			if (blockNumber == 0) { break; }
		}
	}
	return blockList;
}

const std::vector<u_int> FileManager::FindUnallocatedBlocksBestFit(const u_int& blockNumber) {
	//Lista indeks�w blok�w (dopasowanie)
	std::vector<u_int> blockList;
	//Najlepsze dopasowanie
	std::vector<u_int> bestBlockList(DISK.FileSystem.bitVector.size() + 1);

	//Szukanie wolnych blok�w spe�niaj�cych minimum miejsca
	for (u_int i = 0; i < DISK.FileSystem.bitVector.size(); i++) {
		//Je�li blok wolny
		if (DISK.FileSystem.bitVector[i] == BLOCK_FREE) {
			//Dodaj indeks bloku do listy blok�w
			blockList.push_back(i);
		}
		//Je�li blok zaj�ty
		else {
			//Je�li uzyskana lista blok�w jest wi�ksza od ilo�ci blok�w jak� chcemy uzyska�
			//to dodaj uzyskane dopasowanie do listy dopasowa�;
			if (blockList.size() >= blockNumber) {
				//Je�li znalezione dopasowanie mniejsze ni� najlepsze dopasowanie
				if (blockList.size() < bestBlockList.size()) {
					//Przypisanie nowego najlepszego dopasowania
					bestBlockList = blockList;
					if (bestBlockList.size() == blockNumber) { break; }
				}
			}

			//Czy�ci list� blok�w, aby mo�na przygotowa� kolejne dopasowanie
			blockList.clear();
		}
	}

	/*
	Je�li zdarzy si�, �e ostatni blok w wektorze bitowym jest wolny, to
	ostatnie dopasownie nie zostanie dodane do listy dopasowa�, dlatego
	trzeba wykona� poni�szy kod. Je�li ostatni blok w wektorze bitowym
	b�dzie zaj�ty to blockList b�dzie pusty i nie spie�ni warunku
	*/
	if (blockList.size() >= blockNumber) {
		//Je�li blok wolny
		if (blockList.size() < bestBlockList.size()) {
			//Dodaj indeks bloku do listy blok�w
			bestBlockList = blockList;
		}
	}

	//Je�li znalezione najlepsze dopasowanie
	if (bestBlockList.size() < DISK.FileSystem.bitVector.size() + 1) {
		//Odetnij nadmiarowe indeksy z dopasowania (je�li wi�ksze ni� potrzeba)
		bestBlockList.resize(blockNumber);
	}
	//Inaczej zmniejsz dopasowanie do 0, �eby po zwr�ceniu wybrano inn� metod�
	else { bestBlockList.resize(0); }

	return bestBlockList;
}

const std::vector<u_int> FileManager::FindUnallocatedBlocks(const u_int& blockNumber) {
	//Szuka blok�w funkcj� z metod� best-fit
	std::vector<u_int> blockList = FindUnallocatedBlocksBestFit(blockNumber);

	//Je�li funkcja z metod� best-fit nie znajdzie dopasowa�
	if (blockList.empty()) {
		//Szuka niezaalokowanych blok�w, wybieraj�c pierwsze wolne
		blockList = FindUnallocatedBlocksFragmented(blockNumber);
	}

	return blockList;
}
