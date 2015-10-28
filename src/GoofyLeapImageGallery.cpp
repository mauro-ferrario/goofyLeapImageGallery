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
  swipeFree = false;
  isMoving  = false;
  prevSingleHandDetected = false;
  actualImageCount = 0;
  swipeFree = true;
  timeToWait = 1000;
}

void  GoofyLeapImageGallery::setup()
{
  leap.open();
  leap.setupGestures();
}

void  GoofyLeapImageGallery::loadFirtImage()
{
  actualImageCount = 0;
  SingleImagePage tempImage;
  tempImage.setup(urlImages[actualImageCount], ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()));
  actualImage = new SingleImagePage();
  *actualImage = tempImage;
}

void  GoofyLeapImageGallery::update()
{
  leap.updateGestures();
  singleHeadDetected = (leap.getSimpleHands().size() == 1) ? true : false;
  if(!prevSingleHandDetected)
  {
    swipeFree = false;
  }
  
  if (singleHeadDetected)
  {
    if(!isMoving)
      detectMovement();
  }
  if(isMoving)
  {
    if(tweenMainImage.isCompleted())
    {
      isMoving = false;
      switchImage();
      timerStartPosition = timer.getAppTimeMillis();
    }
  }
  if(!swipeFree&&!isMoving)
  {
    if(timer.getAppTimeMillis() > timerStartPosition + timeToWait)
    {
      cout << ofGetTimestampString() << "LIBERO" << endl;
      swipeFree = true;
    }
  }
  prevSingleHandDetected = singleHeadDetected;
  leap.markFrameAsOld();
}

void GoofyLeapImageGallery::detectMovement()
{
  ofPoint handPos = leap.getSimpleHands()[0].fingers[MIDDLE].tip;
  float diff = handPos.x - prevHandPos.x;
  ofVec2f swipeRange(40,100);
  prevHandPos = handPos;
  if(swipeFree)
  {
    if(diff < -swipeRange.x && diff > -swipeRange.y)
      movePrev();
    if(diff > swipeRange.x && diff < swipeRange.y)
      moveNext();
  }
}

void GoofyLeapImageGallery::moveNext(float speed)
{
  actualImageCount--;
  if(actualImageCount > urlImages.size()-1)
    actualImageCount = 0;
  loadNewImage();
  move("right", speed);
}

void GoofyLeapImageGallery::movePrev(float speed)
{
  actualImageCount++;
  if(actualImageCount < 0)
    actualImageCount = urlImages.size()-1;
  loadNewImage();
  move("left", speed);
}

void GoofyLeapImageGallery::move(string direction, float speed)
{
  float startPosMain  = 0;
  float endPosMain    = 0;
  float startPosNew   = 0;
  float endPosNew     = 0;
  if(direction == "left")
  {
    startPosMain = 0;
    endPosMain = -ofGetWindowWidth();
    startPosNew = ofGetWindowWidth();
    endPosNew = 0;
  }
  else
  {
    startPosMain = 0;
    endPosMain = ofGetWindowWidth();
    startPosNew = -ofGetWindowWidth();
    endPosNew = 0;
  }
  tweenMainImage.setParameters(0,easingMainImage,ofxTween::easeOut,startPosMain,endPosMain,speed,0);
  tweenNewImage.setParameters(0,easingNewmage,ofxTween::easeOut,startPosNew, endPosNew,speed,0);
  isMoving = true;
  swipeFree = false;
}

void GoofyLeapImageGallery::loadNewImage()
{
  newImage = new SingleImagePage();
  newImage->setup(urlImages[actualImageCount], ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()));
}

void GoofyLeapImageGallery::switchImage()
{
  delete actualImage;
  actualImage = NULL;
  actualImage = newImage;
  newImage = NULL;
}

void GoofyLeapImageGallery::start()
{
  loadFirtImage();
}

void  GoofyLeapImageGallery::draw()
{
  if(actualImage)
    drawMainImage();
  if(newImage)
    drawNewImage();
  ofDrawBitmapString(ofToString(actualImageCount+1) + "/" + ofToString(urlImages.size()), ofVec2f(20,20));
  if(singleHeadDetected)
  {
    ofSetColor(255);
    ofCircle(100,100,10);
  }
}

void GoofyLeapImageGallery::drawMainImage()
{
  ofPushMatrix();
  if(isMoving)
    ofTranslate(tweenMainImage.update(), 0);
  actualImage->draw();
  ofPopMatrix();
}

void GoofyLeapImageGallery::drawNewImage()
{
  ofPushMatrix();
  ofTranslate(tweenNewImage.update(), 0);
  newImage->draw();
  ofPopMatrix();
}

void GoofyLeapImageGallery::addNewImage(string newUrl)
{
  urlImages.push_back(newUrl);
}