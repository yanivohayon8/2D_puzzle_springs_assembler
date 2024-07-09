#include <Utils.h>

void generate2DVectors(std::vector<b2Vec2>&vectors,int n,int maxX,int maxY,int padding, int seed)
{
	// Use the Mersenne Twister engine for randomness
	std::mt19937 gen(seed);

	// Define the uniform distribution for the given domain
	std::uniform_real_distribution<double> distributionX(padding + 0, maxX - padding);
	std::uniform_real_distribution<double> distributionY(padding + 0, maxY - padding);

	for (int i = 0; i < n; i++)
	{
		double x = distributionX(gen); 
		double y = distributionY(gen);
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

bool isFloatString(const std::string str) {
	try {
		// Attempt to convert the string to a floating-point value
		size_t pos;
		std::stof(str, &pos);

		// Check if the entire string was successfully converted
		return pos == str.size();
	}
	catch (std::invalid_argument&) {
		// The conversion failed due to invalid argument (not a valid number)
		return false;
	}
	catch (std::out_of_range&) {
		// The conversion failed because the number is out of range
		return false;
	}
}