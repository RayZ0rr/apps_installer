#include "arguments.hpp"
#include "processtools.hpp"
#include "stringtools.hpp"
#include "misctools.hpp"

#include <iostream>

namespace UPT = Utilities::ProcessTools;

namespace Utilities::Arguments
{

void CheckCommand(const std::string &command)
{
  if (command.empty())
    throw CLI::ValidationError("Command empty");
  stringV split_cmds;
  bool check{ UPT::CommandExists(command, false) };
  if (not check)
    std::cout << "warning: " << command << " doesn't exist." << std::endl;
}

std::string CheckCommandShow(const std::string &command)
{
  if (command.empty())
    throw CLI::ValidationError("Command empty");
  stringV split_cmds;
  bool check{ UPT::CommandExists(command, true) };
  if (not check)
    return "NULL";
  return command;
}

void UseArgs(CLI::App &apps, arguments &args)
{
  // apps.name(args.m_name);
  // add version output
  // apps.set_version_flag("-V,--version", std::string(CLI11_VERSION));
  apps.set_version_flag("-V,--version",
			"multrun (multiple command runner)\n1.0.0");
  apps.positionals_at_end();
  // apps.allow_extras(); // Do not throw an error if extra arguments are left over.

  CLI::Option *cmd_opt = apps.add_option("cmd", args.m_args, "Commands to run");
  cmd_opt->required();
  cmd_opt->take_all();
  // cmd_opt->allow_extra_args(true) ;
  cmd_opt->allow_extra_args();
  cmd_opt->each(CheckCommand);
  // cmd_opt->each([&](std::string arg) { std::cout << "file callback invoked: " << arg << "\n"; }) ;
  // cmd_opt->check( CLI::Validator("Check if command exists").operation(CheckCommand) ) ;
  args.m_options.push_back(cmd_opt);

  CLI::Option *use_opt =
    apps.add_option("-u,--use", "Which version of algorithm to use");
  use_opt->default_val(1);
  use_opt->option_text(" (default : 1) ");
  use_opt->multi_option_policy(CLI::MultiOptionPolicy::TakeLast);
  args.m_options.push_back(use_opt);

  CLI::Option *out_flag = apps.add_flag("-d,--details", args.m_out,
					"Show detailed command run process");
  out_flag->needs(cmd_opt);
  out_flag->default_val(false);
  out_flag->option_text(" ");
  args.m_flags.push_back(out_flag);

  CLI::Option *ver_flag = apps.add_flag("-v,--verbose", "More verbose output");
  ver_flag->needs(cmd_opt);
  ver_flag->default_val(false);
  ver_flag->option_text(" ");
  args.m_flags.push_back(ver_flag);

  CLI::Option *err_flag =
    apps.add_flag("-e,--error", "Include stderr in the final output");
  err_flag->needs(cmd_opt);
  err_flag->default_str("1");
  err_flag->option_text(" ");
  args.m_flags.push_back(err_flag);

  CLI::Option *simple_flag = apps.add_flag(
    "-s,--simple",
    "Shows a simple output( Takes higher priority than -v and -O )");
  simple_flag->needs(cmd_opt);
  simple_flag->take_first();
  simple_flag->default_val(false);
  simple_flag->option_text(" ");
  args.m_flags.push_back(simple_flag);

  CLI::Option *single_flag = apps.add_flag(
    "-S,--single",
    "Shows a simple output( Takes higher priority than -v and -O )");
  single_flag->needs(cmd_opt);
  single_flag->take_first();
  single_flag->default_val(false);
  single_flag->option_text(" ");
  args.m_flags.push_back(single_flag);
}

}
