#include <iostream>
#include <conio.h>
#include <unordered_map>
#include <windows.h>
using namespace std;

int main(int argc, char* argv[]) {

    int size = argc-1;
    char* mas = new char[size];
    for (int i = 0; i < size; i++) {
        mas[i] = *argv[i + 1];
    }

    unordered_map<char, int> hmap;

    for (int i = 0; i < size; i++)
        hmap[mas[i]]++;

    string oddString = "";
    for (auto x : hmap)
    {
        if (x.second % 2 != 0)
            oddString = oddString + x.first;
    }


    string firstHalf = "", secondHalf = "";

    for (auto x : hmap)
    {

        string s(x.second / 2, x.first);

        firstHalf = firstHalf + s;
        secondHalf = s + secondHalf;
    }

    if (oddString.length() != 0)
    {
        firstHalf = firstHalf + oddString[0];
    }

    secondHalf = secondHalf + "|";
    for (int i = 1; i < oddString.length(); i++)
        secondHalf = secondHalf + oddString[i];


    cout << (firstHalf + secondHalf);
    cin.get();
}