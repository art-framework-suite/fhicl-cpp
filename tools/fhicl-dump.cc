// ======================================================================
//
// Executable for dumping processed configuration files
//
// ======================================================================

#include "boost/program_options.hpp"
#include "cetlib/parsed_program_options.h"
#include "cetlib_except/demangle.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/detail/print_mode.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/parse.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace fhicl;
using namespace fhicl::detail;

using std::string;

namespace {

  string const fhicl_env_var{"FHICL_FILE_PATH"};

  // Error categories
  string const help{"Help"};
  string const processing{"Processing"};
  string const config{"Configuration"};

  struct Options {
    print_mode mode{print_mode::raw};
    bool quiet{false};
    string output_filename;
    string input_filename;
    int lookup_policy{};
    string lookup_path;
  };

  Options process_arguments(int argc, char const* argv[]);

  fhicl::ParameterSet form_pset(string const& filename,
                                cet::filepath_maker& lookup_policy);

  std::unique_ptr<cet::filepath_maker> get_policy(int lookup_policy,
                                                  string const& lookup_path);
}

//======================================================================

int
main(int argc, char const* argv[])
{

  Options opts;
  try {
    opts = process_arguments(argc, argv);
  }
  catch (cet::exception const& e) {
    if (e.category() == help)
      return 1;
    if (e.category() == processing) {
      std::cerr << e.what();
      return 2;
    }
    if (e.category() == config) {
      std::cerr << e.what() << '\n';
      return 3;
    }
  }

  auto const policy = get_policy(opts.lookup_policy, opts.lookup_path);

  ParameterSet pset;
  try {
    pset = form_pset(opts.input_filename, *policy);
  }
  catch (cet::exception const& e) {
    std::cerr << e.what() << '\n';
    return 4;
  }
  catch (...) {
    std::cerr << "Unknown exception\n";
    return 5;
  }

  if (opts.quiet)
    return 0;

  std::ofstream ofs{opts.output_filename};
  std::ostream& os = opts.output_filename.empty() ? std::cout : ofs;

  os << "# Produced from '" << argv[0] << "' using:\n"
     << "#   Input  : " << opts.input_filename << '\n'
     << "#   Policy : "
     << cet::demangle_symbol(typeid(decltype(*policy)).name()) << '\n'
     << "#   Path   : \"" << opts.lookup_path << "\"\n\n"
     << pset.to_indented_string(0, opts.mode);
}

//======================================================================

namespace {

  Options
  process_arguments(int argc, char const* argv[])
  {
    namespace bpo = boost::program_options;

    Options opts;

    bool annotate{false};
    bool prefix_annotate{false};

    bpo::options_description desc("fhicl-dump [-c] <file>\nOptions");
    // clang-format off
    desc.add_options()
      ("help,h", "produce this help message")
      ("config,c", bpo::value<std::string>(&opts.input_filename), "input file")
      ("output,o", bpo::value<std::string>(&opts.output_filename),
         "output file (default is STDOUT)")
      ("annotate,a",
         bpo::bool_switch(&annotate),
         "include source location annotations")
      ("prefix-annotate",
         bpo::bool_switch(&prefix_annotate),
         "include source location annotations on line preceding parameter "
         "assignment (mutually exclusive with 'annotate' option)")
      ("quiet,q", "suppress output to STDOUT")
      ("lookup-policy,l",
         bpo::value<int>(&opts.lookup_policy)->default_value(1),
         "lookup policy code:"
         "\n  0 => cet::filepath_maker"
         "\n  1 => cet::filepath_lookup"
         "\n  2 => cet::filepath_lookup_nonabsolute"
         "\n  3 => cet::filepath_lookup_after1")
      ("path,p",
         bpo::value<std::string>(&opts.lookup_path)->default_value(fhicl_env_var),
         "path or environment variable to be used by lookup-policy");
    // clang-format on

    bpo::positional_options_description p;
    p.add("config", -1);

    auto const vm = cet::parsed_program_options(argc, argv, desc, p);

    if (vm.count("help")) {
      std::cout << desc << '\n';
      throw cet::exception(help);
    }

    if (vm.count("quiet")) {
      if (annotate || prefix_annotate) {
        throw cet::exception(config) << "Cannot specify both '--quiet' and "
                                        "'--(prefix-)annotate' options.\n";
      }
      opts.quiet = true;
    }

    if (annotate && prefix_annotate) {
      throw cet::exception(config) << "Cannot specify both '--annotate' and "
                                      "'--prefix-annotate' options.\n";
    }

    if (annotate)
      opts.mode = print_mode::annotated;
    if (prefix_annotate)
      opts.mode = print_mode::prefix_annotated;

    if (!vm.count("config")) {
      std::ostringstream err_stream;
      err_stream << "\nMissing input configuration file.\n\n" << desc << '\n';
      throw cet::exception(config) << err_stream.str();
    }
    return opts;
  }

  std::unique_ptr<cet::filepath_maker>
  get_policy(int const lookup_policy, std::string const& lookup_path)
  {
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

  fhicl::ParameterSet
  form_pset(std::string const& filename, cet::filepath_maker& lookup_policy)
  {
    fhicl::intermediate_table tbl;
    fhicl::parse_document(filename, lookup_policy, tbl);
    fhicl::ParameterSet pset;
    fhicl::make_ParameterSet(tbl, pset);
    return pset;
  }
}
