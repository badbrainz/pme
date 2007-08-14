#ifndef LOGGER_H
#define LOGGER_H

#include "../Extensions/GLee.h"
#include "../Parsers/XMLParser.h"
#include "../Parsers/IOXMLObject.h"

#define deleteObject(A){ if(A){ delete   A; A = NULL; } }
#define deleteArray(A) { if(A){ delete[] A; A = NULL; } }

#define MAX_TEX_UNITS 8

class Logger
{
  public:
    static void writeImmidiateInfoLog(const String &info);
    static void writeFatalErrorLog(const String &logString);
    static bool writeErrorLog(const String &error);
    static void writeInfoLog(const String &info);
    static void initialize(const char* logfilename = NULL);
    static void flush();

  private:
    static vector<String> logStrings;
    static String         logPath; 
};

#endif