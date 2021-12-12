#include <vector>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
//opencv���� �����ϴ� ������ SURF���(���̼�������)
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "LaplacianBlending.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

const int LOOP_NUM = 10;
const int GOOD_PTS_MAX = 50;
const float GOOD_PORTION = 0.15f;

///SURF�� ����� feature �����
struct SURFDetector 
{
	Ptr<Feature2D> surf;
	SURFDetector(double hessian = 800.0)
	{
		surf = SURF::create(hessian);
	}

	template<class Type>
	void operator()(const Type& in, const Type& mask, vector<cv::KeyPoint>& pts, Type& descriptors, bool useProvided = false)
	{
		//���⼭ detect�� descriptor�� ���ÿ� ����
		surf->detectAndCompute(in, mask, pts, descriptors, useProvided);
	}
};

template<class KPMatcher>
struct SURFMatcher
{
	KPMatcher matcher;

	template<class T>
	void match(const T& in1, const T& in2, vector<DMatch>& matches) 
	{
		matcher.match(in1, in2, matches);
	}
};


void getGoodMatches(vector<DMatch>& matches, vector< DMatch >& good_matches,
	const vector<KeyPoint>& keypoints1, const vector<KeyPoint>& keypoints2, vector<Point2f>& img_1, vector<Point2f>& img_2)
{
	sort(matches.begin(), matches.end());

	//15%�� ���� Ư¡������ ����
	const int ptsPairs = min(GOOD_PTS_MAX, (int)(matches.size() * GOOD_PORTION));
	for (int i = 0; i < ptsPairs; i++)
		good_matches.push_back(matches[i]);

	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//good_match�κ��� good features ����
		img_1.push_back(keypoints1[good_matches[i].queryIdx].pt);
		img_2.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
}

