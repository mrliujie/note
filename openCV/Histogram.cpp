#include <iostream>
#include <cv.h>
#include "opencv2/opencv.hpp"
#include <highgui.h>
using namespace std;
using namespace cv;

int main(void)
{
    string strSrcImageName = "818678538.bmp";

    cv::Mat matSrc = cv::imread(strSrcImageName, 0);

    cv::Mat matSrc1 = cv::imread("819703074.bmp", 0);

    cv::Mat matDst1, matDst2;

    if (matSrc.channels() == 1) {
        int histSize = 256;
        float range[] = {0, 255};
        const float* histRange = {range};
        bool uniform = true;
        bool accumulate = false;

        cv::Mat hist1, hist2;

        cv::calcHist(&matDst1, 1, 0, cv::Mat(), hist1, 1, &histSize, &histRange, uniform, accumulate);
        cv::normalize(hist1, hist1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        cv::calcHist(&matDst2, 1, 0, cv::Mat(), hist2, 1, &histSize, &histRange, uniform, accumulate);
        cv::normalize(hist2, hist2, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        double dSimilarity = cv::compareHist(hist1, hist2, CV_COMP_BHATTACHARYYA  );//,CV_COMP_CHISQR,CV_COMP_INTERSECT,CV_COMP_BHATTACHARYYA

        cout<<"sing channel similarity = "<<dSimilarity<<endl;
    } else {
        cv::cvtColor(matDst1, matDst1, cv::COLOR_BGR2HSV);
        cv::cvtColor(matDst2, matDst2, cv::COLOR_BGR2HSV);

        int h_bins = 50, s_bins = 60;
        int histSize[] = {h_bins, s_bins};
        float h_ranges[] = {0, 180};
        float s_ranges[] = {0, 256};
        const float* ranges[] = {h_ranges, s_ranges};
        int channels[] = {0, 1};

        cv::MatND hist1, hist2;

        cv::calcHist(&matDst1, 1, channels, cv::Mat(), hist1, 2, histSize, ranges, true, false);
        cv::normalize(hist1, hist1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        cv::calcHist(&matDst2, 1, channels, cv::Mat(), hist2, 2, histSize, ranges, true, false);
        cv::normalize(hist2, hist2, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        double dSimilarity = cv::compareHist(hist1, hist2, HISTCMP_CHISQR_ALT  );

        cout<<"similarity = "<<dSimilarity<<endl;
    }
    return 0;
}

