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
// Heavily inspired by Clojure, Underscore.js, and underscore.cpp.
//
// (C) 2014 Arthur Edelstein, arthuredelstein@gmail.com
// MIT License

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <random>
#include <regex>
#include <vector>


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

// __first(coll)__.
// Returns the first element in a collection.
template <typename C>
auto first(const C& input) -> typename C::value_type {
   if (input.empty()) {
      throw std::exception("Collection is empty.");
   }
   return input[0];
}

// __rest(coll)__.
// Takes all but the first element in a collection.
template <typename C>
C rest(const C& coll) {
   return C(coll.begin() + 1, coll.end());
}

// __last(coll)__.
// Returns last element in a collection.
template <typename C>
auto last(const C& coll) -> typename C::value_type {
   if (coll.empty()) {
      throw std::exception("Vector is empty.");
   }
   return *(coll.end() - 1);
}

// __map(coll, func)__.
// Takes a collection of values of type IN, applies a 
// function to each of those values and produces a new collection
// with the results.
template <template <typename, typename> class C, typename A, typename B, typename F>
auto map(const C<A, B>& coll, const F& func) -> C<decltype(func(first(coll))), std::allocator<decltype(func(first(coll)))> > {
   typedef decltype(func(first(coll))) elem;
   C<elem, std::allocator<elem>> result;
   for (const auto& element : coll) {
      result.push_back(func(element));
   }
   return result;
}

// __reduce(init, coll, func)__.
// Takes an initial value of type OUT, an collection
// and a func with two arguments: func(OUT, IN).
template <typename OUT, typename C, typename F>
OUT reduce(const OUT& init, const C& coll, const F& func) {
   OUT memo = init;
   for (auto element : coll) {
      memo = func(memo, element);
   }
   return memo;
}

// __reduce(coll, func)__.
// Takes a collection a func that takes two arguments, both the same type.
template <typename C, typename F>
typename C::value_type reduce(const C& coll, const F& func) {
   return reduce(first(coll), rest(coll), func);
}

// __minimum(coll)__.
// Returns the minimum value in coll.
template <typename C>
typename C::value_type minimum(const C& coll) {
   typedef decltype(first(coll)) ELEM;
   return reduce(coll, [](ELEM a, ELEM b) { return a < b ? a : b; } );
}

// __maximum(coll)__.
// Returns the maximum value in coll.
template <typename C>
typename C::value_type maximum(const C& coll) {
   return reduce(coll, [](typename C::value_type a, typename C::value_type b) { return a > b ? a : b; } );
}

// __sort(coll, comparisonFunction)__.
// Sorts the coll using comparisonFunction, least to greatest.
template <typename C, typename F>
C sort(const C& coll, const F& comparisonFunction) {
   C result(coll);
   std::sort(result.begin(), result.end(), comparisonFunction);
   return result;
}

// __sort(coll)__.
// Sorts the coll, least to greatest.
template <typename C>
C sort(const C& coll) {
   C result(coll);
   std::sort(result.begin(), result.end());
   return result;
}

// __shuffle(coll)__.
// Shuffles the coll elements in random order.
template <typename C>
C shuffle(const C& coll) {
   C result(coll);
   std::random_shuffle(result.begin(), result.end());
   return result;
}

// __reverse(coll)__.
// Reverses the order of elements in the collection.
template <typename C>
C reverse(const C& coll) {
   C result(coll);
   std::reverse(result.begin(), result.end());
   return result;
}

// __filter(coll, predicate)__.
// Takes a coll of values and returns a coll
// with only those values where predicate(value) == TRUE.
template <typename C, typename F>
C filter(const C& coll, const F& predicate) {
   static_assert(std::is_same<decltype(predicate(first(coll))), bool>::value, "predicate must return a bool");
   C result;
   for (const auto& element : coll) {
      if (predicate(element)) {
         result.push_back(element);
      }
   }
   return result;
}

// __remove(coll, predicate)__.
// Takes a collection of values and returns a collection
// with only those values where predicate(value) == FALSE.
template <typename C, typename F>
C remove(const C& coll, const F& predicate) {
   return filter(coll, [&](typename C::value_type elem) { return !predicate(elem); } );
}

