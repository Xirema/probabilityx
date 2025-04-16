#include <iterator>
#include <print>
#include <ranges>

class RegularDie {
  int _min, _max;

 public:
  constexpr RegularDie(int _min, int _max) : _min(_min), _max(_max) {
    if (this->_min > this->_max) {
      std::swap(this->_min, this->_max);
    }
  }
  constexpr RegularDie(int _max) : RegularDie(1, _max) {}

  struct Iterator {
    using difference_type = int;
    using value_type = int;
    int curr;
    constexpr int operator*() const { return curr; }
    constexpr Iterator& operator++() {
      curr++;
      return *this;
    }
    constexpr Iterator& operator--() {
      curr--;
      return *this;
    }
    constexpr Iterator operator++(int) {
      Iterator copy{*this};
      ++*this;
      return copy;
    }
    constexpr Iterator operator--(int) {
      Iterator copy{*this};
      --*this;
      return copy;
    }
    constexpr Iterator& operator+=(size_t i) {
      curr += static_cast<int>(i);
      return *this;
    }
    constexpr Iterator& operator-=(size_t i) {
      curr -= static_cast<int>(i);
      return *this;
    }
    constexpr Iterator operator+(size_t i) const {
      Iterator copy{*this};
      copy += static_cast<int>(i);
      return copy;
    }
    constexpr Iterator operator-(size_t i) const {
      Iterator copy{*this};
      copy -= static_cast<int>(i);
      return copy;
    }
    constexpr friend Iterator operator+(size_t i, Iterator o) { return o + i; }
    constexpr int operator[](int i) const { return curr + i; };
    constexpr int operator-(Iterator const& o) const { return curr - o.curr; }
    constexpr auto operator<=>(Iterator const& o) const = default;
  };

  constexpr Iterator begin() const { return Iterator{.curr = _min}; }

  constexpr Iterator end() const { return Iterator{.curr = _max + 1}; }

  constexpr int min() const { return _min; }
  constexpr int max() const { return _max; }
};

int main() {
  RegularDie testDie{6};
  static_assert(std::input_iterator<RegularDie::Iterator>);
  static_assert(std::ranges::viewable_range<RegularDie>);
  static_assert(std::random_access_iterator<RegularDie::Iterator>);
  static_assert(std::bidirectional_iterator<RegularDie::Iterator>);
  for (int i : testDie) {
    std::print("{}\n", i);
  }
  for (int i : std::views::all(testDie) |
                   std::views::transform([](int i) { return i * 2 - 1; })) {
    std::print("{}\n", i);
  }
}
