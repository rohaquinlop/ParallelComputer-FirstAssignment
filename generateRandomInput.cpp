#include <bits/stdc++.h>

using namespace std;

int main() {
  int n;
  srand(time(NULL));

  cin >> n;

  for (int i = 0; i < 2 * n; i++) {
    cout << rand() % INT_MAX << " ";
  }

  cout << 0 << endl;


  return 0;
}