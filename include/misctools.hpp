#pragma once

#include <string>
#include <filesystem>

namespace stdfs = std::filesystem;

namespace Utilities::MiscTools
{
void PrintLine() ;
void PrintHeader(const std::string &msg) ;

unsigned RandomGen() ;

inline void debugD(const std::string &msg) ;
template <class T> inline void debugV(const T &var, const std::string &msg = "") ;

stdfs::path GetCleanPath(const std::string &src) ;
}
