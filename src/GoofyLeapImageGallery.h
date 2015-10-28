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


class GoofyLeapImageGallery
{
public:
                    GoofyLeapImageGallery();
  void              setup();
  void              draw();
  void              update();
  void              addListeners();
private:
  ofxLeapMotion     leap;
  void              gotSwipeEvent(SwipeEvent& args);
  bool              singleHeadDetected;
  ofPoint           prevHandPos;
  SingleImagePage*  actualImage;
  void              loadFirtImage();
};

#endif /* defined(__goofyLeapImageGallery__GoofyLeapImageGallery__) */
