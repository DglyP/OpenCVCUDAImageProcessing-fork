#include <chrono> // for high_resolution_clock
#include <iostream>
#include <opencv2/cudawarping.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

void startCUDA(cv::cuda::GpuMat &src, cv::cuda::GpuMat &dst, int kernel_size, float sigma);

int main(int argc, char **argv) {
  cv::namedWindow("Original Image", cv::WINDOW_OPENGL | cv::WINDOW_AUTOSIZE);
  cv::namedWindow("Processed Image", cv::WINDOW_OPENGL | cv::WINDOW_AUTOSIZE);

  cv::Mat h_img = cv::imread(argv[1]);
  cv::Mat h_result(h_img.rows, h_img.cols, CV_8UC3, cv::Scalar(0, 255, 0));
  cv::cuda::GpuMat d_img, d_result;

  cv::imshow("Original Image", h_img);

  auto begin = chrono::high_resolution_clock::now();
  const int iter = 1;

  const int KERNEL_SIZE = 31;
  const int KERNEL_DIV_2 = KERNEL_SIZE / 2;
  const float SIGMA = 5;

  // Create input image which corresponds to the source image with an added
  // replication padding.
  cv::copyMakeBorder(h_img, h_img, KERNEL_DIV_2, KERNEL_DIV_2, KERNEL_DIV_2,
		     KERNEL_DIV_2, cv::BORDER_REPLICATE);

  d_img.upload(h_img);
  d_result.upload(h_result);

  for (int i = 0; i < iter; i++) {
    startCUDA(d_img, d_result, KERNEL_SIZE, SIGMA);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - begin;

  cv::imshow("Processed Image", d_result);

  cout << diff.count() << endl;
  cout << diff.count() / iter << endl;
  cout << iter / diff.count() << endl;

  cv::waitKey();
  return 0;

  return 0;
}