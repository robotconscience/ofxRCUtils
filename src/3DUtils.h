//
//  3DUtils.h
//
//  Created by Brett Renfer on 5/22/14.
//

#pragma once

#include "ofMain.h"

namespace rc {
    
    static ofVec2f getCircleVec( float angle, float rad ){
        float x = cos( ofDegToRad(angle) ) * rad;
        float y = -sin( ofDegToRad(angle)) * rad;
        return ofVec2f(x,y);
    }
    
    class ofMeshCircle : public ofMesh {
        public: void generate( float radius, float res = 100 ){
            clear();
            ofMesh m = circle(radius, 0.0, 0.0, res);
            append( m );
        }
        
        public: static ofMesh circle( float radius, float x=0.0, float y=0.0, float res = 100 ){
            ofMesh toReturn;
            float inc = (float) 360.0 / res;
            
            int i = 0;
            
            ofVec2f center(x,y);
            ofFloatColor color(ofRandomuf(),ofRandomuf(),ofRandomuf());
            color.setBrightness(1.0);
            color.setSaturation(0.0);
            
            for ( float a = 0; a < 360.0; a+= inc ){
                ofVec2f vert(getCircleVec(a, radius));
                vert += center;
                toReturn.addTexCoord(vert);
                toReturn.addVertex(vert);
                toReturn.addIndex(i);
                //addColor(color);
                i++;
                
                vert.set(getCircleVec(a + inc, radius));
                vert += center;
                
                toReturn.addTexCoord(vert);
                toReturn.addVertex( vert );
                toReturn.addIndex(i);
                //addColor(color);
                i++;
                
                toReturn.addTexCoord(center);
                toReturn.addVertex(center);
                toReturn.addIndex(i);
                //addColor(color);
                i++;
            }
            return toReturn;
        }
    };
    
    /**
     * Near exact copy of ofMesh::sphere BUT with texcoords for a non power of two texture
     */
    static ofMesh sphereNP2( float radius, int texW, int texH, int res = 12, ofPrimitiveMode mode = OF_PRIMITIVE_TRIANGLES ) {
        
        ofMesh mesh;
        
        float doubleRes = res*2.f;
        float polarInc = PI/(res); // ringAngle
        float azimInc = TWO_PI/(doubleRes); // segAngle //
        
        if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
            mode = OF_PRIMITIVE_TRIANGLE_STRIP;
        }
        mesh.setMode(mode);
        
        ofVec3f vert;
        ofVec2f tcoord;
        
        for(float i = 0; i < res+1; i++) {
            
            float tr = sin( PI-i * polarInc );
            float ny = cos( PI-i * polarInc );
            
            tcoord.y = (i / res) * texH;
            
            for(float j = 0; j <= doubleRes; j++) {
                
                float nx = tr * sin(j * azimInc);
                float nz = tr * cos(j * azimInc);
                
                tcoord.x = (j / (doubleRes)) * texW;
                
                vert.set(nx, ny, nz);
                mesh.addNormal(vert);
                vert *= radius;
                mesh.addVertex(vert);
                mesh.addTexCoord(tcoord);
            }
        }
        
        int nr = doubleRes+1;
        if(mode == OF_PRIMITIVE_TRIANGLES) {
            
            int index1, index2, index3;
            
            for(float iy = 0; iy < res; iy++) {
                for(float ix = 0; ix < doubleRes; ix++) {
                    
                    // first tri //
                    if(iy > 0) {
                        index1 = (iy+0) * (nr) + (ix+0);
                        index2 = (iy+0) * (nr) + (ix+1);
                        index3 = (iy+1) * (nr) + (ix+0);
                        
                        mesh.addIndex(index1);
                        mesh.addIndex(index3);
                        mesh.addIndex(index2);
                    }
                    
                    if(iy < res-1 ) {
                        // second tri //
                        index1 = (iy+0) * (nr) + (ix+1);
                        index2 = (iy+1) * (nr) + (ix+1);
                        index3 = (iy+1) * (nr) + (ix+0);
                        
                        mesh.addIndex(index1);
                        mesh.addIndex(index3);
                        mesh.addIndex(index2);
                        
                    }
                }
            }
            
        } else {
            for(int y = 0; y < res; y++) {
                for(int x = 0; x <= doubleRes; x++) {
                    mesh.addIndex( (y)*nr + x );
                    mesh.addIndex( (y+1)*nr + x );
                }
            }
        }
        
        
        return mesh;
    }
    
    // BB
    static ofRectangle ofMeshBoundingBox( const ofMesh & m ){
        float minX = 999999;
        float maxX = -999999;
        float minY = 999999;
        float maxY = -999999;
        
        for ( auto & v : m.getVertices() ){
            if ( v.x > maxX ) maxX = v.x;
            if ( v.x < minX ) minX = v.x;
            if ( v.y > maxY ) maxY = v.y;
            if ( v.y < minY ) minY = v.y;
        }
        
        return ofRectangle(minX,minY,maxX-minX,maxY-minY);
    }

    /**
     * Color a mesh
     */
    static void ofSetMeshColor( ofMesh & m, ofFloatColor c ){
        for ( auto & index : m.getIndices() ){
            if ( m.getNumColors() > index ){
                m.setColor(index, c);
            } else {
                m.addColor(c);
            }
        }
    }
    static void ofSetMeshColor( ofMesh & m, ofColor c ){
        ofSetMeshColor(m, ofFloatColor(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0));
    }
    
    static void ofSetMeshGradient( ofMesh & m, ofFloatColor a, ofFloatColor b ){
        
        ofRectangle bb = ofMeshBoundingBox(m);
        
        for ( auto & index : m.getIndices() ){
            ofVec3f v = m.getVertex(index);
            float val = ofMap( v.x, bb.x, bb.x + bb.width, 0.0, 1.0, true);
            ofFloatColor d(a);
            ofFloatColor c = d.lerp(b, val );
            if ( m.getNumColors() > index ){
                m.setColor(index, c);
            } else {
                m.addColor(c);
            }
        }
    }
}