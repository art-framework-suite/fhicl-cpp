#ifndef fhiclcpp_types_OptionalSequence_h
#define fhiclcpp_types_OptionalSequence_h

#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/ParameterWalker.h"
#include "fhiclcpp/types/detail/SequenceBase.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/types/detail/check_nargs_for_bounded_sequences.h"

#include <array>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

namespace fhicl {

  class ParameterSet;

  //==================================================================
  // e.g. OptionalSequence<int,4> ====> std::array<int,4>
  //
  template <typename T, std::size_t N = -1ull>
  class OptionalSequence final : public detail::SequenceBase,
                                 private detail::RegisterIfTableMember {
  public:
    static_assert(!tt::is_table_fragment_v<T>, NO_NESTED_TABLE_FRAGMENTS);
    static_assert(!tt::is_optional_parameter_v<T>, NO_OPTIONAL_TYPES);
    static_assert(!tt::is_delegated_parameter_v<T>, NO_DELEGATED_PARAMETERS);

    using ftype = std::array<std::shared_ptr<tt::fhicl_type<T>>, N>;
    using value_type = std::array<tt::return_type<T>, N>;

    explicit OptionalSequence(Name&& name);
    explicit OptionalSequence(Name&& name, Comment&& comment);
    template <fhicl::maybe_use_param F>
    explicit OptionalSequence(Name&& name, Comment&& comment, F maybeUse);

    std::optional<value_type>
    operator()() const
    {
      if (!has_value_)
        return std::nullopt;

      if (auto value = std::get_if<value_type>(&value_)) {
        return std::make_optional(*value); // Do not move!
      }

      value_type result = {{tt::return_type<T>()}};
      cet::transform_all(std::get<ftype>(value_),
                         result.begin(),
                         [](auto const& elem) { return (*elem)(); });
      return std::make_optional(std::move(result));
    }

    // Obsolete interface
    bool
    operator()(value_type& t) const
    {
      auto const result = operator()();
      if (result) {
        t = *result;
      }
      return result.has_value();
    }

    bool
    hasValue() const noexcept
    {
      return has_value_;
    }

  private:
    std::variant<ftype, value_type> value_;
    bool has_value_{false};

    std::size_t
    get_size() const noexcept override
    {
      if (auto value = std::get_if<value_type>(&value_)) {
        return value->size();
      }
      return std::get<ftype>(value_).size();
    }

    void
    do_prepare_elements_for_validation(std::size_t const n) override
    {
      detail::check_nargs_for_bounded_sequences(key(), get_size(), n);
    }

    void
    do_walk_elements(
      detail::ParameterWalker<tt::const_flavor::require_non_const>& pw) override
    {
      // We only enter here if we do not have a preset value.
      cet::for_all(std::get<ftype>(value_),
                   [&pw](auto& e) { pw.walk_over(*e); });
    }

    void
    do_walk_elements(detail::ParameterWalker<tt::const_flavor::require_const>&
                       pw) const override
    {
      // We only enter here if we do not have a preset value.
      cet::for_all(std::get<ftype>(value_),
                   [&pw](auto const& e) { pw.walk_over(*e); });
    }

    bool do_preset_value(fhicl::ParameterSet const&) override;
    void do_set_value(fhicl::ParameterSet const&) override;
  };

