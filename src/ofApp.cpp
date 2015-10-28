#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofSetLogLevel(OF_LOG_WARNING);
  gallery.setup();
  //ofSetWindowPosition(3000, 0);
  ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::update(){
  gallery.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackgroundGradient(ofColor(44), ofColor(0));
  gallery.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  if(key == OF_KEY_COMMAND)
  {
    
  }

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
  if( dragInfo.files.size() > 0 )
  {
    ofDirectory dir;
    dir.listDir(dragInfo.files[0]);
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < (int)dir.size(); i++){
      cout << dir.getPath(i) << endl;
      gallery.addNewImage(dir.getPath(i));
    }
    gallery.start();
  }
}
