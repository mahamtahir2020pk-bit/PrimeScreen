#ifndef SEATMAP_H
#define SEATMAP_H

#include<iostream>
#include<string>
using namespace std;

class SeatMap {
	private;
	int rows;
	int cols;
	char** seats; //2D array
public:
    //constructor
    SeatMap(int r, int c) : rows(r), cols(c) {
        seats = new char* [rows];
        for (int i = 0; i < rows; i++) {
            seats[i] = new char[cols];
            for (int j = 0; j < cols; j++) seats[i][j] = '0';
        }
    }
    //destructor
    ~SeatMap() {
        for (int i = 0; i < rows; i++) delete[] seats[i];
        delete[] seats;
    }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int capacity() const { return rows * cols; }

    bool isFree(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols && seats[r][c] == '0';
    }

    // checks for k available seats
    bool isBlockFree(int row, int startCol, int k) const {
        if (k < 1 || row < 0 || row >= rows || startCol < 0 || startCol + k > cols) return false;
        for (int i = 0; i < k; i++)
            if (seats[row][startCol + i] != '0') return false;
        return true;
    }

    bool bookBlock(int row, int startCol, int k) {
        if (!isBlockFree(row, startCol, k)) return false;
        for (int i = 0; i < k; i++) seats[row][startCol + i] = 'X';
        return true;
    }

    void cancelBlock(int row, int startCol, int k) {
        for (int i = 0; i < k; i++)
            if (row >= 0 && row < rows && startCol + i >= 0 && startCol + i < cols)
                seats[row][startCol + i] = '0';
    }

    int countFree() const {
        int n = 0;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (seats[i][j] == '0') n++;
        return n;
    }

    void reset() {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) seats[i][j] = '0';
    }

    //Prints the seating layout 
    void display() {
        int width = cols * 3;
        int pad(width > 11) ? (width - 11) / 2 : 0;
        cout << "\n     " << string(pad, ' ') << "S C R E E N\n";
        cout << "     " << string(width, '=') << "\n";
        cout << "     ";
        for (int c = 0; c < cols; c++) {
            int n = c + 1;
            if (n < 10) cout << ' ';
            cout << n << ' ';
        }
        cout << "\n";
        for (int r = 0; r < rows; r++) {
            cout << "  " << (char)('A' + r) << "  ";
            for (int c = 0; c < cols; c++) {
                char ch = seat[r][c];
                cout < ' ' << ch < ' ';
            }
            cout << "  " << (char)('A' + r) << "\n";

        }
        cout << "\n 0 = available     X = taken\n";
        cout << " Free seats: " << countFree() << " / " << capacity() << "\n";
    }
};

#endif