  //==================================================================
  // e.g. OptionalSequence<int> ====> std::vector<int>
  //
  template <typename T>
  class OptionalSequence<T, -1ull> final
    : public detail::SequenceBase,
      private detail::RegisterIfTableMember {
  public:
    static_assert(!tt::is_table_fragment_v<T>, NO_NESTED_TABLE_FRAGMENTS);
    static_assert(!tt::is_optional_parameter_v<T>, NO_OPTIONAL_TYPES);
    static_assert(!tt::is_delegated_parameter_v<T>, NO_DELEGATED_PARAMETERS);

    using ftype = std::vector<std::shared_ptr<tt::fhicl_type<T>>>;
    using value_type = std::vector<tt::return_type<T>>;

    explicit OptionalSequence(Name&& name);
    explicit OptionalSequence(Name&& name, Comment&& comment);
    template <fhicl::maybe_use_param F>
    explicit OptionalSequence(Name&& name, Comment&& comment, F maybeUse);

    std::optional<value_type>
    operator()() const
    {
      if (!has_value_)
        return std::nullopt;

      if (auto value = std::get_if<value_type>(&value_)) {
        return std::make_optional(std::move(*value));
      }

      value_type result;
      cet::transform_all(std::get<ftype>(value_),
                         std::back_inserter(result),
                         [](auto const& elem) { return (*elem)(); });
      return std::make_optional(std::move(result));
    }

    // Obsolete
    bool
    operator()(value_type& t) const
    {
      if (auto result = operator()()) {
        t = *result;
        return true;
      }
      return false;
    }

    bool
    hasValue() const noexcept
    {
      return has_value_;
    }

  private:
    std::variant<ftype, value_type> value_;
    bool has_value_{false};

    void
    do_prepare_elements_for_validation(std::size_t const n) override
    {
      // We only enter here if we do not have a preset value.
      auto& value = std::get<ftype>(value_);

      if (n < value.size()) {
        value.resize(n);
      } else if (n > value.size()) {

        std::string key_fragment{key()};
        // When emplacing a new element, do not include in the key
        // argument the current name-stack stem--it will
        // automatically be prepended.
        if (!NameStackRegistry::empty()) {
          std::string const& current_stem = NameStackRegistry::current();
          std::size_t const pos =
            key_fragment.find(current_stem) != std::string::npos ?
              current_stem.size() + 1ul : // + 1ul to account for the '.'
              0ul;
          key_fragment.replace(0ul, pos, "");
        }

        for (auto i = value.size(); i != n; ++i) {
          value.emplace_back(
            new tt::fhicl_type<T>{Name::sequence_element(key_fragment, i)});
        }
      }
    }

    std::size_t
    get_size() const noexcept override
    {
      if (auto value = std::get_if<value_type>(&value_)) {
        return value->size();
      }
      return std::get<ftype>(value_).size();
    }

    void
    do_walk_elements(
      detail::ParameterWalker<tt::const_flavor::require_non_const>& pw) override
    {
      // We only enter here if we do not have a preset value.
      cet::for_all(std::get<ftype>(value_),
                   [&pw](auto& e) { pw.walk_over(*e); });
    }

    void
    do_walk_elements(detail::ParameterWalker<tt::const_flavor::require_const>&
                       pw) const override
    {
      // We only enter here if we do not have a preset value.
      cet::for_all(std::get<ftype>(value_),
                   [&pw](auto const& e) { pw.walk_over(*e); });
    }

    bool do_preset_value(fhicl::ParameterSet const&) override;
    void do_set_value(fhicl::ParameterSet const&) override;
  };
}

#include "fhiclcpp/detail/printing_helpers.h"

namespace fhicl {

  //==================================================================
  // e.g. OptionalSequence<int,4> ====> std::array<int,4>
  //
  template <typename T, std::size_t N>
  OptionalSequence<T, N>::OptionalSequence(Name&& name)
    : OptionalSequence{std::move(name), Comment("")}
  {}

  template <typename T, std::size_t N>
  OptionalSequence<T, N>::OptionalSequence(Name&& name, Comment&& comment)
    : SequenceBase{std::move(name),
                   std::move(comment),
                   par_style::OPTIONAL,
                   par_type::SEQ_ARRAY,
                   detail::AlwaysUse()}
    , RegisterIfTableMember{this}
    , value_{ftype{nullptr}}
  {
    auto& value = std::get<ftype>(value_);
    for (std::size_t i{}; i != N; ++i) {
      value.at(i) =
        std::make_shared<tt::fhicl_type<T>>(Name::sequence_element(i));
    }
    NameStackRegistry::end_of_ctor();
  }

