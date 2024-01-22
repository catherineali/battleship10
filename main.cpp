#include <bits/stdc++.h>
using namespace std;

int guessCount = 1, coord, x, y, hitCount = 0, sinkCount = 0;
int board[15][15];
double prob[15][15]; // 0:miss, 1:neutral, 2:hit, 3:sink
bool rem[5];
double weight = 2.0;
char c;

int bestGuess() { // returns position with highest associated probability
  double maxP = 0.0;
  int maxPos; 
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (prob[i][j] > maxP && board[i][j] == 1) {
        maxP = prob[i][j];
        maxPos = 10 * j + i;
      }
    }
  }
  return maxPos; // CHANNEL DECODER
}

void calc() { // calculates probability of each cell
  for (int i = 0; i < 10; i++) { // resets probability
    for (int j = 0; j < 10; j++) {
      prob[i][j] = 0.0; 
    }
  }
  int len[5] = {5, 4, 3, 3, 2};
  for (int r = 0; r < 5; r++) {
    if (!rem[r]) break; // if given ship has already sunk, then skip the search
    for (int i = 0; i < 11 - len[r]; i++) {
      for (int j = 0; j < 10; j++) {
        int sh = 1, sv = 1;
        for (int k = 0; k < len[r]; k++) {
          if (board[i + k][j] == 0 || board[i + k][j] == 3) sh = 0; // if segment contains a MISS or a SINK, then segment cannot contain the ship
          else if (board[i + k][j] == 2) sh *= weight; // the more HITs, the greater the probability of the cell being part of a ship and thus a HIT
          if (board[j][i + k] == 0 || board[j][i + k] == 3) sv = 0; // repeat vertically
          else if (board[j][i + k] == 2) sv *= weight;
        }
        for (int k = 0; k < len[r]; k++) {
          prob[i + k][j] += sh;
          prob[j][i + k] += sv;
        }
      }
    }
  }
}

void process() {
  if (c == 'H') {
    board[x][y] = 2; // CHANNEL ENCODER
    hitCount++;
  } else if (c == 'S') {
    board[x][y] = 3; // CHANNEL ENCODER
    hitCount++;
    sinkCount++;
    // sinking ships through bounding
    if (sinkCount == 4) {
      if (hitCount <= 14) rem[4] = false;
      if (hitCount <= 13) rem[3] = false, rem[2] = false;
      if (hitCount <= 12) rem[1] = false;
    } else if (sinkCount == 3) {
      if (hitCount <= 9) rem[4] = false;
    }
  } else if (c == 'M') {
    board[x][y] = 0; // CHANNEL ENCODER
  }
  calc();
}

void setup() {
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      board[i][j] = 1;
      prob[i][j] = 0.0;
    }
  }
  
  for (int i = 0; i < 5; i++)
    rem[i] = 1;
}

int main() {
  setup();
  calc();
  
  while (1) {
    coord = bestGuess();
    x = coord % 10, y = coord / 10; // SOURCE DECODER
    cout << "Shot #" << guessCount << ": ";
    cout << "Coordinate(" << (char) (x + 'A') << ", " << y + 1 << ")" << endl; // SENT TO RECEIVER (CATHERINE & OLIVIA)

    string s;
    cin >> s; // SOURCE (DR. AIYER)
    c = s[0]; // SOURCE ENCODER
    process(); // CHANNEL
    if (hitCount == 17) {
      cout << "Score: " << guessCount << endl;
      break;
    }
    guessCount++;
  }
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (board[j][i] >= 2) cout << "■";
      else cout << "□";
    } cout << endl;
  }
}