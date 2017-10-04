class ConstrainedCubicSplineInterpolation
{
	public struct DataPoint
    {
        public DataPoint(float x_, float y_)
        {
            x = x_;
            y = y_;
        }
        public float x;
        public float y;
    }

    public ConstrainedCubicSplineInterpolation()
    {
        _points = new DataPoint[0];
    }

    public ConstrainedCubicSplineInterpolation(DataPoint[] points)
    {
        _points = points;
        _cubics = new Cubic[_points.Length - 1];
        for(int i = 0; i < _cubics.Length; ++i)
        {
            _cubics[i] = cubic_(i);
        }
    }

    private float sqr(float x)
    {
        return x * x;
    }
    private float cube(float x)
    {
        return x * x * x;
    }

    public float Evaluate(float x)
    {
        if (_points.Length == 0)
        {
            return 0.0f;
        }
        if (_points.Length == 1)
        {
            return _points[0].y;
        }

        int upper = (int)_points.Length - 1;
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
            else
            {
                lower = mid;
            }
        }

        int i = upper;
        var cubic = _cubics[i - 1];
        return cubicEvaluate(x, cubic.a, cubic.b, cubic.c, cubic.d);
    }

    // i = function index
    private Cubic cubic_(int i)
    {
        i++;

        float f_dot_dot_i_minus_1 =
            -2.0f * (f_dot_(i) + 2.0f * f_dot_(i - 1)) / (_points[i].x - _points[i - 1].x)
            +
            6.0f * (_points[i].y - _points[i - 1].y) / sqr(_points[i].x - _points[i - 1].x);

        float f_dot_dot_i =
            2.0f * (2.0f * f_dot_(i) + f_dot_(i - 1)) / (_points[i].x - _points[i - 1].x)
            -
            6.0f * (_points[i].y - _points[i - 1].y) / sqr(_points[i].x - _points[i - 1].x);

        float d = (f_dot_dot_i - f_dot_dot_i_minus_1) / (6.0f * (_points[i].x - _points[i - 1].x));
        float c = (_points[i].x * f_dot_dot_i_minus_1 - _points[i - 1].x * f_dot_dot_i) / (2.0f * (_points[i].x - _points[i - 1].x));
        float b = ((_points[i].y - _points[i - 1].y) - c * (sqr(_points[i].x) - sqr(_points[i - 1].x)) - d * (cube(_points[i].x) - cube(_points[i - 1].x))) / (_points[i].x - _points[i - 1].x);
        float a = _points[i - 1].y - b * _points[i - 1].x - c * sqr(_points[i - 1].x) - d * cube(_points[i - 1].x);
        return new Cubic(a, b, c, d);
    }

    int Size
    {
        get
        {
            return _points.Length;
        }
    }
    public DataPoint this[int i]
    {
        get
        {
            return _points[i];
        }
        set
        {
            _points[i] = value;
        }
    }

    // x_0....x_n
    private float f_dot_(int i)
    {
        if (i == 0)
        {
            return 3.0f * (_points[1].y - _points[0].y) / (2.0f * (_points[1].x - _points[0].x)) - f_dot_(1) / 2.0f;
        }
        int n = (int)_points.Length - 1;
        if (i == n)
        {
            return 3.0f * (_points[n].y - _points[n - 1].y) / (2.0f * (_points[n].x - _points[n - 1].x)) - f_dot_(n - 1) / 2.0f;
        }

        float slope = 2.0f / (
            (_points[i + 1].x - _points[i].x) / (_points[i + 1].y - _points[i].y)
            +
            (_points[i].x - _points[i - 1].x) / (_points[i].y - _points[i - 1].y)
            );

        float s_lhs = (_points[i + 1].y - _points[i].y) / (_points[i + 1].x - _points[i].x);
        float s_rhs = (_points[i].y - _points[i - 1].y) / (_points[i].x - _points[i - 1].x);
        if (System.Math.Sign(s_lhs) == System.Math.Sign(s_rhs))
        {
            return slope;
        }
        return 0.0f;
    }

    private float fma(float a, float b, float c)
    {
        return a * b + c;
    }
    private float cubicEvaluate(float x, float a, float b, float c, float d) {
		// d * x * x * x + c * x * x + b * x + a
		// ((d * x + c) * x + b) * x + a
		return fma(fma(fma(d, x, c), x, b), x, a);
	}

    struct Cubic
    {
        public float a;
        public float b;
        public float c;
        public float d;

        public Cubic(float a_, float b_, float c_, float d_)
        {
            a = a_;
            b = b_;
            c = c_;
            d = d_;
        }
    }

    DataPoint[] _points;
    Cubic[] _cubics;
}