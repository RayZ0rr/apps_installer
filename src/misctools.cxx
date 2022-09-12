#include "misctools.hpp"
#include <iostream>
#include <string>
#include <random>
#include <filesystem>

namespace stdfs = std::filesystem;

namespace Utilities::MiscTools
{

void PrintLine()
{
  const std::string line{ "\n" + std::string().assign(10, '-') + "\n" };
  std::cout << line;
}

void PrintHeader(const std::string &msg)
{
  const std::string line{ "\n" + std::string().assign(10, '-') + "\n" };
  std::cout << line;
  std::cout << msg;
  std::cout << line;
}

using rng_type = std::mt19937;
unsigned RandomGen()
{
  std::uniform_int_distribution<rng_type::result_type> uniformInt_dist(
    1, 900'000'000);

  // seed rng first:
  std::random_device seed;
  rng_type::result_type const seedval = seed();
  rng_type rng;
  rng.seed(seedval);

  rng_type::result_type random_number = uniformInt_dist(rng);
  return static_cast<unsigned>(random_number);
}

inline void debugD(const std::string &msg)
{
  std::cout << msg << std::endl;
}

template <class T> inline void debugV(const T &var, const std::string &msg)
{
  std::cout << msg;
  std::cout << var << std::endl;
}
template void debugV(const double &var, const std::string &msg);
template void debugV(const std::string &var, const std::string &msg);

stdfs::path GetCleanPath(const std::string &src)
{
  const stdfs::path tmp_src{ src };
  const stdfs::path lexical_src{ tmp_src.lexically_normal() };
  const stdfs::path abs_src{ stdfs::absolute(lexical_src) };
  return stdfs::weakly_canonical(abs_src);
}

}