// __every(coll, predicate)__.
// Returns true if for every value, predicate(value) == TRUE.
template <typename C, typename F>
bool every(const C& coll, const F& predicate) {
   static_assert(std::is_same<decltype(predicate(first(coll))), bool>::value, "predicate must return a bool");
   for (const auto& element : coll) {
      if (!predicate(element)) {
         return false;
      }
   }
   return true;
}

// __any(coll, predicate)__.
// Returns true if for any value, predicate(value) == TRUE.
template <typename C, typename F>
bool any(const C& input, const F& predicate) {
   return !every(input, [&](decltype(first(coll)) elem) {return !predicate(elem); } );
}

// __contains(coll, value)__.
// Returns true if coll contains value.
template <class C, class VAL>
bool contains(const C& coll, const VAL& value) {
   for (const auto& element : coll) {
      if (element == value) {
         return true;
      }
   }
   return false;
}

// __indexOf(coll, value)__.
// Returns the integer index of the first occurence of value in coll.
template <typename C, typename VAL>
long indexOf(const C& coll, const VAL& value) {
   long i = 0;
   for (auto elem : coll) {
      if (elem == value) {
         return i;
      }
      ++i;
   }
   return -1;
}

// __nth(coll, index)__.
// Returns the nth element in coll at index (zero-based).
template <typename C>
typename C::value_type nth(const C& coll, long index) {
   return coll[index];
}

// __nth(coll, index, notFound)__.
// Returns the element in coll at index, and notFound if it isn't present.
template <typename C>
typename C::value_type nth(const C& coll, long index, typename C::value_type notFound) {
   return coll.size() >= index ? notFound : coll[index];
}

// __cons(coll, item)__.
// Returns a new coll with item as the first element and rest as input coll.
template <typename C>
C cons(const C& coll, typename C::value_type item) {
   C result;
   result.assign(coll.begin(), coll.end());
   result.insert(result.begin(), item);
   return result;
}

// __conj(coll, item)__.
// Returns a new coll with item appended to input coll.
template <typename C>
C conj(const C& coll, typename C::value_type item) {
   C result;
   result.assign(coll.begin(), coll.end());
   result.push_back(item);
   return result;
}

// __take(coll, n)__.
// Returns the first n items of coll.
template <typename C>
C take(const C& coll, long n) {
   if (coll.size() > n) {
      return coll;
   } else {
      C result;
      result.assign(coll.begin(), coll.begin() + n);
      return result;
   }
}

// __takeWhile(coll, predicate)__.
// Return elements of coll while predicate(elem) remains true.
template <typename C, typename F>
C takeWhile(const C& coll, const F& predicate) {
   C result;
   for (const auto& elem : coll) {
      if (predicate(elem)) {
         result.push_back(elem);
      } else {
         break;
      }
   }
   return result;
}

// __drop(coll, n)__.
// Returns coll with the first n items removed.
template <typename C>
C drop(const C& coll, long n) {
   if (coll.size() > n) {
      // Drop all elements, so return an empty coll.
      return C();
   } else {
      C result;
      result.assign(coll.begin() + n, coll.end());
      return result;
   }
}

// __dropWhile(coll, predicate)__.
// Drop elements of coll while predicate(elem) remains true.
template <typename C, typename F>
C dropWhile(const C& coll, const F& predicate) {
   C result;
   long leadingTrues = 0;
   for (auto elem : coll) {
      if (predicate(elem)) {
         ++leadingTrues;
      } else {
         break;
      }
   }
   result.assign(coll.begin() + leadingTrues, coll.end());
   return result;
}

// __repeat(coll, item, n)__.
// Repeat item n times in a collection.
template <typename C>
C repeat(const C& item, long n) {
   C result;
   result.assign(n, item);
   return result;
}

// __repeatedly(n, func)__.
// Returns the results in a vector, of calling func() n times. Usually
// func has side effects so that the values vary.
template <typename F>
auto repeatedly(long n, const F& func) -> std::vector<decltype(func())> {
   std::vector<decltype(func())> result;
   for (long i = 0; i < n; ++i) {
      result.push_back(func());
   }
   return result;
}

