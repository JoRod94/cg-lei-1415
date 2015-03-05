#include <vector>

using namespace std;

class point
{
public:
	float x, z, y;
	point(float x, float y, float z) : x(x), y(y), z(z) {};
	point() : x(0), y(0), z(0) {};

	bool operator== (const point& p) const;

	void add_to(vector<float> vec);
};

