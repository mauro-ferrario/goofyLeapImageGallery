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
  isMoving  = false;
  prevSingleHandDetected = false;
  actualImageCount = 0;
  swipeFree = true;
  timeToWait = 500;
  notActiveScale = 1.1;
  activeScale = 1;
  actualScale = notActiveScale;
  setupBlurShader();
}


void GoofyLeapImageGallery::setupBlurShader()
{
  shaderBlurX.load("blur/shadersGL2/shaderBlurX");
  shaderBlurY.load("blur/shadersGL2/shaderBlurY");
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
  if(!prevSingleHandDetected&&singleHeadDetected)
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
  if(actualImageCount < 0)
    actualImageCount = urlImages.size()-1;
  loadNewImage();
  move("right", speed);
}

void GoofyLeapImageGallery::movePrev(float speed)
{
  actualImageCount++;
  if(actualImageCount > urlImages.size() -1)
    actualImageCount = 0;
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
  setupBlurShader();
  fboBlurOnePass.allocate(ofGetWindowWidth(), ofGetWindowHeight());
  fboBlurTwoPass.allocate(ofGetWindowWidth(), ofGetWindowHeight());
  fboBlurOnePass.begin();
  ofClear(0);
  fboBlurOnePass.end();
  fboBlurTwoPass.begin();
  ofClear(0);
  fboBlurTwoPass.end();
  loadFirtImage();
}

void  GoofyLeapImageGallery::draw()
{
  drawForBlur();
  if(singleHeadDetected)
    actualScale -= .01;
  else
    actualScale += .01;
  actualScale = ofClamp(actualScale, activeScale, notActiveScale);
  ofPushMatrix();
  ofTranslate(ofGetWindowWidth()*.5, ofGetWindowHeight()*.5);
  ofScale(actualScale,actualScale);
  ofPushMatrix();
  ofTranslate(-ofGetWindowWidth()*.5, -ofGetWindowHeight()*.5);
  ofSetColor(ofMap(actualScale, activeScale, notActiveScale,255,100));
  fboBlurTwoPass.draw(0, 0);
  ofPopMatrix();
  ofPopMatrix();
  ofSetColor(255);
  ofDrawBitmapString(ofToString(actualImageCount+1) + "/" + ofToString(urlImages.size()), ofVec2f(20,20));
}

void GoofyLeapImageGallery::drawForBlur()
{
  float blur = ofMap(actualScale, activeScale, notActiveScale, 0, 4);
  fboBlurOnePass.begin();
  shaderBlurX.begin();
  ofClear(0);
  shaderBlurX.setUniform1f("blurAmnt", blur);
  if(actualImage)
    drawMainImage();
  if(newImage)
    drawNewImage();
  shaderBlurX.end();
  fboBlurOnePass.end();
  fboBlurTwoPass.begin();
  ofClear(0);
  shaderBlurY.begin();
  shaderBlurY.setUniform1f("blurAmnt", blur);
  fboBlurOnePass.draw(0, 0);
  shaderBlurY.end();
  fboBlurTwoPass.end();
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