#include "LaplacianBlending.h"

void LaplacianBlending::buildGaussianPyramid() {//마스크의 가우시안 피라미드 구축하기

	assert(leftLapPyr.size() > 0);// leftLapPyr의 입력이 제대로 되었는지!?

	maskGaussianPyramid.clear();//마스크에 대한 가우시안 피라미드 집합 벡터 비워놓기
	Mat currentImg;
	cvtColor(blendMask, currentImg, COLOR_GRAY2BGR);// blend mask >> currentImg로 저장하며 컬러로 변환
	maskGaussianPyramid.push_back(currentImg); //highest level

	currentImg = blendMask;
	for (int l = 1; l < levels + 1; l++) {//level의 개수만큼
		Mat _down;
		if (leftLapPyr.size() > l) {// 해당 level보다 사이즈가 크다면
			pyrDown(currentImg, _down, leftLapPyr[l].size());//downsmapling하여 _down에 저장
		}
		else {//더 작다면
			pyrDown(currentImg, _down, leftSmallestLevel.size()); //smallest level
		}

		Mat down;
		cvtColor(_down, down, COLOR_GRAY2BGR);
		maskGaussianPyramid.push_back(down);
		currentImg = _down;
	}
}

//#4-1 한 이미지씩 피라미드 빌드
void LaplacianBlending::buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& smallestLevel) 
{
	lapPyr.clear();//피라미드 빌드전 clear
	Mat currentImg = img;
	for (int l = 0; l < levels; l++) {// 미리 입력한 level만큼
		Mat down, up;
		pyrDown(currentImg, down);//downsampling @size
		pyrUp(down, up, currentImg.size());//upsampling @size
		Mat lap = currentImg - up;//입력 이미지에 대해 upsampling image를 빼 lap에 저장
		lapPyr.push_back(lap);//lap을 피라미드
		currentImg = down;//downsmapling을 currentImg
	}
	currentImg.copyTo(smallestLevel);//currentImg(downsmapling)를 smallestLevel에 input
}

Mat_<Vec3f> LaplacianBlending::reconstructImgFromLapPyramid() {
	Mat currentImg = resultSmallestLevel;
	for (int l = levels - 1; l >= 0; l--) {
		Mat up;

		pyrUp(currentImg, up, resultLapPyr[l].size());
		currentImg = up + resultLapPyr[l];
	}
	return currentImg;
}

void LaplacianBlending::blendLapPyrs() 
{
	resultSmallestLevel = leftSmallestLevel.mul(maskGaussianPyramid.back()) +
		rightSmallestLevel.mul(Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid.back());
	for (int l = 0; l < levels; l++) {
		Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
		Mat antiMask = Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid[l];
		Mat B = rightLapPyr[l].mul(antiMask);
		Mat_<Vec3f> blendedLevel = A + B;

		resultLapPyr.push_back(blendedLevel);
	}
}
