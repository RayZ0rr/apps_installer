#include "filetools.hpp"
#include "stringtools.hpp"

// Stdlib header file for input and output.
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

namespace UST = Utilities::StringTools;

namespace Utilities::FileTools
{

template <typename T>
void GetArgs(const int argCount, char *argVector[], std::vector<T> &fileVector,
	     bool show)
{
  fileVector.clear();
  // Loop through each argument and store its value to the provided string
  // vector
  for (int count{ 1 }; count < argCount; ++count) {
    if (show)
      std::cout << "Getting arg : " << argVector[count] << std::endl;
    fileVector.push_back(argVector[count]);
  }
}
template void GetArgs<char *>(const int argCount, char *argVector[],
			      std::vector<char *> &fileVector, bool show);
template void GetArgs<std::string>(const int argCount, char *argVector[],
				   std::vector<std::string> &fileVector,
				   bool show);

std::string NameWithoutExt(const std::string &name)
{
  std::cout << "Executing NameWithoutExt ..." << '\n';
  const std::string::size_type dotPos = name.find_last_of(".");
  if (!(dotPos == std::string::npos)) {
    std::cout << "dotPos : " << dotPos << '\n';
    const std::string &nameOnly = name.substr(0, dotPos);
    std::cout << "nameOnly : " << nameOnly << '\n';
    const std::string::size_type slashPos = nameOnly.find_last_of("/");
    if (!(slashPos == std::string::npos)) {
      std::cout << "slashPos : " << slashPos << '\n';
      const std::string &lastNameOnly =
	nameOnly.substr(slashPos + 1, nameOnly.length() - 1);
      std::cout << "lastNameOnly : " << lastNameOnly << '\n';
      return lastNameOnly;
    }
  }
  const std::string::size_type slashPos = name.find_last_of("/");
  if (!(slashPos == std::string::npos)) {
    std::cout << "slashPos : " << slashPos << '\n';
    const std::string &lastNameOnly =
      name.substr(slashPos + 1, name.length() - 1);
    std::cout << "lastNameOnly : " << lastNameOnly << '\n';
    return lastNameOnly;
  }
  return name;
}

const std::string &NameWithoutExtRef(const std::string &name)
{
  const std::string::size_type dotPos = name.find_last_of(".");
  if (!(dotPos == std::string::npos)) {
    static const std::string &nameOnly = name.substr(0, dotPos);
    const std::string::size_type slashPos = nameOnly.find_last_of("/");
    if (!(slashPos == std::string::npos)) {
      static const std::string &lastNameOnly =
	nameOnly.substr(slashPos + 1, nameOnly.length() - 1);
      return lastNameOnly;
    }
  }
  const std::string::size_type slashPos = name.find_last_of("/");
  if (!(slashPos == std::string::npos)) {
    static const std::string &lastNameOnly =
      name.substr(slashPos + 1, name.length() - 1);
    return lastNameOnly;
  }
  return name;
}

bool FileCheck( const std::string &fileName)
{
  std::ifstream iFile{ fileName };
  if (!iFile || !iFile.good()) {
    std::cerr << "\nThe file at '" << fileName << "' is not good.\n";
    return false ;
  }
  return true ;
}

std::string File2Str(const std::string &file_name, bool strip_newline)
{
  std::ifstream file;
  std::ostringstream str_stream;

  file.open(file_name);
  if (file.is_open()) {
    str_stream << file.rdbuf();
    file.close();
  }

  std::string final_str = str_stream.str();

  if (strip_newline) {
    UST::RemoveTrailingEmpty(final_str);
  }

  return final_str;
}

int FileLineCount(const std::string &filename)
{
  std::ifstream aFile(filename);
  int lines_count{ 0 };
  std::string line;
  while (std::getline(aFile, line))
    ++lines_count;
  // aFile.seekg(std::ios_base::beg);
  return lines_count;
}

int FileLineCount(std::istream &aFile)
{
  int lines_count{ 0 };
  std::string line;
  while (std::getline(aFile, line))
    ++lines_count;
  // aFile.seekg(std::ios_base::beg);
  return lines_count;
}

void ClearFullStream(std::istream &input)
{
  input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void StreamStatusCheck(std::istream &input)
{
  if (input.fail()) {
    // clear the error state to allow further operations on input
    input.clear();
    ClearFullStream(input);
  } else {
    std::cerr << "\nExtraction successfull" << std::endl;
  }
}

void vec2csv(std::ofstream &outfile, double *vect, const int vectSize)
{
  outfile << "(";
  for (std::size_t idx{ 0 }; static_cast<int>(idx) < vectSize; ++idx) {
    if (static_cast<int>(idx) != (vectSize - 1))
      outfile << vect[idx] << ',';
    else
      outfile << vect[idx] << ')';
  }
  outfile << std::endl;
}

template <typename type>
void vec2csv(std::ofstream &outfile, const std::vector<type> &vect)
{
  outfile << "(";
  for (std::size_t idx{ 0 }; idx < vect.size(); ++idx) {
    if (idx != (vect.size() - 1))
      outfile << vect[idx] << ',';
    else
      outfile << vect[idx] << ')';
  }
  outfile << std::endl;
}
template void vec2csv<double>(std::ofstream &outfile,
			      const std::vector<double> &vect);

void variableWrite(std::ofstream &outfile, const std::string &arrayName,
		   double *vect, const int vectSize)
{
  outfile << arrayName << "(";
  for (std::size_t idx{ 0 }; static_cast<int>(idx) < vectSize; ++idx) {
    if (static_cast<int>(idx) != (vectSize - 1))
      outfile << vect[idx] << ',';
    else
      outfile << vect[idx] << ')';
  }
  outfile << std::endl;
}

template <typename type>
void variableWrite(std::ofstream &outfile, const std::string &arrayName,
		   const std::vector<type> &vect)
{
  outfile << arrayName << "(";
  for (std::size_t idx{ 0 }; idx < vect.size(); ++idx) {
    if (idx != (vect.size() - 1))
      outfile << vect[idx] << ',';
    else
      outfile << vect[idx] << ')';
  }
  outfile << std::endl;
}
template void variableWrite<double>(std::ofstream &outfile,
				    const std::string &arrayName,
				    const std::vector<double> &vect);

template <typename type, std::size_t N>
void variableWrite(std::ofstream &outfile, const std::string &arrayName,
		   const std::array<type, N> &array)
{
  outfile << arrayName << "(";
  for (std::size_t idx{ 0 }; idx < array.size(); ++idx) {
    if (idx != (array.size() - 1))
      outfile << array[idx] << ',';
    else
      outfile << array[idx] << ')';
  }
  outfile << std::endl;
}

void variableWrite(std::ofstream &outfile, const std::string &arrayName)
{
  outfile << arrayName;
}

void FilePrepend(const std::string &text, const std::string &fileName)
{
  std::ifstream inFile(fileName);
  std::stringstream total_data;
  total_data << text;
  total_data << inFile.rdbuf();
  inFile.close();

  std::ofstream outFile(fileName) ;
  outFile << total_data.rdbuf();
}

} // namespace Utilities::FileTools
