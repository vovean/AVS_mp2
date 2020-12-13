#include <iostream>
#include <thread>
#include "windows.h"

using namespace std;

#pragma region Global Vars
// N - amount of cannibals
int N, currentPieces, potCapacity;
// semaphores
HANDLE outputSemaphore, eatQueueSemaphore;
// dynamic array of threads
thread* threads;
#pragma endregion

#pragma region Consts
const int MIN_SAVAGES = 1;
const int MAX_SAVAGES = 10;
const int MIN_PIECES = 1;
const int MAX_PIECES = 10;
const int EACH_EATS_PIECES = 5;
const int SECOND = 1000;
const int MAX = 4294967295;
#pragma endregion


int readInt(string prompt, int lowerBound = 1, int upperBound = 10) {
	int tmp;
	cout << prompt;
	while (true)
	{
		cin >> tmp;
		cin.clear();
		if (cin.fail())
		{
			cout << "You should enter a number [" << lowerBound << "; " << upperBound << "]\n";
			continue;
		}
		if (tmp < lowerBound || tmp > upperBound)
		{
			cout << "You should enter a number [" << lowerBound << "; " << upperBound << "]\n";
			continue;
		}
		break;
	}
	return tmp;
}

void fillThePot() {
	cout << "Povar is starting to cook another missionaire\n";
	Sleep(SECOND);
	currentPieces = potCapacity;
	cout << "Now we have " << currentPieces << " new pieces of meat\n\n";
	ReleaseSemaphore(eatQueueSemaphore, potCapacity + 1, 0);
	Sleep((int)(0.5 * SECOND));
}

void takePeaceAndEat(int savageID) {
	for (int eatCircles = 0; eatCircles < EACH_EATS_PIECES; eatCircles++)
	{
		// obtain each semaphore
		WaitForSingleObject(eatQueueSemaphore, MAX);
		WaitForSingleObject(outputSemaphore, MAX);
		// check if there is any meat left
		if (currentPieces == 0)
		{
			fillThePot();
		}
		// take a peace
		currentPieces -= 1;
		cout << "Savage " << savageID << " obtains 1 piece from the pot. There " << currentPieces << " pieces remain in the pot\n";
		Sleep(SECOND);
		// release all semaphores with logging
		ReleaseSemaphore(outputSemaphore, 1, 0);
		WaitForSingleObject(outputSemaphore, MAX);
		cout << "Savage " << savageID << " has eaten his piece of missionaire\n";
		ReleaseSemaphore(outputSemaphore, 1, 0);
		Sleep(SECOND);
		WaitForSingleObject(outputSemaphore, MAX);
		cout << "\n By now savage #" << savageID << " has eaten [" << eatCircles + 1 << "] pieces of missionaire\n\n";
		ReleaseSemaphore(outputSemaphore, 1, 0);
	}
}

int main() {
	N = readInt("Enter N (number of savages): ", MIN_SAVAGES, MAX_SAVAGES);
	potCapacity = readInt("Enter M (number of pieces of missionary): ", MIN_SAVAGES, MAX_SAVAGES);

	currentPieces = potCapacity;
	eatQueueSemaphore = CreateSemaphore(0, currentPieces + 1, currentPieces + 1, 0);
	outputSemaphore = CreateSemaphore(0, 1, 1, 0);

	// create multiple savages as threads
	threads = new thread[N];
	for (int i = 0; i < N; i++)
	{
		threads[i] = thread(takePeaceAndEat, i + 1);
	}

	// join all threads
	for (int i = 0; i < N; i++)
	{
		threads[i].join();
	}

	cout << "Everyone has eaten [" << EACH_EATS_PIECES << "] pieces of missionaire\nFinish the program";
	// free the memory
	delete[] threads;
}