#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/stitching/detail/blenders.hpp>

using namespace std;
using namespace cv;


class LaplacianBlending {
private://�����
    Mat_<Vec3f> left;
    Mat_<Vec3f> right;
    Mat_<float> blendMask;

    vector<Mat_<Vec3f> > leftLapPyr, rightLapPyr, resultLapPyr;
    Mat leftSmallestLevel, rightSmallestLevel, resultSmallestLevel;
    vector<Mat_<Vec3f> > maskGaussianPyramid; //"masks" are 3-channels for easier multiplication with RGB

    int levels;
//#4
    void buildPyramids() {//left,right �̹����� ���� ���ö�þ� �Ƕ�̵� ����
        buildLaplacianPyramid(left, leftLapPyr, leftSmallestLevel);//#4-1
        buildLaplacianPyramid(right, rightLapPyr, rightSmallestLevel);
        buildGaussianPyramid();//#4-2
    }

    void buildGaussianPyramid() {//����ũ�� ����þ� �Ƕ�̵� �����ϱ�

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
    void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& smallestLevel) {
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

    Mat_<Vec3f> reconstructImgFromLapPyramid() {//#6
        Mat currentImg = resultSmallestLevel;
        for (int l = levels - 1; l >= 0; l--) {
            Mat up;

            pyrUp(currentImg, up, resultLapPyr[l].size());
            currentImg = up + resultLapPyr[l];
        }
        return currentImg;
    }

//#5
    void blendLapPyrs() {
        resultSmallestLevel = leftSmallestLevel.mul(maskGaussianPyramid.back()) +
            rightSmallestLevel.mul(Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid .back());
        for (int l = 0; l < levels; l++) {
            Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
            Mat antiMask = Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid[l];
            Mat B = rightLapPyr[l].mul(antiMask);
            Mat_<Vec3f> blendedLevel = A + B;

            resultLapPyr.push_back(blendedLevel);
        }
    }

//#3
public:
    LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, const Mat_<float>& _blendMask, int _levels) :
        left(_left), right(_right), blendMask(_blendMask), levels(_levels)
    {
        assert(_left.size() == _right.size());// �� �̹��� �� ����ũ�� ������ �����ϴٴ� ����
        assert(_left.size() == _blendMask.size());
        buildPyramids();//#4
        blendLapPyrs();//#5
    };

    Mat_<Vec3f> blend() {//#6
        return reconstructImgFromLapPyramid();
    }
};

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
    warpPerspective(src, src_warped, transf, Size(dst_pad_w*2, dst_pad_h),
        flags, borderMode, borderValue);
}

