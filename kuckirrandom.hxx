#include <limits>

namespace kuckir
{

int random_position = 0;
int scale = std::numeric_limits<int>::max();

class kuckir_random
{

public:

	static float next_random_float()
	{
                int a = random_position;
                a = (a ^ 61) ^ (a >> 16);
                a = a + (a << 3);
                a = a ^ (a >> 4);
                a = a * 0x27d4eb2d;
                a = a ^ (a >> 15);

                random_position = a;

                return ((float) a )/((float) scale);

	}

};
}
