#define BOOST_TEST_MODULE (writesqlitedb test)

#include "boost/test/unit_test.hpp"

#include "fhiclcpp/DatabaseSupport.h"

#include "sqlite3.h"

BOOST_AUTO_TEST_SUITE(write_sqlitedb_test)

BOOST_AUTO_TEST_CASE(write_sqlite)
{
  // Create an empty sqlite db on the filesystem, with no schema.
  sqlite3* db = nullptr;
  int rc = sqlite3_open_v2(
    "test.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

  BOOST_TEST(rc == SQLITE_OK);
  fhicl::parse_file_and_fill_db("db_2.fcl", db);
  rc = sqlite3_close(db);
  BOOST_TEST(rc == SQLITE_OK);
}

BOOST_AUTO_TEST_SUITE_END()
