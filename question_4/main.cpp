#include <iostream>
#include "dynarray.h"

using namespace std;

void displayArray(DynArray& da) {
    if (da.count == 0) {
        cout << "Array is empty." << endl;
        return;
    }
    cout << "Array (size=" << da.count << ", capacity=" << da.capacity << "): [";
    for (int i = 0; i < da.count; i++) {
        cout << da.data[i];
        if (i < da.count - 1) cout << ", ";
    }
    cout << "]" << endl;
}

void showMenu() {
    cout << endl << "===== DYNAMIC ARRAY MENU =====" << endl;
    cout << "1. Push (insert at end)" << endl;
    cout << "2. Pop (remove from end)" << endl;
    cout << "3. Display Array" << endl;
    cout << "4. Show Capacity" << endl;
    cout << "0. Exit" << endl;
    cout << "Choice: ";
}

int main() {
    DynArray da;
    initArray(da, 2);

    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                int val;
                cout << "Enter value: ";
                cin >> val;
                push(da, val);
                cout << "Pushed " << val << endl;
                displayArray(da);
                break;
            }
            case 2: {
                int val = pop(da);
                if (val == -1) {
                    cout << "Array is empty, nothing to pop." << endl;
                } else {
                    cout << "Popped " << val << endl;
                }
                displayArray(da);
                break;
            }
            case 3: {
                displayArray(da);
                break;
            }
            case 4: {
                cout << "Count: " << da.count << ", Capacity: " << da.capacity << endl;
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 0);

    freeArray(da);
    return 0;
}