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
#include <sys/stat.h>

struct stat sb;


int main(int argc, char* argv[])
{
  // We expect 2 arguments: the program name, the source path and the destination path
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <image_name>.png" << std::endl;
    return 1;
  }

  std::vector<std::string> sources;
  std::string midPath;
  std::string inDir = "../01_segmented/";
  std::string outDir = "../02_deslanted/";
  for (int i = 1; i < argc; ++i) {
    std::cout << std::string(midPath + " " + argv[i]) << std::endl;

    if (std::string(argv[i]) == "--midpath") {
      if (i + 1 < argc) { // Make sure we aren't at the and of argv!
        midPath = argv[++i]; // increment 'i' so we don't get the argument as the next argv[i].
      }
      else { // Uh-oh, there was no argument to the destination opiton.
        std::cerr << "--midpath option requires one argument." << std::endl;
        return 1;
      }
    }
    else {
      sources.push_back(argv[i]);
    }
  }

  std::cout << "../01_segmented/ => ../02_deslanted/" << std::endl;

  try {
    if (stat(std::string(outDir + midPath).c_str(), &sb) != 0 && !S_ISDIR(sb.st_mode)) {
      mkdir(std::string(outDir + midPath).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    for (std::vector<std::string>::const_iterator itr = sources.begin();
         itr != sources.end();
         ++itr)
      {
        std::cout << inDir + midPath + *itr + ".png" << std::endl;
        std::cout << outDir + midPath + *itr + ".png" << std::endl;
        const cv::Mat img = cv::imread(inDir  + midPath + *itr + ".png", cv::IMREAD_GRAYSCALE);
        cv::Mat res = htr::deslantImg(img, 255);
        cv::imwrite(outDir + midPath + *itr + ".png", res);
      }
  }
  catch(const std::exception& e) {
    std::cerr << "Standard exception: " << e.what()  << std::endl;
    return 1;
  }

  return 0;
}
