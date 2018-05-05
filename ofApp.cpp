#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofBackground(39);
	ofSetWindowTitle("Insta");

	ofSetCircleResolution(36);
	this->fbo.allocate(ofGetWidth(), ofGetHeight() / 2);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);

	// Draw Sun To Fbo
	ofPushMatrix();
	ofTranslate(this->fbo.getWidth() / 3, this->fbo.getHeight() / 5 * 2);

	ofSetColor(239, 39, 39);
	ofDrawCircle(ofPoint(), 130);

	ofPopMatrix();

	// Draw Firefry To Fbo
	ofSetColor(239, 239, 39);
	for (int i = 0; i < 15; i++) {

		int x = ofRandom(ofGetWidth());
		int y = (int)(ofRandom(this->fbo.getHeight()) + ofGetFrameNum()) % (int)this->fbo.getHeight();

		ofDrawCircle(x, y, 2);
	}

	// Draw Tree To Fbo
	ofSetColor(239);
	for (int i = 0; i < 60; i++) {

		int x = ofRandom(this->fbo.getWidth());
		this->draw_tree(ofPoint(x, this->fbo.getHeight()), -90, ofRandom(30, 70), 8);
	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	// Draw Fbo 
	ofSetColor(255, 255);
	this->fbo.draw(-ofGetWidth() / 2, -ofGetHeight() / 2);
	
	// Draw Shadow
	ofPixels pixels;
	this->fbo.readToPixels(pixels);

	for (int y = 0; y < pixels.getHeight(); y++) {

		int noise_x = ofMap(ofNoise(y * 0.05 + ofGetFrameNum() * 0.05), 0, 1, -20, 20);
		noise_x = noise_x * ofMap(y, 0, pixels.getHeight(), 1, 0.1);

		if (noise_x > 0) {

			for (int x = pixels.getWidth() - 1; x > noise_x; x--) {

				pixels.setColor(x, y, pixels.getColor(x - noise_x, y));
			}
			for (int x = 0; x < noise_x; x++) {

				pixels.setColor(x, y, ofColor(39));
			}
		}
		else if(noise_x < 0) {

			for (int x = 0; x < pixels.getWidth() + noise_x; x++) {

				pixels.setColor(x, y, pixels.getColor(x - noise_x, y));
			}
			for (int x = pixels.getWidth() + noise_x; x < pixels.getWidth(); x++) {

				pixels.setColor(x, y, ofColor(39));
			}
		}

	}
	ofImage image;
	image.setFromPixels(pixels);

	ofRotate(180);
	ofRotateY(180);
	ofSetColor(255, 64);
	image.draw(-ofGetWidth() / 2, -ofGetHeight() / 2);

}

//--------------------------------------------------------------
void ofApp::draw_tree(ofPoint point, float deg, int len, int depth){

	ofSetLineWidth(0.1 + depth / 8);
	ofPoint target(len * cos(deg * DEG_TO_RAD), len * sin(deg * DEG_TO_RAD));
	ofDrawLine(point, point + target);

	if (depth > 1) {

		this->draw_tree(point + target, deg - ofRandom(-5, 15), len * 0.4, depth - 1);
		this->draw_tree(point + target, deg + ofRandom(-5, 15), len * 0.8, depth - 1);
	}

}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}