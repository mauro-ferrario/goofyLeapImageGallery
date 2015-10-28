//
//  SingleImagePage.cpp
//  goofyLeapImageGallery
//
//  Created by Mauro Ferrario on 28/10/15.
//
//

#include "SingleImagePage.h"

SingleImagePage::SingleImagePage()
{
  
}

void SingleImagePage::setup(string imgUrl, ofVec2f gallerySize)
{
  this->gallerySize = gallerySize;
  image.loadImage(imgUrl);
}

void SingleImagePage::draw()
{
  if(image.isAllocated())
  {
    ofRectangle imgCoords;
    if(image.width < image.height)
    {
      imgCoords.y = 0;
      imgCoords.height = gallerySize.y;
      imgCoords.width = float(image.width)/float(image.height)*float(imgCoords.height);
      imgCoords.x = (gallerySize.x - imgCoords.width)*.5;
    }
    else
    {
      imgCoords.x = 0;
      imgCoords.width = gallerySize.x;
      imgCoords.height = float(imgCoords.width)/(float(image.width)/float(image.height));
      imgCoords.y = (gallerySize.y - imgCoords.height)*.5;
    }
    
    ofPushStyle();
    ofSetColor(255);
    image.draw(imgCoords);
    ofPopStyle();
  }
}