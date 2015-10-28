//
//  GoofyLeapImageGallery.cpp
//  goofyLeapImageGallery
//
//  Created by Mauro Ferrario on 28/10/15.
//
//

#include "GoofyLeapImageGallery.h"

GoofyLeapImageGallery::GoofyLeapImageGallery()
{
  
}

void  GoofyLeapImageGallery::setup()
{
  leap.open();
  leap.setupGestures();
  addListeners();
  loadFirtImage();
}

void  GoofyLeapImageGallery::loadFirtImage()
{
  SingleImagePage tempImage;
  tempImage.setup("parigi.jpg", ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()));
  actualImage = new SingleImagePage();
  *actualImage = tempImage;
}

void  GoofyLeapImageGallery::update()
{
  leap.updateGestures();
  singleHeadDetected = (leap.getSimpleHands().size() == 1) ? true : false;
  if (singleHeadDetected) {
    ofPoint handPos = leap.getSimpleHands()[0].fingers[MIDDLE].tip;
    float diff = handPos.x - prevHandPos.x;
    ofVec2f swipeRange(40,100);
//    cout << diff << endl;
    prevHandPos = handPos;
    if(diff < -swipeRange.x && diff > -swipeRange.y)
      cout << ofGetTimestampString() << "SWIPE LEFT" << endl;
    
    if(diff > swipeRange.x && diff < swipeRange.y)
      cout << ofGetTimestampString() << "SWIPE RIGHT" << endl;
  }
  leap.markFrameAsOld();
}

void  GoofyLeapImageGallery::draw()
{
  if(singleHeadDetected)
  {
    ofSetColor(255);
    ofCircle(100,100,10);
  }
  actualImage->draw();
}

void  GoofyLeapImageGallery::addListeners()
{
  ofAddListener(leap.swipeEvent, this, &GoofyLeapImageGallery::gotSwipeEvent);
}

void GoofyLeapImageGallery::gotSwipeEvent(SwipeEvent& args)
{
  return;
  switch (args.idGesture) {
    case 4:
      cout << "SWIPE EVENT LEFT" << args.idGesture << endl;
      break;
    case 3:
      cout << "SWIPE EVENT RIGHT" << args.idGesture << endl;
      break;
    default:
      break;
  }
}