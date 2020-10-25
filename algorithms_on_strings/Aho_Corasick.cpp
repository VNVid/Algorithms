/*

Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут
встречаться символы “?”. Найти позиции всех вхождений шаблона в тексте длины n.
Каждое вхождение шаблона предполагает, что все обычные символы совпадают с
соответствующими из текста, а вместо символа “?” в тексте встречается
произвольный символ. Гарантируется, что сам “?” в тексте не встречается.

Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона
“между вопросиками” в исходном тексте. Пример 1 Ввод 	Вывод

ab??aba     2
ababacaba
*/

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>


class Trie {
 public:
  Trie()
      : tree(std::vector<Vertex>(
            1)){};  // there's only root when we create the trie
  void AddWord(const std::string& word, int num) {
    int curVert = 0;
    for (auto i : word) {
      if (!tree[curVert].next.count(i)) {  // no edge with character i
        tree.push_back(Vertex());
        tree.back().parent = curVert;
        tree.back().parentToVertex = i;
        tree[curVert].next[i] = tree.size() - 1;
      }
      curVert = tree[curVert].next[i];
    }
    tree[curVert].wordNumber.push_back(num);
    tree[curVert].wordSize = word.size();
  }

  void ProcessText(const std::string& text, const std::vector<int>& delta,
                   std::vector<int>& counter) {
    int curVert = 0;
    for (int i = 0; i < text.size(); i++) {
      curVert = Go(curVert, text[i]);
      VisitAllDictionaryLinks(curVert, delta, counter, i, NULL_VERTEX);
    }
  }

 private:
  struct Vertex {
    std::vector<int> wordNumber;  // number of word in pattern
    int wordSize;
    std::map<char, int> next;
    int parent = -1;
    char parentToVertex;
    int suffixLink = -1;
    std::map<char, int> Go;
    int dictionaryLink = -1;
  };

  std::vector<Vertex> tree;
  const int NULL_VERTEX = -1;  // a vertex that doesn't exist

  int GetSuffixLink(int vert) {
    if (tree[vert].suffixLink == -1) {
      if (vert == 0 || tree[vert].parent == 0) {
        tree[vert].suffixLink = 0;
      } else {
        tree[vert].suffixLink =
            Go(GetSuffixLink(tree[vert].parent), tree[vert].parentToVertex);
      }
    }
    return tree[vert].suffixLink;
  }
  int Go(int vert, char c) {
    if (!tree[vert].Go.count(c)) {
      if (tree[vert].next.count(c)) {
        tree[vert].Go[c] = tree[vert].next[c];
      } else if (vert == 0) {
        tree[vert].Go[c] = 0;
      } else {
        tree[vert].Go[c] = Go(GetSuffixLink(vert), c);
      }
    }
    return tree[vert].Go[c];
  }

  int GetDictionaryLink(int vert) {
    if (tree[vert].dictionaryLink == -1) {
      if (vert == 0 || tree[vert].parent == 0) {
        tree[vert].dictionaryLink = 0;
      } else {
        tree[vert].dictionaryLink = GetDictionaryLink(GetSuffixLink(vert));
      }
    }
    if (tree[vert].wordNumber.size() != 0) {
      return vert;
    }
    return tree[vert].dictionaryLink;
  }
  void VisitAllDictionaryLinks(int vert, const std::vector<int>& delta,
                               std::vector<int>& counter, int index,
                               int prevVert) {
    if (vert == 0 || vert == prevVert) {
      return;
    }
    for (size_t i = 0; i < tree[vert].wordNumber.size(); i++) {
      if (index - tree[vert].wordSize + 1 + delta[tree[vert].wordNumber[i]] <
          counter.size()) {
        ++counter[index - tree[vert].wordSize + 1 +
                  delta[tree[vert].wordNumber[i]]];
      }
    }
    GetDictionaryLink(vert);
    VisitAllDictionaryLinks(tree[vert].dictionaryLink, delta, counter, index,
                            vert);
  }
};

void PatternFound(int index) { std::cout << index << " "; }

class Solver {
 public:
  Solver(std::string& pattern): pattern(pattern) {
    std::string word;
    for (int i = 0; i < pattern.size(); i++) {
      if (pattern[i] == static_cast<char>('?')) {
        if (word.size()) {
          tree.AddWord(word, delta.size());
          delta.push_back(pattern.size() - i + word.size() - 1);
          word = "";
        }
      } else {
        word += pattern[i];
      }
    }
    if (word.size()) {
      tree.AddWord(word, delta.size());
      delta.push_back(word.size() - 1);
      word = "";
    }
  }

  void Solve(std::string& text) {
    std::vector<int> counter(text.size(), 0);
    tree.ProcessText(text, delta, counter);
    int num_of_words = delta.size();
    for (int i = 0; i < text.size(); i++) {
      if (counter[i] == num_of_words &&
          i + 1 - static_cast<int>(pattern.size()) >= 0) {
        PatternFound(i + 1 - pattern.size());
      }
    }
  }

 private:
  std::string pattern;
  Trie tree;
  std::vector<int> delta;  // the length of suffix of pattern(without previous words)
};

int main() {
  std::string pattern;
  std::string text;
  std::cin >> pattern >> text;
  Solver slv(pattern);
  slv.Solve(text);
  return 0;
}