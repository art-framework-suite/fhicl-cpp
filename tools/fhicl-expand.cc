// ======================================================================
//
// fhicl-expand: read/write specified text, replacing each #include
//               directive with the corresponding file's contents
//
//  If called with no arguments, fhicl-expand reads from standard in.
//  If called with one or more arguments, each argument is
//  interepreted as the name of a file to be processed. If '-' (a
//  single hyphen) is passed as a filename, this is interpreted as
//  instruction to read from standard input.
//
//  By default, the expanded inputs are all written to standard
//  output, and any errors during processing are written to standard
//  error. The -o (or --output) and -e (or --error) options can be
//  used to send output to the named files.
//  ======================================================================

#include "boost/program_options.hpp"
#include "cetlib/container_algorithms.h"
#include "cetlib/filepath_maker.h"
#include "cetlib/includer.h"
#include "cetlib/ostream_handle.h"
#include "cetlib/parsed_program_options.h"
#include "cetlib_except/exception.h"

#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <string>
#include <vector>

namespace {

  std::string const fhicl_env_var{"FHICL_FILE_PATH"};

  // Error categories
  std::string const help{"Help"};
  std::string const processing{"Processing"};

  using std::string;
  using strings = std::vector<string>;

  struct Options {
    string error_filename;
    string output_filename;
    strings input_filenames;
    int lookup_policy{};
    string lookup_path;
  };

  Options process_arguments(int argc, char** argv);

  std::unique_ptr<cet::filepath_maker> get_policy(
    int const lookup_policy,
    std::string const& lookup_path);

  int do_including(std::string const& starting_filename,
                   cet::filepath_maker& lookup_policy,
                   std::ostream& to,
                   std::ostream& err);

  cet::ostream_handle
  select_stream(std::ostream& os, std::string const& filename)
  {
    return empty(filename) ? cet::ostream_handle{os} :
                             cet::ostream_handle{filename};
  }
}

// ===================================================================

int
main(int argc, char** argv)
{
  Options opts;
  try {
    opts = process_arguments(argc, argv);
  }
  catch (cet::exception const& e) {
    if (e.category() == help)
      return 0;
    if (e.category() == processing)
      return 1;
  }

  // Set output/error streams
  auto out = select_stream(std::cout, opts.output_filename);
  auto err = select_stream(std::cerr, opts.error_filename);

  auto const policy = get_policy(opts.lookup_policy, opts.lookup_path);

  int nfailures{};
  for (std::string const& filename : opts.input_filenames)
    nfailures += do_including(filename, *policy, out, err);

  return nfailures;
}

// ===================================================================

namespace {

  Options
  process_arguments(int argc, char** argv)
  {
    using namespace std;
    namespace bpo = boost::program_options;

    // Parse the command line:

    bpo::options_description desc("fhicl-expand <options> [files]\nOptions");
    Options opts;
    // clang-format off
    desc.add_options()
      ("help,h", "produce help message")
      ("inputs,i", bpo::value<strings>(&opts.input_filenames), "input files")
      ("output,o", bpo::value<string>(&opts.output_filename), "output file")
      ("error,e", bpo::value<string>(&opts.error_filename), "error file")
      ("lookup-policy,l",
         bpo::value<int>(&opts.lookup_policy)->default_value(1),
         "lookup policy code:"
         "\n  0 => cet::filepath_maker"
         "\n  1 => cet::filepath_lookup"
         "\n  2 => cet::filepath_lookup_nonabsolute"
         "\n  3 => cet::filepath_lookup_after1")
      ("path,p",
         bpo::value<string>(&opts.lookup_path)->default_value(fhicl_env_var),
        "path or environment variable to be used by lookup-policy");
    // clang-format on

    bpo::positional_options_description pd;
    pd.add("inputs", -1);

    auto const varmap = cet::parsed_program_options(argc, argv, desc, pd);

    // Interpret options:

    if (varmap.count("help")) {
      std::cout << desc << "\n";
      throw cet::exception(help);
    }

    if (opts.input_filenames.empty())
      opts.input_filenames.push_back("-");

    return opts;
  }

  std::unique_ptr<cet::filepath_maker>
  get_policy(int const lookup_policy, std::string const& lookup_path)
  {
    std::cerr << "Policy is " << lookup_policy << "; path is \"" << lookup_path
              << "\"\n";
    switch (lookup_policy) {
      case 0:
        return std::make_unique<cet::filepath_maker>();
      case 1:
        return std::make_unique<cet::filepath_lookup>(lookup_path);
      case 2:
        return std::make_unique<cet::filepath_lookup_nonabsolute>(lookup_path);
      case 3:
        return std::make_unique<cet::filepath_lookup_after1>(lookup_path);
      default:
        std::ostringstream err_stream;
        err_stream << "Error: command line lookup-policy " << lookup_policy
                   << " is unknown; choose 0, 1, 2, or 3\n";
        throw std::runtime_error(err_stream.str());
    }
  }

  int
  do_including(std::string const& starting_filename,
               cet::filepath_maker& lookup_policy,
               std::ostream& to,
               std::ostream& err) try {
    cet::includer inc{starting_filename, lookup_policy};
    cet::copy_all(inc, std::ostream_iterator<char>{to});
    return 0;
  }
  catch (cet::exception const& e) {
    err << "Error for file: " << starting_filename << '\n' << e.what() << '\n';
    return 1;
  }
}
