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
        static void setLogLevel(char log_level_mask);
        static void setUseLongLogNames(bool use);
        static void setUseColor(bool use);

        void setLogName(string nameShort); // set log name example: class name
        void setLogName(string nameShort, string nameLong); // set log name example: class name

        void err(string text);  // error message
        void warn(string text); // warning message
        void ok(string text);   // ok message
        void debug(string text);// debug message
        void info(string text); // information message
        void trace(string text);// trace info message

        void out(string text, bool error, string errorText); // ok/err - also print error if ok=false
        void out(string text, bool error);                   // ok/err - also print error if ok=false

    private:
        string logNameShort, logNameLong;
        static char logLevelMask;
        static bool useLongLogNames, useColors;
        static int  maxNameLongLen;

        void print(string tag, string text, char tagMask);
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
