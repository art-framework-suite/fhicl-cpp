#define BOOST_TEST_MODULE (fhicl - get test)

#include "boost/test/unit_test.hpp"

#include "tools/fhicl_get_impl.h"

#include <initializer_list>
#include <iostream>
#include <tuple>
#include <vector>

using fhicl::detail::fhicl_get_impl;

namespace {
  class redirect_streams {
  public:
    ~redirect_streams()
    {
      std::cout.rdbuf(oldstdout_);
      std::cerr.rdbuf(oldstderr_);
    }

    auto
    stdout() const
    {
      return stdout_.str();
    }
    auto
    stderr() const
    {
      return stderr_.str();
    }

  private:
    std::stringstream stdout_{};
    std::stringstream stderr_{};
    std::streambuf* oldstdout_{std::cout.rdbuf(stdout_.rdbuf())};
    std::streambuf* oldstderr_{std::cerr.rdbuf(stderr_.rdbuf())};
  };

  auto
  run(std::vector<char const*> args)
  {
    args.insert(begin(args), "fhicl-get");
    redirect_streams sentry{};
    auto const rc = fhicl_get_impl(size(args), const_cast<char**>(data(args)));
    return make_tuple(rc, sentry.stdout(), sentry.stderr());
  }
}

BOOST_AUTO_TEST_SUITE(fhicl_get_test)

BOOST_AUTO_TEST_CASE(no_arguments)
{
  auto const [rc, stdout, stderr] = run({});
  BOOST_TEST(rc == 1);
  BOOST_TEST(stdout == "");
  BOOST_TEST(stderr.find("Missing input configuration file.") !=
             std::string::npos);
}

BOOST_AUTO_TEST_CASE(help_message)
{
  auto const [rc, stdout, stderr] = run({"-h"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout.find("Usage: fhicl-get [options] [key] <file>") !=
             std::string::npos);
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(missing_key)
{
  auto const [rc, stdout, stderr] =
    run({"--atom-as", "string", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 1);
  BOOST_TEST(stdout == "");
  BOOST_TEST(
    stderr.find(
      "A key must be specified unless the '--names-in' option is used.") !=
    std::string::npos);
}

BOOST_AUTO_TEST_CASE(retrieve_string)
{
  auto const [rc, stdout, stderr] =
    run({"--atom-as", "string", "a.billy", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "the kid\n");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(retrieve_double)
{
  auto const [rc, stdout, stderr] =
    run({"--atom-as", "double", "c", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "1.25\n");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(retrieve_int)
{
  auto const [rc, stdout, stderr] =
    run({"--atom-as", "int", "d", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "42\n");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(retrieve_bool)
{
  auto const [rc, stdout, stderr] =
    run({"--atom-as", "bool", "e", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "0\n");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(retrieve_sequence_int)
{
  auto const [rc, stdout, stderr] =
    run({"--sequence-of", "int", "f", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "1\n3\n");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(unsupported_types)
{
  auto const [rc, stdout, stderr] =
    run({"--atom-as", "float", "e", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 1);
  BOOST_TEST(stdout == "");
  BOOST_TEST(stderr.find("The specified type 'float' is not supported") !=
             std::string::npos);
}

BOOST_AUTO_TEST_CASE(names_in_file)
{
  auto const [rc, stdout, stderr] = run({"--names-in", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "a\nb\nc\nd\ne\nf\n");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(names_in_nonempty_table)
{
  auto const [rc, stdout, stderr] = run({"--names-in", "a", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "billy\n");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(names_in_empty_table)
{
  auto const [rc, stdout, stderr] = run({"--names-in", "b", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(names_in_nonexistent_table_1)
{
  auto const [rc, stdout, stderr] =
    run({"--names-in", "g.h", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 1);
  BOOST_TEST(stdout == "");
  BOOST_TEST(
    stderr.find(
      "A parameter with the fully-qualified key 'g.h' does not exist.") !=
    std::string::npos);
}

BOOST_AUTO_TEST_CASE(names_in_nonexistent_table_2)
{
  auto const [rc, stdout, stderr] =
    run({"--names-in", "g.h", "--allow-missing", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 0);
  BOOST_TEST(stdout == "");
  BOOST_TEST(stderr == "");
}

BOOST_AUTO_TEST_CASE(names_in_nontable)
{
  auto const [rc, stdout, stderr] =
    run({"--names-in", "c", "--allow-missing", "fhicl-get-t.fcl"});
  BOOST_TEST(rc == 1);
  BOOST_TEST(stdout == "");
  BOOST_TEST(
    stderr.find("The parameter named 'c' does not have a table value.") !=
    std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
