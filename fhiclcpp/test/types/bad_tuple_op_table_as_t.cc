#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/OptionalTableAs.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

struct Person {

  string name_;
  unsigned age_{};

  Person() = default;
  Person(string name, unsigned age) : name_{name}, age_{age} {}

  bool
  operator==(Person const& p) const
  {
    return (name_ == p.name_) && (age_ == p.age_);
  }

  bool
  operator!=(Person const& p) const
  {
    return !operator==(p);
  }
};

struct PersonConfig {
  Atom<string> name{Name{"name"}};
  Atom<unsigned> age{Name{"age"}};
};

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalTableAs<Person, PersonConfig>> t{
    Name{"Tuple"}};
}
