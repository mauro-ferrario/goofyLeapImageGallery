#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofSetLogLevel(OF_LOG_WARNING);
  gallery.setup();
  ofSetWindowPosition(3000, 0);
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
    int totFolders = dragInfo.files.size();
    struct stat sb;
    for(int a = 0; a < totFolders; a++)
    {
      int stringSize = dragInfo.files[a].size();
      string lastPart = dragInfo.files[a].substr(stringSize-5);
      if(lastPart.find('.') > 10000)
      {
        ofDirectory dir;
        dir.listDir(dragInfo.files[a]);
        dir.sort();
        for(int i = 0; i < (int)dir.size(); i++)
          gallery.addNewImage(dir.getPath(i));
      }
      else
      {
        gallery.addNewImage(dragInfo.files[a]);
      }
    }
    gallery.start();
  }
}
