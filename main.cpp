// PrimeScreen is a Smart Cinema Ticketing and Seat Allocation System
// Group 7
// PROGRESS: 2D array (SeatMap), singly linked list (BookingList), stack (cancellation undo)
// Compiled and ran with g++ -std=c++17 main.cpp -o cinema

#include <iostream>
#include <cstdlib>
#include <string>
#include "SeatMap.h"
#include "Booking.h"
#include "Stack.h"
using namespace std;

const int ROWS = 8;
const int COLS = 10;
const int PRICE = 12;

// input helpers

bool readLine(const string& prompt, string& out) {
    cout << prompt;
    return (bool)getline(cin, out);
}

// Keeps asking until the user types a whole number between lo = lower and  hi = higher. 
// Returns -1 if input is closed.

int readInt(const string& prompt, int lo, int hi) {
    string s;
    while (readLine(prompt, s)) {
        if (s.empty() || s.size() > 6) { 
            cout << "  [!] Enter a number from " << lo << " to " << hi << ".\n"; 
            continue; 
        }
        bool ok = true;
        int v = 0;
        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] < '0' || s[i] > '9') { 
                ok = false; 
                break; 
            }
            v = v * 10 + (s[i] - '0');
        }
        if (!ok || v < lo || v > hi) { 
            cout << "  [!] Enter a number from " << lo << " to " << hi << ".\n"; 
            continue; 
        }
        return v;
    }
    return -1;
}

void clearScreen() {
    system("cls");
}

void pause() {
    string s;
    readLine("\n  Press Enter to continue...", s);
}

void printHeader() {
    cout << "\n  =====================================================\n"
        << "                   P R I M E S C R E E N\n"
        << "        Smart Cinema Ticketing & Seat Allocation\n"
        << "  =====================================================\n"
        << "  SeatMap + BookingList + Stack\n";
}

// This function reads a seat in row letter and colomn nubmer such as "C5" (row letter + seat number).
bool parseSeat(const string& s, int& row, int& col) {
    if (s.size() < 2) return false;
    char ch = s[0];

    if (ch >= 'a' && ch <= 'z') 
        ch = (char)(ch - 'a' + 'A');

    if (ch < 'A' || ch >= 'A' + ROWS) return false;
    int n = 0;

    for (size_t i = 1; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9' || i > 3) return false;
        n = n * 10 + (s[i] - '0');
    }

    if (n < 1 || n > COLS) return false;
    row = ch - 'A';
    col = n - 1;
    return true;
}

string seatRange(int row, int col, int count) {
    string a = string(1, (char)('A' + row)) + to_string(col + 1);
    if (count <= 1) return a;
    return a + "-" + string(1, (char)('A' + row)) + to_string(col + count);
}

void printTicket(const Booking& b) {
    cout << "\n  ---------------- TICKET ----------------\n"
        << "  Ticket ID : " << b.ticketID << "\n"
        << "  Customer  : " << b.customerName << "\n"
        << "  Seats     : " << seatRange(b.row, b.startCol, b.seatCount) << "  (" << b.seatCount << " seat(s))\n"
        << "  Total     : $" << b.totalPrice << "\n"
        << "  ----------------------------------------\n";
}

// Menu

void bookSeats(SeatMap& map, BookingList& list, int& nextTicketID) {
    map.display();
    int k = readInt("\n  How many seats together (1-8, 0 = back): ", 0, 8);
    if (k <= 0) return;

    string name;

    if (!readLine("  Customer name: ", name) || name.empty()) 
    { 
        cout << "  [!] Name cannot be empty.\n"; 
        return; 
    }

    string text;
    if (!readLine("  First seat of the block (example C5): ", text)) return;

    int row = 0, col = 0;
    if (!parseSeat(text, row, col)) 
    { 
        cout << "  [!] That seat does not exist.\n"; 
        return; 
    }

    if (!map.bookBlock(row, col, k)) {                       // 2D array checks and marks the seats
        cout << "  [!] " << k << " seats from " << seatRange(row, col, 1) << " are not all free.\n";
        return;
    }

    Booking b;
    b.ticketID = nextTicketID++;
    b.customerName = name;
    b.showID = 1;
    b.row = row;
    b.startCol = col;
    b.seatCount = k;
    b.totalPrice = k * PRICE;
    list.addBooking(b);                                      // linked list and add at the tail
    printTicket(b);
}

