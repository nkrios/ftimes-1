/*-
 ***********************************************************************
 *
 * $Id: ftimes.h,v 1.30 2004/04/24 06:26:52 mavrik Exp $
 *
 ***********************************************************************
 *
 * Copyright 2000-2004 Klayton Monroe, All Rights Reserved.
 *
 ***********************************************************************
 */

/*-
 ***********************************************************************
 *
 * Defines
 *
 ***********************************************************************
 */
#define PROGRAM_NAME "ftimes"
#define VERSION "3.4.0"

#define LF            "\n"
#define CRLF        "\r\n"
#define NEWLINE_LENGTH  3

#ifdef WIN32
#define UNIX_EPOCH_IN_NT_TIME 0x019db1ded53e8000
#define UNIX_LIMIT_IN_NT_TIME 0x01e9fcf4ebcfe180
#define DEFAULT_STREAM_NAME_W L"::$DATA"
#define FTIMES_STREAM_INFO_SIZE  0x8000
#define FTIMES_MAX_STREAM_INFO_SIZE 0x00100000
#define FTIMES_INVALID_STREAM_COUNT 0xffffffff
#endif

#ifdef UNIX
#ifdef FALSE
#undef FALSE
#endif

#ifdef TRUE
#undef TRUE
#endif

typedef enum _BOOL
{
  FALSE = 0,
  TRUE
} BOOL;
#endif

#define FTIMES_MAX_32BIT_SIZE             36 /* (prefix [+-](0x|0|)) 3 + (binary string) 32 + (NULL) 1 */
#define FTIMES_MAX_64BIT_SIZE             68 /* (prefix [+-](0x|0|)) 3 + (binary string) 64 + (NULL) 1 */
#define FTIMES_DATETIME_SIZE              15
#define FTIMES_TIME_SIZE                  20
#define FTIMES_PID_SIZE                   11
#define FTIMES_RUNTIME_FORMAT      "%H:%M:%S"
#define FTIMES_RUNDATE_FORMAT      "%Y/%m/%d"
#define FTIMES_RUNZONE_FORMAT            "%Z"
#define FTIMES_SUFFIX_SIZE                64
#define FTIMES_ZONE_SIZE                  64
#ifdef UNIX
#define FTIMES_TIME_FORMAT_SIZE           20
#define FTIMES_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#endif
#ifdef WIN32
#define FTIMES_TIME_FORMAT_SIZE           24
#define FTIMES_TIME_FORMAT "%04d-%02d-%02d %02d:%02d:%02d|%d"
#define FTIMES_OOB_TIME_FORMAT_SIZE       19
#define FTIMES_OOB_TIME_FORMAT "%04d%02d%02d%02d%02d%02d|%x"
#endif

#ifdef WIN32
#define FTIMES_ROOT_PATH                 "c:"
#define FTIMES_SLASH                     "\\"
#define FTIMES_SLASHCHAR                 '\\'
#define FTIMES_MAX_PATH                  260
#endif
#ifdef UNIX
#define FTIMES_ROOT_PATH                  "/"
#define FTIMES_SLASH                      "/"
#define FTIMES_SLASHCHAR                  '/'
#define FTIMES_MAX_PATH                 4096
#endif
#define FTIMES_DOT                        "."
#define FTIMES_DOTCHAR                    '.'
#define FTIMES_DOTDOT                    ".."
#define FTIMES_SEPARATOR                  "="

#define FTIMES_MIN_STRING_REPEATS          0
#define FTIMES_MAX_STRING_REPEATS 0x7fffffff
#define FTIMES_MAX_HOSTNAME_LENGTH       256
#define FTIMEX_MAX_MD5_LENGTH (((MD5_HASH_SIZE)*2)+1)
#define FTIMES_MAX_USERNAME_LENGTH        32
#define FTIMES_MAX_PASSWORD_LENGTH        32

#define FTIMES_MIN_FILE_SIZE_LIMIT         0
#define FTIMES_MAX_FILE_SIZE_LIMIT        ~0

#define FTIMES_MAX_LINE                 8192

