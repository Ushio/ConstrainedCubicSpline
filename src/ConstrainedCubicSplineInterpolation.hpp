#pragma once

#include <cmath>
#include <vector>

/*

*/
template <class T>
class ConstrainedCubicSplineInterpolation {
public:
	struct DataPoint {
		DataPoint() {}
		DataPoint(T x_, T y_) :x(x_), y(y_) {}

		T x = T(0.0);
		T y = T(0.0);
	};

	ConstrainedCubicSplineInterpolation() {

	}
	ConstrainedCubicSplineInterpolation(std::initializer_list<DataPoint> points) {
		for (auto p : points) {
			_points.push_back(p);
		}
	}
	ConstrainedCubicSplineInterpolation(const std::vector<DataPoint> &points) :_points(points) {

	}

	T evaluate(T x) const {
		if (_points.size() == 0) {
			return T(0.0);
		}
		if (_points.size() == 1)
		{
			return _points[0].y;
		}

		int upper = (int)_points.size() - 1;
		int lower = 0;

		if (x <= _points[lower].x)
		{
			return _points[lower].y;
		}
		if (_points[upper].x <= x)
		{
			return _points[upper].y;
		}

		while (lower + 1 != upper)
		{
			int mid = (lower + upper) / 2;
			if (x < _points[mid].x)
			{
				upper = mid;
			}
			else {
				lower = mid;
			}
		}

		auto sqr = [](T x) {
			return x * x;
		};
		auto cube = [](T x) {
			return x * x * x;
		};

		int i = upper;

		T f_dot_dot_i_minus_1 =
			-T(2.0) * (f_dot_(i) + T(2.0) * f_dot_(i - 1)) / (_points[i].x - _points[i - 1].x)
			+
			T(6.0) * (_points[i].y - _points[i - 1].y) / sqr(_points[i].x - _points[i - 1].x);

		T f_dot_dot_i =
			T(2.0) * (T(2.0) * f_dot_(i) + f_dot_(i - 1)) / (_points[i].x - _points[i - 1].x)
			-
			T(6.0) * (_points[i].y - _points[i - 1].y) / sqr(_points[i].x - _points[i - 1].x);

		T d = (f_dot_dot_i - f_dot_dot_i_minus_1) / (T(6.0) * (_points[i].x - _points[i - 1].x));
		T c = (_points[i].x * f_dot_dot_i_minus_1 - _points[i - 1].x * f_dot_dot_i) / (T(2.0) * (_points[i].x - _points[i - 1].x));
		T b = ((_points[i].y - _points[i - 1].y) - c * (sqr(_points[i].x) - sqr(_points[i - 1].x)) - d * (cube(_points[i].x) - cube(_points[i - 1].x))) / (_points[i].x - _points[i - 1].x);
		T a = _points[i - 1].y - b * _points[i - 1].x - c * sqr(_points[i - 1].x) - d * cube(_points[i - 1].x);

		return cubicEvaluate(x, a, b, c, d);
	}

	std::size_t size() const {
		return _points.size();
	}
	DataPoint &operator[](int i) {
		return _points[i];
	}
	const DataPoint &operator[](int i) const {
		return _points[i];
	}

private:
	// x_0....x_n
	T f_dot_(int i) const {
		if (i == 0) {
			return T(3.0) * (_points[1].y - _points[0].y) / (T(2.0) * (_points[1].x - _points[0].x)) - f_dot_(1) / T(2.0);
		}
		int n = (int)_points.size() - 1;
		if (i == n) {
			return T(3.0) * (_points[n].y - _points[n - 1].y) / (T(2.0) * (_points[n].x - _points[n - 1].x)) - f_dot_(n - 1) / T(2.0);
		}

		T slope = T(2.0) / (
			(_points[i + 1].x - _points[i].x) / (_points[i + 1].y - _points[i].y)
			+
			(_points[i].x - _points[i - 1].x) / (_points[i].y - _points[i - 1].y)
			);

		T s_lhs = (_points[i + 1].y - _points[i].y) / (_points[i + 1].x - _points[i].x);
		T s_rhs = (_points[i].y - _points[i - 1].y) / (_points[i].x - _points[i - 1].x);
		if (std::signbit(s_lhs) == std::signbit(s_rhs)) {
			return slope;
		}
		return T(0.0);
	}

	inline T cubicEvaluate(T x, T a, T b, T c, T d) const {
		// d * x * x * x + c * x * x + b * x + a
		// ((d * x + c) * x + b) * x + a
		return std::fma(std::fma(std::fma(d, x, c), x, b), x, a);
	}

	std::vector<DataPoint> _points;
};