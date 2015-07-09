#include "OpenCVHelpers.hpp"

typedef std::pair<int, int> _pT;
#define _pV(v1, v2) std::pair<int,int>(v1, v2)  

inline _pT _pSize(cv::Mat & m) {
	return _pV(m.cols, m.rows);
}

inline _pT _pScale(_pT x, float f ) {
	return _pV(x.first * f, x.second * f);
}

inline _pT _pSub(_pT p1, _pT p2) {
	return _pV(p1.first - p2.first, p1.second - p2.second);
}

std::pair<cv::Rect, cv::Size> pad(cv::Mat & m, float padding) {
	auto paddedSize = _pScale(_pSize(m), (1.0f - (padding)));
	auto borderSize = _pSub(_pSize(m),paddedSize);
	auto orig       = _pV(borderSize.first/2, borderSize.second/2);
	auto rect       = cv::Rect(orig.first, orig.second, paddedSize.first, paddedSize.second);
	auto rectSize   = cv::Size(paddedSize.first, paddedSize.second);
	return std::pair<cv::Rect, cv::Size>(rect, rectSize);
}

void resizeKeepAspectRatio(cv::Mat & in, cv::Mat & out) {
	float wWidth = out.cols;
	float wHeight = out.rows;
	debugMat(out);

	float iw = in.cols;
	float ih = in.rows;
	if(iw > wWidth) {
		float ratio = wWidth/ iw;
		iw = iw * ratio;
		ih = ih * ratio;
	}
	if(ih > wHeight) {
		float ratio = wHeight/ ih;
		iw = iw * ratio;
		ih = ih * ratio;
	}
	int iiw = (int) iw;
	int iih = (int) ih;
	auto deltaW = std::max(0,(out.cols - iiw) >> 1);
	auto deltaH = std::max(0,(out.rows - iih) >> 1);
	cv::Mat dst1;
	cv::Mat dst2;
	resize(in, dst1, cvSize(iw, ih), 0, 0, cv::INTER_CUBIC);
	//debugShouldEqual(dst1.cols + deltaW * 2, out.cols);
	//debugShouldEqual(dst1.rows + deltaH * 2, out.rows);
	auto _hRem = out.rows - deltaH - dst1.rows;
	auto _wRem = out.cols - deltaW - dst1.cols;
	debugShouldEqual(out.rows, dst1.rows + deltaH + _hRem);
	debugShouldEqual(out.cols, dst1.cols + deltaW + _wRem);
	copyMakeBorder(dst1, out, deltaH, _hRem, deltaW, _wRem, cv::BORDER_CONSTANT, 0);
}

// Helper function to put text in the center of a rectangle
void centerText(cv::Mat& im, const std::string & label, double scale)
{
    int fontface = cv::FONT_HERSHEY_DUPLEX;
    int thickness = 1;
    int baseline = 0;
    
    auto r = cv::Rect(0, 0, im.cols, im.rows);
    
    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Point pt(r.x + (r.width-text.width)/2, r.y + (r.height+text.height)/2);
    
    cv::putText(im, label, pt, fontface, scale, CV_RGB(255,255,255), thickness, 8);
}


