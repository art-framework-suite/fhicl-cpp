#ifndef fhiclcpp_stdmap_shims_h
#define fhiclcpp_stdmap_shims_h

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "fhiclcpp/parse_shims_opts.h"

namespace shims {

  template <class Key,
            class T,
            class Compare = std::less<Key>,
            class Allocator = std::allocator<std::pair<const Key, T>>>
  class map {
  public:
    using mapmap_t = std::map<const Key, T, Compare, Allocator>;
    using listmap_t = std::list<std::pair<const Key, T>, Allocator>;

    static_assert(std::is_same_v<typename mapmap_t::key_type,
                                 typename listmap_t::value_type::first_type>,
                  "type mismatch for key_type");
    static_assert(std::is_same_v<typename mapmap_t::mapped_type,
                                 typename listmap_t::value_type::second_type>,
                  "type mismatch for mapped_type");
    static_assert(std::is_same_v<typename mapmap_t::value_type,
                                 typename listmap_t::value_type>,
                  "type mismatch for value_type");
    static_assert(std::is_same_v<typename mapmap_t::size_type,
                                 typename listmap_t::size_type>,
                  "type mismatch for size_type");

    using size_type = typename mapmap_t::size_type;

    using iterator_tag = std::input_iterator_tag;
    struct iterator_tuple {
      typename mapmap_t::iterator mapmap_iter;
      typename listmap_t::iterator listmap_iter;
    };

    template <class Category,
              class TT,
              class Distance = std::ptrdiff_t,
              class Pointer = TT*,
              class Reference = TT&>
    struct iter : std::iterator<Category, TT, Distance, Pointer, Reference> {
      using type = TT;

      iter(typename mapmap_t::iterator it) noexcept { _iters.mapmap_iter = it; }
      iter(typename listmap_t::iterator it) noexcept
      {
        _iters.listmap_iter = it;
      }

      TT& operator*() noexcept
      {
        return isSnippetMode() ? *_iters.listmap_iter : *_iters.mapmap_iter;
      }

      TT* operator->() noexcept
      {
        return isSnippetMode() ? &*_iters.listmap_iter : &*_iters.mapmap_iter;
      }

      TT const* operator->() const noexcept
      {
        return isSnippetMode() ? &*_iters.listmap_iter : &*_iters.mapmap_iter;
      }

      TT&
      operator++()
      {
        return isSnippetMode() ? *(_iters.listmap_iter++) :
                                 *(_iters.mapmap_iter++);
      }

      bool
      operator==(iter other) const noexcept
      {
        return isSnippetMode() ?
                 _iters.listmap_iter == other._iters.listmap_iter :
                 _iters.mapmap_iter == other._iters.mapmap_iter;
      }

      bool
      operator!=(iter other) const noexcept
      {
        return !operator==(other);
      }

      template <typename II>
      std::enable_if_t<std::is_same_v<typename mapmap_t::iterator, II>, II> get(
        II)
      {
        return _iters.mapmap_iter;
      }

      template <typename II>
      std::enable_if_t<std::is_same_v<typename listmap_t::iterator, II>, II>
        get(II)
      {
        return _iters.listmap_iter;
      }

      template <typename IIL, typename IIR>
      friend std::enable_if_t<
        !std::is_same_v<IIL, IIR> &&
          std::is_same_v<std::remove_const_t<typename IIL::type>,
                         std::remove_const_t<typename IIR::type>>,
        bool>
      operator==(IIL, IIR) noexcept;

      template <typename IIL, typename IIR>
      friend std::enable_if_t<
        !std::is_same_v<IIL, IIR> &&
          std::is_same_v<std::remove_const_t<typename IIL::type>,
                         std::remove_const_t<typename IIR::type>>,
        bool>
      operator!=(IIL, IIR) noexcept;

    private:
      iterator_tuple _iters;
    };

    using iterator = iter<iterator_tag, std::pair<Key const, T>>;
    using const_iterator = iter<iterator_tag, std::pair<Key const, T> const>;

    struct maps_tuple {
      mapmap_t mapmap;
      listmap_t listmap;
    };

    T& operator[](Key const& key)
    {
      if (isSnippetMode()) {
        for (auto& [stored_key, value] : _maps.listmap) {
          if (stored_key == key)
            return value;
        }
        _maps.listmap.emplace_back(key, T{});
        return _maps.listmap.back().second;
      }
      return _maps.mapmap[key];
    }

    iterator
    begin() noexcept
    {
      return isSnippetMode() ? iterator{std::begin(_maps.listmap)} :
                               iterator{std::begin(_maps.mapmap)};
    }

    const_iterator
    begin() const noexcept
    {
      auto& maps = const_cast<maps_tuple&>(_maps);
      return isSnippetMode() ? const_iterator{std::begin(maps.listmap)} :
                               const_iterator{std::begin(maps.mapmap)};
    }

