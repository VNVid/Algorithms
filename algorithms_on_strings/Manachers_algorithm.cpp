/*
https://contest.yandex.ru/contest/19772/problems/B/
Для заданной строки s длины n (1 ≤ n ≤ 105) требуется подсчитать число пар (i,
j), 1 ≤ i < j ≤ n, таких что подстрока s[i..j] является палиндромом.
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Solution {
 private:
  static std::vector<int> CountPalindromes(
      const std::string& str, int palindrome_type) {  // 1 - even, 0 - odd
    std::vector<int> palindromes(str.size(), (palindrome_type + 1) % 2);
    for (int i = 0, l = 0, r = -1; i < str.size(); i++) {
      if (i <= r) {
        palindromes[i] =
            std::min(r - i + 1, palindromes[l + r - i + palindrome_type]);
      }
      while (i + palindromes[i] < str.size() &&
             i - palindromes[i] - palindrome_type >= 0 &&
             str[i + palindromes[i]] ==
                 str[i - palindromes[i] - palindrome_type]) {
        ++palindromes[i];
      }
      if (i + palindromes[i] - 1 > r) {
        r = i + palindromes[i] - 1;
        l = i - palindromes[i] + (palindrome_type + 1) % 2;
      }
    }
    return palindromes;
  }

 public:
  static size_t Solve(const std::string& str) {
    std::vector<int> odd_palindromes = CountPalindromes(str, 0);
    std::vector<int> even_palindromes = CountPalindromes(str, 1);  // right letter in the central pair is the centre

    size_t ans = 0;
    for (auto i : odd_palindromes) {
      ans += i - 1;
    }
    for (auto i : even_palindromes) {
      ans += i;
    }
    return ans;
  }
};

int main() {
  std::string str;
  std::cin >> str;
  std::cout << Solution::Solve(str);
  return 0;
}