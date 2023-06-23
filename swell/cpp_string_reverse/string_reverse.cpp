#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <list>

static std::stringstream INPUT_STR("This is a coding test, I like coding because it is fun");

class Interview
{
public:
  Interview() {}

  virtual ~Interview() {}

  static std::string reverse(std::stringstream &ss)
  {
    std::string word;
    std::list<std::string> words;

    // search for duplicates
    auto is_dup = [&words](std::string w)
    {
      return std::find(std::begin(words), std::end(words), w) != std::end(words);
    };

    // append words for return
    auto append = [&words, &word]()
    {
      word.clear();
      for (auto &w : words)
        word += w.append(" ");

      // trim any whitespace
      return word.substr(word.find_first_not_of(" "), word.find_last_not_of(" ") + 1);
    };

    // reverse the string by pushing to front of the list
    while (ss >> word)
    {
      // avoid duplicates
      if (!is_dup(word))
        words.push_front(word);
    }

    return append();
  }
};

int main()
{
  std::cout << "Input:\t" << INPUT_STR.str() << std::endl;
  std::cout << "Output:\t" << Interview::reverse(INPUT_STR) << std::endl;

  return 0;
}