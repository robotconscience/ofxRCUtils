#pragma once

#include "3DUtils.h"
#include "AppUtils.h"

namespace rc {
    static ofColor ofRandomRCColor( float brightness = 255, float sat = 100 ){
        ofColor c = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
        c.setBrightness(brightness);
        c.setSaturation(sat);
        return c;
    }
    
    static void ofTranslateCenter(){
        ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    }
    
    static void ofTranslateNegCenter(){
        ofTranslate(-ofGetWidth()/2.0, -ofGetHeight()/2.0);
    }
    
    static ofPoint ofCenter(){
        return ofPoint( ofGetWidth()/2.0, ofGetHeight()/2.0 );
    }
}