#include "processtools.hpp"
#include "stringtools.hpp"
#include "filetools.hpp"
#include "misctools.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

namespace UFT = Utilities::FileTools;
namespace UMT = Utilities::MiscTools;

namespace stdfs = std::filesystem;

namespace Utilities::ProcessTools
{

bool CommandExists(const std::string &command, const bool show,
		   const std::string &msg)
{
  std::size_t first_space = command.find_first_of(' ');
  std::string cmd_name = command.substr(0, first_space);

  std::istringstream path_stream{ getenv("PATH") };
  std::string path_string;
  while (std::getline(path_stream, path_string, ':')) {
    path_string = path_string.append("/");
    path_string = path_string.append(cmd_name);
    if (std::filesystem::exists(path_string)) {
      if (show) {
	if (!msg.empty()) {
	  std::cout << msg;
	}
	std::cout << cmd_name.append(" : exists at ").append(path_string)
		  << std::endl;
      }
      return true;
    }
  }
  if (show)
    std::cout << cmd_name.append(" : doesn't exist in PATH.") << std::endl;
  return false;
}

int CommandRunPipe(const std::string &command, std::string &output)
{
  const std::unique_ptr<FILE, void (*)(FILE*)> process_pipe{
    popen(command.c_str(), "r"), [](FILE*) { return; }
  };
  if (!process_pipe) {
    // throw std::runtime_error("process ('popen()') failed !");
    return -1 ;
  }

  const unsigned length = BUFFER_SIZE;
  // const unsigned length = 4096 ;
  // std::unique_ptr<char[]> tmp_buffer{ std::make_unique<char[]>(BUFFER_SIZE) };
  // std::vector<char> tmp_buffer(BUFFER_SIZE);
  // std::array<char, 128> tmp_buffer;
  // std::array<char, BUFFER_SIZE> tmp_buffer;
  std::string tmp_buffer;
  tmp_buffer.resize(length);
  const unsigned int buffer_elmnt_size = sizeof(char);
  // const unsigned int buffer_elmnt_size = sizeof(tmp_buffer[0]);

  std::size_t count = fread(tmp_buffer.data(), buffer_elmnt_size, length, process_pipe.get());
  while ( count > 0 ) {
    if (count < length)
      tmp_buffer.resize(count);
    auto buffer_it_start = std::begin(tmp_buffer);
    auto buffer_it_end = std::next(buffer_it_start, static_cast<int>(count));
    output.insert(output.end(), buffer_it_start, buffer_it_end);
    count =
      fread(tmp_buffer.data(), buffer_elmnt_size, length, process_pipe.get());
  }

  int exit_code = pclose(process_pipe.get());
  return exit_code;
}

int CommandRunWrite(const std::string &cmd, std::string &std_total,
		    int stdoutputs) noexcept(false)
{
  // create temp files
  std::error_condition ok;
  std::error_code ec1{ -1, std::system_category() };
  stdfs::path tmp_dir{ stdfs::temp_directory_path(ec1) };
  if (ec1 != ok)
    throw std::runtime_error("\nerror: temporary directory path failed.");

  if (!stdfs::is_directory(tmp_dir))
    if (!std::filesystem::create_directory(tmp_dir)) {
      // Directory could not be created.
      throw std::runtime_error("\nerror: temporary directory creation failed.");
      // throw stdfs::filesystem_error();
    }
  std::string stdout_file = tmp_dir.string() + "/stdout";
  std::string stderr_file = tmp_dir.string() + "/stderr";

  // execute the command "cmd > stdout_file 2> stderr_file"
  std::string cli = cmd + " > " + stdout_file + " 2> " + stderr_file;
  int exit_code{ -1 };
  exit_code = system(cli.c_str());
  exit_code = WEXITSTATUS(exit_code);

  bool remove_last_newline = true;
  const std::string stdout_str =
    UFT::File2Str(stdout_file, remove_last_newline);
  const std::string stderr_str =
    UFT::File2Str(stderr_file, remove_last_newline);

  const std::string custom_sep{ "||SEPARATOR||" };
  if (stdoutputs > 1) {
    std_total = stdout_str + custom_sep + stderr_str;
  } else {
    std_total = stdout_str;
  }

  // Cleanup ( rid of the temp files )
  std::error_code ec2{ -2, std::system_category() };
  remove(stdout_file.c_str());
  remove(stderr_file.c_str());
  if (stdfs::is_empty(tmp_dir) && (tmp_dir.string() != "/tmp")) {
    remove(tmp_dir, ec2);
    if (ec2 != std::error_condition())
      std::cout << ec2.category().name()
		<< " at Error " + std::to_string(ec2.value()) + " :- "
		<< ec2.message() << std::endl;
  }

  return exit_code;
}

int CommandPrintPipe(const std::string &command)
{
  const std::unique_ptr<FILE, void (*)(FILE*)> process_pipe{
    popen(command.c_str(), "r"), [](FILE*) { return; }
  };
  if (!process_pipe) {
    return -1;
  }

  const unsigned length = BUFFER_SIZE;
  std::string tmp_buffer;
  tmp_buffer.resize(length);
  const unsigned int buffer_elmnt_size = sizeof(char);
  std::size_t count = fread(tmp_buffer.data(), buffer_elmnt_size, length, process_pipe.get());
  while ( count > 0 ) {
    if (count < length)
      tmp_buffer.resize(count);
    std::cout << tmp_buffer;
    count =
      fread(tmp_buffer.data(), buffer_elmnt_size, length, process_pipe.get());
  }

  int exit_code = pclose(process_pipe.get());
  return exit_code;
}

int CommandPrintWrite(const std::string &cmd, int stdoutputs, const std::string &msg1,
		    const std::string &msg2) noexcept(false)
{
  // create temp files
  std::error_condition ok;
  std::error_code ec1{ -1, std::system_category() };
  stdfs::path tmp_dir{ std::string(stdfs::temp_directory_path(ec1)) +
		       "/multrun" };
  if (ec1 != ok)
    throw std::runtime_error("\nError: temporary directory path failed.\n");

  if (!stdfs::is_directory(tmp_dir)) {
    if (!std::filesystem::create_directories(tmp_dir)) {
      // Directory could not be created.
      throw std::runtime_error(
	"\nError: temporary directory creation failed.\n");
    }
  }
  // throw stdfs::filesystem_error();
  const std::string random1_str{ std::to_string(UMT::RandomGen()) } ;
  const std::string random2_str{ std::to_string(UMT::RandomGen()) } ;
  const std::string stdout_file = tmp_dir.string() + "/stdout_" + random1_str;
  const std::string stderr_file = tmp_dir.string() + "/stderr_" + random2_str;

  // execute the command "cmd > stdout_file 2> stderr_file"
  std::string cli = cmd + " > " + stdout_file + " 2> " + stderr_file;
  int exit_code{ -1 };
  exit_code = system(cli.c_str());
  exit_code = WEXITSTATUS(exit_code);

  bool remove_last_newline = true;
  if (not msg1.empty()) {
    std::cout << msg1;
  }
  std::cout << UFT::File2Str(stdout_file, remove_last_newline);

  if (stdoutputs > 1) {
    if (not msg2.empty()) {
      std::cout << msg2;
    }
    std::cout << UFT::File2Str(stderr_file, remove_last_newline);
  }

  // Cleanup ( removal of the temp files )
  std::error_code ec2{ -2, std::system_category() };
  remove(stdout_file.c_str());
  remove(stderr_file.c_str());
  if (stdfs::is_empty(tmp_dir) && (tmp_dir.string() != "/tmp")) {
    remove(tmp_dir, ec2);
    if (ec2 != std::error_condition())
      std::cout << ec2.category().name()
		<< " at Error " + std::to_string(ec2.value()) + " :- "
		<< ec2.message() << std::endl;
  }

  return exit_code;
}

} // namespace Utilities::Process
