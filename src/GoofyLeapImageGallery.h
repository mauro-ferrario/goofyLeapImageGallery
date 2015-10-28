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
#include "ofxMSATimer.h"


class GoofyLeapImageGallery
{
public:
                    GoofyLeapImageGallery();
  void              setup();
  void              draw();
  void              update();
private:
  ofxLeapMotion     leap;
  bool              singleHeadDetected;
  ofPoint           prevHandPos;
  SingleImagePage*  actualImage;
  SingleImagePage*  newImage;
  void              loadFirtImage();
  void              loadNewImage();
  ofxTween          tweenMainImage;
  ofxTween          tweenNewImage;
  ofxEasingExpo     easingMainImage;
  ofxEasingExpo     easingNewmage;
  ofxMSATimer       timer;
  bool              isMoving;
  bool              swipeFree;
  bool              prevSingleHandDetected;
  void              switchImage();
  void              move(string direction, float speed = 1000);
  void              moveNext(float speed = 1000);
  void              movePrev(float speed = 1000);
  void              detectMovement();
  void              drawMainImage();
  void              drawNewImage();
};

#endif /* defined(__goofyLeapImageGallery__GoofyLeapImageGallery__) */
