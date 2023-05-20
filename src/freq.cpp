#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <stdexcept>

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

void checkArguments(int argc, char *argv[], std::ifstream& ifs, std::ofstream& ofs) {
  const char* usage = 
    "Usage: dict input_file [output_file]\n" \
    "If output_file is not specified, std::cout will be used for output";

  if(argc < 2 || argc > 3) {
    std::cout << usage << std::endl;
    if(argc < 2) {
      throw std::invalid_argument("Error: input_file is not specified");
    }
  }

  std::ifstream ifile(argv[1]);
  if(!ifile) {
    throw std::invalid_argument(std::string("Cannot open file ") + argv[1]);
  }

  if( argc == 3 ){
    std::ofstream ofile(argv[2]);
    if(!ofile) {
      throw std::invalid_argument(std::string("Cannot create file ") + argv[2]);
    }
    ofs = std::move(ofile);
  }

  ifs = std::move(ifile);
}

int main(int argc, char *argv[]) {
  try {
    std::ifstream ifs;
    std::ofstream ofs;
    checkArguments(argc, argv, ifs, ofs);
    auto mp = readStream(ifs);
    map2Stream(mp, ofs.is_open() ? ofs : std::cout);
  }
  catch(const std::exception& ex){
    std::cout << ex.what();
    return 1;
  }

  return 0;
}