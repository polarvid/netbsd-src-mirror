/******************************************************************************
 *
 * Module Name: aslcompiler.h - common include file for iASL
 *
 *****************************************************************************/

/*
 * Copyright (C) 2000 - 2023, Intel Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#ifndef __ASLCOMPILER_H
#define __ASLCOMPILER_H

#include "acpi.h"
#include "accommon.h"
#include "amlresrc.h"
#include "acdebug.h"

/* Microsoft-specific */

#if (defined WIN32 || defined WIN64)

/* warn : used #pragma pack */
#pragma warning(disable:4103)

/* warn : named type definition in parentheses */
#pragma warning(disable:4115)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

/* Compiler headers */

#include "asldefine.h"
#include "asltypes.h"
#include "aslmessages.h"
#include "aslglobal.h"
#include "preprocess.h"
#include "dtcompiler.h"


/*******************************************************************************
 *
 * Compiler prototypes
 *
 ******************************************************************************/

/*
 * Main ASL parser - generated from flex/bison, lex/yacc, etc.
 */
ACPI_PARSE_OBJECT *
AslDoError (
    void);

int
AslCompilerlex(
    void);

void
AslResetCurrentLineBuffer (
    void);

void
AslInsertLineBuffer (
    int                     SourceChar);

int
AslPopInputFileStack (
    void);

void
AslPushInputFileStack (
    FILE                    *InputFile,
    char                    *Filename);

void
AslParserCleanup (
    void);


/*
 * aslstartup - entered from main()
 */
void
AslInitializeGlobals (
    void);

typedef
ACPI_STATUS (*ASL_PATHNAME_CALLBACK) (
    char *);

ACPI_STATUS
AslDoOneFile (
    char                    *Filename);

ACPI_STATUS
AslCheckForErrorExit (
    void);


/*
 * aslcompile - compile mainline
 */
void
AslCompilerSignon (
    UINT32                  FileId);

void
AslCompilerFileHeader (
    UINT32                  FileId);

ACPI_STATUS
CmDoCompile (
    void);

int
CmDoAslMiddleAndBackEnd (
    void);

void
CmDoOutputFiles (
    void);

int
CmCleanupAndExit (
    void);

ACPI_STATUS
AslDoDisassembly (
    void);


/*
 * aslallocate - memory allocation
 */
void *
UtLocalCalloc (
    UINT32                  Size);

void
UtExpandLineBuffers (
    void);

void
UtReallocLineBuffers (
    char                    **Buffer,
    UINT32                  OldSize,
    UINT32                  NewSize);

void
UtFreeLineBuffers (
    void);


/*
 * aslcache - local cache support
 */
char *
UtLocalCacheCalloc (
    UINT32                  Length);

ACPI_PARSE_OBJECT *
UtParseOpCacheCalloc (
    void);

DT_SUBTABLE *
UtSubtableCacheCalloc (
    void);

DT_FIELD *
UtFieldCacheCalloc (
    void);

void
UtDeleteLocalCaches (
    void);


/*
 * aslascii - ascii support
 */
ACPI_STATUS
FlIsFileAsciiSource (
    char                    *Filename,
    BOOLEAN                 DisplayErrors);


/*
 * aslwalks - semantic analysis and parse tree walks
 */