#define FTIMES_FILETYPE_ERROR              0
#define FTIMES_FILETYPE_BLOCK              1
#define FTIMES_FILETYPE_CHARACTER          2
#define FTIMES_FILETYPE_DIRECTORY          3
#define FTIMES_FILETYPE_DOOR               4
#define FTIMES_FILETYPE_FIFO               5
#define FTIMES_FILETYPE_LINK               6
#define FTIMES_FILETYPE_REGULAR            7
#define FTIMES_FILETYPE_SOCKET             8
#define FTIMES_FILETYPE_WHITEOUT           9
#define FTIMES_FILETYPE_UNKNOWN           10

#define FTIMES_TEST_NORMAL                 0
#define FTIMES_TEST_STRICT                 1

#ifdef WIN32
#define Have_Nothing                       0
#define Have_GetFileAttributes             1
#define Have_GetFileAttributesEx           2
#define Have_MapGetFileHandle              3
#define Have_GetFileInformationByHandle    4
#define Have_NTQueryInformationFile        5
#define Have_MapCountNamedStreams          6
#endif
#ifdef UNIX
#define Have_Nothing                       0
#define Have_lstat                         1
#endif

#define FTIMES_FILETYPE_BUFSIZE         1280 /* XMAGIC_MAX_LEVEL * XMAGIC_DESCRIPTION_BUFSIZE */
#ifdef WIN32
typedef struct _FTIMES_FILE_DATA
{
  char               *pcRawPath;
  char               *pcNeuteredPath;
  DWORD               dwVolumeSerialNumber;
  DWORD               dwFileIndexHigh;
  DWORD               dwFileIndexLow;
  DWORD               dwFileAttributes;
  FILETIME            sFTATime;
  FILETIME            sFTMTime;
  FILETIME            sFTCTime;
  FILETIME            sFTChTime;
  DWORD               dwFileSizeHigh;
  DWORD               dwFileSizeLow;
  int                 iStreamCount;
  int                 iFSType;
  int                 iFileFlags;
  char                acType[FTIMES_FILETYPE_BUFSIZE];
  unsigned char       aucFileMD5[MD5_HASH_SIZE];
  unsigned char      *pucStreamInfo;
} FTIMES_FILE_DATA;
#endif
#ifdef UNIX
typedef struct _FTIMES_FILE_DATA
{
  char               *pcRawPath;
  char               *pcNeuteredPath;
  struct stat         sStatEntry;
  int                 iFSType;
  int                 iFileFlags;
  char                acType[FTIMES_FILETYPE_BUFSIZE];
  unsigned char       aucFileMD5[MD5_HASH_SIZE];
} FTIMES_FILE_DATA;
#endif

typedef struct _FILE_LIST
{
  char                acPath[FTIMES_MAX_PATH];
  struct _FILE_LIST  *psNext;
} FILE_LIST;

#ifdef WIN32
#define VOLUME_SET     0x00000001
#define FINDEX_SET     0x00000002
#define ATTRIBUTES_SET 0x00000004
#define ATIME_SET      0x00000008
#define MTIME_SET      0x00000010
#define CTIME_SET      0x00000020
#define CHTIME_SET     0x00000040
#define SIZE_SET       0x00000080
#define ALTSTREAMS_SET 0x00000100
#define RESERVED_SET   0x00000200 /* reserved */
#define MAGIC_SET      0x00000400
#define MD5_SET        0x00000800

#define ALL_MASK       0x00000dff /* ALL-reserved */
#define DEFAULT_MASK   0x000009ff /* ALL-reserved-magic */
#endif

#ifdef UNIX
#define DEV_SET        0x00000001
#define INODE_SET      0x00000002
#define MODE_SET       0x00000004
#define NLINK_SET      0x00000008
#define UID_SET        0x00000010
#define GID_SET        0x00000020
#define RDEV_SET       0x00000040
#define ATIME_SET      0x00000080
#define MTIME_SET      0x00000100
#define CTIME_SET      0x00000200
#define SIZE_SET       0x00000400
#define RESERVED_SET   0x00000800 /* reserved */
#define MAGIC_SET      0x00001000
#define MD5_SET        0x00002000

#define ALL_MASK       0x000037ff /* ALL-reserved */
#define DEFAULT_MASK   0x000027ff /* ALL-reserved-magic */
#endif

typedef struct _MASK_TABLE
{
  char                MaskName[32];
  char                HeaderName[32];
  unsigned long       Mask;
} MASK_TABLE;

#define FTIMES_CMPDATA "cmp"
#define FTIMES_DIGDATA "dig"
#define FTIMES_MAPDATA "map"
#define FTIMES_MAX_DATA_TYPE 4

#define FTIMES_CFGTEST 0x00000001
#define FTIMES_CMPMODE 0x00000002
#define FTIMES_DECODER 0x00000004
#define FTIMES_DIGAUTO 0x00000008
#define FTIMES_DIGFULL 0x00000010
#define FTIMES_DIGLEAN 0x00000020
#define FTIMES_GETMODE 0x00000040
#define FTIMES_MAPAUTO 0x00000080
#define FTIMES_MAPFULL 0x00000100
#define FTIMES_MAPLEAN 0x00000200
#define FTIMES_PUTMODE 0x00000400
#define FTIMES_VERSION 0x00000800

#define MODES_AnalyzeDeviceFiles ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_AnalyzeRemoteFiles ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_BaseName          ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_BaseNameSuffix    ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_Compress          ((FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_DataType          ((FTIMES_PUTMODE))
#define MODES_DateTime          ((FTIMES_PUTMODE))
#define MODES_DigString         ((FTIMES_DIGAUTO) | (FTIMES_DIGFULL) | (FTIMES_DIGLEAN))
#define MODES_EnableRecursion   ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_Exclude           ((FTIMES_DIGAUTO) | (FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPAUTO) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_FieldMask         ((FTIMES_CMPMODE) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN) | (FTIMES_PUTMODE))
#define MODES_FileSizeLimit     ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_GetAndExec        ((FTIMES_GETMODE))
#define MODES_GetFileName       ((FTIMES_GETMODE))
#define MODES_HashDirectories   ((FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_HashSymbolicLinks ((FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_Import            ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_Include           ((FTIMES_DIGAUTO) | (FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPAUTO) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_LogDir            ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_LogFileName       ((FTIMES_PUTMODE))
#define MODES_MagicFile         ((FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_MatchLimit        ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN))
#define MODES_NewLine           ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_OutDir            ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_OutFileHash       ((FTIMES_PUTMODE))
#define MODES_OutFileName       ((FTIMES_PUTMODE))
#define MODES_RequirePrivilege  ((FTIMES_DIGFULL) | (FTIMES_DIGLEAN) | (FTIMES_MAPFULL) | (FTIMES_MAPLEAN))
#define MODES_RunType           ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE))
#define MODES_URLAuthType       ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_URLCreateConfig   ((FTIMES_DIGFULL) | (FTIMES_MAPFULL))
#define MODES_URLGetRequest     ((FTIMES_GETMODE))
#define MODES_URLGetURL         ((FTIMES_GETMODE))
#define MODES_URLPassword       ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_URLPutSnapshot    ((FTIMES_DIGFULL) | (FTIMES_MAPFULL))
#define MODES_URLPutURL         ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE))
#define MODES_URLUnlinkOutput   ((FTIMES_DIGFULL) | (FTIMES_MAPFULL))
#define MODES_URLUsername       ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#ifdef USE_SSL
#define MODES_SSLBundledCAsFile ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_SSLExpectedPeerCN ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_SSLMaxChainLength ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_SSLPassPhrase     ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_SSLPrivateKeyFile ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_SSLPublicCertFile ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_SSLUseCertificate ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#define MODES_SSLVerifyPeerCert ((FTIMES_DIGFULL) | (FTIMES_MAPFULL) | (FTIMES_PUTMODE) | (FTIMES_GETMODE))
#endif

#define KEY_AnalyzeDeviceFiles  "AnalyzeDeviceFiles"
#define KEY_AnalyzeRemoteFiles  "AnalyzeRemoteFiles"
#define KEY_BaseName            "BaseName"
#define KEY_BaseNameSuffix      "BaseNameSuffix"
#define KEY_Compress            "Compress"
#define KEY_DataType            "DataType"
#define KEY_DateTime            "DateTime"
#define KEY_DigString           "DigString"
#define KEY_EnableRecursion     "EnableRecursion"
#define KEY_Exclude             "Exclude"
#define KEY_FieldMask           "FieldMask"
#define KEY_FileSizeLimit       "FileSizeLimit"
#define KEY_GetAndExec          "GetAndExec"
#define KEY_GetFileName         "GetFileName"
#define KEY_HashDirectories     "HashDirectories"
#define KEY_HashSymbolicLinks   "HashSymbolicLinks"
#define KEY_Import              "Import"
#define KEY_Include             "Include"
#define KEY_LogDir              "LogDir"
#define KEY_LogFileName         "LogFileName"
#define KEY_MagicFile           "MagicFile"
#define KEY_MapRemoteFiles      "MapRemoteFiles"
#define KEY_MatchLimit          "MatchLimit"
#define KEY_NewLine             "NewLine"
#define KEY_OutDir              "OutDir"
#define KEY_OutFileHash         "OutFileHash"
#define KEY_OutFileName         "OutFileName"
#define KEY_RequirePrivilege    "RequirePrivilege"
#define KEY_RunType             "RunType"
#define KEY_URLAuthType         "URLAuthType"
#define KEY_URLCreateConfig     "URLCreateConfig"
#define KEY_URLGetRequest       "URLGetRequest"
#define KEY_URLGetURL           "URLGetURL"
#define KEY_URLPassword         "URLPassword"
#define KEY_URLPutSnapshot      "URLPutSnapshot"
#define KEY_URLPutURL           "URLPutURL"
#define KEY_URLUnlinkOutput     "URLUnlinkOutput"
#define KEY_URLUsername         "URLUsername"
#ifdef USE_SSL
#define KEY_SSLBundledCAsFile   "SSLBundledCAsFile"
#define KEY_SSLExpectedPeerCN   "SSLExpectedPeerCN"
#define KEY_SSLMaxChainLength   "SSLMaxChainLength"
#define KEY_SSLPassPhrase       "SSLPassPhrase"
#define KEY_SSLPrivateKeyFile   "SSLPrivateKeyFile"
#define KEY_SSLPublicCertFile   "SSLPublicCertFile"
#define KEY_SSLUseCertificate   "SSLUseCertificate"
#define KEY_SSLVerifyPeerCert   "SSLVerifyPeerCert"
#endif

typedef struct _CONTROLS_FOUND
{
  BOOL                bAnalyzeDeviceFilesFound;
  BOOL                bAnalyzeRemoteFilesFound;
  BOOL                bBaseNameFound;
  BOOL                bBaseNameSuffixFound;
  BOOL                bCompressFound;
  BOOL                bDataTypeFound;
  BOOL                bDateTimeFound;
  BOOL                bEnableRecursionFound;
  BOOL                bFieldMaskFound;
  BOOL                bFileSizeLimitFound;
  BOOL                bGetAndExecFound;
  BOOL                bGetFileNameFound;
  BOOL                bHashDirectoriesFound;
  BOOL                bHashSymbolicLinksFound;
  BOOL                bLogDirFound;
  BOOL                bLogFileNameFound;
  BOOL                bMagicFileFound;
  BOOL                bMatchLimitFound;
  BOOL                bNewLineFound;
  BOOL                bOutDirFound;
  BOOL                bOutFileHashFound;
  BOOL                bOutFileNameFound;
  BOOL                bRequirePrivilegeFound;
  BOOL                bRunTypeFound;
  BOOL                bURLAuthTypeFound;
  BOOL                bURLCreateConfigFound;
  BOOL                bURLGetRequestFound;
  BOOL                bURLGetURLFound;
  BOOL                bURLPasswordFound;
  BOOL                bURLPutSnapshotFound;
  BOOL                bURLPutURLFound;
  BOOL                bURLUnlinkOutputFound;
  BOOL                bURLUsernameFound;
#ifdef USE_SSL
  BOOL                bSSLBundledCAsFileFound;
  BOOL                bSSLExpectedPeerCNFound;
  BOOL                bSSLMaxChainLengthFound;
  BOOL                bSSLPassPhraseFound;
  BOOL                bSSLPrivateKeyFileFound;
  BOOL                bSSLPublicCertFileFound;
  BOOL                bSSLUseCertificateFound;
  BOOL                bSSLVerifyPeerCertFound;
#endif
} CONTROLS_FOUND;

typedef struct _ANALYSIS_STAGES
{
#define STAGE_DESCRIPTION_SIZE 64
  char                acDescription[STAGE_DESCRIPTION_SIZE];
  int                 iError;
  int               (*piRoutine)();
} ANALYSIS_STAGES;

typedef struct _RUNMODE_STAGES
{
  char                acDescription[STAGE_DESCRIPTION_SIZE];
  int                 iError;
  int               (*piRoutine)();
} RUNMODE_STAGES;

typedef struct _FTIMES_PROPERTIES
{
#define MAX_ANALYSIS_STAGES 32
  ANALYSIS_STAGES     asAnalysisStages[MAX_ANALYSIS_STAGES];
  BOOL                bAnalyzeDeviceFiles;
  BOOL                bAnalyzeRemoteFiles;
  BOOL                bCompress;
  BOOL                bEnableRecursion;
  BOOL                bGetAndExec;
  BOOL                bHashDirectories;
  BOOL                bHashSymbolicLinks;
  BOOL                bRequirePrivilege;
  BOOL                bURLCreateConfig;
  BOOL                bURLPutSnapshot;
  BOOL                bURLUnlinkOutput;
  char                acBaseName[FTIMES_MAX_PATH];
  char                acBaseNameSuffix[FTIMES_SUFFIX_SIZE];
  char                acCfgFileName[FTIMES_MAX_PATH];
  char                acConfigFile[FTIMES_MAX_PATH];
  char                acDataType[FTIMES_MAX_DATA_TYPE];
  char                acDateTime[FTIMES_TIME_SIZE];
  char                acGetFileName[FTIMES_MAX_PATH];
  char                acLogDirName[FTIMES_MAX_PATH];
  char                acLogFileName[FTIMES_MAX_PATH];
  char                acMagicFileName[FTIMES_MAX_PATH];
  char                acMagicHash[FTIMEX_MAX_MD5_LENGTH];
  char                acMaskString[ALL_FIELDS_MASK_SIZE];
  char                acNewLine[NEWLINE_LENGTH];
  char                acOutDirName[FTIMES_MAX_PATH];
  char                acOutFileName[FTIMES_MAX_PATH];
  char                acOutFileHash[FTIMEX_MAX_MD5_LENGTH];
  char                acPid[FTIMES_PID_SIZE];
  char                acRunDateTime[FTIMES_TIME_SIZE];
#define RUNTYPE_BUFSIZE 16
  char                acRunType[RUNTYPE_BUFSIZE];
  char                acStartDate[FTIMES_TIME_SIZE];
  char                acStartTime[FTIMES_TIME_SIZE];
  char                acStartZone[FTIMES_ZONE_SIZE];
#define GET_REQUEST_BUFSIZE 16
  char                acURLGetRequest[GET_REQUEST_BUFSIZE];
  char                acURLPassword[FTIMES_MAX_PASSWORD_LENGTH];
  char                acURLUsername[FTIMES_MAX_USERNAME_LENGTH];
  char               *pcBaselineFile;
  char               *pcProgram;
  char               *pcRunModeArgument;
  char               *pcSnapshotFile;
  char              **ppcMapList;
  CONTROLS_FOUND      sFound;
  FILE               *pFileLog;
  FILE               *pFileOut;
  FILE_LIST          *psExcludeList;
  FILE_LIST          *psIncludeList;
#define MAX_RUNMODE_STAGES 32
  RUNMODE_STAGES      sRunModeStages[MAX_RUNMODE_STAGES];
  HTTP_URL           *psGetURL;
  HTTP_URL           *psPutURL;
  int                 iImportRecursionLevel;
  int                 iLastAnalysisStage;
  int                 iLastRunModeStage;
  int                 iLogLevel;
  int                 iMaskTableLength;
  int                 iMatchLimit;
  int                 iRunMode;
  int                 iNextRunMode;
  int                 iTestLevel;
  int                 iTestRunMode;
  int                 iURLAuthType;
  int               (*piDevelopDigOutput)();
  int               (*piDevelopMapOutput)();
  int               (*piRunModeFinalStage)();
  int               (*piRunModeProcessArguments)();
  MASK_TABLE         *psMaskTable;
#ifdef USE_SSL
  SSL_PROPERTIES     *psSSLProperties;
#endif
  MD5_CONTEXT         sOutFileHashContext;
  time_t              tStartTime;
  unsigned long       ulFieldMask;
  unsigned long       ulFileSizeLimit;
} FTIMES_PROPERTIES;

/*-
 ***********************************************************************
 *
 * Function Prototypes (analyze.c)
 *
 ***********************************************************************
 */
int                 AnalyzeFile(FTIMES_PROPERTIES *psProperties, FTIMES_FILE_DATA *psFTData, char *pcError);
int                 AnalyzeDoDigest(unsigned char *pucBuffer, int iBufferLength, int iBufferType, int iBufferOverhead, FTIMES_FILE_DATA *psFTData, char *pcError);
int                 AnalyzeDoDig(unsigned char *pucBuffer, int iBufferLength, int iBufferType, int iBufferOverhead, FTIMES_FILE_DATA *psFTData, char *pcError);
int                 AnalyzeDoXMagic(unsigned char *pucBuffer, int iBufferLength, int iBufferType, int iBufferOverhead, FTIMES_FILE_DATA *psFTData, char *pcError);

void                AnalyzeEnableDigestEngine(FTIMES_PROPERTIES *psProperties);
int                 AnalyzeEnableXMagicEngine(FTIMES_PROPERTIES *psProperties, char *pcError);
void                AnalyzeEnableDigEngine(FTIMES_PROPERTIES *psProperties);
K_UINT32            AnalyzeGetFileCount(void);
K_UINT64            AnalyzeGetByteCount(void);

/*-
 ***********************************************************************
 *
 * Function Prototypes (compare.c)
 *
 ***********************************************************************
 */
int                 CompareParseStringMask(char *pcMask, unsigned long *ulMask, int iRunMode, MASK_TABLE *psMaskTable, int iMaskTableLength, char *pcError);

/*-
 ***********************************************************************
 *
 * Function Prototypes (develop.c)
 *
 ***********************************************************************
 */
int                 DevelopNoOutput(FTIMES_PROPERTIES *psProperties, char *pcOutData, int *iWriteCount, FTIMES_FILE_DATA *psFTData, char *pcError);
int                 DevelopNormalOutput(FTIMES_PROPERTIES *psProperties, char *pcOutData, int *iWriteCount, FTIMES_FILE_DATA *psFTData, char *pcError);
int                 DevelopCompressedOutput(FTIMES_PROPERTIES *psProperties, char *pcOutData, int *iWriteCount, FTIMES_FILE_DATA *psFTData, char *pcError);
int                 DevelopCompressHex(unsigned char *pcData, unsigned long ulHex, unsigned long ulOldHex);
int                 DevelopCountHexDigits(unsigned long ulHex);

/*-
 ***********************************************************************
 *
 * Function Prototypes (ftimes.c)
 *
 ***********************************************************************
 */
