#include <iostream>
#include <math.h>
#include <string>
#include <fstream>

using namespace std;

enum e_target
{
	AIRCRAFT = 1,
	MISSILE = 2
};

#define DT 1.

class TTarget
{
public:

	double _x, _y;

	TTarget()
	{
		cout << "Default constructor from TTarget colled" << endl;
	}

	TTarget(double x, double y, double V, double K)
	{
		cout << "Param constructor from TTarget colled" << endl;
		_x0 = x;
		_y0 = y;
		_V = V;
		_K = K;
		_t = 0;
	}

	virtual void Move(double ti)
	{
		ti = 0;
	}

	~TTarget()
	{
		cout << "Destructor from TTarget colled" << endl;
	}

protected:

	double _x0, _y0, _V, _K, _t;
	int _type;
};

class TAircraft : public TTarget
{
public:

	TAircraft()
	{
		cout << "Default constructor from TAircraft colled" << endl;
	}

	TAircraft(double x, double y, double V, double K)
		: TTarget(x, y, V, K)
	{
		cout << "Param constructor from TAircraft colled" << endl;
		this->_type = AIRCRAFT;
	}

	virtual void Move(double ti)
	{
		_x = _x0 - _V * cos(_K) * (ti - _t);
		_y = _y0 - _V * sin(_K) * (ti - _t);
	}

	~TAircraft() {}

};

class TMissile : public TTarget
{
public:

	TMissile()
	{
		cout << "Default constructor from TMissile colled" << endl;
	}

	TMissile(double x, double y, double V, double K, double N)
		: TTarget(x, y, V, K), _N(N)
	{
		cout << "Param constructor from TMissile colled" << endl;
		this->_type = MISSILE;
	}

	virtual void Move(double ti)
	{
		_x = _x0 - (_V + _N * (ti - _t)) * cos(_K) * (ti - _t);
		_y = _y0 - (_V + _N * (ti - _t)) * sin(_K) * (ti - _t);
	}

	~TMissile() {}

private:

	double _N;
};

class RLS
{
public:

	RLS()
	{
		cout << "Defaul constructor from RLS colled" << endl;
	}

	RLS(double x, double y, double R, TTarget *targets, int tarLen)
		: _x(x), _y(y), _R(R), _targets(targets), _tarLen(tarLen)
	{
		cout << "Param constructor from RLS colled" << endl;
	}

	void Peleng(double t0, double tk)
	{
		std::ofstream	fout("result");
		if (!fout)
		{
			std::cout << "file open error";
			return ;
		}

		for (int t = t0; t < tk; t += DT)
		{
			for (int i = 0; i < _tarLen; i++)
			{
				_targets[i].Move(t);
				double D = sqrt(pow(_targets[i]._x - _x, 2) + pow(_targets[i]._y - _y, 2));
				if (D <= _R)
				{
					double Az0 = atan((_targets[i]._y - _y) / (_targets[i]._x - _x));
					fout << t << ") " << D << " ; " << Az0 << endl;
				}
			}
		}
		fout.close();
	}

	~RLS()
	{
		cout << "Destructor from RLS colled" << endl;
	}

private:

	double _x, _y, _R;
	int _tarLen;
	TTarget *_targets;
};

int main( void )
{
	/*
	_plaNum++;
	TLA **newPLA = new TLA* [_plaNum];
	for (int i = 0; i < _plaNum - 1; i++)
		newPLA[i] = _PLA[i];
	delete [] _PLA;
	newPLA[_plaNum - 1] = newTLA;
	_PLA = newPLA;
	*/

	TTarget *targets = new TTarget [2];
	targets[0] = TAircraft(1, 1, 20, 5);
	targets[1] = TMissile(2, 2, 30, 6, 1);

	RLS rls(1, 2, 3, targets, 2);
	rls.Peleng(0, 10);

	return (0);
}