ACPI_STATUS
AnOtherSemanticAnalysisWalkBegin (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
AnOtherSemanticAnalysisWalkEnd (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
AnOperandTypecheckWalkEnd (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
AnMethodTypingWalkEnd (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);


/*
 * aslmethod - Control method analysis walk
 */
ACPI_STATUS
MtMethodAnalysisWalkBegin (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
MtMethodAnalysisWalkEnd (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

UINT32
MtProcessTypeOp (
    ACPI_PARSE_OBJECT       *TypeOp);

UINT8
MtProcessParameterTypeList (
    ACPI_PARSE_OBJECT       *ParamTypeOp,
    UINT32                  *TypeList);


/*
 * aslbtypes - bitfield data types
 */
UINT32
AnMapObjTypeToBtype (
    ACPI_PARSE_OBJECT       *Op);

UINT32
AnMapArgTypeToBtype (
    UINT32                  ArgType);

UINT32
AnGetBtype (
    ACPI_PARSE_OBJECT       *Op);

void
AnFormatBtype (
    char                    *Buffer,
    UINT32                  Btype);


/*
 * aslanalyze - Support functions for parse tree walks
 */
void
AnCheckId (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_NAME               Type);

/* Values for Type argument above */

#define ASL_TYPE_HID        0
#define ASL_TYPE_CID        1

BOOLEAN
AnIsInternalMethod (
    ACPI_PARSE_OBJECT       *Op);

UINT32
AnGetInternalMethodReturnType (
    ACPI_PARSE_OBJECT       *Op);

BOOLEAN
AnLastStatementIsReturn (
    ACPI_PARSE_OBJECT       *Op);

void
AnCheckMethodReturnValue (
    ACPI_PARSE_OBJECT       *Op,
    const ACPI_OPCODE_INFO  *OpInfo,
    ACPI_PARSE_OBJECT       *ArgOp,
    UINT32                  RequiredBtypes,
    UINT32                  ThisNodeBtype);

BOOLEAN
AnIsResultUsed (
    ACPI_PARSE_OBJECT       *Op);

void
ApCheckForGpeNameConflict (
    ACPI_PARSE_OBJECT       *Op);

void
ApCheckRegMethod (
    ACPI_PARSE_OBJECT       *Op);

BOOLEAN
ApFindNameInScope (
    char                    *Name,
    ACPI_PARSE_OBJECT       *Op);

BOOLEAN
ApFindNameInDeviceTree (
    char                    *Name,
    ACPI_PARSE_OBJECT       *Op);

/*
 * aslerror - error handling/reporting
 */
void
AslAbort (
    void);

void
AslDualParseOpError (
    UINT8                   Level,
    UINT16                  MainMessageId,
    ACPI_PARSE_OBJECT       *MainOp,
    char                    *MainMessage,
    UINT16                  SecondMessageId,
    ACPI_PARSE_OBJECT       *SecondOp,
    char                    *SecondaryMessage);

void
AslError (
    UINT8                   Level,
    UINT16                  MessageId,
    ACPI_PARSE_OBJECT       *Op,
    char                    *ExtraMessage);

void
AslCheckExpectedExceptions (
    void);

ACPI_STATUS
AslLogExpectedException (
    char                    *MessageIdString);

ACPI_STATUS
AslElevateException (
    char                    *MessageIdString);

ACPI_STATUS
AslDisableException (
    char                    *MessageIdString);

BOOLEAN
AslIsExceptionIgnored (
    char                    *Filename,
    UINT32                  LineNumber,
    UINT8                   Level,
    UINT16                  MessageId);

void
AslLogExpectedExceptionByLine (
    char                    *MessageIdString);

void
AslCoreSubsystemError (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_STATUS             Status,
    char                    *ExtraMessage,
    BOOLEAN                 Abort);

int
AslCompilererror(
    const char              *s);

void
AslCommonError (
    UINT8                   Level,
    UINT16                  MessageId,
    UINT32                  CurrentLineNumber,
    UINT32                  LogicalLineNumber,
    UINT32                  LogicalByteOffset,
    UINT32                  Column,
    char                    *Filename,
    char                    *ExtraMessage);

void
AslCommonError2 (
    UINT8                   Level,
    UINT16                  MessageId,
    UINT32                  LineNumber,
    UINT32                  Column,
    char                    *SourceLine,
    char                    *Filename,
    char                    *ExtraMessage);

void
AePrintException (
    UINT32                  FileId,
    ASL_ERROR_MSG           *Enode,
    char                    *Header);

void
AePrintErrorLog (
    UINT32                  FileId);

void
AeClearErrorLog (
    void);


/*
 * asllisting - generate all "listing" type files
 */
void
LsDoListings (
    void);

void
LsWriteNodeToAsmListing (
    ACPI_PARSE_OBJECT       *Op);

void
LsWriteNode (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  FileId);

void
LsDumpParseTree (
    void);


/*
 * asllistsup - Listing file support utilities
 */
void
LsDumpAscii (
    UINT32                  FileId,
    UINT32                  Count,
    UINT8                   *Buffer);

void
LsDumpAsciiInComment (
    UINT32                  FileId,
    UINT32                  Count,
    UINT8                   *Buffer);

void
LsCheckException (
    UINT32                  LineNumber,
    UINT32                  FileId);

void
LsFlushListingBuffer (
    UINT32                  FileId);

void
LsWriteListingHexBytes (
    UINT8                   *Buffer,
    UINT32                  Length,
    UINT32                  FileId);

void
LsWriteSourceLines (
    UINT32                  ToLineNumber,
    UINT32                  ToLogicalLineNumber,
    UINT32                  FileId);

UINT32
LsWriteOneSourceLine (
    UINT32                  FileId);

void
LsPushNode (
    char                    *Filename);

ASL_LISTING_NODE *
LsPopNode (
    void);


/*
 * aslhex - generate all "hex" output files (C, ASM, ASL)
 */
void
HxDoHexOutput (
    void);


/*
 * aslfold - constant folding
 */
ACPI_STATUS
OpcAmlConstantWalk (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);


/*
 * aslmessages - exception strings
 */
const char *
AeDecodeMessageId (
    UINT16                  MessageId);

const char *
AeDecodeExceptionLevel (
    UINT8                   Level);

UINT16
AeBuildFullExceptionCode (
    UINT8                   Level,
    UINT16                  MessageId);

/*
 * asloffset - generate C offset file for BIOS support
 */
ACPI_STATUS
LsAmlOffsetWalk (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

void
LsDoOffsetTableHeader (
    UINT32                  FileId);

void
LsDoOffsetTableFooter (
    UINT32                  FileId);


/*
 * aslopcodes - generate AML opcodes
 */
ACPI_STATUS
OpcAmlOpcodeWalk (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
OpcAmlOpcodeUpdateWalk (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

void
OpcGenerateAmlOpcode (
    ACPI_PARSE_OBJECT       *Op);

UINT32
OpcSetOptimalIntegerSize (
    ACPI_PARSE_OBJECT       *Op);

void
OpcGetIntegerWidth (
    ACPI_PARSE_OBJECT       *Op);


/*
 * asloperands - generate AML operands for the AML opcodes
 */
ACPI_PARSE_OBJECT  *
UtGetArg (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Argn);

void
OpnGenerateAmlOperands (
    ACPI_PARSE_OBJECT       *Op);

void
OpnDoPackage (
    ACPI_PARSE_OBJECT       *Op);


/*
 * aslopt - optimization
 */
void
OptOptimizeNamePath (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Flags,
    ACPI_WALK_STATE         *WalkState,
    char                    *AmlNameString,
    ACPI_NAMESPACE_NODE     *TargetNode);


/*
 * aslpld - ToPLD macro support
 */
void
OpcDoPld (
    ACPI_PARSE_OBJECT       *Op);


/*
 * aslprintf - Printf/Fprintf macros
 */
void
OpcDoPrintf (
    ACPI_PARSE_OBJECT       *Op);

void
OpcDoFprintf (
    ACPI_PARSE_OBJECT       *Op);


/*
 * aslprune - parse tree pruner
 */
void
AslPruneParseTree (
    UINT32                  PruneDepth,
    UINT32                  Type);


/*
 * aslcodegen - code generation
 */
void
CgGenerateAmlOutput (
    void);

void
CgLocalWriteAmlData (
    ACPI_PARSE_OBJECT       *Op,
    void                    *Buffer,
    UINT32                  Length);


/*
 * aslfile
 */
void
FlOpenFile (
    UINT32                  FileId,
    char                    *Filename,
    char                    *Mode);


/*
 * asllength - calculate/adjust AML package lengths
 */
ACPI_STATUS
LnPackageLengthWalk (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
LnInitLengthsWalk (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

void
CgGenerateAmlLengths (
    ACPI_PARSE_OBJECT       *Op);


/*
 * aslmap - opcode mappings and reserved method names
 */
ACPI_OBJECT_TYPE
AslMapNamedOpcodeToDataType (
    UINT16                  Opcode);


/*
 * aslpredef - ACPI predefined names support
 */
BOOLEAN
ApCheckForPredefinedMethod (
    ACPI_PARSE_OBJECT       *Op,
    ASL_METHOD_INFO         *MethodInfo);

void
ApCheckPredefinedReturnValue (
    ACPI_PARSE_OBJECT       *Op,
    ASL_METHOD_INFO         *MethodInfo);

UINT32
ApCheckForPredefinedName (
    ACPI_PARSE_OBJECT       *Op,
    char                    *Name);

void
ApCheckForPredefinedObject (
    ACPI_PARSE_OBJECT       *Op,
    char                    *Name);

ACPI_STATUS
ApCheckObjectType (
    const char              *PredefinedName,
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  ExpectedBtypes,
    UINT32                  PackageIndex);

void
ApDisplayReservedNames (
    void);


/*
 * aslprepkg - ACPI predefined names support for packages
 */
void
ApCheckPackage (
    ACPI_PARSE_OBJECT           *ParentOp,
    const ACPI_PREDEFINED_INFO  *Predefined);


/*
 * asltransform - parse tree transformations
 */
ACPI_STATUS
TrAmlTransformWalkBegin (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
TrAmlTransformWalkEnd (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);


/*
 * aslexternal - External opcode support
 */
ACPI_STATUS
ExAmlExternalWalkBegin (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

ACPI_STATUS
ExAmlExternalWalkEnd (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    void                    *Context);

void
ExDoExternal (
    ACPI_PARSE_OBJECT       *Op);

/* Values for "Visitation" parameter above */

#define ASL_WALK_VISIT_DOWNWARD         0x01
#define ASL_WALK_VISIT_UPWARD           0x02
#define ASL_WALK_VISIT_DB_SEPARATELY    0x04
#define ASL_WALK_VISIT_TWICE            (ASL_WALK_VISIT_DOWNWARD | ASL_WALK_VISIT_UPWARD)


/*
 * aslparseop.c - Parse op create/allocate/cache
 */
ACPI_PARSE_OBJECT *
TrCreateOp (
    UINT32                  ParseOpcode,
    UINT32                  NumChildren,
    ...);

ACPI_PARSE_OBJECT *
TrCreateLeafOp (
    UINT32                  ParseOpcode);

ACPI_PARSE_OBJECT *
TrCreateNullTargetOp (
    void);

ACPI_PARSE_OBJECT *
TrCreateAssignmentOp (
    ACPI_PARSE_OBJECT       *Target,
    ACPI_PARSE_OBJECT       *Source);

ACPI_PARSE_OBJECT *
TrCreateTargetOp (
    ACPI_PARSE_OBJECT       *OriginalOp,
    ACPI_PARSE_OBJECT       *ParentOp);

ACPI_PARSE_OBJECT *
TrCreateValuedLeafOp (
    UINT32                  ParseOpcode,
    UINT64                  Value);

ACPI_PARSE_OBJECT *
TrCreateConstantLeafOp (
    UINT32                  ParseOpcode);

ACPI_PARSE_OBJECT *
TrAllocateOp (
    UINT32                  ParseOpcode);

void
TrPrintOpFlags (
    UINT32                  Flags,
    UINT32                  OutputLevel);


/*
 * asltree.c - Parse tree management
 */
void
TrSetOpParent (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_PARSE_OBJECT       *ParentOp);

ACPI_PARSE_OBJECT *
TrSetOpIntegerValue (
    UINT32                  ParseOpcode,
    ACPI_PARSE_OBJECT       *Op);

void
TrSetOpEndLineNumber (
    ACPI_PARSE_OBJECT       *Op);

void
TrSetOpCurrentFilename (
    ACPI_PARSE_OBJECT       *Op);

void
TrSetOpIntegerWidth (
    ACPI_PARSE_OBJECT       *TableSignature,
    ACPI_PARSE_OBJECT       *Revision);

ACPI_PARSE_OBJECT *
TrLinkOpChildren (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  NumChildren,
    ...);

ACPI_PARSE_OBJECT *
TrLinkPeerOp (
    ACPI_PARSE_OBJECT       *Op1,
    ACPI_PARSE_OBJECT       *Op2);

ACPI_PARSE_OBJECT *
TrLinkChildOp (
    ACPI_PARSE_OBJECT       *Op1,
    ACPI_PARSE_OBJECT       *Op2);

ACPI_PARSE_OBJECT *
TrSetOpFlags (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Flags);

ACPI_PARSE_OBJECT *
TrSetOpAmlLength (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Length);

ACPI_PARSE_OBJECT *
TrLinkPeerOps (
    UINT32                  NumPeers,
    ...);

ACPI_STATUS
TrWalkParseTree (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Visitation,
    ASL_WALK_CALLBACK       DescendingCallback,
    ASL_WALK_CALLBACK       AscendingCallback,
    void                    *Context);


/*
 * aslfiles - File I/O support
 */
void
FlAddIncludeDirectory (
    char                    *Dir);

char *
FlMergePathnames (
    char                    *PrefixDir,
    char                    *FilePathname);

void
FlOpenIncludeFile (
    ACPI_PARSE_OBJECT       *Op);

void
FlFileError (
    UINT32                  FileId,
    UINT8                   ErrorId);

UINT32
FlGetFileSize (
    UINT32                  FileId);

ACPI_STATUS
FlReadFile (
    UINT32                  FileId,
    void                    *Buffer,
    UINT32                  Length);

void
FlWriteFile (
    UINT32                  FileId,
    void                    *Buffer,
    UINT32                  Length);

void
FlSeekFile (
    UINT32                  FileId,
    long                    Offset);

void
FlSeekFileSet (
    UINT32                  FileId,
    long                    Offset);

void
FlCloseFile (
    UINT32                  FileId);

ACPI_PRINTF_LIKE (2)
void
FlPrintFile (
    UINT32                  FileId,
    char                    *Format,
    ...);

void
FlDeleteFile (
    UINT32                  FileId);

void
FlSetLineNumber (
    UINT32                  LineNumber);

void
FlSetFilename (
    char                    *Filename);

ACPI_STATUS
FlOpenInputFile (
    char                    *InputFilename);

ACPI_STATUS
FlOpenAmlOutputFile (
    char                    *InputFilename);

ACPI_STATUS
FlOpenMiscOutputFiles (
    char                    *InputFilename);

ACPI_STATUS
FlInitOneFile (
    char                    *InputFilename);

ASL_FILE_SWITCH_STATUS
FlSwitchFileSet (
    char                    *InputFilename);

FILE *
FlGetFileHandle (
    UINT32                  OutFileId,
    UINT32                  InFileId,
    char                    *Filename);

ASL_GLOBAL_FILE_NODE *
FlGetFileNode (
    UINT32                  FileId,
    char                    *Filename);

ASL_GLOBAL_FILE_NODE *
FlGetCurrentFileNode (
    void);


/*
 * aslhwmap - hardware map summary
 */
void
MpEmitMappingInfo (
    void);


/*
 * asload - load namespace in prep for cross reference
 */
ACPI_STATUS
LdLoadNamespace (
    ACPI_PARSE_OBJECT       *RootOp);

/*
 * asllookup - namespace lookup functions
 */
void
LkFindUnreferencedObjects (
    void);


/*
 * aslhelp - help screens
 */
void
Usage (
    void);

void
AslFilenameHelp (
    void);

void
AslDisassemblyHelp (
    void);


/*
 * aslnamesp - namespace output file generation
 */
ACPI_STATUS
NsDisplayNamespace (
    void);

void
NsSetupNamespaceListing (
    void                    *Handle);


/*
 * asloptions - command line processing
 */
int
AslCommandLine (
    int                     argc,
    char                    **argv);


/*
 * aslxref - namespace cross reference
 */
ACPI_STATUS
XfCrossReferenceNamespace (
    void);


/*
 * aslxrefout
 */
void
OtPrintHeaders (
    char                    *Message);

void
OtCreateXrefFile (
    void);

void
OtXrefWalkPart1 (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    ASL_METHOD_INFO         *MethodInfo);


/*
 * aslutils - common compiler utilities
 */
ACPI_PRINTF_LIKE(2)
void
DbgPrint (
    UINT32                  Type,
    char                    *Format,
    ...);

/* Type values for above */

#define ASL_DEBUG_OUTPUT    0
#define ASL_PARSE_OUTPUT    1
#define ASL_TREE_OUTPUT     2

BOOLEAN
UtIsIdInteger (
    UINT8                   *Target);

UINT8
UtIsBigEndianMachine (
    void);

BOOLEAN
UtQueryForOverwrite (
    char                    *Pathname);

void
UtDumpStringOp (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level);

void
UtDumpIntegerOp (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level,
    UINT32                  IntegerLength);

void
UtDumpBasicOp (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level);

ACPI_NAMESPACE_NODE *
UtGetParentMethodNode (
    ACPI_NAMESPACE_NODE     *Node);

ACPI_PARSE_OBJECT *
UtGetParentMethodOp (
    ACPI_PARSE_OBJECT       *Op);

BOOLEAN
UtNodeIsDescendantOf (
    ACPI_NAMESPACE_NODE     *Node1,
    ACPI_NAMESPACE_NODE     *Node2);

void
UtDisplaySupportedTables (
    void);

void
UtDisplayConstantOpcodes (
    void);

UINT8
UtBeginEvent (
    char                    *Name);

void
UtEndEvent (
    UINT8                   Event);

void
UtDisplaySummary (
    UINT32                  FileId);

void
UtDisplayOneSummary (
    UINT32                  FileId,
    BOOLEAN                 DisplayErrorSummary);

void
UtConvertByteToHex (
    UINT8                   RawByte,
    UINT8                   *Buffer);

void
UtConvertByteToAsmHex (
    UINT8                   RawByte,
    UINT8                   *Buffer);

char *
UtGetOpName (
    UINT32                  ParseOpcode);

void
UtSetParseOpName (
    ACPI_PARSE_OBJECT       *Op);

ACPI_STATUS
UtInternalizeName (
    char                    *ExternalName,
    char                    **ConvertedName);

BOOLEAN
UtNameContainsAllPrefix (
    ACPI_PARSE_OBJECT       *Op);

void
UtAttachNamepathToOwner (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_PARSE_OBJECT       *NameNode);

ACPI_PARSE_OBJECT *
UtCheckIntegerRange (
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  LowValue,
    UINT32                  HighValue);

UINT64
UtDoConstant (
    char                    *String);

char *
AcpiUtStrdup (
    char                    *String);

char *
AcpiUtStrcat (
    char                    *String1,
    char                    *String2);


/*
 * asluuid - UUID support
 */
ACPI_STATUS
AuValidateUuid (
    char                    *InString);

ACPI_STATUS
AuConvertUuidToString (
    char                    *UuIdBuffer,
    char                    *OutString);


/*
 * aslresource - Resource template generation utilities
 */
void
RsSmallAddressCheck (
    UINT8                   Type,
    UINT32                  Minimum,
    UINT32                  Maximum,
    UINT32                  Length,
    UINT32                  Alignment,
    ACPI_PARSE_OBJECT       *MinOp,
    ACPI_PARSE_OBJECT       *MaxOp,
    ACPI_PARSE_OBJECT       *LengthOp,
    ACPI_PARSE_OBJECT       *AlignOp,
    ACPI_PARSE_OBJECT       *Op);

void
RsLargeAddressCheck (
    UINT64                  Minimum,
    UINT64                  Maximum,
    UINT64                  Length,
    UINT64                  Granularity,
    UINT8                   Flags,
    ACPI_PARSE_OBJECT       *MinOp,
    ACPI_PARSE_OBJECT       *MaxOp,
    ACPI_PARSE_OBJECT       *LengthOp,
    ACPI_PARSE_OBJECT       *GranOp,
    ACPI_PARSE_OBJECT       *Op);

UINT16
RsGetStringDataLength (
    ACPI_PARSE_OBJECT       *InitializerOp);

ASL_RESOURCE_NODE *
RsAllocateResourceNode (
    UINT32                  Size);

void
RsCreateResourceField (
    ACPI_PARSE_OBJECT       *Op,
    char                    *Name,
    UINT32                  ByteOffset,
    UINT32                  BitOffset,
    UINT32                  BitLength);

void
RsSetFlagBits (
    UINT8                   *Flags,
    ACPI_PARSE_OBJECT       *Op,
    UINT8                   Position,
    UINT8                   DefaultBit);

void
RsSetFlagBits16 (
    UINT16                  *Flags,
    ACPI_PARSE_OBJECT       *Op,
    UINT8                   Position,
    UINT8                   DefaultBit);

ACPI_PARSE_OBJECT *
RsCompleteNodeAndGetNext (
    ACPI_PARSE_OBJECT       *Op);

void
RsCheckListForDuplicates (
    ACPI_PARSE_OBJECT       *Op);

ASL_RESOURCE_NODE *
RsDoOneResourceDescriptor (
    ASL_RESOURCE_INFO       *Info,
    UINT8                   *State);

/* Values for State above */

#define ACPI_RSTATE_NORMAL              0
#define ACPI_RSTATE_START_DEPENDENT     1
#define ACPI_RSTATE_DEPENDENT_LIST      2

UINT32
RsLinkDescriptorChain (
    ASL_RESOURCE_NODE       **PreviousRnode,
    ASL_RESOURCE_NODE       *Rnode);

void
RsDoResourceTemplate (
    ACPI_PARSE_OBJECT       *Op);


/*
 * aslrestype1 - Miscellaneous Small descriptors
 */
ASL_RESOURCE_NODE *
RsDoEndTagDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoEndDependentDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoMemory24Descriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoMemory32Descriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoMemory32FixedDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoStartDependentDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoStartDependentNoPriDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoVendorSmallDescriptor (
    ASL_RESOURCE_INFO       *Info);


/*
 * aslrestype1i - I/O-related Small descriptors
 */
ASL_RESOURCE_NODE *
RsDoDmaDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoFixedDmaDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoFixedIoDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoIoDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoIrqDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoIrqNoFlagsDescriptor (
    ASL_RESOURCE_INFO       *Info);


/*
 * aslrestype2 - Large resource descriptors
 */
ASL_RESOURCE_NODE *
RsDoInterruptDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoVendorLargeDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoGeneralRegisterDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoGpioIntDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoGpioIoDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoI2cSerialBusDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoSpiSerialBusDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoUartSerialBusDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoCsi2SerialBusDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoPinFunctionDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoPinConfigDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoPinGroupDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoPinGroupFunctionDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoPinGroupConfigDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoClockInputDescriptor (
    ASL_RESOURCE_INFO       *Info);

/*
 * aslrestype2d - DWord address descriptors
 */
ASL_RESOURCE_NODE *
RsDoDwordIoDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoDwordMemoryDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoDwordPccDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoDwordSpaceDescriptor (
    ASL_RESOURCE_INFO       *Info);

/*
 * aslrestype2e - Extended address descriptors
 */
ASL_RESOURCE_NODE *
RsDoExtendedIoDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoExtendedMemoryDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoExtendedSpaceDescriptor (
    ASL_RESOURCE_INFO       *Info);


/*
 * aslrestype2q - QWord address descriptors
 */
ASL_RESOURCE_NODE *
RsDoQwordIoDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoQwordMemoryDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoQwordPccDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoQwordSpaceDescriptor (
    ASL_RESOURCE_INFO       *Info);


/*
 * aslrestype2w - Word address descriptors
 */
ASL_RESOURCE_NODE *
RsDoWordIoDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoWordPccDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoWordSpaceDescriptor (
    ASL_RESOURCE_INFO       *Info);

ASL_RESOURCE_NODE *
RsDoWordBusNumberDescriptor (
    ASL_RESOURCE_INFO       *Info);


/*
 * Entry to data table compiler subsystem
 */
ACPI_STATUS
DtDoCompile(
    void);

ACPI_STATUS
DtCreateTemplates (
    char                    **argv);


/*
 * ASL/ASL+ converter debug
 */
ACPI_PRINTF_LIKE (1)
void
CvDbgPrint (
    char                    *Fmt,
    ...);


#endif /*  __ASLCOMPILER_H */