void viewBookings(const BookingList& list) {

    cout << "\n  Bookings (" << list.getCount() << " tickets, oldest first)\n";

    if (list.getCount() == 0) { cout << "  No bookings yet.\n"; return; }

    cout << "  Ticket    Customer              Seats     Price\n";

    for (BookingNode* n = list.getHead(); n != nullptr; n = n->next)

        cout << "  " << n->data.ticketID << "    " << n->data.customerName
        << string(n->data.customerName.size() < 22 ? 22 - n->data.customerName.size() : 1, ' ')
        << seatRange(n->data.row, n->data.startCol, n->data.seatCount)
        << string(10 - seatRange(n->data.row, n->data.startCol, n->data.seatCount).size(), ' ')
        << "$" << n->data.totalPrice << "\n";
}

void cancelTicket(SeatMap& map, BookingList& list, Stack<Booking>& undo) {

    int id = readInt("\n  Ticket ID to cancel (0 = back): ", 0, 999999);
    if (id <= 0) return;

    Booking* b = list.findBooking(id);
    if (b == nullptr) { cout << "  [!] No active ticket with ID " << id << ".\n"; return; }

    Booking copy = *b;                                       // copy first, the node is deleted below
    map.cancelBlock(copy.row, copy.startCol, copy.seatCount);
    list.removeBooking(id);
    undo.push(copy);                                         // newest cancellation on top of the stack
    cout << "  Ticket " << id << " cancelled. Seats " << seatRange(copy.row, copy.startCol, copy.seatCount)
        << " are free again. Cancelled tickets on the stack: " << undo.size() << "\n";
}

void undoCancellation(SeatMap& map, BookingList& list, Stack<Booking>& undo) {

    Booking b;
    if (!undo.pop(b)) { cout << "\n  There is no cancellation to undo.\n"; return; }

    if (!map.bookBlock(b.row, b.startCol, b.seatCount)) {
        cout << "\n  [!] Ticket " << b.ticketID << " could not be restored because its seats were taken again.\n";
        return;
    }
    list.addBooking(b);
    cout << "\n  Restored the last cancelled ticket:";
    printTicket(b);
}

void viewStack(const Stack<Booking>& undo) {

    cout << "\n  Cancellation stack (top first), " << undo.size() << " item(s)\n";
    if (undo.isEmpty()) { cout << "  Stack is empty.\n"; return; }

    for (Stack<Booking>::Node* n = undo.getTop(); n != nullptr; n = n->next)
        cout << "  Ticket " << n->data.ticketID << "  " << n->data.customerName << "  "
        << seatRange(n->data.row, n->data.startCol, n->data.seatCount) << "\n";
}

//  main

int main() {
    SeatMap map(ROWS, COLS);               // 2D array
    BookingList bookings;                  // singly linked list
    Stack<Booking> cancelStack;            // stack
    int nextTicketID = 100001;

    while (true) {
        clearScreen();
        printHeader();

        cout << "\n  Seats free: " << map.countFree() << " / " << map.capacity()
            << "   Tickets: " << bookings.getCount() << "\n"
            << "   1. View Seat Map\n"
            << "   2. Book Seats\n"
            << "   3. View All Bookings\n"
            << "   4. Cancel Ticket\n"
            << "   5. Undo Last Cancellation\n"
            << "   6. View Cancellation Stack\n"
            << "   0. Exit\n";

        int choice = readInt("\n  Choose an option: ", 0, 6);
        if (choice <= 0) break;

        switch (choice) {
        case 1: map.display(); break;
        case 2: bookSeats(map, bookings, nextTicketID); break;
        case 3: viewBookings(bookings); break;
        case 4: cancelTicket(map, bookings, cancelStack); break;
        case 5: undoCancellation(map, bookings, cancelStack); break;
        case 6: viewStack(cancelStack); break;
        }
        pause();
    }
    cout << "\n  Goodbye!\n";
    return 0;
}