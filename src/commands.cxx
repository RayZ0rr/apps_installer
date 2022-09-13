#include "commands.hpp"
#include "filetools.hpp"
#include "processtools.hpp"
#include "stringtools.hpp"
#include "misctools.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace UPT = Utilities::ProcessTools;
namespace UST = Utilities::StringTools;
namespace UMT = Utilities::MiscTools;

namespace Utilities::Commands
{

std::pair<bool, std::string> GetCommand(std::string pack)
{
  UST::RemoveUselessEmpty(pack);
  if (UST::InsensitiveStringCompare(pack, "arch")) {
    return std::make_pair(true, "pacman");
  } else if (UST::InsensitiveStringCompare(pack, "fedora")) {
    return std::make_pair(true, "dnf");
  } else if (UST::InsensitiveStringCompare(pack, "debian")) {
    return std::make_pair(true, "apt");
  } else if (UST::InsensitiveStringCompare(pack, "ubuntu")) {
    return std::make_pair(true, "apt");
  } else if (UST::InsensitiveStringCompare(pack, "yay")) {
    return std::make_pair(true, "yay");
  }
  return std::make_pair(false, "");
}

std::string InstallCommand(const std::string &pack)
{
  if (pack == "pacman") {
    return "sudo pacman -S";
  } else if (pack == "dnf") {
    return "sudo dnf install";
  } else if (pack == "apt") {
    return "sudo apt install";
  } else if (pack == "yay") {
    return "yay -S";
  }
  std::cerr
    << "Error (InstallCommand): Could not get proper install command.\n";
  exit(EXIT_FAILURE);
}

bool GetPackages(const std::string &fileName, stringMap_t &packsMap,
		 char sep)
{
  std::ifstream inFile{ fileName };
  std::string line;
  while (std::getline(inFile, line)) {
    if (UST::CheckComment(line) || line.empty())
      continue;
    std::istringstream line_stream{ line };
    std::string tmp_str;
    std::size_t columns{ 0 };
    stringV elmnt_pair;
    while (std::getline(line_stream, tmp_str, sep)) {
      if (columns == 2)
	break;
      ++columns;
      UST::RemoveUselessEmpty(tmp_str);
      if (tmp_str.empty())
	tmp_str = "default";
      tmp_str = UST::LowerString(tmp_str) ;
      elmnt_pair.push_back(tmp_str);
    }
    if (columns != 2) {
      std::cerr << "Error: Could not process " << fileName << '\n';
      return false;
    }
    packsMap.insert(std::make_pair(elmnt_pair[0], elmnt_pair[1]));
  }
  return true;
}

void InstallDefault(const std::string &pack, const stringMap_t &packsMap)
{
  const std::string &install_cmd{ InstallCommand(pack) + " " };
  std::string output;
  const mapIteratorPair_t range = packsMap.equal_range("default");
  std::for_each(range.first, range.second,
	   [install_cmd, &output](const stringMap_t::value_type &x) {
	     UPT::CommandRunWrite(install_cmd + x.second, output);
	   });
  // std::vector<mapIteratorPair_t> ranges(2, mapIteratorPair_t());
  // ranges[0] = packsMap.equal_range("d");
  // ranges[2] = packsMap.equal_range("default");
  // for (const auto &elmnt : ranges) {
  //   // if (elmnt.first == elmnt.second)
  //   //   break;
  //   for_each(elmnt.first, elmnt.second,
	 //     [&install_cmd, &output](stringMap_t::value_type &x) {
	 //       UPT::CommandRunWrite(install_cmd + x.second, output);
	 //     });
  // }
}

void InstallPip(const stringMap_t &packsMap)
{
  const std::string &install_cmd{ "pip install --user --upgrade " };
  std::string output;
  mapIteratorPair_t range = packsMap.equal_range("pip");
  std::for_each(range.first, range.second,
	   [&install_cmd, &output](const stringMap_t::value_type &x) {
	     UPT::CommandRunWrite(install_cmd + x.second, output);
	   });
}

void InstallScripts(const stringMap_t &packsMap)
{
  std::string output;
  mapIteratorPair_t range = packsMap.equal_range("script");
  std::for_each(range.first, range.second,
	   [&output](const stringMap_t::value_type &x) {
	       UPT::CommandRunWrite(UMT::GetCleanPath(x.second), output);
	   });
}

void InstallAUR(const stringMap_t &packsMap)
{
  const std::string &install_cmd{ InstallCommand("yay") + " " };
  std::string output;
  mapIteratorPair_t range = packsMap.equal_range("aur");
  std::for_each(range.first, range.second,
	   [&install_cmd, &output](const stringMap_t::value_type &x) {
	     UPT::CommandRunWrite(install_cmd + x.second, output);
	   });
}

void InstallCustom(const std::string &pack, const std::string &name, const stringMap_t &packsMap)
{
  const std::string &install_cmd{ InstallCommand(pack) + " " };
  std::string output;
  mapIteratorPair_t range = packsMap.equal_range(name);
  std::for_each(range.first, range.second,
	   [&install_cmd, &output](const stringMap_t::value_type &x) {
	     UPT::CommandRunWrite(install_cmd + x.second, output);
	   });
}

void InstallPackages(const std::string &pack, const stringMap_t &packsMap)
{
  InstallDefault(pack, packsMap);
  InstallPip(packsMap);
  InstallScripts(packsMap);
}

} // namespace Utilities::Commands