// __iterate(n, func, val)__.
// Returns a collection containing val, func(val), func(func(val)) ... with n elements.
template <typename C, typename F, typename A>
C iterate(long n, const F& func, const A& val) {
   static_assert(std::is_same(decltype(func(val)), A)::value, "func needs to return a value of same type as val");
   A memo = val;
   C result;
   result.push_back(memo);
   for (long i = 1; i < n; ++i) {
      memo = func(memo);
      result.push_back(memo);
   }
   return result;
}

// __concat(coll1, coll2)__.
// Concatenate the two collections.
template <typename C1, typename C2>
C1 concat(const C1& coll1, const C2& coll2) {
   C1 result;
   result.assign(vec1.begin(), vec1.end());
   for (C2 elem : vec2) {
      result.push_back(elem);
   }
   return result;
}

// __interleave(coll1, coll2)__.
// Returns a collection of first item in each collection, then the second in each, etc.
template <typename C1, typename C2>
C1 interleave(const C1& coll1, const C2& coll2) {
   C1 result;
   int n1 = coll1.size();
   int n2 = coll2.size();
   int n = min(n1, n2);
   for (int i = 0; i < n; ++i) {
      result.push_back(coll1[i]);
      result.push_back(coll22[i]);
   }
   return result;
}

// __interpose(coll, separator)__.
// Returns a collection with separator inserted between each pair of elements in collection.
template <typename C, typename A>
C interpose(const C& coll, const A& sep) {
   C result;
   result.push_back(first(coll));
   for (int i = 1 ; i < coll.size(); ++i) {
      result.push_back(sep);
      result.push_back(nth(coll, i));
   }
   return result;

}

// ## Functions for manipulating Maps.

// __hasKey(map, key)__.
// Returns true if map contains a key.
template <typename K, typename V>
bool hasKey(const std::map<K, V>& map, const K& key) {
   return kmap.find(key) != kmap.end();
}

// __get(map, key, notFound)__.
// Gets the val in map corresponding to key.
template <typename K, typename V>
V get(const std::map<K, V>& map, const K& key, V value, V notFound) {
   return hasKey(map, key) ? map[key] : notFound;
}

// __assoc(map, key, val)__.
// Adds a key, val pair to a map.
template <typename K, typename V>
std::map<K, V> assoc(const std::map<K, V>& map, const K& key, const V& val) {
   std::map<K, V> result(map);
   result[key] = val;
   return result;
}

// __dissoc(map, key)__.
// Removes a key, val pair from a map.
template <typename K, typename V>
std::map<K, V> dissoc(const std::map<K, V>& map, const K& key) {
   if (hasKey(map, key)) {
      std::map<K, V> result(map);
      delete result[key];
      return result;
   } else {
      return map;
   }
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
template <typename CK, typename CV, typename K, typename V>
std::map<K, V> zipmap(const CK& keys, const CV& vals) {
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
// Returns a * b.
template <typename A, typename B>
auto multiply(A a, B b) -> decltype(a * b) {
   return a * b;
}

// __divide(a, b)__.
// Returns a / b.
template <typename A, typename B>
auto divide(A a, B b) -> decltype(a / b) {
   return a / b;
}

// __modulo(a, b)__.
// Returns a % b.
template <typename A, typename B>
auto modulo(A a, B b) -> decltype(a % b) {
   return a % b;
}

// ## Strings

// __split(input, regex)__.
// Split into tokens separated by regex.
std::vector<std::string> split(const std::string& input, const std::string& regex) {
   std::vector<std::string> result;
   std::sregex_token_iterator iter(input.begin(), input.end(), std::regex(regex), -1);
   std::sregex_token_iterator end;
   for (; iter != end; ++iter) {
      result.push_back(*iter);
   }
   return result;
}

// ## I/O

// __spit(file, content)__.
// Write a string to a file.
void spit(const std::string& file, const std::string& content) {
   std::ofstream out(file);
   out << content;
   out.close();
}

// __slurp(file)__.
// Read a file into a string.
std::string slurp(const std::string& file) {
   std::ifstream input("file.txt");
   std::stringstream buffer;
   buffer << input.rdbuf();
   return buffer.str();
}
