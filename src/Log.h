/* 
 * File:   Log.cpp
 * Author: Joshua Johannson
 *
 * 
 * --------------------------------------
 * Log CLASS
 * loggable object
 * --------------------------------------
 */


#ifndef PANUI_LOG_H
#define PANUI_LOG_H

// include
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "const.h"

using namespace std;
namespace ui
{

/* Log class
 */
class Log
{
    public:
        static void setLogLevel(UI_LOG_LEVEL log_level);
        void setLogName(string name); // set log name example: class name

        void err(string text);  // error message
        void warn(string text); // warning message
        void ok(string text);   // ok message
        void debug(string text);// debug message
        void info(string text); // information message
        void trace(string text);// trace info message

        void out(string text, bool error, string errorText); // ok/err - also print error if ok=false

    private:
        string logName;
        static UI_LOG_LEVEL logLevel;

        void print(string text);
};


// to_string for android
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

template <typename T>
std::string str(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}


};     /* END NAMESPACE */
#endif /* PANUI_LOG_H */
