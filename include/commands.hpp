#pragma once

#include <string>
#include <unordered_map>
#include <utility>

namespace Utilities::Commands
{

std::pair<bool, std::string> GetCommand(std::string pack) ;

std::string InstallCommand(const std::string &pack) ;

using stringMap_t = std::unordered_multimap<std::string, std::string>;
using stringPair_t = std::pair<std::string, std::string>;
bool GetPackages(const std::string &fileName, stringMap_t &packsMap,
		 char sep = ',') ;

using mapIteratorPair_t =
  std::pair<stringMap_t::const_iterator, stringMap_t::const_iterator>;
void InstallDefault(const std::string &pack, const stringMap_t &packsMap) ;

void InstallPip(const stringMap_t &packsMap) ;

void InstallScripts(const stringMap_t &packsMap) ;

void InstallAUR(const stringMap_t &packsMap) ;

void InstallCustom(const std::string &pack, const std::string &name, const stringMap_t &packsMap) ;

void InstallPackages(const std::string &pack, const stringMap_t &packsMap) ;

} // namespace Utilities::Commands
