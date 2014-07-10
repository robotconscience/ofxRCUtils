//
//  AppUtils.h
//  RCUtils
//
//  Created by Brett Renfer on 5/28/14.
//
//

#pragma once

#include "ofMain.h"

#include <tr1/functional>
#define FUNCTION tr1::function

namespace rc {
    class KeyManager {
    public:
        
        typedef std::tr1::function<void()> Delegate;
        
        KeyManager(){
            ofAddListener(ofEvents().keyPressed, this, &KeyManager::keyPressed );
        }
        
        ~KeyManager(){
            ofRemoveListener(ofEvents().keyPressed, this, &KeyManager::keyPressed );
        }
        
        void add( char c, bool * value ){
            if ( keys.count(0) == 0 ){
                keys[c] = vector<bool*>();
            }
            keys[c].push_back(value);
        }
        
        void addTrigger( char c, Delegate func ){
            if ( triggers.count(0) == 0 ){
                triggers[c] = vector<Delegate>();
            }
            triggers[c].push_back(func);
        }
        
        void remove( char c, bool * value ){
            if ( keys.count(c) != 0 ){
                for ( int i=0; i<keys[c].size(); i++){
                    if ( keys[c][i] == value ){
                        keys[c].erase(keys[c].begin()+ i);
                    }
                }
            }
        }
        
        void keyPressed( ofKeyEventArgs & e ){
            if ( keys.count(e.key) > 0 ){
                for ( auto * b : keys[e.key] ){
                    *b = !*b;
                }
            }
            if ( triggers.count(e.key) > 0 ){
                for ( auto & f : triggers[e.key] ){
                    f();
                }
            }
        }
        
    protected:
        map<char, vector<bool*> > keys;
        map<char, vector<Delegate> > triggers;
    };
    
#pragma mark TextSettings
    
    class TextSettings {
    public:
        TextSettings(){};
        
        TextSettings( string location ){
            load(location);
        }
        
        bool load( string location ){
            file.clear();
            if ( file.open(location) ){
                buffer = file.readToBuffer();
            } else {
                return false;
            }
        }
        
        string getSetting( string def, int position=-1 ){
            string ret = def;
            
            // aka next setting
            if ( position == -1 ){
                string get = buffer.getNextLine();
                if ( get != "" ){
                    ret = get;
                }
            // specific line
            } else {
                ofLogWarning()<<"[rc::TextSettings] Reseting line reader, must call with position from here on out";
                buffer.resetLineReader(); // kind of a bad idea
                for ( int i=0; i<position; i++){
                    buffer.getNextLine();
                }
                ret = getSetting( def );
            }
            
            return ret;
        }
        
    protected:
        ofFile      file;
        ofBuffer    buffer;
    };

#pragma mark Timer
    
    class Timer {
    public:
        
        Timer(){
            stop();
        }
        
        void start( int intervalMillis ){
            interval = intervalMillis;
            timeStarted = ofGetElapsedTimeMillis();
            lastFired   = ofGetElapsedTimeMillis();
        }
        
        void stop(){
            interval = timeStarted = lastFired = 0;
        }
        
        float getPercentage( int time = -1 ){
            if ( !hasStarted() ) return 0.0;
            if ( time == -1 ) time = ofGetElapsedTimeMillis();
            return ofMap( (float) (time - lastFired), 0.0, interval, 0.0, 1.0);
        }
        
        // call empty to use current time
        bool isReady( int time = -1 ){
            if ( !hasStarted() ) return false;
            if ( time == -1 ) time = ofGetElapsedTimeMillis();
            if ( time - lastFired > interval ){
                lastFired = time;
                return true;
            } else {
                return false;
            }
        }
        
        int getTimeStarted(){
            return timeStarted;
        }
        
        bool hasStarted(){
            return !(timeStarted == 0);
        }
        
    protected:
        int timeStarted, lastFired, interval;
        
    };
    
    
}