#include "Shell.h"

/*
 * Wszystkie moduły posiadają swój globalny obiekt
 * extern użyte, aby w każdym pliku gdzie załączamy moduł był używany
 * ten sam obiekt (tworzony jest w pliku cpp modułu od swojego obiektu)
 */

int main() {
	Shell system;
	system.boot();
	return 0;
}