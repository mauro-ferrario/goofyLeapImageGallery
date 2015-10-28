//
//  SingleImagePage.h
//  goofyLeapImageGallery
//
//  Created by Mauro Ferrario on 28/10/15.
//
//

#ifndef __goofyLeapImageGallery__SingleImagePage__
#define __goofyLeapImageGallery__SingleImagePage__

#include "ofMain.h"

class SingleImagePage
{
public:
          SingleImagePage();
  void    setup(string imgUrl, ofVec2f gallerySize);
  void    draw();
  ofImage image;
  ofVec2f gallerySize;
};

#endif /* defined(__goofyLeapImageGallery__SingleImagePage__) */
