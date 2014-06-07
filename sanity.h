// SANITY.H
// ----------
// ... stop the insanity!
//
// The purpose of sanity.h is to provide a set of simple
// functions for manipulating immutable collections, such as
// vectors, maps, sets, and so on, with higher-order functions
// such as map, reduce, and filter. We use STL collections
// and treat them as immutable.
//
// Heavily inspired by Underscore.js and Clojure.
//
// (C) 2014 Arthur Edelstein, arthuredelstein@gmail.com
// MIT License

#include <vector>
#include <functional>

// __first(vec)__.
// Returns the first element in a vector.
template <typename A>
A first(const std::vector<A>& input) {
   if (input.empty()) {
      throw std::exception("Vector is empty.");
   }
   return input[0];
}

// __rest(vec)__.
// Takes all but the first element in a vector.
template <typename A>
std::vector<A> rest(const std::vector<A>& input) {
   return std::vector<A>(input.begin() + 1, input.end());
}

// __last(vec)__.
// Returns last element in a vector.
template <typename A>
A last(const std::vector<A>& input) {
   if (input.empty()) {
      throw std::exception("Vector is empty.");
   }
   return *(input.end() - 1);
}

// __map(vec, func)__.
// Takes a vector of values of type IN, applies a 
// function to each of those values and produces a new vector
// with the results.
template <typename IN, typename F>
auto map(const std::vector<IN>& input, const F& func) -> std::vector<decltype(func(*(input.begin())))> {
   std::vector<decltype(func(*(input.begin())))> result;
   for (IN element : input) {
      result.push_back(func(element));
   }
   return result;
}

// __reduce(init, vec, func)__.
// Takes an initial value of type OUT, an vector with elements of type IN,
// and a func with two arguments: func(OUT, IN).
template <typename OUT, typename IN, typename F>
OUT reduce(const OUT& init, const std::vector<IN>& input, const F& func) {
   OUT memo = init;
   for (const IN& element : input) {
      memo = func(memo, element);
   }
   return memo;
}

// __reduce(vec, func)__.
// Takes a vector with elements of type ELEM, and a func that takes
// two arguments, both of type ELEM and returns a result of type ELEM.
template <typename ELEM, typename F>
ELEM reduce(const std::vector<ELEM>& input, const F& func) {
   return reduce(first(input), rest(input), func);
}

// __minimum(vec)__.
template <typename ELEM>
ELEM minimum(const std::vector<ELEM>& input) {
   return reduce(input, [](ELEM a, ELEM b) { return a < b ? a : b; } );
}

// __maximum(vec)__.
template <typename ELEM>
ELEM maximum(const std::vector<ELEM>& input) {
   return reduce(input, [](ELEM a, ELEM b) { return a > b ? a : b; } );
}

// __filter(vec, predicate)__.
// Takes a vector of values and returns a vector
// with only those values where predicate(value) == TRUE.
template <class ELEM, typename F>
std::vector<ELEM> filter(const std::vector<ELEM>& input, const F& predicate) {
   static_assert(std::is_same<decltype(predicate(*(input.begin()))), bool>::value, "predicate must return a bool");
   std::vector<ELEM> result;
   for (const ELEM& element : input) {
      if (predicate(element)) {
         result.push_back(element);
      }
   }
   return result;
}

// __remove(vec, predicate)__.
// Takes a vector of values and returns a vector
// with only those values where predicate(value) == FALSE.
template <class ELEM, typename F>
std::vector<ELEM> remove(const std::vector<ELEM>& input, const F& predicate) {
   return filter(input, [&](ELEM elem) { return !predicate(elem); } );
}

// __every(vec, predicate)__.
// Returns true if for every value, predicate(value) == TRUE.
template <class ELEM, typename F>
bool every(const std::vector<ELEM>& input, const F& predicate) {
   static_assert(std::is_same<decltype(predicate(*(input.begin()))), bool>::value, "predicate must return a bool");
   for (const ELEM& element : input) {
      if (!predicate(element)) {
         return false;
      }
   }
   return true;
}

// __any(vec, predicate)__.
// Returns true if for any value, predicate(value) == TRUE.
template <class ELEM, typename F>
bool any(const std::vector<ELEM>& input, const F& predicate) {
   return !every(input, [&](ELEM elem) {return !predicate(elem); } );
}

// __contains(vec, value)__.
// Returns true if vec contains value.
template <class ELEM, class VAL>
bool contains(const std::vector<ELEM>& input, const VAL& value) {
   for (const ELEM& element : input) {
      if (element == value) {
         return true;
      }
   }
   return false;
}

// __range(start, end, step)__.
// Returns an arithmetic progression of numbers.
template <typename A, typename B, typename C>
auto range(A start, B end, C step) -> std::vector<decltype(start + step)> {
   std::vector<decltype(start + step)> result;
   for (decltype(start + step) i = start; i < end; i+=step) {
      result.push_back(i);
   }
   return result;
}

// __range(start, end)__.
// Returns an arithmetic progress of numbers, with steps of 1.
template <typename A, typename B>
std::vector<A> range(A start, B end) {
   return range(start, end, 1);
}

// __range(end)__.
// Returns an arithmetic progression of long integers starting with zero.
template <typename A>
std::vector<long> range(A end) {
   return range((long) 0, end, 1);
}

// __inc(x)__.
// Increases x by 1.
template <typename A>
A inc(A x) {
   return x + 1;
}

// __dec(x)__.
// Decreases x by 1.
template <typename A>
A dec(A x) {
   return x - 1;
}

