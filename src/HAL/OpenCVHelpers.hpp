#include <algorithm>
#include <utility>
#include <opencv2/opencv.hpp>
#include "debug.hxx"
#include "cppformat/format.h"


#define lambda(m) [&](Mat & m) -> void
#define no(op)    ([&](Mat & op) -> void {})

#define white  CV_RGB(255,255,255)
#define black  CV_RGB(0,0,0)

#define debugMat(name) debugm(fmt::format("{} ({}x{})", #name, name.cols, name.rows))
#define debugVal(name) debugm(fmt::format("temporary ({})", name))
#define debugShouldEqual(v1,v2) { debugm(fmt::format("{} ~ {}", v1, v2)) ; assert(v1 == v2); }

extern std::pair<cv::Rect, cv::Size> pad(cv::Mat & m, float padding);

template<typename Func>
void overlayOn(cv::Mat & out, float opacity, Func lam) {
	cv::Mat in;
	out.copyTo(in);
	lam(in);
	addWeighted(in, opacity, out, 1.0 - opacity, 0.0, out);
}

template<typename Func1, typename Func2>
void splitV(cv::Mat & in, float alpha, Func1 lam1, Func2 lam2) {
	float ih = (int) in.rows;
	int ih1 = (int) (in.rows * alpha);
	debugMat(in);
	auto rng1 = in.rowRange(0, ih1);
	auto rng2 = in.rowRange(ih1, ih);
	debugMat(rng1);
	debugMat(rng2);
	lam1(rng1);
	lam2(rng2);
}

template<typename Func>
void padCanvas(cv::Mat & m, float padding, Func lam1) {
	auto r = pad(m, padding).first;
	auto rcol = cv::Range(r.x, r.x + r.width);
	auto rrow = cv::Range(r.y, r.y + r.height);
	auto submatrix = m(rrow, rcol);
	lam1(submatrix);
}

template<typename Func1, typename Func2>
void splitH(cv::Mat & in, float alpha, Func1 lam1, Func2 lam2) {
    float ih = (int) in.cols;
    int ih1 = (int) (in.cols * alpha);
    debugMat(in);
    auto rng1 = in.colRange(0, ih1);
    auto rng2 = in.colRange(ih1, ih);
    debugMat(rng1);
    debugMat(rng2);
    lam1(rng1);
    lam2(rng2);
}

extern void resizeKeepAspectRatio(cv::Mat & in, cv::Mat & out);
extern void centerText(cv::Mat& im, const std::string & label, double scale=0.9);
