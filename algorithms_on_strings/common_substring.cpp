/*
Заданы две строки s, t и целое число k. Рассмотрим множество всех таких непустых
строк, которые встречаются как подстроки в s и t одновременно. Найдите k-ую в
лексикографическом порядке строку из этого множества. В первой строке выведите
искомую строку или -1, если такой не существует.
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class MinimumTree {
 public:
  MinimumTree(std::vector<int>& some_vector, const int max_value)
      : max_value(max_value) {
    tree.resize(4 * some_vector.size(), max_value);
    Build(some_vector, 1, 0, some_vector.size() - 1);
  }

  int GetMin(int vertex, int tree_left, int tree_right, int left, int right) {
    if (left > right) {
      return max_value;
    }
    if (left == tree_left && right == tree_right) {
      return tree[vertex];
    }

    int tree_middle = (tree_left + tree_right) / 2;
    return std::min(GetMin(2 * vertex, tree_left, tree_middle, left,
                           std::min(right, tree_middle)),
                    GetMin(2 * vertex + 1, tree_middle + 1, tree_right,
                           std::max(left, tree_middle + 1), right));
  }

 private:
  std::vector<int> tree;
  const int max_value;

  void Build(std::vector<int>& sv, int vertex, int tree_left, int tree_right) {
    if (tree_left == tree_right) {
      tree[vertex] = sv[tree_left];
    } else {
      int tree_middle = (tree_left + tree_right) / 2;
      Build(sv, 2 * vertex, tree_left, tree_middle);
      Build(sv, 2 * vertex + 1, tree_middle + 1, tree_right);
      tree[vertex] = std::min(tree[2 * vertex], tree[2 * vertex + 1]);
    }
  }
};

class Solver {
 public:
  std::string CommonSubstringNumberK(std::string& str1, std::string& str2,
                                     long long k) {
    int size1 = str1.size();
    str1 += '$' + str2 + '#';
    CountSuffixArray(str1);
    CountLCP(str1);
    MinimumTree min_tree_lcp(longest_common_prefix, str1.size());

    int prev_ind = 0;
    int break_index = -1;
    for (int i = 0; i < str1.size() - 1; i++) {
      // if suffixes belong to different strings
      if (suffix_array[i] < size1 && suffix_array[i + 1] > size1 ||
          suffix_array[i] > size1 && suffix_array[i + 1] < size1) {
        int new_common_substrs =
            longest_common_prefix[i] -
            min_tree_lcp.GetMin(1, 0, str1.size() - 1, prev_ind, i);
        if (k - new_common_substrs > 0) {
          k -= new_common_substrs;
        } else {
          break_index = i;
          break;
        }
        prev_ind = i;
      }
    }
    if (break_index == -1) {
      return "$";
    } else {
      return str1.substr(suffix_array[break_index],
                         k + min_tree_lcp.GetMin(1, 0, str1.size() - 1,
                                                 prev_ind, break_index));
    }
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
  std::string str1, str2;
  long long k;
  std::cin >> str1 >> str2 >> k;

  Solver solution;
  std::string res = solution.CommonSubstringNumberK(str1, str2, k);
  if (res == "$") {
    std::cout << -1;
  } else {
    std::cout << res;
  }
  return 0;
}