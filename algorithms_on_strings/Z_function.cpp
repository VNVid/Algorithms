/*
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n.
Время O(n + p), доп. память – O(p). p ≤ 30000, n ≤ 300000.
*/

#include <algorithm>
#include <cstdio>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

void PatternFound(int index) { std::cout << index << " "; }

class Solution {
 public:
  static void Solve(const std::string& pattern) {
    std::vector<size_t> zfunc = Solution::GetZfunctionForPattern(pattern);
    char stri;
    std::deque<char> str;
    size_t l = 0;
    size_t r = 0;
    size_t i = 0;  // char with pos i will be the first in deque
    bool finish = false;

    getchar();
    for (size_t i = 0; i < pattern.size(); i++) {
      str.push_back(getchar());
      if (str.back() == EOF) {
        finish = true;
        break;
      }
    }

    while (!finish) {
      size_t zfuncStr = 0;
      if (i <= r) {
        zfuncStr = std::min(r - i + 1, zfunc[i - l]);
      }
      while (zfuncStr < str.size() && pattern[zfuncStr] == str[zfuncStr]) {
        ++zfuncStr;
      }
      if (std::max(i + zfuncStr, (size_t)1) - 1 > r) {
        l = i;
        r = i + zfuncStr - 1;
      }
      if (zfuncStr == pattern.size()) {
        PatternFound(i);
      }
      ++i;
      str.push_back(getchar());
      if (str.back() == EOF) {
        str.pop_back();
      }
      str.pop_front();
      if (str.size() == 0) {
        finish = true;
      }
    }
  }

 private:
  static std::vector<size_t> GetZfunctionForPattern(
      const std::string& pattern) {
    std::vector<size_t> zfunc(pattern.size(), 0);

    for (size_t i = 1, l = 0, r = 0; i < pattern.size(); i++) {
      if (i <= r) {
        zfunc[i] = std::min(r - i + 1, zfunc[i - l]);
      }
      while (zfunc[i] + i < pattern.size() &&
             pattern[zfunc[i]] == pattern[zfunc[i] + i]) {
        ++zfunc[i];
      }
      if (i + zfunc[i] - 1 > r) {
        l = i;
        r = i + zfunc[i] - 1;
      }
    }

    return zfunc;
  }
};

int main() {
  std::string pattern;
  std::cin >> pattern;
  Solution::Solve(pattern);
  return 0;
}
