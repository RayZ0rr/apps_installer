#pragma once

// #ifndef INC_FILETOOLS_H
// #define INC_FILETOOLS_H

// Stdlib header file for input and output.
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using doubleV = std::vector<double>;
using stringV = std::vector<std::string>;

namespace Utilities::FileTools
{

template <typename T> void GetArgs(int argCount, char *argVector[], std::vector<T> &fileVector, bool show=false);

bool FileCheck( const std::string &fileName) ;

std::string File2Str(const std::string &file_name, bool strip_newline=false) ;

int FileLineCount(const std::string &filename) ;

int FileLineCount(std::istream &aFile) ;

std::string NameWithoutExt(const std::string &name = __FILE__);

const std::string &NameWithoutExtRef(const std::string &name = __FILE__);

void vec2csv(std::ofstream &outfile, double *vect,  int vectSize);

template <typename type> void vec2csv(std::ofstream &outfile, const std::vector<type> &vect);

void variableWrite(std::ofstream &outfile, const std::string &arrayName, double *vect,  int vectSize);

template <typename type>
void variableWrite(std::ofstream &outfile, const std::string &arrayName, const std::vector<type> &vect);

template <typename type, std::size_t N>
void variableWrite(std::ofstream &outfile, const std::string &arrayName, const std::array<type, N> &array);

void variableWrite(std::ofstream &outfile, const std::string &arrayName);

void FilePrepend(const std::string &text, const std::string &fileName) ;

} // namespace Utilities::FileTools

// #endif
