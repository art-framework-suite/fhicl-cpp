#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/detail/Prettifier.h"
#include "fhiclcpp/detail/Indentation.h"
#include "fhiclcpp/detail/printing_helpers.h"

using namespace fhicl;
using namespace fhicl::detail;

//==========================================================================

Prettifier::Prettifier(unsigned const initial_indent_level)
  : buffer_()
  , indent_{initial_indent_level}
  , sequence_sizes_{{-1u}}
  , seq_size_{ sequence_sizes_.top() }
  , table_size_{0u}
{}

//==========================================================================
void
Prettifier::before_action(key_t const& key, any_t const& a, ParameterSet const* ps)
{
  if ( !is_table(a) ) return;
  table_size_ = ps->get<fhicl::ParameterSet>(key).get_all_keys().size();
}


//==========================================================================

void
Prettifier::enter_table(std::string const& key, boost::any const &)
{
  buffer_ << indent_()
          << table::printed_prefix(key)
          << maybe_nl_(table_size_);
  indent_.push();
}

void
Prettifier::exit_table(std::string const& key, boost::any const &)
{
  indent_.pop();
  buffer_ << maybe_indent_(table_size_)
          << table::closing_brace()
          << printed_suffix(key, seq_size_)
          << nl();
}

//==========================================================================

void
Prettifier::enter_sequence(std::string const& key,
                           boost::any const& a)
{
  push_size_(a);
  buffer_ << indent_()
          << sequence::printed_prefix(key)
          << maybe_nl_(seq_size_);
  indent_.push();
}

void
Prettifier::exit_sequence(std::string const& key,
                          boost::any const&)
{
  indent_.pop();
  buffer_ << maybe_indent_(seq_size_)
          << sequence::closing_brace()
          << printed_suffix(key, seq_size_)
          << nl();
  pop_size_();
}

//==========================================================================

void
Prettifier::atom(std::string const& key, boost::any const & a)
{
  buffer_ << indent_()
          << atom::printed_prefix(key)
          << atom::value(a)
          << printed_suffix(key, seq_size_)
          << nl();
}

//=========================================================================

void
Prettifier::push_size_(boost::any const& a)
{
  sequence_sizes_.emplace( boost::any_cast<ps_sequence_t>(a).size() );
  seq_size_ = sequence_sizes_.top();
}

void
Prettifier::pop_size_()
{
  sequence_sizes_.pop();
  seq_size_ = sequence_sizes_.top();
}

std::string
Prettifier::maybe_indent_(std::size_t const sz)
{
  return sz ? indent_() : "";
}

std::string
Prettifier::maybe_nl_(std::size_t const sz)
{
  return sz ? nl() : "";
}