    const_iterator
    cbegin() const noexcept
    {
      return begin();
    }

    iterator
    end() noexcept
    {
      return isSnippetMode() ? iterator{std::end(_maps.listmap)} :
                               iterator{std::end(_maps.mapmap)};
    }

    const_iterator
    end() const noexcept
    {
      auto& maps = const_cast<maps_tuple&>(_maps);
      return isSnippetMode() ? const_iterator{std::end(maps.listmap)} :
                               const_iterator{std::end(maps.mapmap)};
    }

    const_iterator
    cend() const noexcept
    {
      return end();
    }

    T&
    at(Key const& key)
    {
      if (isSnippetMode()) {
        auto it =
          std::find_if(_maps.listmap.begin(),
                       _maps.listmap.end(),
                       [&key](auto& element) { return element.first == key; });
        if (it == _maps.listmap.end())
          throw std::out_of_range("Key <" + key + "> not found.");
        return it->second;
      } else {
        return _maps.mapmap.at(key);
      }
    }

    T const&
    at(Key const& key) const
    {
      if (isSnippetMode()) {
        auto it = std::find_if(
          _maps.listmap.cbegin(),
          _maps.listmap.cend(),
          [&key](auto const& element) { return element.first == key; });
        if (it == _maps.listmap.cend())
          throw std::out_of_range("Key <" + key + "> not found.");
        return it->second;
      } else {
        return _maps.mapmap.at(key);
      }
    }

    iterator
    find(Key const& key)
    {
      if (isSnippetMode()) {
        return std::find_if(
          _maps.listmap.begin(), _maps.listmap.end(), [&key](auto& element) {
            return element.first == key;
          });
      } else {
        return _maps.mapmap.find(key);
      }
    }

    const_iterator
    find(Key const& key) const
    {
      maps_tuple& maps = *const_cast<maps_tuple*>(&_maps);

      if (isSnippetMode()) {
        return std::find_if(
          maps.listmap.begin(),
          maps.listmap.end(),
          [&key](auto const& element) { return element.first == key; });
      } else {
        return maps.mapmap.find(key);
      }
    }

    size_t
    erase(Key const& key)
    {
      if (isSnippetMode()) {
        auto erase_count = size_t{0};
        auto i = _maps.listmap.begin();
        auto e = _maps.listmap.end();

        while (i != e) {
          if (key == i->first) {
            i = _maps.listmap.erase(i);
            ++erase_count;
          } else {
            i++;
          }
        }

        return erase_count;
      } else {
        return _maps.mapmap.erase(key);
      }
    }

    bool
    empty() const noexcept
    {
      return isSnippetMode() ? _maps.listmap.empty() : _maps.mapmap.empty();
    }

    size_type
    size() const noexcept
    {
      return isSnippetMode() ? _maps.listmap.size() : _maps.mapmap.size();
    }

    iterator
    erase(iterator it)
    {
      if (isSnippetMode()) {
        return _maps.listmap.erase(it.get(typename listmap_t::iterator{}));
      }
      return _maps.mapmap.erase(it.get(typename mapmap_t::iterator{}));
    }

    iterator
    erase(const_iterator& it)
    {
      if (isSnippetMode()) {
        return _maps.listmap.erase(it.get(typename listmap_t::iterator{}));
      } else {
        return _maps.mapmap.erase(it.get(typename mapmap_t::iterator{}));
      }
    }

    template <class... Args>
    std::pair<iterator, bool>
    emplace(Args&&... args)
    {
      if (isSnippetMode()) {
        _maps.listmap.emplace_back(std::forward<Args>(args)...);
        return std::make_pair(iterator{std::prev(_maps.listmap.end())}, true);
      } else {
        auto result = _maps.mapmap.emplace(std::forward<Args>(args)...);
        return std::make_pair(iterator{result.first}, result.second);
      }
    }

    maps_tuple _maps;
  };
  template <typename IIL, typename IIR>
  std::enable_if_t<!std::is_same_v<IIL, IIR> &&
                     std::is_same_v<std::remove_const_t<typename IIL::type>,
                                    std::remove_const_t<typename IIR::type>>,
                   bool>
  operator==(IIL left, IIR right) noexcept
  {
    return isSnippetMode() ?
             left._iters.listmap_iter == right._iters.listmap_iter :
             left._iters.mapmap_iter == right._iters.mapmap_iter;
  }

  template <typename IIL, typename IIR>
  std::enable_if_t<!std::is_same_v<IIL, IIR> &&
                     std::is_same_v<std::remove_const_t<typename IIL::type>,
                                    std::remove_const_t<typename IIR::type>>,
                   bool>
  operator!=(IIL left, IIR right) noexcept
  {
    return !operator==(left, right);
  }
}

#endif /* fhiclcpp_stdmap_shims_h */

// Local Variables:
// mode: c++
// End:
