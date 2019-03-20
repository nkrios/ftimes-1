/*-
 ***********************************************************************
 *
 * $Id: pool2string.c,v 1.2 2004/04/04 07:09:50 mavrik Exp $
 *
 ***********************************************************************
 *
 * Copyright 2002-2004 Klayton Monroe, All Rights Reserved.
 *
 ***********************************************************************
 */
#include <stdio.h>
#include "pool2string.h"

/*-
 ***********************************************************************
 *
 * Main
 *
 ***********************************************************************
 */
int
main()
{
  unsigned char aucPool[POOL2STRING_POOL_SIZE];
  unsigned char aucTaps[POOL2STRING_POOL_TAPS];
  
  POOL2STRING_NEW_POOL(aucPool, POOL2STRING_POOL_SIZE, POOL2STRING_POOL_SEED);
  POOL2STRING_TAP_POOL(aucTaps, aucPool);
  printf("String='%s'\n", aucTaps);
  return 0;
}
