#pragma once

#include <string>
#include <vector>

using doubleV = std::vector<double>;
using stringV = std::vector<std::string>;

namespace Utilities::StringTools
{

bool InsensitiveStringCompare(const std::string &str1, const std::string &str2) ;
std::string LowerString(const std::string &str) ;
bool CheckComment(const std::string &input, const std::string &comment = "#");

int SplitString(const std::string &input, stringV &out_vect, char sep = ':',
		bool show = false);
int SplitString(const std::string &input, stringV &out_vect,
		const std::string &sep = "||", bool show = false);

void PrintString(const std::string &input, char sepIn = '\n',
		 char sepOut = '\n', bool show = false);
void PrintStrings(const stringV &inputs, char sep = '\n',
		  const std::string &msg = "", bool show = false);

void RemoveUselessEmpty(std::string &text, int version = 1);
std::string ltrim(const std::string &s) ;
std::string rtrim(const std::string &s) ;
void RemoveTrailingEmpty(std::string &text, int version = 1);
void RemoveTrailingEmpty(char *text);
char *ltrim(char *const s);
char *rtrim(char *const s);

}
