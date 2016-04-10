/* 
 * File:   Log.cpp
 * Author: Joshua Johannson
 *
 * 
 * --------------------------------------
 * Log CLASS
 * @...
 * --------------------------------------
 */

#include "Log.h"
#ifndef pl_pi
#include <SDL.h>
#endif
using namespace ui;

// static
UI_LOG_LEVEL Log::logLevel;


// -- SET LOG NAME ----------------------
void Log::setLogName(string name)
{
    this->logName = name;
}

void Log::setLogLevel(UI_LOG_LEVEL log_level)
{
    logLevel = log_level;
}


// -- OUT -------------------------------
void Log::err(string text)
{
    if (logLevel >= UI_LOG_LEVEL_ERROR)
        print("[ err  ] " + text);
}

void Log::warn(string text)
{
    if (logLevel >= UI_LOG_LEVEL_WARNING)
        print("[ warn ] " + text);
}

void Log::ok(string text)
{
    if (logLevel >= UI_LOG_LEVEL_OK)
        print("[  ok  ] " + text);
}

void Log::debug(string text)
{
    if (logLevel >= UI_LOG_LEVEL_DEBUG)
        print("[debug ] " + text);
}

void Log::info(string text)
{
    if (logLevel >= UI_LOG_LEVEL_INFORMATION)
        print("[ info ] " + text);
}

void Log::trace(string text)
{
    if (logLevel >= UI_LOG_LEVEL_TRACE)
        print("[ trac ] " + text);
}

void Log::out(string text, bool error, string errorText)
{
     if (error && (logLevel >= UI_LOG_LEVEL_ERROR))
         print("[ err  ] " + text + ": '" + errorText + "'");
     else
         this->ok(text);
}



void Log::print(string text)
{
    cout << "[ " << logName << " ] " << text << endl;

    // android
#ifdef pl_andr
    string out =  "[ " + logName + " ] " + text;
    SDL_Log(out.c_str());
#endif
}