int main() {


	Mat imgL = imread("streetL.png", IMREAD_GRAYSCALE);// �� �̹����� ���� Mat ���·� �����ϱ�
	Mat imgR = imread("streetR.png", IMREAD_GRAYSCALE);

	Size size(imgL.cols / 2, imgL.rows / 2);
	resize(imgL, imgL, size);
	resize(imgR, imgR, size);

	Ptr<DescriptorMatcher> matcher;//Ư¡�� descriptor ��Ī�� ���� DescriptorMatcher Ŭ������ ������ ����

	matcher = DescriptorMatcher::create("FlannBased"); // FlannBased ��Ī �˰������� ����� matcher  

	Ptr<SIFT> ext = SIFT::create();//SIFT Ŭ������ Ȱ���Ͽ� ������ Ư¡�� ������ ���� ext
	vector<KeyPoint> kp1, kp2;//img1,img2�� Ư¡�� 
	Mat desc1, desc2;//kp1,kp2�� descriptor�� ��� Mat ����

	ext->detectAndCompute(imgL, Mat(), kp1, desc1);//img1�� ���� Ư¡����(kp1), descriptor(desc1) ���
	ext->detectAndCompute(imgR, Mat(), kp2, desc2);//img2�� ���� Ư¡����, descriptor ���

	//��Ī �� �׸���
	vector<vector<DMatch> > rawMatches;// DMatch Ŭ������ ���� rawMatches 
	vector<Point2f> p1, p2,p3,p4;//img1,2�� knn �˰������ ratio test ���� ��Ī���� Ư¡����
	vector<float> distances;//Ư¡�� ���� ���� �Ÿ� 
	matcher->knnMatch(desc1, desc2, rawMatches, 2);//k==2���� Ư¡�� ��Ī�� ���Ͽ� matcher(FLANN based �˰����� Ȱ��)�� ����

	//filter_matches
	for (size_t i = 0; i < rawMatches.size(); i++) {//knn�� ���� ������ match �ֵ鿡 ���� matching�� ���� ��ŭ loop
		const vector<DMatch>& m = rawMatches[i];//�� ��Ī(rawmatches[i]�� ���� DMatch Ŭ������ ������ ���� ����
		if (m.size() == 2 && m[0].distance < m[1].distance * 0.7)// k==2���� ��Ī�ֿ� ���� ratio(=0.75) test�� ����� ��ġ�鿡 ���� Ư¡�� �� distance �� push_back
		{
			p1.push_back(kp1[m[0].queryIdx].pt);//��Ī�� ���� img1 keypoints
			p2.push_back(kp2[m[0].trainIdx].pt);//��Ī�� ���� img2 keypoints
			distances.push_back(m[0].distance);//��Ī�� ���� distance
		}
	}

	vector<uchar> status;
	vector<pair<Point2f, Point2f> > pointPairs;//Ư¡�� ��

    double data3[] = { 1,0,0,0,1,0,0,0,1 };
   
    Mat I(3, 3, CV_64F, data3);


    Mat H = findHomography(p1, p2, status, RANSAC);// Ư¡�� �ְ� RANSAC�˰������� ����Ͽ� �̹������� ��ȯ��� H ����
    Mat H2 = findHomography(p2, p1, status, RANSAC);
    Mat& h = H;
    Mat& h2 = H2;

    Mat dst,dst2; 
    Mat src,src2;
   // warpPerspectivePadded(imgR,imgL, h ,src,dst, 1,0,0);


    //Mask of iamge to be combined so you can get resulting mask
    Mat mask1(imgL.size(), CV_8UC1, Scalar::all(255));
    Mat mask2 (imgR.size(), CV_8UC1, Scalar::all(0));
    Mat image1Updated, image2Updated;
    Mat test1 = imread("streetL.png");
    Mat test2 = imread("streetR.png");


    Size size2(test1.cols / 2, test2.rows / 2);
    resize(test1, test1, size2);
    resize(test2, test2, size2);

    warpPerspective(test1, image1Updated, I, Size(imgR.cols * 2, imgR.rows * 1));
    warpPerspective(test2, image2Updated, H2, Size(imgR.cols * 2, imgR.rows * 1));
    warpPerspective(mask1, mask1, I, Size(imgR.cols * 2, imgR.rows*1), INTER_LINEAR, BORDER_TRANSPARENT, 0);
    warpPerspective(mask2, mask2,H2, Size(imgR.cols * 2, imgR.rows*1), INTER_LINEAR, BORDER_CONSTANT, 0);


    Mat image1uc_(image1Updated.rows, image1Updated.cols, CV_16SC3);
    Mat image2uc_(image2Updated.rows, image2Updated.cols, CV_16SC3);
  
    Mat image1uc_2(image1Updated.rows, image1Updated.cols, CV_32F);
    Mat image2uc_2(image2Updated.rows, image2Updated.cols, CV_32F);
    image1uc_2 = image1Updated;
    image2uc_2 = image2Updated;

    Mat_<Vec3f> l;
    image1uc_2.convertTo(l, CV_32F, 1.0 / 255.0);// 1/255 ���Ͽ� normalize (?)
    Mat_<Vec3f> r;
    image2uc_2.convertTo(r, CV_32F, 1.0 / 255.0);
    
    Mat_<float> m(image1uc_2.rows, image2uc_2.cols, 0.0); // �̹����� ������ ũ���� ����ũ ����
    m(Range::all(), Range(0, m.cols / 2.25)) = 0.99 ;//��� ����ũ (������ ����)

    Mat_<Vec3f> blend = LaplacianBlend(l,r,m);//LaplacianBlend�Ͽ� blend ����� #2

    imshow("ResultBeforeCrop", blend); // ����� �����ش�

        // ��ģ �Ŀ� ������ ������ �κ��� ����� ���� �ڵ�
   // vector<Point> nonBlackList;
   // nonBlackList.reserve(blend.rows * blend.cols);

    int cols_n=0;

    
        for (int i = 0; i < blend.cols; ++i)
        {
            if (blend.at<Vec3f>(image1Updated.rows-1, i) == Vec3f(0, 0, 0))
            {
                //printf("hi");
                cols_n = i+1;
            }

        }

    cv::Mat crop = blend(cv::Rect(0, 0, cols_n-1-300, image1Updated.rows-1)); // �� ��ǥ ���ϴ� �κ� �ٽ� �ؾ��մϴ�
 
    imshow("Result", crop); // ����� �����ش�
    waitKey(0);
    return 0;

    //imshow("mask", m);
 
}