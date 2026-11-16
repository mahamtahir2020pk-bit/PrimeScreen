/* 
This is the booking header file which will 
have the Booking record, Customer record and singly linked list used as the data structure for bookings.
*/

#ifndef BOOKING_H
#define BOOKING_H

#include <string>
using namespace std;

// One ticket, a group of seatCount seats side by side in one row.
struct Booking {
    int ticketID;
    string customerName;
    int showID;
    int row;
    int startCol;
    int seatCount;
    int totalPrice;
    Booking() : ticketID(0), showID(0), row(0), startCol(0), seatCount(0), totalPrice(0) {}
};

struct Customer {
    int customerID;
    string name;
    int showID;
    int groupSize;
    int arrivalSec;      
    Customer() : customerID(0), showID(0), groupSize(1), arrivalSec(0) {}
};

struct BookingNode {
    Booking data;
    BookingNode* next;
    BookingNode() : next(nullptr) {}
};

// Every Show owns one BookingList The list owns its nodes and deletes them in the destructor.
class BookingList {
private:
    BookingNode* head;
    BookingNode* tail;
    int count;

public:
    BookingList() : head(nullptr), tail(nullptr), count(0) {}
    ~BookingList() { clear(); }

    // This function Adds a new element at the tail so its in O(1), time is constant no matter 
    // how big the list gets. Returns a pointer to the stored booking.
    Booking* addBooking(const Booking& b) {
        BookingNode* node = new BookingNode;
        node->data = b;

        if (tail == nullptr) { head = node; tail = node; }

        else { tail->next = node; tail = node; }

        count++;
        return &node->data;
    }

    Booking* findBooking(int ticketID) const {
        for (BookingNode* cur = head; cur != nullptr; cur = cur->next)
            if (cur->data.ticketID == ticketID) return &cur->data;
        return nullptr;
    }

    bool removeBooking(int ticketID) {
        BookingNode* prev = nullptr;
        BookingNode* cur = head;
        while (cur != nullptr && cur->data.ticketID != ticketID) 
                { prev = cur; cur = cur->next; }

        if (cur == nullptr) return false;
        if (prev == nullptr) 
            head = cur->next; 

       else prev->next = cur->next;

        if (cur == tail) tail = prev;
        delete cur;
        count--;
        return true;
    }

    void clear() {
        while (head != nullptr) {
            BookingNode* next = head->next;
            delete head;
            head = next;
        }
        tail = nullptr;
        count = 0;
    }

    BookingNode* getHead() const { return head; }
    int getCount() const { return count; }
};
#endif