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
  ofSetFrameRate(60);
  isMoving                = false;
  prevSingleHandDetected  = false;
  actualImageCount        = 0;
  swipeFree               = true;
  swipeRange              = ofVec2f(60,100);
  transitionDuration      = 1000;
  maxOffsetXHandOutside   = 130;
  counterFrameToWait       = 0;
  cam.setOrientation(ofPoint(-20, 0, 0));
  
  GoofyOSCController::addMapValue("Transition Duration", transitionDuration, 10000);
  GoofyOSCController::addMapValue("Swipe range min", swipeRange.x, 200);
  GoofyOSCController::addMapValue("Swipe range max", swipeRange.y, 200);
  GoofyOSCController::addMapValue("Swipe range max", maxOffsetXHandOutside, 500);
  
}

void GoofyLeapImageGallery::setup()
{
  leap.open();
  leap.setupGestures();
}

bool GoofyLeapImageGallery::getSingleHandDetected()
{
  return (leap.getSimpleHands().size() == 1) ? true : false;
}

void  GoofyLeapImageGallery::update()
{
  singleHeadDetected = (leap.getSimpleHands().size() == 1) ? true : false;
  if(!prevSingleHandDetected&&singleHeadDetected)
  {
    handStartPos = leap.getSimpleHands()[0].fingers[INDEX].tip;
  }
  else if(singleHeadDetected)
  {
    if(getSingleHandDetected())
      mainOffsetX = - (handStartPos.x - leap.getSimpleHands()[0].fingers[INDEX].tip.x);
    direction = (mainOffsetX > 0) ? SWIPE_RIGHT : SWIPE_LEFT;
  }
  
  if(prevSingleHandDetected&&!singleHeadDetected&&!isMoving)
  {
    handGoOut();
  }
  
  if(singleHeadDetected)
  {
    if(!isMoving)
      counterFrameToWait--;
    if(!isMoving&&counterFrameToWait <= 0)
      detectMovement();
  }

  if(isMoving)
  {
    if(tweenMainImage.isCompleted())
    {      
      tweenCompleted();
    }
  }
  updateFingers();
  prevSingleHandDetected = singleHeadDetected;
  leap.markFrameAsOld();
}

void GoofyLeapImageGallery::updateFingers()
{
  fingersFound.clear();
  simpleHands = leap.getSimpleHands();
  if( leap.isFrameNew() && simpleHands.size() ){
    
    leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
    leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
    leap.setMappingZ(-150, 150, -200, 200);
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
      for (int f=0; f<5; f++) {
        int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
        ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
        ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
        ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
        ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
        fingersFound.push_back(id);
      }
    }
  }
}

void GoofyLeapImageGallery::drawHand()
{
  cam.begin();
  fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
  for(int i = 0; i < simpleHands.size(); i++)
  {
    bool isLeft = simpleHands[i].isLeft;
    ofPoint tip = simpleHands[i].fingers[ fingerTypes[3] ].tip;  // fingertip
    ofSetColor(0, 100);
    ofDrawSphere(tip.x, tip.y, tip.z, 50);
  }
  cam.end();
}

void GoofyLeapImageGallery::handGoOut()
{
  if(direction == SWIPE_RIGHT)
  {
    if(actualImageCount > 0&&abs(mainOffsetX) > maxOffsetXHandOutside)
      moveNext(transitionDuration, &easingElastic);
    else
      move(SWIPE_STOP, transitionDuration, &easingElastic);
  }
  else
  {
    if(actualImageCount < urlImages.size() - 1&&abs(mainOffsetX) > maxOffsetXHandOutside)
      movePrev(transitionDuration, &easingElastic);
    else
      move(SWIPE_STOP, transitionDuration, &easingElastic);
  }
}

void GoofyLeapImageGallery::tweenCompleted()
{
  mainOffsetX = 0;
  isMoving    = false;
  swipeFree   = true;
  if(singleHeadDetected)
    handStartPos = leap.getSimpleHands()[0].fingers[INDEX].tip;
  switchImage();
}

