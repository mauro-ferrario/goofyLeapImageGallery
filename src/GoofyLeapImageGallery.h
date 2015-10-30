//
//  GoofyLeapImageGallery.h
//  goofyLeapImageGallery
//
//  Created by Mauro Ferrario on 28/10/15.
//
//

#ifndef __goofyLeapImageGallery__GoofyLeapImageGallery__
#define __goofyLeapImageGallery__GoofyLeapImageGallery__

#include "ofMain.h"
#include "ofxLeapMotion2.h"
#include "SingleImagePage.h"
#include "ofxTween.h"
#include "GoofyOSCControllerUtils.h"

enum galleryDirection
{
  SWIPE_STOP,
  SWIPE_LEFT,
  SWIPE_RIGHT
};

class GoofyLeapImageGallery
{
public:
                    GoofyLeapImageGallery();
  void              setup();
  void              draw();
  void              update();
  void              addNewImage(string newUrl);
  void              start();
private:
  ofxLeapMotion     leap;
  bool              singleHeadDetected;
  ofPoint           prevHandPos;
  SingleImagePage*  actualImage;
  SingleImagePage*  nextImage;
  SingleImagePage*  prevImage;
  ofxTween          tweenMainImage;
  ofxEasingElastic  easingElastic;
  ofxEasingExpo     easingExpo;
  bool              isMoving;
  bool              swipeFree;
  bool              prevSingleHandDetected;
  void              switchImage();
  void              handGoOut();
  void              move(galleryDirection direction, float speed = 1000, ofxEasing* easingType = NULL);
  void              moveNext(float speed = 1000, ofxEasing* easingType = NULL);
  void              movePrev(float speed = 1000, ofxEasing* easingType = NULL);
  void              detectMovement();
  void              drawImage(int offsetX, SingleImagePage* img);
  vector<string>    urlImages;
  int               actualImageCount;
  void              tweenCompleted();
  SingleImagePage*  loadImage(int id, SingleImagePage* img);
  ofVec2f           handStartPos;
  float             mainOffsetX;
  galleryDirection  direction;
  ofVec2f           swipeRange;
  float             maxOffsetXHandOutside;
  float             transitionDuration;
  int               counterFrameToWait;
};

#endif /* defined(__goofyLeapImageGallery__GoofyLeapImageGallery__) */