int                 main(int iArgumentCount, char *ppcArgumentVector[]);
int                 FTimesBootstrap(char *pcError);
int                 FTimesProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);
int                 FTimesStagesLoop(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 FTimesFinalStage(FTIMES_PROPERTIES *psProperties, char *pcError);
void                FTimesDisplayStatistics(FTIMES_PROPERTIES *psProperties);

void                FTimesUsage(void);
int                 FTimesVersion(FTIMES_PROPERTIES *psProperties, char *pcError);
void                FTimesSetPropertiesReference(FTIMES_PROPERTIES *psProperties);
FTIMES_PROPERTIES  *FTimesGetPropertiesReference(void);
FTIMES_PROPERTIES  *FTimesNewProperties(char *pcError);
int                 FTimesCreateConfigFile(FTIMES_PROPERTIES *psProperties, char *pcError);
void                FTimesEraseFiles(FTIMES_PROPERTIES *psProperties, char *pcError);
#ifdef USE_SSL
int                 SSLCheckDependencies(SSL_PROPERTIES *psProperties, char *pcError);
#endif

/*-
 ***********************************************************************
 *
 * Function Prototypes (xxxmode.c)
 *
 ***********************************************************************
 */
int                 CfgTestProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);
int                 CmpModeProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);
int                 DecoderProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);
int                 DigModeProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);
int                 GetModeProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);
int                 MapModeProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);
int                 PutModeProcessArguments(FTIMES_PROPERTIES *psProperties, int iArgumentCount, char *ppcArgumentVector[], char *pcError);

int                 CmpModeInitialize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DecoderInitialize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DigModeInitialize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 GetModeInitialize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 MapModeInitialize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PutModeInitialize(FTIMES_PROPERTIES *psProperties, char *pcError);

