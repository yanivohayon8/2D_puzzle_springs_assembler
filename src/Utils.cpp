#include <Utils.h>

void generate2DVectors(std::vector<b2Vec2>&vectors,int n,int maxX,int maxY,int padding, int seed)
{
	std::mt19937 gen(seed); 
	std::uniform_int_distribution<int> distributionX(padding + 0, maxX-padding);
	std::uniform_int_distribution<int> distributionY(padding + 0, maxY-padding);

	for (int i = 0; i < n; i++)
	{
		int x = distributionX(gen);
		int y = distributionY(gen);
		vectors.push_back(b2Vec2(x, y));
	}
	
}


void getScreenSize(int& oHeight, int& oWidth)
{
	oHeight = GetDeviceCaps(GetDC(NULL), VERTRES);
	oWidth = GetDeviceCaps(GetDC(NULL), HORZRES);
}

int sampleIntUniformly(double max, double min, int seed)
{

	std::mt19937 gen(seed);

	if (seed==-1)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
	}

	std::uniform_int_distribution<int> distribution(min,max);
	return distribution(gen);
}


void getRoatationMatrix(Eigen::MatrixX2d& oRotation, double angle)
{
	oRotation(0, 0) = std::cos(angle);
	oRotation(1, 0) = std::sin(angle);
	oRotation(1, 0) = -std::sin(angle);
	oRotation(1, 1) = std::cos(angle);
}

void getRoatationMatrix(Eigen::MatrixX2d& oRotation, const b2Rot& rot)
{
	oRotation(0, 0) = rot.c;
	oRotation(1, 0) = rot.s;
	oRotation(1, 0) = -rot.s;
	oRotation(1, 1) = rot.c;
}

