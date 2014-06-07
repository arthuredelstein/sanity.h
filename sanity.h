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

// __first(vec)__. Takes the first element in a vector.
template <typename A>
A first(const std::vector<A>& input) {
   return input[0];
}

// __rest(vec)__.
// Takes all but the first element in a vector.
template <typename A>
std::vector<A> rest(const std::vector<A>& input) {
   return std::vector<A>(input.begin() + 1, input.end());
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
   for (IN element : input) {
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

// __filter(vec, predicate)__.
// Takes a vector of values and returns a vector
// with only those values where predicate(value) == TRUE.
template <class ELEM, typename F>
std::vector<ELEM> filter(const std::vector<ELEM>& input, const F& predicate) {
   static_assert(std::is_same<decltype(predicate(*(input.begin()))), bool>::value, "predicate must return a bool");
   std::vector<ELEM> result;
   for (ELEM element : input) {
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

