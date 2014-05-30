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
}