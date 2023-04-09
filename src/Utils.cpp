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

void generateColors(std::vector<cv::Scalar>& oColors)
{
	std::mt19937 gen(0); // seed
	std::uniform_int_distribution<int> distribution(20, 255);

	std::generate(begin(oColors), end(oColors), [&distribution, &gen] {
		return cv::Scalar(distribution(gen), distribution(gen), distribution(gen));
	});
}

void getScreenSize(int& oHeight, int& oWidth)
{
	oHeight = GetDeviceCaps(GetDC(NULL), VERTRES);
	oWidth = GetDeviceCaps(GetDC(NULL), HORZRES);
}