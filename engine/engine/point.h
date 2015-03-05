
#include <vector>

using namespace std;

class point
{
public:
	float x, z, y;
	point(float x, float y, float z) : x(x), y(y), z(z) {};
	point() : x(0), y(0), z(0) {};

	bool operator== (const point& p) const {
		return x == p.x && y == p.y && z == p.z;
	}

	void add_to(vector<float> vec) {
		vec.push_back(x);
		vec.push_back(y);
		vec.push_back(z);
	}
};

