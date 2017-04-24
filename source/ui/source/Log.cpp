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
#include <termcolor/termcolor.hpp>
#ifndef pl_pi
#include <SDL.h>
#endif
using namespace ui;

// static
char Log::logLevelMask    = UI_LOG_LEVEL_ALL;
bool Log::useLongLogNames = false;
bool Log::useColors       = false;
int  Log::maxNameLongLen  = 0;


// -- SET LOG NAME ----------------------
void Log::setLogName(string nameShort)
{
    this->logNameShort = nameShort;
    this->logNameLong  = "";
}
void Log::setLogName(string nameShort, string nameLong)
{
    setLogName(nameShort);
    this->logNameLong = nameLong;

    if (logNameLong.length() > maxNameLongLen)
        maxNameLongLen = logNameLong.length();
}

void Log::setLogLevel(char log_levelMask)
{
    logLevelMask = log_levelMask;
}


void Log::setUseLongLogNames(bool use)
{
    useLongLogNames = use;
}

void Log::setUseColor(bool use)
{
    useColors = use;
}


// -- OUT -------------------------------
void Log::err(string text)
{
    if ((logLevelMask & UI_LOG_LEVEL_ERROR) > 0)
        print("[ err  ] ", text, UI_LOG_LEVEL_ERROR);
}

void Log::err(string text, string error)
{
  if ((logLevelMask & UI_LOG_LEVEL_ERROR) > 0)
    print("[ err  ] ", text + ": '" + error + "'", UI_LOG_LEVEL_ERROR);
}

void Log::warn(string text)
{
    if ((logLevelMask & UI_LOG_LEVEL_WARNING) > 0)
        print("[ warn ] ", text, UI_LOG_LEVEL_WARNING);
}

void Log::ok(string text)
{
    if ((logLevelMask & UI_LOG_LEVEL_OK) > 0)
        print("[  ok  ] ", text, UI_LOG_LEVEL_OK);
}

void Log::debug(string text)
{
    if ((logLevelMask & UI_LOG_LEVEL_DEBUG) > 0)
        print("[debug ] ", text, UI_LOG_LEVEL_DEBUG);
}

void Log::info(string text)
{
    if ((logLevelMask & UI_LOG_LEVEL_INFORMATION) > 0)
        print("[ info ] ", text, UI_LOG_LEVEL_INFORMATION);
}

void Log::trace(string text)
{
    if ((logLevelMask & UI_LOG_LEVEL_TRACE) > 0)
        print("[ trac ] ", text, UI_LOG_LEVEL_TRACE);
}

void Log::out(string text, bool error, string errorText)
{
    if (error && ((logLevelMask & UI_LOG_LEVEL_ERROR) > 0))
        print("[ err  ] ", text + ": '" + errorText + "'", UI_LOG_LEVEL_ERROR);
    else
        this->ok(text);
}

void Log::out(string text, bool error)
{
    out(text, error, "");
}



void Log::print(string tag, string text, char tagMask)
{
    if (useColors)
    {
        // colors
        switch (tagMask)
        {
            case UI_LOG_LEVEL_ERROR:
                cout << termcolor::reset << termcolor::red;
                break;

            case UI_LOG_LEVEL_WARNING:
                cout << termcolor::reset << termcolor::yellow;
                break;

            case UI_LOG_LEVEL_OK:
                cout << termcolor::reset << termcolor::green;
                break;

            case UI_LOG_LEVEL_DEBUG:
                cout << termcolor::reset << termcolor::blue;
                break;

            case UI_LOG_LEVEL_INFORMATION:
                cout << termcolor::reset;
                break;

            case UI_LOG_LEVEL_TRACE:
                cout << termcolor::reset << termcolor::cyan;
                break;
        }
    }


    // output
    if (!useLongLogNames)
        cout << "[ " << logNameShort.substr(0, 4) << " ] " << tag << "" << text << endl;
    else
    {
        if (logNameLong != "")
            cout << "[ " << logNameLong.substr(0, maxNameLongLen) << " ] " << tag << text  << endl;
        else
            cout << "[ " << logNameShort.substr(0, maxNameLongLen) << " ] " << tag << text << endl;

    }


    // android
#ifdef ANDROID
    string out =  "[ " + logNameShort.substr(0, 4) + " ] " + tag + text;
    SDL_Log(out.c_str());
#endif
}
