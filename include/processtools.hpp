#ifndef INC_PROCESS_H
#define INC_PROCESS_H

#include <string>
#include <vector>
#include <limits>

using doubleV = std::vector<double>;
using stringV = std::vector<std::string>;

namespace Utilities::ProcessTools
{

constexpr std::size_t STREAM_MAX = std::numeric_limits<std::streamsize>::max();
constexpr std::size_t BUFFER_SIZE = 128;

bool CommandExists(const std::string &command, bool show = false,
		   const std::string &msg = "");

int CommandRunPipe(const std::string &command, std::string &output) ;
int CommandRunWrite(const std::string &cmd, std::string &std_total,
		    int stdoutputs = 1) noexcept(false) ;

int CommandPrintPipe(const std::string &command) ;
int CommandPrintWrite(const std::string &cmd, int stdoutputs=1, const std::string &msg1="",
		    const std::string &msg2="") noexcept(false) ;

} // namespace Utilities::ProcessTools


#endif
