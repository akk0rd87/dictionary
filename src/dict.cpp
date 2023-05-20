#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unordered_map>
#include <algorithm>

using wordsMap = std::unordered_map<std::string, unsigned>;

bool isAlphaLower(char c) {
  return 'a' <= c && c <= 'z';
}

bool isAlphaUpper(char c) {
  return 'A' <= c && c <= 'Z';
}

bool isAlpha(char c) {
  return isAlphaLower(c) || isAlphaUpper(c);
}

void toLower(std::string& s) {
  for(auto& c : s) {
    c = std::tolower(c);
  }
}

void add2Map(wordsMap& map, std::string& word) {
  toLower(word);
  ++map[word];
}

wordsMap readStream(std::istream& inStream) {
  wordsMap wMap;
  std::string buffer;
  char c;

  while(inStream && inStream.get(c)) {
    if(!isAlpha(c)) {
      if(!buffer.empty()) {
        add2Map(wMap, buffer);
        buffer.clear();
      }
    }
    else {
      buffer += c;
    }
  }

  if(!buffer.empty()) {
    add2Map(wMap, buffer);
  }

  return wMap;
}

void map2Stream(const wordsMap& wMap, std::ostream& os) {
  using pair_type = std::pair<std::string, unsigned>;
  std::vector<pair_type> vc;
  vc.reserve(wMap.size());

  for(const auto& v : wMap) {
    vc.emplace_back(v.first, v.second);
  }

  const auto Comparator = [](const pair_type& v1, const pair_type& v2) {
    return v1.second > v2.second || v1.second == v2.second && v1.first < v2.first;
  };

  std::sort(std::begin(vc), std::end(vc), Comparator);
  for(const auto& v : vc) {
    os << v.second << ": " << v.first << std::endl;
  }
}

int main() {
  std::stringstream ss;
  ss << "The time has come, the Walrus said,\nto talk of many things...";
  auto mp = readStream(ss);
  map2Stream(mp, std::cout);

  return 0;
}