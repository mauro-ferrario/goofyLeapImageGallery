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
}

void  GoofyLeapImageGallery::setup()
{
  leap.open();
  leap.setupGestures();
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
  if(!prevSingleHandDetected)
  {
    
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
  if(diff < -swipeRange.x && diff > -swipeRange.y)
    movePrev();
  if(diff > swipeRange.x && diff < swipeRange.y)
    moveNext();
}

void GoofyLeapImageGallery::moveNext(float speed)
{
  move("right", speed);
}

void GoofyLeapImageGallery::movePrev(float speed)
{
  move("left", speed);
}

void GoofyLeapImageGallery::move(string direction, float speed)
{
  loadNewImage();
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
}

void GoofyLeapImageGallery::loadNewImage()
{
  newImage = new SingleImagePage();
  newImage->setup("io.jpg", ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()));
}

void GoofyLeapImageGallery::switchImage()
{
  delete actualImage;
  actualImage = NULL;
  actualImage = newImage;
  newImage = NULL;
}

void  GoofyLeapImageGallery::draw()
{
  if(singleHeadDetected)
  {
    ofSetColor(255);
    ofCircle(100,100,10);
  }
  drawMainImage();
  if(newImage)
    drawNewImage();
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