#pragma once
#include "../Roll.hpp"

namespace probx::dice {
  class RegularDie {
    Integer minimum;
    Integer maximum;
  public:
    constexpr RegularDie(): RegularDie(0, 0) {}
    constexpr RegularDie(Integer maximum): RegularDie(1, maximum) {}
    constexpr RegularDie(Integer minimum, Integer maximum): minimum(minimum), maximum(maximum) {
      if(this->minimum > this->maximum) {
	std::swap(this->minimum, this->maximum);
      }
    }

    struct Iterator{
      using difference_type = Integer;
      using value_type = Outcome;
      Integer curr;
      constexpr Outcome operator*() const{return Outcome{curr};}
      constexpr Iterator& operator++() {curr++; return *this;}
      constexpr Iterator& operator--() {curr--; return *this;}
      constexpr Iterator operator++(int) {Iterator copy{*this}; ++*this; return copy;}
      constexpr Iterator operator--(int) {Iterator copy{*this}; --*this; return copy;}
      constexpr Iterator& operator+=(size_t i) {curr += static_cast<Integer>(i); return *this;}
      constexpr Iterator& operator-=(size_t i) {curr -= static_cast<Integer>(i); return *this;}
      constexpr Iterator operator+(size_t i) const {Iterator copy{*this}; copy += static_cast<Integer>(i); return copy;}
      constexpr Iterator operator-(size_t i) const {Iterator copy{*this}; copy -= static_cast<Integer>(i); return copy;}
      constexpr friend Iterator operator+(size_t i, Iterator o) {return o + i;}
      constexpr Outcome operator[](Integer i) const {return Outcome{curr + i};};
      constexpr Integer operator-(Iterator const& o) const {return curr - o.curr;}
      constexpr auto operator<=>(Iterator const& o) const = default;
    };

    constexpr Iterator begin() const {return Iterator{.curr=minimum};}
    constexpr Iterator end() const{return Iterator{.curr=maximum + 1};}

    template<typename PRNG>
    requires std::uniform_random_bit_generator<std::remove_reference_t<PRNG>>
    constexpr Outcome operator()(PRNG && engine) const{
      std::uniform_int_distribution<Integer> dist{minimum, maximum};
      return Outcome{.result=dist(engine)};
    }

    constexpr BigInt operator()(Outcome o) const{
      if(o.extra != 0) {
	return 0;
      }
      if(o.result < minimum || o.result > maximum) {
	return 0;
      }
      return 1;
    }

    constexpr size_t size() const {
      return maximum - minimum + 1;
    }

    constexpr BigInt trials() const {
      return size();
    }
  };

  static_assert(Rollable<RegularDie>);
}