  template <typename T, std::size_t N>
  template <fhicl::maybe_use_param F>
  OptionalSequence<T, N>::OptionalSequence(Name&& name,
                                           Comment&& comment,
                                           F maybeUse)
    : SequenceBase{std::move(name),
                   std::move(comment),
                   par_style::OPTIONAL_CONDITIONAL,
                   par_type::SEQ_ARRAY,
                   maybeUse}
    , RegisterIfTableMember{this}
    , value_{ftype{nullptr}}
  {
    auto& value = std::get<ftype>(value_);
    for (std::size_t i{}; i != N; ++i) {
      value.at(i) =
        std::make_shared<tt::fhicl_type<T>>(Name::sequence_element(i));
    }
    NameStackRegistry::end_of_ctor();
  }

  template <typename T, std::size_t N>
  bool
  OptionalSequence<T, N>::do_preset_value(fhicl::ParameterSet const& ps)
  {
    if constexpr (std::is_same_v<tt::fhicl_type<T>, Atom<T>>) {
      auto const trimmed_key = detail::strip_first_containing_name(key());
      value_ = ps.get<value_type>(trimmed_key);
      has_value_ = true;
      return true;
    }
    return false;
  }

  template <typename T, std::size_t N>
  void
  OptionalSequence<T, N>::do_set_value(fhicl::ParameterSet const&)
  {
    // We do not explicitly set the sequence values here as the
    // individual elements are set one at a time.  However, this
    // function is reached in the ValidateThenSet algorithm if the
    // optional parameter is present.  Otherwise, this override is
    // skipped.
    has_value_ = true;
  }

  //==================================================================
  // e.g. OptionalSequence<int> ====> std::vector<int>
  //

  template <typename T>
  OptionalSequence<T, -1ull>::OptionalSequence(Name&& name)
    : OptionalSequence{std::move(name), Comment("")}
  {}

  template <typename T>
  OptionalSequence<T, -1ull>::OptionalSequence(Name&& name, Comment&& comment)
    : SequenceBase{std::move(name),
                   std::move(comment),
                   par_style::OPTIONAL,
                   par_type::SEQ_VECTOR,
                   detail::AlwaysUse()}
    , RegisterIfTableMember{this}
    , value_{
        ftype{std::make_shared<tt::fhicl_type<T>>(Name::sequence_element(0ul))}}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <typename T>
  template <fhicl::maybe_use_param F>
  OptionalSequence<T, -1ull>::OptionalSequence(Name&& name,
                                               Comment&& comment,
                                               F maybeUse)
    : SequenceBase{std::move(name),
                   std::move(comment),
                   par_style::OPTIONAL_CONDITIONAL,
                   par_type::SEQ_VECTOR,
                   maybeUse}
    , RegisterIfTableMember{this}
    , value_{
        ftype{std::make_shared<tt::fhicl_type<T>>(Name::sequence_element(0ul))}}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <typename T>
  bool
  OptionalSequence<T, -1ull>::do_preset_value(fhicl::ParameterSet const& ps)
  {
    if constexpr (std::is_same_v<tt::fhicl_type<T>, Atom<T>>) {
      auto const trimmed_key = detail::strip_first_containing_name(key());
      value_ = ps.get<value_type>(trimmed_key);
      has_value_ = true;
      return true;
    }
    return false;
  }

  template <typename T>
  void
  OptionalSequence<T, -1ull>::do_set_value(fhicl::ParameterSet const&)
  {
    // We do not explicitly set the sequence values here as the
    // individual elements are set one at a time.  However, this
    // function is reached in the ValidateThenSet algorithm if the
    // optional parameter is present.  Otherwise, this override is
    // skipped.
    has_value_ = true;
  }
}

#endif /* fhiclcpp_types_OptionalSequence_h */

// Local variables:
// mode: c++
// End:
