#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

class LaplacianBlending {
private://선언부
	Mat_<Vec3f> left;
	Mat_<Vec3f> right;
	Mat_<float> blendMask;

	vector<Mat_<Vec3f> > leftLapPyr, rightLapPyr, resultLapPyr;
	Mat leftSmallestLevel, rightSmallestLevel, resultSmallestLevel;
	vector<Mat_<Vec3f> > maskGaussianPyramid; //"masks" are 3-channels for easier multiplication with RGB

	int levels;

	//#4
	void buildPyramids() {//left,right 이미지에 대한 라플라시안 피라미드 구축
		buildLaplacianPyramid(left, leftLapPyr, leftSmallestLevel);//#4-1
		buildLaplacianPyramid(right, rightLapPyr, rightSmallestLevel);
		buildGaussianPyramid();//#4-2
	}

	//마스크의 가우시안 피라미드 구축하기
	void buildGaussianPyramid();

	//#4-1 한 이미지씩 피라미드 빌드
	void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& smallestLevel);
	
	//#6
	Mat_<Vec3f> reconstructImgFromLapPyramid();

	//#5
	void blendLapPyrs();

	//#3
public:
	LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, const Mat_<float>& _blendMask, int _levels) :
		left(_left), right(_right), blendMask(_blendMask), levels(_levels)
	{
		assert(_left.size() == _right.size());// 두 이미지 및 마스크의 사이즈 동일하다는 조건
		assert(_left.size() == _blendMask.size());
		buildPyramids();//#4
		blendLapPyrs();//#5
	};

	//#6
	Mat_<Vec3f> blend() { return reconstructImgFromLapPyramid(); }
};

