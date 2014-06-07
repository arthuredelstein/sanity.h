// sanitycheck.cpp : Tests for sanity.h
//

#include "stdafx.h"

#include "sanity.h"

double times2(double x) {
   return 2 * x;
}

bool positive(double x) {
   return x > 0;
}

double plus(double a, double b) {
	return a + b;
}

int _tmain(int argc, _TCHAR* argv[]) {
   std::vector<double> x;
   x.push_back(1);
   x.push_back(2);
   x.push_back(3);
   x.push_back(-10);
   x.push_back(-1);
   x.push_back(4);
   auto a = map(x, [](double q) -> double { return 3 * q; });
   auto y = filter(map(x, times2), positive);
   auto y1 = filter(x, [](double q) -> bool { return q < 2; });
   auto y2 = remove(x, [](double q) -> bool { return q < 2; });
   auto z = reduce(10.0, x, plus);
   auto z1 = reduce(x, plus);
   auto r1 = range((int) 1, (int) 10, (double) 1.4);
   auto r2 = range((int) 1, (int) 30);
   auto r3 = map(range(30), times2);
   return 0;
}