static Mat drawGoodMatches(const Mat& img1, const Mat& img2, 
	const vector<KeyPoint>& keypoints1, const vector<KeyPoint>& keypoints2,
	vector<DMatch>& good_matches, vector<Point2f>& img2_corners_, Mat& H)
{
	//img1���� corner�� ����
	vector<Point2f> img1_corners(4);
	img1_corners[0] = Point(0, 0);
	img1_corners[1] = Point(img1.cols, 0);
	img1_corners[2] = Point(img1.cols, img1.rows);
	img1_corners[3] = Point(0, img1.rows);
	
	vector<Point2f> img2_corners(4);
	
	Mat img_matches;
	drawMatches(img1, keypoints1, img2, keypoints2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//Draw lines between the corners
	img2_corners = img2_corners_;
	perspectiveTransform(img2_corners, img1_corners, H);

	for (int i = 0; i < 4; i++)
	{
		line(img_matches,
			img2_corners[i] + Point2f((float)img1.cols, 0), img2_corners[(i + 1) % 4] + Point2f((float)img1.cols, 0),
			Scalar(0, 255, 0), 2, LINE_AA);
	}

	return img_matches;
}

///img1�� img2�� stitching�� ��� output
static Mat stitchingImage(const Mat img1, const Mat img2, const Mat& globalH)
{
	Mat Stitched;
	warpPerspective(img2, Stitched, globalH, Size(img2.cols * 2, img2.rows), INTER_CUBIC);
	Mat matROI(Stitched, Rect(0, 0, img1.cols, img1.rows));
	img1.copyTo(matROI);

	return Stitched;
}

///src �̹����� ��� Ư¡���� print
static Mat drawAllKeypoints(const Mat& src, const vector<KeyPoint>& srcKeypoints)
{
	Mat printKeyPoint;
	drawKeypoints(src, srcKeypoints, printKeyPoint, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	return printKeyPoint;
}

Mat getHomography_basicDLT(const vector<Point2f>& src_points, const vector<Point2f>& dst_points) 
{
	///���� �⺻������ ���� �� �ִ� homography���� ���� using basic DLT algorithm	
	int feautreSize = src_points.size();

	//build A matrix
	//A matrix: transform ������ ������ �� �������迡 �ִ� ��ǥ��� �������� ���
	Mat A(feautreSize * 2, 9, CV_32FC1);
	for (int i = 0; i < feautreSize; i++)
	{
		A.at<float>(2 * i, 0) = -src_points[i].x;
		A.at<float>(2 * i, 1) = -src_points[i].y;
		A.at<float>(2 * i, 2) = -1.0f;
		A.at<float>(2 * i, 3) = 0.0f;
		A.at<float>(2 * i, 4) = 0.0f;
		A.at<float>(2 * i, 5) = 0.0f;
		A.at<float>(2 * i, 6) = dst_points[i].x * src_points[i].x;
		A.at<float>(2 * i, 7) = dst_points[i].x * src_points[i].y;
		A.at<float>(2 * i, 8) = dst_points[i].x;
	
		A.at<float>(2 * i + 1, 0) = 0.0f;
		A.at<float>(2 * i + 1, 1) = 0.0f;
		A.at<float>(2 * i + 1, 2) = 0.0f;
		A.at<float>(2 * i + 1, 3) = -src_points[i].x;
		A.at<float>(2 * i + 1, 4) = -src_points[i].y;
		A.at<float>(2 * i + 1, 5) = -1.0f;
		A.at<float>(2 * i + 1, 6) = dst_points[i].y * src_points[i].x;
		A.at<float>(2 * i + 1, 7) = dst_points[i].y * src_points[i].y;
		A.at<float>(2 * i + 1, 8) = dst_points[i].y;
	}
	Mat u, d, vt, V;	//A = u d vt -> SVD�� ����
	SVD::compute(A, d, u, vt, SVD::FULL_UV); //singluar value decomposition�� �̿��� vt����� ����
	transpose(vt, V);
	
	Mat customH(3, 3, V.type());	//matrix vt�� ������ column vector�� customH�� ������ ��
	//h_33�� 1�� ����� �ֱ� ���ؼ� ������������ ��ü customH ��� ��ҵ��� ��������
	float lastVal = V.at<float>(8, 8);
	for (int h = 0; h < 3; h++)
		for (int w = 0; w < 3; w++)
		{
			customH.at<float>(h, w) = V.at<float>(h * 3 + w, 8) / lastVal;
		}

	return customH;
}


void pointNormalization(const vector<Point2f>& points, vector<Point2f>& normalized_points, Mat& T) 
{
	//data normalization by hartely
	//1) ��ǥ���� centroid���� origin(0,0)�� ��ġ�ϰ� ��ȯ
	//2) ��ǥ���� scale�Ǽ� ��ǥ�鿡�� origin���� �Ÿ��� ����� sqrt(2)
	int featureSize = points.size();
	float xCenter = 0.0f, yCenter = 0.0f;
	float dist = 0.0f;
	float scaling = 0.0f;	//similarity transfrom T�� ����ϱ� ���� scalar

	//centroid ���ϱ�
	for (int i = 0; i < featureSize; i++)
	{
		xCenter += points[i].x;
		yCenter += points[i].y;
	}
	xCenter /= featureSize;
	yCenter /= featureSize;

	for (int i = 0; i < featureSize; i++)
		dist += ((float)points[i].x - xCenter) * ((float)points[i].x - xCenter) + ((float)points[i].y - yCenter) * ((float)points[i].y - yCenter);

	dist = (float)pow(dist / (2*featureSize), 0.5);

	//similarity transfrom T����
	//x' = Htemp * x
	//where Htemp = {{sR , t}, {0^t, 1}} for s = scaling
	scaling = 1.0f / dist;
	float t[3 * 3] = { scaling,0,-scaling * xCenter,0,scaling, -scaling * yCenter, 0,0,1 };
	memcpy(T.data, t, 3 * 3 * sizeof(float));

	float x, y, w;
	for (int i = 0; i < featureSize; i++)
	{
		x = points[i].x;
		y = points[i].y;
		w = t[6] * x + t[7] * y + t[8];

		//homogenious coordinate�� ǥ��
		normalized_points[i].x = (t[0] * x + t[1] * y + t[2]) / w;
		normalized_points[i].y = (t[3] * x + t[4] * y + t[5]) / w;
	}
}

Mat getHomography_normalizedDLT(const vector<Point2f>& src_points, const vector<Point2f>& dst_points) 
{
	///normalized DLT algorithm�� ����ؼ� homography�� ����
	//H = inv(Tp) * customH * T
	int featureSize = src_points.size();
	Mat T(3, 3, CV_32FC1);	//similiarity transfrom of src_points
	Mat Tp(3, 3, CV_32FC1);	//similiarity transfrom of dst_points

	vector<Point2f> src_points_norm = src_points;
	vector<Point2f> dst_points_norm = dst_points;
	pointNormalization(src_points, src_points_norm, T);
	pointNormalization(dst_points, dst_points_norm, Tp);

	Mat A(featureSize * 2, 9, CV_32FC1);
	for (int i = 0; i < featureSize; i++)
	{
		A.at<float>(2 * i, 0) = -src_points_norm[i].x;
		A.at<float>(2 * i, 1) = -src_points_norm[i].y;
		A.at<float>(2 * i, 2) = -1.0f;
		A.at<float>(2 * i, 3) = 0.0f;
		A.at<float>(2 * i, 4) = 0.0f;
		A.at<float>(2 * i, 5) = 0.0f;
		A.at<float>(2 * i, 6) = dst_points_norm[i].x * src_points_norm[i].x;
		A.at<float>(2 * i, 7) = dst_points_norm[i].x * src_points_norm[i].y;
		A.at<float>(2 * i, 8) = dst_points_norm[i].x;

		A.at<float>(2 * i + 1, 0) = 0.0f;
		A.at<float>(2 * i + 1, 1) = 0.0f;
		A.at<float>(2 * i + 1, 2) = 0.0f;
		A.at<float>(2 * i + 1, 3) = -src_points_norm[i].x;
		A.at<float>(2 * i + 1, 4) = -src_points_norm[i].y;
		A.at<float>(2 * i + 1, 5) = -1.0f;
		A.at<float>(2 * i + 1, 6) = dst_points_norm[i].y * src_points_norm[i].x;
		A.at<float>(2 * i + 1, 7) = dst_points_norm[i].y * src_points_norm[i].y;
		A.at<float>(2 * i + 1, 8) = dst_points_norm[i].y;
	}

	Mat D, U, VT, V;
	SVD::compute(A, D, U, VT, SVD::FULL_UV);
	transpose(VT, V);
	
	Mat customH(3, 3, CV_32FC1);	//matrix vt�� ������ column vector�� customH�� ������ ��
	for (int h = 0; h < 3; h++)
		for (int w = 0; w < 3; w++)
			customH.at<float>(h, w) = V.at<float>(h * 3 + w, 8);

	float lastValue = customH.at<float>(2, 2);
	for (int h = 0; h < 3; h++)
		for (int w = 0; w < 3; w++)
			customH.at<float>(h, w) /= lastValue;

	//H = inv(Tp) * customH * T (denormalization�ϴ� ����)
	Mat invertTp, Htemp, resultH;
	invert(Tp, invertTp);
	multiply(invertTp, customH, Htemp);
	multiply(Htemp, T, resultH);
	
	//float lastValue = resultH.at<float>(2, 2);
	//for (int h = 0; h < 3; h++)
	//	for (int w = 0; w < 3; w++)
	//		resultH.at<float>(h, w) /= lastValue;

	for (int h = 0; h < 3; h++)
	{
		for (int w = 0; w < 3; w++)
			cout << resultH.at<float>(h, w) << " ";
		cout << endl;
	}

	return resultH;
}

//#2
Mat_<Vec3f> LaplacianBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r, const Mat_<float>& m) {
	LaplacianBlending lb(l, r, m, 4);//���� �̹���, ������ �̹���, ����ũ, level=4 ������ �Է� #3
	return lb.blend();
}

void warpPerspectivePadded(
	const Mat& src, const Mat& dst, const Mat& M, // input matrices
	Mat& src_warped, Mat& dst_padded,
	int flags, int borderMode, const Scalar& borderValue) // OpenCV params
{

	Mat transf = M / M.at<double>(2, 2); // ensure a legal homography
	if (flags == WARP_INVERSE_MAP ||
		flags == INTER_LINEAR + WARP_INVERSE_MAP ||
		flags == INTER_NEAREST + WARP_INVERSE_MAP)
	{
		invert(transf, transf);
		flags -= WARP_INVERSE_MAP;
	}

	// it is enough to find where the corners of the image go to find
	// the padding bounds; points in clockwise order from origin
	int src_h = src.rows;
	int src_w = src.cols;
	Vec<Point2f, 4> init_pts, transf_pts;
	init_pts[0] = Point2f(0, 0);
	init_pts[1] = Point2f(src_w, 0);
	init_pts[2] = Point2f(src_w, src_h);
	init_pts[3] = Point2f(0, src_h);
	perspectiveTransform(init_pts, transf_pts, transf);

	// find min and max points
	int min_x, min_y, max_x, max_y;
	min_x = floor(min(
		min(transf_pts[0].x, transf_pts[1].x),
		min(transf_pts[2].x, transf_pts[3].x)));
	min_y = floor(min(
		min(transf_pts[0].y, transf_pts[1].y),
		min(transf_pts[2].y, transf_pts[3].y)));
	max_x = ceil(max(
		max(transf_pts[0].x, transf_pts[1].x),
		max(transf_pts[2].x, transf_pts[3].x)));
	max_y = ceil(max(
		max(transf_pts[0].y, transf_pts[1].y),
		max(transf_pts[2].y, transf_pts[3].y)));

	// add translation to transformation matrix to shift to positive values
	int anchor_x, anchor_y = 0;
	Mat transl_transf = Mat::eye(3, 3, CV_64F);
	if (min_x < 0)
	{
		anchor_x = -min_x;
		transl_transf.at<double>(0, 2) += anchor_x;
	}
	if (min_y < 0)
	{
		anchor_y = -min_y;
		transl_transf.at<double>(1, 2) += anchor_y;
	}

	transf = transl_transf * transf;
	transf /= transf.at<double>(2, 2);

	// create padded destination image
	int dst_h = dst.rows;
	int dst_w = dst.cols;
	int pad_top = anchor_y;
	int pad_bot = max(max_y, dst_h) - dst_h;
	int pad_left = anchor_x;
	int pad_right = max(max_x, dst_w) - dst_w;
	copyMakeBorder(dst, dst_padded, pad_top, pad_bot, pad_left, pad_right,
		borderMode, borderValue);

	// transform src into larger window
	int dst_pad_h = dst_padded.rows;
	int dst_pad_w = dst_padded.cols;
	warpPerspective(src, src_warped, transf, Size(dst_pad_w * 2, dst_pad_h),
		flags, borderMode, borderValue);
}


int main() 
{
	//video �ҷ�����
	VideoCapture video1("left.mp4");
	VideoCapture video2("right.mp4");

	if (!video1.isOpened())
	{
		cout << "video1 is not opened" << endl;
		return 0;
	}
	if (!video2.isOpened()) 
	{
		cout << "video2 is not opened" << endl;
		return 0;
	}

	int v_width = video1.get(cv::CAP_PROP_FRAME_WIDTH);
	int v_height = video1.get(cv::CAP_PROP_FRAME_HEIGHT);
	int frames_count1 = video1.get(cv::CAP_PROP_FRAME_COUNT);
	int frames_count2 = video2.get(cv::CAP_PROP_FRAME_COUNT);

	cout << "VIDEO WIDTH: " << v_width << endl;
	cout << "VIDEO HEIGTH: " << v_height << endl;
	cout << "VIDEO1/VIDEO2 FRAME COUNT: " << frames_count1 << "/" << frames_count2 << endl;

	//stitching�� ����� �����ϴ� videoWriter
	VideoWriter stitched_writer;
	stitched_writer.open("resultVideo.avi", CV_FOURCC('D', 'I', 'V', 'X'), 24, Size(1250, 480), true);
	
	if (!stitched_writer.isOpened())
	{
		cout << "writer is not opened" << endl;
		return 0;
	}

	vector<KeyPoint> keypoints1, keypoints2;
	vector<DMatch> bf_matches;

	Mat descriptors1, descriptors2;

	SURFDetector surf;
	SURFMatcher<BFMatcher> bf_matcher;
	
	Mat staticHomography(3, 3, CV_64F); // ���� ȣ��׷���, blending �Լ��� 64F ������ �������� ȣ��׷��� ���� 
	double data3[] = { 1,0,0,0,1,0,0,0,1 }; // ���� �ÿ� ���� �̹����� �׵���ĺ�ȯ�� �Ͽ� �״�� �ӹ��� ������ �ǵ��� ���Ѵ�.
	Mat I(3, 3, CV_64F, data3);

	//������ ù frames���� ������ homography�� ����
	Mat frame1, frame2;
	video1 >> frame1;
	video2 >> frame2;

	Mat frame1_gray, frame2_gray;
	cvtColor(frame1, frame1_gray, CV_BGR2GRAY);
	cvtColor(frame2, frame2_gray, CV_BGR2GRAY);

	//start stitching
	for (int i = 0; i < LOOP_NUM; i++)
	{
		//surf�� feature detect and descriptor ����
		surf(frame1_gray, Mat(), keypoints1, descriptors1);
		surf(frame2_gray, Mat(), keypoints2, descriptors2);

		//feature�� matching
		bf_matcher.match(descriptors1, descriptors2, bf_matches);
	}

	//matching�Ǵ� feature���� �Ѵ��� ���� �Լ�
	vector<DMatch> good_matches;
	vector<Point2f> img_1, img_2; //good features
	vector<Point2f> corner;

	getGoodMatches(bf_matches, good_matches, keypoints1, keypoints2, img_1, img_2);
	
	//opencv �����Լ�
	Mat globalH = findHomography(img_2, img_1, CV_RANSAC);
	
	cout << "global homography" << endl;
	for (int h = 0; h < 3; h++)
	{
		for (int w = 0; w < 3; w++)
			cout << globalH.at<double>(h, w) << " ";
		cout << endl;
	}
	cout << endl;

	//basic DLT algorithm�� ���
	Mat basicH = getHomography_basicDLT(img_2, img_1);
	
	//normalized DLT algorithm�� ���
	Mat normH = getHomography_normalizedDLT(img_2, img_1);
	
	staticHomography = basicH;

	int cnt = 0; //����ǰ� �ִ� frame ���� Ȯ��
	while (true)
	{
		UMat imgL, imgR; // �÷� �̹����� ��ȯ�� ���� �ʴ´�.
		video1 >> imgL;
		video2 >> imgR;

		if (imgL.empty() || imgR.empty())
			break;

		++cnt;
		cout << "=================" << cnt << "=================" << endl;

		//�̹����� ����ũ�� warping, ����ũ�� ���� ���� �� ��濡�� �ʿ��ϴ�.
		Mat image1Updated, image2Updated;

		warpPerspective(imgL, image1Updated, I, Size(imgR.cols * 2, imgR.rows * 1));
		warpPerspective(imgR, image2Updated, staticHomography, Size(imgR.cols * 2, imgR.rows * 1));

		Mat image1uc(image1Updated.rows, image1Updated.cols, CV_32F);
		Mat image2uc(image2Updated.rows, image2Updated.cols, CV_32F);
		image1uc = image1Updated;
		image2uc = image2Updated;

		Mat_<Vec3f> l;
		image1uc.convertTo(l, CV_32F, 1.0 / 255.0);// 1/255 ���Ͽ� normalize 
		Mat_<Vec3f> r;
		image2uc.convertTo(r, CV_32F, 1.0 / 255.0);

		Mat_<float> m(image1uc.rows, image2uc.cols, 0.0); // �̹����� ������ ũ���� ����ũ ����
		m(Range::all(), Range(0, m.cols / 2.1)) = 0.99;//��� ����ũ

		Mat_<Vec3f> blend = LaplacianBlend(l, r, m);//LaplacianBlend�Ͽ� blend ����� #2
		Mat crop = blend(Rect(0, 0, 1250, 480));// (0,0)�� (1200,36)�� ��ǥ�� �밢���� ���� �簢������ ũ��

		/*---------- Mat ����----------------- */
		//blend : �� ������ ���� �� �̹���
		//crop : blend�� ũ���� �̹���
		//blend_8UC3, crop_8UC3 : ���� �� �ڷ����� mat �̹����� ���� videowrite ������� ������ ������ ���� �ʾҽ��ϴ�. �׷��� ������ ���� ���� ������ ������ ��
		// imwrite�ÿ� �ʿ��� �ڷ������� blend�� crop�� �����Ͽ����ϴ�.

		Mat3b crop_8UC3;
		crop.convertTo(crop_8UC3, CV_8UC3, 255); // crop�� 8UC3 �ڷ������� ��ȯ��Ű�� �Լ�

		stitched_writer.write(crop_8UC3);
	}

	return 0;
}