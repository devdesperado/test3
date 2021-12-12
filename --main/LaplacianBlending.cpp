#include "LaplacianBlending.h"

void LaplacianBlending::buildGaussianPyramid() {//����ũ�� ����þ� �Ƕ�̵� �����ϱ�

	assert(leftLapPyr.size() > 0);// leftLapPyr�� �Է��� ����� �Ǿ�����!?

	maskGaussianPyramid.clear();//����ũ�� ���� ����þ� �Ƕ�̵� ���� ���� �������
	Mat currentImg;
	cvtColor(blendMask, currentImg, COLOR_GRAY2BGR);// blend mask >> currentImg�� �����ϸ� �÷��� ��ȯ
	maskGaussianPyramid.push_back(currentImg); //highest level

	currentImg = blendMask;
	for (int l = 1; l < levels + 1; l++) {//level�� ������ŭ
		Mat _down;
		if (leftLapPyr.size() > l) {// �ش� level���� ����� ũ�ٸ�
			pyrDown(currentImg, _down, leftLapPyr[l].size());//downsmapling�Ͽ� _down�� ����
		}
		else {//�� �۴ٸ�
			pyrDown(currentImg, _down, leftSmallestLevel.size()); //smallest level
		}

		Mat down;
		cvtColor(_down, down, COLOR_GRAY2BGR);
		maskGaussianPyramid.push_back(down);
		currentImg = _down;
	}
}

//#4-1 �� �̹����� �Ƕ�̵� ����
void LaplacianBlending::buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& smallestLevel) 
{
	lapPyr.clear();//�Ƕ�̵� ������ clear
	Mat currentImg = img;
	for (int l = 0; l < levels; l++) {// �̸� �Է��� level��ŭ
		Mat down, up;
		pyrDown(currentImg, down);//downsampling @size
		pyrUp(down, up, currentImg.size());//upsampling @size
		Mat lap = currentImg - up;//�Է� �̹����� ���� upsampling image�� �� lap�� ����
		lapPyr.push_back(lap);//lap�� �Ƕ�̵�
		currentImg = down;//downsmapling�� currentImg
	}
	currentImg.copyTo(smallestLevel);//currentImg(downsmapling)�� smallestLevel�� input
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