void GoofyLeapImageGallery::detectMovement()
{
  if(leap.getSimpleHands()[0].fingers.size() >= 4)
  {
    // Crash
    singleHeadDetected = (leap.getSimpleHands().size() == 1) ? true : false;
    if(getSingleHandDetected())
    {
      ofPoint handPos = leap.getSimpleHands()[0].fingers[INDEX].tip;
      float diff = handPos.x - prevHandPos.x;
      prevHandPos = handPos;
      if(swipeFree)
      {
        if(diff > -swipeRange.y && diff < -swipeRange.x)
        {
          if(actualImageCount < urlImages.size() - 1)
            movePrev();
        }
        if(diff > swipeRange.x && diff < swipeRange.y)
        {
          if(actualImageCount > 0)
            moveNext();
        }
      }
    }
  }
}

void GoofyLeapImageGallery::moveNext(float speed, ofxEasing* easingType)
{
  actualImageCount--;
  if(actualImageCount < 0)
    actualImageCount = 0;
  move(SWIPE_RIGHT, speed, &easingExpo);
}

void GoofyLeapImageGallery::movePrev(float speed, ofxEasing* easingType)
{
  actualImageCount++;
  if(actualImageCount > urlImages.size() -1)
    actualImageCount = urlImages.size() - 1;
  move(SWIPE_LEFT, speed, &easingExpo);
}

void GoofyLeapImageGallery::move(galleryDirection direction, float speed, ofxEasing* easingType)
{
  easingType = NULL;
  isMoving = true;
  swipeFree = false;
  float startPosMain  = 0;
  float endPosMain    = 0;
  switch (direction) {
    case SWIPE_LEFT:
      startPosMain = mainOffsetX;
      endPosMain = -ofGetWindowWidth();
      break;
    case SWIPE_RIGHT:
      startPosMain = mainOffsetX;
      endPosMain = ofGetWindowWidth();
      break;
    default:
      startPosMain = mainOffsetX;
      endPosMain = 0;
      break;
  }
  tweenMainImage.setParameters(0,easingElastic,ofxTween::easeOut,startPosMain,endPosMain,speed,0);
}

void GoofyLeapImageGallery::switchImage()
{
  delete actualImage;
  actualImage = NULL;
  if(actualImageCount < urlImages.size())
    actualImage = loadImage(actualImageCount, actualImage);
  
  delete nextImage;
  nextImage = NULL;
  if(actualImageCount+1 < urlImages.size())
    nextImage = loadImage(actualImageCount+1, nextImage);
  
  delete prevImage;
  prevImage = NULL;
  if(actualImageCount-1 >= 0)
    prevImage = loadImage(actualImageCount-1, prevImage);
}

SingleImagePage* GoofyLeapImageGallery::loadImage(int id, SingleImagePage* img)
{
  img = new SingleImagePage();
  img->setup(urlImages[id], ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()));
  return img;
}

void GoofyLeapImageGallery::start()
{
  if(urlImages.size() > 0)
    switchImage();
}

void GoofyLeapImageGallery::draw()
{
  ofPushMatrix();
  if(!isMoving)
    ofTranslate(mainOffsetX, 0);
  else
  {
    ofTranslate(tweenMainImage.update(), 0);
  }
  if(prevImage)
    drawImage(-ofGetWindowWidth(),prevImage);
  if(actualImage)
    drawImage(0,actualImage);
  if(nextImage)
    drawImage(ofGetWindowWidth(),nextImage);
  ofPopMatrix();
  
  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  drawHand();
  
  //glDisable(GL_DEPTH_TEST);
  glDisable(GL_NORMALIZE);
  ofDrawBitmapString(ofToString(actualImageCount+1) + "/" + ofToString(urlImages.size()), ofVec2f(20,20));
}

void GoofyLeapImageGallery::drawImage(int offsetX, SingleImagePage* img)
{
  ofPushMatrix();
  ofTranslate(offsetX, 0);
  img->draw();
  ofPopMatrix();
}

void GoofyLeapImageGallery::addNewImage(string newUrl)
{
  urlImages.push_back(newUrl);
}