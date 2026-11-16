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