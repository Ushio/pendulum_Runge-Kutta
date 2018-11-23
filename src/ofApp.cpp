#include "ofApp.h"

Pen D_solve(Pen pen, double h, double G, double l) {
	auto f = [G, l](double t, double x, double v) {
		return -G / l * sin(x);
	};
	auto g = [](double t, double x, double v) {
		return v;
	};

	// https://www.sit.ac.jp/user/konishi/JPN/L_Support/SupportPDF/Runge-KuttaMethod.pdf
	// dv/dt = f(t, x, v) = -g / l * sin(x)
	// dx/dt = g(t, x, v) = v
	double k1_v = h * f(0.0, pen.x_theta, pen.v_theta);
	double k1_x = h * g(0.0, pen.x_theta, pen.v_theta);

	double k2_v = h * f(0.0, pen.x_theta + k1_x * 0.5, pen.v_theta + k1_v * 0.5);
	double k2_x = h * g(0.0, pen.x_theta + k1_x * 0.5, pen.v_theta + k1_v * 0.5);
	double k3_v = h * f(0.0, pen.x_theta + k2_x * 0.5, pen.v_theta + k2_v * 0.5);
	double k3_x = h * g(0.0, pen.x_theta + k2_x * 0.5, pen.v_theta + k2_v * 0.5);

	double k4_v = h * f(0.0, pen.x_theta + k3_x, pen.v_theta + k3_v);
	double k4_x = h * g(0.0, pen.x_theta + k3_x, pen.v_theta + k3_v);

	pen.v_theta += (k1_v + k2_v * 2.0 + k3_v * 2.0 + k4_v) / 6.0;
	pen.x_theta += (k1_x + k2_x * 2.0 + k3_x * 2.0 + k4_x) / 6.0;

	return pen;
}

static double remap(double value, double inputMin, double inputMax, double outputMin, double outputMax)
{
	return (value - inputMin) * ((outputMax - outputMin) / (inputMax - inputMin)) + outputMin;
}

Pen initialize(double theta_x, double theta_0, double g, double l) {
	Pen pen;
	pen.x_theta = theta_x;

	double cosTheta = theta_x;
	double cosTheta_0 = std::copysign(theta_0, theta_x);
	pen.v_theta = std::sqrt(std::max(2.0 * (g / l) * (std::cos(cosTheta) - std::cos(cosTheta_0)), 0.0));
	return pen;
}

//--------------------------------------------------------------
void ofApp::setup() {
	_camera.setNearClip(0.1f);
	_camera.setFarClip(100.0f);
	_camera.setDistance(5.0f);

	//Pen p;
	//p.x_theta = glm::radians(160.0);
	//p.v_theta = 0.0;
	//_pens.push_back(p);

	int N = 20;
	for (int i = 0; i < N; ++i) {
		double theta = remap(i, 0, N - 1, glm::radians(-140.0), glm::radians(140.0));
		// printf("%.2f\n", glm::degrees(theta));
		Pen p = initialize(theta, glm::radians(160.0), 9.8, 1.0);
		_pens.push_back(p);
	}
}
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();

	ofClear(0);

	_camera.begin();
	ofPushMatrix();
	ofRotateZDeg(90.0f);
	ofSetColor(64);
	ofDrawGridPlane(1.0f);
	ofPopMatrix();

	ofDrawAxis(50);

	ofSetColor(255);

	for (int i = 0; i < _pens.size(); ++i) {
		_pens[i] = D_solve(_pens[i], 1.0 / 60.0, 9.8, 1);
	}
	for (int i = 0; i < _pens.size(); ++i) {
		float z = remap(i, 0, _pens.size() - 1, -1.2, 1.2);
		//float z = 0.0;
		
		float theta = _pens[i].x_theta;
		glm::vec3 d = glm::vec3(sin(theta), -cos(theta), z);
		ofDrawLine(glm::vec3(0.0f, 0.0f, z), d);
		ofDrawSphere(d, 0.02f);
	}

	_camera.end();

	ofDisableDepthTest();
	ofSetColor(255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
