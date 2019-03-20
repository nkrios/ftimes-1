/*
 ***********************************************************************
 *
 * $Id: error.c,v 1.1.1.1 2002/01/18 03:17:24 mavrik Exp $
 *
 ***********************************************************************
 *
 * Copyright 2000-2002 Klayton Monroe, Exodus Communications, Inc.
 * All Rights Reserved.
 *
 ***********************************************************************
 */

#include "all-includes.h"

static int            giWarnings = 0;
static int            giFailures = 0;

/*-
 ***********************************************************************
 *
 * ErrorGetWarnings
 *
 ***********************************************************************
 */
int
ErrorGetWarnings()
{
  return giWarnings;
}


/*-
 ***********************************************************************
 *
 * ErrorGetFailures
 *
 ***********************************************************************
 */
int
ErrorGetFailures()
{
  return giFailures;
}


/*-
 ***********************************************************************
 *
 * ErrorHandler
 *
 ***********************************************************************
 */
void
ErrorHandler(int iError, char *pcError, int iSeverity)
{
  switch (iSeverity)
  {
  case ERROR_WARNING:
    MessageHandler(MESSAGE_QUEUE_IT, MESSAGE_WARNING, MESSAGE_WARNING_STRING, pcError);
    giWarnings++;
    break;
  case ERROR_FAILURE:
    MessageHandler(MESSAGE_QUEUE_IT, MESSAGE_FAILURE, MESSAGE_FAILURE_STRING, pcError);
    giFailures++;
    break;
  case ERROR_CRITICAL:
    MessageHandler(MESSAGE_FLUSH_IT, MESSAGE_CRITICAL, MESSAGE_CRITICAL_STRING, pcError);
    exit((iError >= 0 && iError < XER_MaxExternalErrorCode) ? iError : XER_Abort);
    break;
  default:
    ErrorHandler(ER_InvalidSeverity, "ErrorHandler(): Invalid Severity", ERROR_CRITICAL);
    break;
  }
}


#ifdef FTimes_WIN32
/*-
 ***********************************************************************
 *
 * ErrorFormatWin32Error
 *
 ***********************************************************************
 */
void
ErrorFormatWin32Error(char **ppcMessage)
{
  static char         cMessage[ERRBUF_SIZE / 2];
  char               *pc;
  int                 i,
                      j;
  LPVOID              lpvMessage;

  cMessage[0] = 0;

  *ppcMessage = cMessage;

  FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),        /* default language */
                (LPTSTR) & lpvMessage,
                0,
                NULL
    );

  /*
   * Replace linefeeds with spaces. Eliminate carriage returns.
   */
  for (i = 0, j = 0, pc = (char *) lpvMessage; (i < (int) strlen(lpvMessage)) && (i < (ERRBUF_SIZE / 2)); i++, j++)
  {
    if (pc[i] == '\n')
    {
      cMessage[j] = ' ';
    }
    else if (pc[i] == '\r')
    {
      j--;
      continue;
    }
    else
    {
      cMessage[j] = pc[i];
    }
  }
  cMessage[j] = 0;

  LocalFree(lpvMessage);
}
#endif