#include "stringtools.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>

namespace Utilities::StringTools
{

bool InsensitiveStringCompare(const std::string &str1, const std::string &str2)
{
  if (str1.size() != str2.size()) {
    return false;
  }
  return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(),
		    [](char a, char b) { return tolower(a) == tolower(b); });
  return true;
}

std::string LowerString(const std::string &str)
{
  std::string result ;
  std::transform(str.begin(), str.end(), result.begin(),
    [](unsigned char c){ return std::tolower(c); });
  return result ;
}

bool CheckComment(const std::string &input, const std::string &comment)
{
  std::istringstream line_stream{ input };
  std::string tmp_str;
  line_stream >> tmp_str;
  std::size_t found{ tmp_str.find(comment) };
  return (found == 0);
}

int SplitString(const std::string &input, stringV &out_vect, char sep,
		bool show)
{
  int counter{ 0 };
  std::istringstream sstream{ input };
  std::string tmp_string;
  while (std::getline(sstream, tmp_string, sep)) {
    out_vect.push_back(tmp_string);
    if (show)
      std::cerr << "Splitted string #" << counter << " : " << tmp_string
		<< std::endl;
    ++counter;
  }
  if (show)
    std::cerr << "Split count : " << counter << std::endl;
  return counter;
}

int SplitString(const std::string &input, stringV &out_vect,
		const std::string &sep, bool show)
{
  int counter{ 0 };
  std::string tmp_string;
  const std::size_t sep_length = sep.length();
  std::size_t pos_begin{ 0 }, pos_sep{ 0 };
  do {
    pos_sep = input.find(sep, pos_begin);
    if (pos_sep == std::string::npos)
      pos_sep = input.length();
    const std::string &token = input.substr(pos_begin, pos_sep - pos_begin);
    out_vect.push_back(token);
    pos_begin = pos_sep + sep_length;
    ++counter;
    if (show)
      std::cerr << "Splitted string #" << counter << " : " << token
		<< std::endl;
  } while (pos_sep < input.length() && pos_begin < input.length());
  if (show)
    std::cerr << "Split count : " << counter << std::endl;
  return counter;
}

void PrintString(const std::string &input, char sepIn, char sepOut, bool show)
{
  std::istringstream sstream{ input };
  std::string tmp_string;
  int counter{ 1 };
  while (std::getline(sstream, tmp_string, sepIn)) {
    std::cout << counter << " : " << tmp_string << sepOut;
    ++counter;
  }
  std::cout << std::endl;
  if (show)
    std::cout << "Total : " << (counter - 1) << std::endl;
}

void PrintStrings(const stringV &inputs, char sep, const std::string &msg,
		  bool show)
{
  if (not msg.empty())
    std::cout << msg;
  int counter{ 1 };
  for (const std::string &elmnt : inputs) {
    std::cout << "#" << counter << " : " << elmnt << sep;
    ++counter;
  }
  std::cout.flush();
  if (show)
    std::cout << "Total : " << (counter - 1) << std::endl;
}

void RemoveUselessEmpty(std::string &text, int version)
{
  switch (version) {
  case 1:
  default: {
    std::locale loc;
    auto start = text.begin();
    while (start != text.end() && std::isspace(*start, loc)) {
      start++;
    }

    auto end = text.end();
    do {
      end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end, loc));

    text = std::string(start, end + 1);
    break;
  }
  case 2: {
    auto it_begin = std::find_if(text.begin(), text.end(), [](char c) {
      return !std::isspace<char>(c, std::locale::classic());
    });
    if (it_begin != text.end())
      text.erase(text.begin(), it_begin);
    auto it_end = std::find_if(text.rbegin(), text.rend(), [](char c) {
      return !std::isspace<char>(c, std::locale::classic());
    });
    if (it_end != text.rend())
      text.erase(it_end.base(), text.end());
    break;
  }
  }
}

const std::string WHITESPACE = " \n\r\t\f\v";
std::string ltrim(const std::string &s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

void RemoveTrailingEmpty(std::string &text, int version)
{
  switch (version) {
  case 1:
  default: {
    std::locale loc;
    while (!text.empty() && std::isspace(text[text.length() - 1], loc)) {
      text.erase(text.length() - 1);
    }
    break;
  }
  case 2:
    while (!text.empty() && (text[text.length() - 1] == '\n')) {
      // text.erase(text.length() - 1);
      text.pop_back();
    }
    break;

  case 3:
    std::size_t check_end = text.find_last_of('\n');
    while (check_end != std::string::npos) {
      if (check_end == (text.length() - 1)) {
	// std::cout << "End by newline" << std::endl;
	// std::cout << "Newline at " << check_end << std::endl;
	text.pop_back();
	check_end = text.find_last_of('\n');
      } else
	break;
    }
    break;
  }
}

/* Remove leading and trailing whitespaces */
void RemoveTrailingEmpty(char *text)
{
  // rtrim(text); // order matters
  // ltrim(text);
  char *ptr;
  if (text && *text) {
    for (ptr = text + strlen(text) - 1; (ptr >= text) && isspace(*ptr); --ptr)
      ;
    // ptr[1] = '\0';
    ptr[isspace(*ptr) ? 0 : 1] = '\0';
  }
}

/* Remove leading whitespaces */
char *ltrim(char *const s)
{
  size_t len;
  char *cur;

  if (s && *s) {
    len = strlen(s);
    cur = s;

    while (*cur && isspace(*cur))
      ++cur, --len;

    if (s != cur)
      memmove(s, cur, len + 1);
  }

  return s;
}

/* Remove trailing whitespaces */
char *rtrim(char *const s)
{
  size_t len;
  char *cur;

  if (s && *s) {
    len = strlen(s);
    cur = s + len - 1;

    while (cur != s && isspace(*cur))
      --cur, --len;

    cur[isspace(*cur) ? 0 : 1] = '\0';
  }

  return s;
}

} // namespace Utilities::StringTools
