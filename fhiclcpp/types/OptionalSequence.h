#ifndef fhiclcpp_types_OptionalSequence_h
#define fhiclcpp_types_OptionalSequence_h

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/types/detail/SequenceBase.h"
#include "fhiclcpp/types/detail/ostream_helpers.h"
#include "fhiclcpp/types/detail/SeqVectorBase.h"
#include "fhiclcpp/type_traits.h"

#include <array>
#include <string>
#include <type_traits>

namespace fhicl {

  class ParameterSet;

  //==================================================================
  // e.g. OptionalSequence<int,4> ====> std::array<int,4>
  //
  template<typename T, std::size_t N = -1ull>
  class OptionalSequence final :
    public  detail::SequenceBase,
    private detail::RegisterIfTableMember {
  public:

    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS );
    static_assert(!tt::is_optional_parameter<T>::value, NO_OPTIONAL_TYPES );

    using ftype = std::array< std::shared_ptr< tt::fhicl_type<T> >, N >;
    using rtype = std::array< tt::return_type<T>, N >;

    explicit OptionalSequence(Name&& name);
    explicit OptionalSequence(Name&& name, Comment&& comment);
    explicit OptionalSequence(Name&& name, Comment&& comment, std::function<bool()> maybeUse);

    bool operator()(rtype& t) const {

      if (!has_value_) return false;

      rtype result = { tt::return_type<T>() };
      cet::transform_all(value_,
                         result.begin(),
                         [](auto const& elem){
                           return (*elem)();
                         } );

      std::swap(result, t);
      return true;
    }

  private:

    ftype value_;
    bool has_value_ {false};

    std::size_t get_size() const override { return value_.size(); }

    void do_walk_elements(detail::ParameterWalker<tt::const_flavor::require_non_const>& pw) override
    {
      cet::for_all(value_, [&pw](auto& e){ pw(*e); } );
    }

    void do_walk_elements(detail::ParameterWalker<tt::const_flavor::require_const>& pw) const override
    {
      cet::for_all(value_, [&pw](auto const& e){ pw(*e); } );
    }

    void do_set_value(fhicl::ParameterSet const&, bool const trimParents) override;

  };

  //==================================================================
  // e.g. OptionalSequence<int> ====> std::vector<int>
  //
  template<typename T>
  class OptionalSequence<T,-1ull> final :
    public  detail::SeqVectorBase,
    private detail::RegisterIfTableMember {
  public:

    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS );
    static_assert(!tt::is_optional_parameter<T>::value, NO_OPTIONAL_TYPES );

    using ftype = std::vector< std::shared_ptr<tt::fhicl_type<T>> >;
    using rtype = std::vector< tt::return_type<T> >;

    explicit OptionalSequence(Name&& name);
    explicit OptionalSequence(Name&& name, Comment&& comment);
    explicit OptionalSequence(Name&& name, Comment&& comment, std::function<bool()> maybeUse);

    bool operator()(rtype& t) const {

      if (!has_value_) return false;

      rtype result;
      cet::transform_all(value_, std::back_inserter(result),
                         [](auto const& e){
                           return (*e)();
                         } );

      std::swap(result, t);
      return true;
    }

  private:

    ftype value_;
    bool has_value_ {false};

    // To be used only for reassigning keys when ParameterSet
    // validation is being performed.
    void do_resize_sequence(std::size_t n) override
    {
      if ( n < value_.size() ) {
        value_.resize(n);
      }
      else if ( n > value_.size() ) {

        std::string key_fragment {key()};
        // When emplacing a new element, do not include in the key
        // argument the current name-stack stem--it will
        // automatically be prepended.
        auto const& nsr = NameStackRegistry::instance();
        if ( !nsr.empty() ) {
          std::string const& current_stem = nsr.current();
          std::size_t const pos =
            key_fragment.find(current_stem) != std::string::npos ?
            current_stem.size()+1ul : // + 1ul to account for the '.'
            0ul;
          key_fragment.replace(0ul, pos, "");
        }

        for ( auto i = value_.size(); i != n; ++i ) {
          value_.emplace_back(new tt::fhicl_type<T>{ Name::sequence_element(key_fragment, i) });
        }
      }
    }

    std::size_t get_size() const override { return value_.size(); }

    void do_walk_elements(detail::ParameterWalker<tt::const_flavor::require_non_const>& pw) override
    {
      cet::for_all(value_, [&pw](auto& e){ pw(*e); } );
    }

    void do_walk_elements(detail::ParameterWalker<tt::const_flavor::require_const>& pw) const override
    {
      cet::for_all(value_, [&pw](auto const& e){ pw(*e); } );
    }


    void do_set_value(fhicl::ParameterSet const&, bool const trimParents) override;

  };

}

#include "fhiclcpp/types/detail/OptionalSequence.icc"

#endif

// Local variables:
// mode: c++
// End:
