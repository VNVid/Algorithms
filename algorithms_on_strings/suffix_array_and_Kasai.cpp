/*


Дана строка длины n. Найти количество ее различных подстрок. Используйте
суффиксный массив.

Построение суффиксного массива выполняйте за O(n log n). Вычисление количества
различных подстрок выполняйте за O(n).

*/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


class Solver {
 public:
    long long NumberOfDifferentSubstrings(std::string& str) {
    str += '$';
    CountSuffixArray(str);
    CountLCP(str);

    long long res = str.size() * (str.size() - 1) / 2;  // sum of
                                                        // suffix_array[i]
    for (size_t i = 0; i < str.size() - 1; i++) {
      res -= longest_common_prefix[i];
    }
    return res;
  }

 private:
  std::vector<int> suffix_array;
  std::vector<int> component;
  const size_t alphabet_size = 256;
  std::vector<int> longest_common_prefix;

  void CountSuffixArray(std::string& str) {
    std::vector<int> counting_sort(alphabet_size, 0);
    suffix_array.resize(str.size(), 0);
    component.resize(str.size(), 0);

    // zero step: for substrings with len=1
    for (size_t i = 0; i < str.size(); i++) {
      ++counting_sort[str[i]];
    }
    for (size_t i = 1; i < alphabet_size; i++) {
      counting_sort[i] += counting_sort[i - 1];
    }
    for (size_t i = 0; i < str.size(); i++) {
      suffix_array[--counting_sort[str[i]]] = i;
    }

    int number_of_components = 0;
    for (size_t i = 1; i < str.size(); i++) {
      if (str[suffix_array[i]] != str[suffix_array[i - 1]]) {
        ++number_of_components;
      }
      component[suffix_array[i]] = number_of_components;
    }
    ++number_of_components;

    // substrings of len 2 * (2^k)
    for (int k = 0; (1 << k) < str.size(); k++) {
      number_of_components = CountOneStep(k, str.size(), number_of_components);
    }
  }
  int CountOneStep(int power, int str_size, int comp_num) {
    // auxilary(temporary) vectors
    std::vector<int> suffix_array_tmp(str_size, 0);
    std::vector<int> component_tmp(str_size, 0);
    std::vector<int> counting_sort(comp_num, 0);

    for (size_t i = 0; i < str_size; i++) {
      suffix_array_tmp[i] =
          (suffix_array[i] - (1 << power) + str_size) % str_size;
    }
    for (size_t i = 0; i < str_size; i++) {
      ++counting_sort[component[suffix_array_tmp[i]]];
    }
    for (size_t i = 1; i < comp_num; i++) {
      counting_sort[i] += counting_sort[i - 1];
    }
    for (int i = str_size - 1; i >= 0; i--) {
      suffix_array[--counting_sort[component[suffix_array_tmp[i]]]] =
          suffix_array_tmp[i];
    }

    int number_of_components_tmp = 0;
    for (size_t i = 1; i < str_size; i++) {
      int second_half_cur = (suffix_array[i] + (1 << power)) % str_size;
      int second_half_prev = (suffix_array[i - 1] + (1 << power)) % str_size;
      if (component[suffix_array[i]] != component[suffix_array[i - 1]] ||
          component[second_half_cur] != component[second_half_prev]) {
        ++number_of_components_tmp;
      }
      component_tmp[suffix_array[i]] = number_of_components_tmp;
    }
    ++number_of_components_tmp;

    for (size_t i = 0; i < str_size; i++) {
      component[i] = component_tmp[i];
    }

    return number_of_components_tmp;
  }

  // Kasai's algorithm
  void CountLCP(std::string& str) {
    longest_common_prefix.resize(str.size(), 0);
    //(suf[i] = j) ==> (position[j] = i)
    std::vector<int> position(str.size(), 0);

    for (size_t i = 0; i < str.size(); i++) {
      position[suffix_array[i]] = i;
    }

    int delta = 0;
    for (int i = 0; i < str.size(); i++) {
      if (delta > 0) {
        --delta;
      }
      if (position[i] == str.size() - 1) {
        longest_common_prefix[str.size() - 1] = -1;
        delta = 0;
        continue;
      } else {
        int j = suffix_array[position[i] + 1];
        while (std::max(i + delta, j + delta) < str.size() &&
               str[i + delta] == str[j + delta]) {
          ++delta;
        }
        longest_common_prefix[position[i]] = delta;
      }
    }
  }
};

int main() {
  std::string str;
  std::cin >> str;
  Solver solution;
  std::cout << solution.NumberOfDifferentSubstrings(str);
  return 0;
}