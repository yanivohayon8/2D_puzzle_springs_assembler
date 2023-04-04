#include <Utils.h>


void generateColors(std::vector<cv::Scalar>& oColors)
{
	std::mt19937 gen(0); // seed
	std::uniform_int_distribution<int> distribution(20, 255);

	std::generate(begin(oColors), end(oColors), [&distribution, &gen] {
		return cv::Scalar(distribution(gen), distribution(gen), distribution(gen));
	});
}