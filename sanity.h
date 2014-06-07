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
// Heavily inspired by Underscore.js, underscore.cpp and Clojure.
//
// (C) 2014 Arthur Edelstein, arthuredelstein@gmail.com
// MIT License

#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <random>

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
// Returns the minimum value in vec.
template <typename ELEM>
ELEM minimum(const std::vector<ELEM>& input) {
   return reduce(input, [](ELEM a, ELEM b) { return a < b ? a : b; } );
}

// __maximum(vec)__.
// Returns the maximum value in vec.
template <typename ELEM>
ELEM maximum(const std::vector<ELEM>& input) {
   return reduce(input, [](ELEM a, ELEM b) { return a > b ? a : b; } );
}

// __sort(vec, comparisonFunction)__.
// Sorts the vec using comparisonFunction, least to greatest.
template <typename ELEM, typename F>
std::vector<ELEM> sort(const std::vector<ELEM>& input, const F& comparisonFunction) {
   std::vector<ELEM> result(input);
   std::sort(result.begin(), result.end(), comparisonFunction);
   return result;
}

// __sort(vec)__.
// Sorts the vec, least to greatest.
template <typename ELEM>
std::vector<ELEM> sort(const std::vector<ELEM>& input) {
   std::vector<ELEM> result(input);
   std::sort(result.begin(), result.end());
   return result;
}

// __shuffle(vec)__.
// Shuffles the vec elements in random order.
template <typename ELEM>
std::vector<ELEM> shuffle(const std::vector<ELEM>& input) {
   std::vector<ELEM> result(input);
   std::random_shuffle(result.begin(), result.end());
   return result;
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

// __indexOf(vec, value)__.
// Returns the integer index of the first occurence of value in vec.
template <typename A, typename VAL>
long indexOf(const std::vector<A>& vec, const VAL& value) {
   long i = 0;
   for (A elem : vec) {
      if (elem == value) {
         return i;
      }
      ++i;
   }
   return -1;
}

// ## Functions for manipulating Maps.

// __assoc(map, key, val)__.
// Adds a key, val pair to a map.
template <typename K, typename V>
std::map<K, V> assoc(const std::map<K, V>& map, const K& key, const V& val) {
   std::map<K, V> result(map);
   result[key] = val;
   return result;
}

// __dissoc(map, key)__.
// Adds a key, val pair to a map.
template <typename K, typename V>
std::map<K, V> dissoc(const std::map<K, V>& map, const K& key) {
   std::map<K, V> result(map);
   delete result[key];
   return result;
}

// __hasKey(map, key)__.
// Returns true if map contains a key.
template <typename K, typename V>
bool hasKey(const std::map<K, V>& map, const K& key) {
   return kmap.find(key) != kmap.end();
}

// __get(map, key)__.
// Adds a key, val pair to a map.
template <typename K, typename V>
V get(const std::map<K, V>& map, const K& key, V value, V notFound) {
   return hasKey(map, key) ? map[key] : notFound;
}

// __keys(map)__.
// Returns the keys from a map.
template <typename K, typename V>
std::vector<K> keys(const std::map<K, V>& m) {
   std::vector<K> result;
   for (auto& kv : m) {
      result.push_back(kv.first);
   }
   return result;
}

// __vals(map)__.
// Returns the vals from a map.
template <typename K, typename V>
std::vector<V> vals(const std::map<K, V>& m) {
   std::vector<V> result;
   for (auto& kv : m) {
      result.push_back(kv.second);
   }
   return result;
}

// __pairs(map)__.
// Returns the [key, value] pairs from a map.
template <typename K, typename V>
std::vector<std::pair<K, V>> pairs(const std::map<K, V>& m) {
   std::vector<std::pair<K, V>> result;
   for (auto& kv : m) {
      result.push_back(std::pair<K,V>(kv.first, kv.second));
   }
   return result;
}

// __zipmap(keys, vals)__.
// Returns a map using keys, vals.
template <typename K, typename V>
std::map<K, V> zipmap(const std::vector<K>& keys, const std::vector<V>& vals) {
   int n = keys.size();
   if (n != vals.size()) {
      throw std::exception("keys and vals vectors are different lengths");
   }
   std::map<K, V> result;
   for (int i = 0; i < keys.size(); ++i) {
      result[keys[i]] = vals[i];
   }
   return result;
}

// __merge(map1, map2)__.
// Merge two maps. Keys from map2 overwrite any matching keys in map1.
template <typename K, typename V>
std::map<K, V> merge(const std::map<K, V>& map1, const std::map<K, V>& map2) {
   std::map<K, V> result(map1);
   for (auto& kv : map2) {
      result[kv.first] = kv.second;
   }
   return result;
}

// __mergeWith(function, map1, map2)__.
// Merge two maps. If keys occur in both maps, then call function(v1, v2)
template <typename K, typename V, typename F>
std::map<K, V> mergeWith(const F& func, const std::map<K, V>& map1, const std::map<K, V>& map2) {
   std::map<K, V> result(map1);
   for (auto& kv2 : map2) {
      result[kv2.first] = hasKey(map1, kv2.first) ? func(get(map1, kv2.first), kv2.second) : kv2.second;
   }
   return result;
}

// __renameKeys(map, kmap)__.
// Returns the map with the keys in kmap renamed to the vals in kmap.
template <typename K, typename V>
std::map<K, V> renameKeys(const std::map<K, V>& map, const std::map<K, K>& kmap) {
   std::map<K, V> result;
   for (auto& kv : map) {
      if (hasKey(kmap, kv.first)) {
         result[kmap[kv.first]] = kv.second;
      } else {
         result[kv.first] = kv.second;
      }
   }
}


// ## Numerical functions.

// __isEven(x)__.
// Returns true if x is even.
template <typename A>
bool isEven(A x) {
   return x % 2 == 0;
}

// __isOdd(x)__.
// Returns true if x is odd.
template <typename A>
bool isOdd(A x) {
   return !isEven(x);
}

// __isZero(x)__.
// Returns true if x is zero.
template <typename A>
bool isZero(A x) {
   return x == 0;
}

// __isPositive(x)__.
// Returns true if x is more than zero.
template <typename A>
bool isPositive(A x) {
   return x > 0;
}

// __isNegative(x)__.
// Returns true if x is less than zero.
template <typename A>
bool isNegative(A x) {
   return x < 0;
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

// __add(a, b)__.
// Returns a + b.
template <typename A, typename B>
auto add(A a, B b) -> decltype(a + b) {
   return a + b;
}

// __subtract(a, b)__.
// Returns a - b.
template <typename A, typename B>
auto subtract(A a, B b) -> decltype(a - b) {
   return a - b;
}

// __multiply(a, b)__.
// Returns a + b.
template <typename A, typename B>
auto multiply(A a, B b) -> decltype(a * b) {
   return a * b;
}

// __divide(a, b)__.
// Returns a - b.
template <typename A, typename B>
auto divide(A a, B b) -> decltype(a / b) {
   return a / b;
}

// __modulo(a, b)__.
// Returns a - b.
template <typename A, typename B>
auto modulo(A a, B b) -> decltype(a % b) {
   return a % b;
}
