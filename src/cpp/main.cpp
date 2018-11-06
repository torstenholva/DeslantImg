// uncomment next line to use GPU via OpenCL
//#define USE_GPU


#ifdef USE_GPU
#include "DeslantImgGPU.hpp"
#else
#include "DeslantImgCPU.hpp"
#endif
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <exception>    // std::exception


int main(int argc, char* argv[])
{
	// We expect 2 arguments: the program name, the source path and the destination path
	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " <image_name>.png" << std::endl;
		return 1;
	}

	std::vector<std::string> sources;
	std::string destination;
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "--destination") {
			if (i + 1 < argc) { // Make sure we aren't at the and of argv!
				destination = argv[i++]; // increment 'i' so we don't get the argument as the next argv[i].
			}
			else { // Uh-oh, there was no argument to the destination opiton.
				std::cerr << "--destination option requires one argument." << std::endl;
				return 1;
			}
		}
		else {
			sources.push_back(argv[i]);
		}
	}

	std::cout << "../01_segmented/ => ../02_deslanted/" << std::endl;

	try {
		for (std::vector<std::string>::const_iterator itr = sources.begin();
			 itr != sources.end();
			 ++itr)
		{
			std::cout << *itr + ".png" << std::endl;
			const cv::Mat img = cv::imread("../01_segmented/" + *itr + ".png", cv::IMREAD_GRAYSCALE);
			cv::Mat res = htr::deslantImg(img, 255);
			cv::imwrite("../02_deslanted/" + *itr + ".png", res);
		}
	}
	catch(const std::exception& e) {
		std::cerr << "Standard exception: " << e.what()  << std::endl;
		return 1;
	}

	return 0;
}