int                 CmpModeCheckDependencies(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DecoderCheckDependencies(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DigModeCheckDependencies(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 GetModeCheckDependencies(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 MapModeCheckDependencies(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PutModeCheckDependencies(FTIMES_PROPERTIES *psProperties, char *pcError);

int                 CmpModeFinalize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DecoderFinalize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DigModeFinalize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 GetModeFinalize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 MapModeFinalize(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PutModeFinalize(FTIMES_PROPERTIES *psProperties, char *pcError);

int                 CmpModeWorkHorse(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DecoderWorkHorse(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DigModeWorkHorse(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 GetModeWorkHorse(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 MapModeWorkHorse(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PutModeWorkHorse(FTIMES_PROPERTIES *psProperties, char *pcError);

int                 CmpModeFinishUp(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DecoderFinishUp(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DigModeFinishUp(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 GetModeFinishUp(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 MapModeFinishUp(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PutModeFinishUp(FTIMES_PROPERTIES *psProperties, char *pcError);

int                 CmpModeFinalStage(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DecoderFinalStage(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 DigModeFinalStage(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 GetModeFinalStage(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 MapModeFinalStage(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PutModeFinalStage(FTIMES_PROPERTIES *psProperties, char *pcError);

/*-
 ***********************************************************************
 *
 * Function Prototypes (map.c)
 *
 ***********************************************************************
 */
int                 MapFile(FTIMES_PROPERTIES *psProperties, char *pcPath, char *pcError);
int                 MapGetAttributes(FTIMES_FILE_DATA *psFTData, char *pcError);
int                 MapGetDirectoryCount();
int                 MapGetFileCount();
int                 MapGetRecordCount();
int                 MapGetSpecialCount();
int                 MapGetIncompleteRecordCount();
int                 MapTree(FTIMES_PROPERTIES *psProperties, char *pcPath, int iFSType, unsigned char *pucTreeHash, char *pcError);
int                 MapWriteHeader(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 MapWriteRecord(FTIMES_PROPERTIES *psProperties, FTIMES_FILE_DATA *psFTData, char *pcError);
#ifdef WINNT
int                 MapCountNamedStreams(HANDLE hFile, int *piStreamCount, unsigned char **ppucStreamInfo, char *pcError);
int                 MapGetStreamCount();
void                MapStream(FTIMES_PROPERTIES *psProperties, FTIMES_FILE_DATA *psFTData, MD5_CONTEXT *psDirHashBlock, char *pcError);
#endif
#ifdef WIN32
HANDLE              MapGetFileHandle(char *path);
#endif

/*-
 ***********************************************************************
 *
 * Function Prototypes (properties.c)
 *
 ***********************************************************************
 */
void                PropertiesDisplaySettings(FTIMES_PROPERTIES *psProperties);
int                 PropertiesReadFile(char *pcFilename, FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PropertiesReadLine(char *pcLine, FTIMES_PROPERTIES *psProperties, char *pcError);
int                 PropertiesTestFile(FTIMES_PROPERTIES *psProperties, char *pcError);

/*-
 ***********************************************************************
 *
 * Function Prototypes (support.c)
 *
 ***********************************************************************
 */
FILE_LIST          *SupportAddListItem(char *pcPath, FILE_LIST *psHead, char *pcError);
int                 SupportAddToList(char *pcPath, FILE_LIST **ppList, char *pcError);
#ifdef WIN32
BOOL                SupportAdjustPrivileges(LPCTSTR lpcPrivilege);
#endif
int                 SupportChopEOLs(char *pcLine, int iStrict, char *pcError);
void                SupportDisplayRunStatistics(FTIMES_PROPERTIES *psProperties);
FILE_LIST          *SupportDropListItem(FILE_LIST *psHead, FILE_LIST *psDrop);
int                 SupportEraseFile(char *pcName, char *pcError);
int                 SupportExpandDirectoryPath(char *pcPath, char *pcFullPath, int iFullPathSize, char *pcError);
int                 SupportExpandPath(char *pcPath, char *pcFullPath, int iFullPathSize, int iForceExpansion, char *pcError);
void                SupportFreeData(void *pcData);
int                 SupportGetFileType(char *pcPath);
char               *SupportGetHostname(void);
char               *SupportGetMyVersion(void);
char               *SupportGetSystemOS(void);
FILE_LIST          *SupportIncludeEverything(char *pcError);
int                 SupportMakeName(char *pcPath, char *pcBaseName, char *pcBaseNameSuffix, char *pcExtension, char *pcFilename, char *pcError);
FILE_LIST          *SupportMatchExclude(FILE_LIST *psHead, char *pcPath);
FILE_LIST          *SupportMatchSubTree(FILE_LIST *psHead, FILE_LIST *psTarget);
char               *SupportNeuterString(char *pcData, int iLength, char *pcError);
#ifdef WIN32
char               *SupportNeuterStringW(unsigned short *pusData, int iLength, char *pcError);
#endif
FILE_LIST          *SupportPruneList(FILE_LIST *psList);
int                 SupportRequirePrivilege(char *pcError);
int                 SupportSetLogLevel(char *pcLevel, int *piLevel, char *pcError);
#ifdef WIN32
int                 SupportSetPrivileges(char *pcError);
#endif
int                 SupportWriteData(FILE *pFile, char *pcData, int iLength, char *pcError);

/*-
 ***********************************************************************
 *
 * Function Prototypes (time.c)
 *
 ***********************************************************************
 */
time_t              TimeGetTime(char *pcDate, char *pcTime, char *pcZone, char *pcDateTime);
#ifdef WIN32
int                 TimeFormatTime(FILETIME *psFileTime, char *pcTime);
int                 TimeFormatOutOfBandTime(FILETIME *psFileTime, char *pcTime);
#endif
#ifdef UNIX
int                 TimeFormatTime(time_t *pTimeValue, char *pcTime);
#endif

/*-
 ***********************************************************************
 *
 * Function Prototypes (url.c)
 *
 ***********************************************************************
 */
int                 URLGetRequest(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 URLPingRequest(FTIMES_PROPERTIES *psProperties, char *pcError);
int                 URLPutRequest(FTIMES_PROPERTIES *psProperties, char *pcError);

/*-
 ***********************************************************************
 *
 * Macros
 *
 ***********************************************************************
 */
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define PUTBIT(x, v, p) (x) = ((x) & ~(1 << (p))) | (((v) & 1)<< (p))
#define GETBIT(x, p) ((x) & (1 << (p))) >> (p)
#define MEMORY_FREE(pv) if (pv) { free(pv); }
#define RUN_MODE_IS_SET(mask, mode) (((mask) & (mode)) == (mode))

/*-
 ***********************************************************************
 *
 * External Variables
 *
 ***********************************************************************
 */
#ifdef WINNT
extern NQIF         NtdllNQIF;
#endif
