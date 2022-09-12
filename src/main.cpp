#include "filetools.hpp"
#include "commands.hpp"

#include <iostream>
#include <string>

namespace UC = Utilities::Commands;
namespace UFT = Utilities::FileTools;

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Nothing to do. Provide package manager name\n";
    std::cerr << argv[0] << " (optional filename) [apt|pacman|dnf|yay]"
	      << std::endl;
    return 1;
  }

  stringV args_vect;
  UFT::GetArgs<std::string>(argc, argv, args_vect);

  std::string pack_filename{ "./apps.csv" };
  auto [pack_check, pack_cmd] = UC::GetCommand(args_vect[0]);

  if (not pack_check && argc < 2) {
    std::cerr
      << "Error: Invalid package manager name provided.\nChoose one from the following:\n";
    std::cerr << argv[0] << " [apt|pacman|dnf|yay]" << std::endl;
    return 1;
  } else if (not pack_check) {
    pack_filename = args_vect[0];
    std::tie(pack_check, pack_cmd) = UC::GetCommand(args_vect[1]);
    if (not pack_check) {
      std::cerr
	<< "Error: Invalid package manager name provided.\nChoose one from the following:\n";
      std::cerr << argv[0] << " [apt|pacman|dnf|yay]" << std::endl;
      return 1;
    }
  }

  if (not UFT::FileCheck(pack_filename)) {
    std::cerr
      << "Error: Invalid file name provided.\nThe following file could not be processed:\n";
    std::cerr << pack_filename << '\n';
    return 1;
  }
  UC::stringMap_t packages;
  if (not UC::GetPackages(pack_filename, packages)) {
    std::cerr
      << "Error: parsing packaage list from " + pack_filename + " failed.\n";
    return 1;
  }
  UC::InstallPackages(pack_cmd, packages);
  if (pack_cmd == "yay" || pack_cmd == "arch") {
    UC::InstallAUR(packages);
  }
  if (pack_cmd == "dnf" ) {
    UC::InstallCustom(pack_cmd,"fedora",packages);
  }
  if (pack_cmd == "pacman" ) {
    UC::InstallCustom(pack_cmd,"arch",packages);
  }
  if (pack_cmd == "apt" ) {
    UC::InstallCustom(pack_cmd,"debian",packages);
  }

  return 0;
}
