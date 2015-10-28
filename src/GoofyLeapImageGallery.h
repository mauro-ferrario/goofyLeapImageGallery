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


class GoofyLeapImageGallery
{
public:
        GoofyLeapImageGallery();
  void          setup();
  void          draw();
  void          update();
  void          addListeners();
private:
  ofxLeapMotion leap;
};

#endif /* defined(__goofyLeapImageGallery__GoofyLeapImageGallery__) */
