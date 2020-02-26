#include <clang-c/Index.h>
#include <iostream>
#include <map>

namespace {
  unsigned EditingOptions() {
    // See cpp/llvm/include/clang-c/Index.h file for detail on these options.
    return CXTranslationUnit_DetailedPreprocessingRecord |
           CXTranslationUnit_Incomplete | 
           CXTranslationUnit_IncludeBriefCommentsInCodeCompletion |
           CXTranslationUnit_CreatePreambleOnFirstParse |
           CXTranslationUnit_KeepGoing |
           clang_defaultEditingTranslationUnitOptions();
  }

  std::string CXStringToString( CXString text ) {
    std::string final_string;

    if ( !text.data ) {
      return final_string;
    }

    final_string = std::string( clang_getCString( text ) );
    clang_disposeString( text );
    return final_string;
  }

  std::string CXLocationToString( CXSourceLocation sl ) {
    unsigned int ignored;
    CXFile file;
    unsigned int line;
    unsigned int column;
    clang_getExpansionLocation( sl, &file, &line, &column, &ignored );
    std::string o;
    o = CXStringToString( clang_getFileName( file ) );
    o += ":";
    o += std::to_string( line );
    o += ":";
    o += std::to_string( column );
    return o;
  }

  std::string CXTypeToString( CXType type )
  {
    std::string s = CXStringToString( clang_getTypeSpelling( type ) );
    if (s.empty()) {
      return "<unknown type>";
    }
    return s;
  }

  std::map<CXCursorKind, std::string> cursorKinds_ = {
    { CXCursor_UnexposedDecl                                      , "CXCursor_UnexposedDecl" },
    { CXCursor_StructDecl                                         , "CXCursor_StructDecl" },
    { CXCursor_UnionDecl                                          , "CXCursor_UnionDecl" },
    { CXCursor_ClassDecl                                          , "CXCursor_ClassDecl" },
    { CXCursor_EnumDecl                                           , "CXCursor_EnumDecl" },
    { CXCursor_FieldDecl                                          , "CXCursor_FieldDecl" },
    { CXCursor_EnumConstantDecl                                   , "CXCursor_EnumConstantDecl" },
    { CXCursor_FunctionDecl                                       , "CXCursor_FunctionDecl" },
    { CXCursor_VarDecl                                            , "CXCursor_VarDecl" },
    { CXCursor_ParmDecl                                           , "CXCursor_ParmDecl" },
    { CXCursor_ObjCInterfaceDecl                                  , "CXCursor_ObjCInterfaceDecl" },
    { CXCursor_ObjCCategoryDecl                                   , "CXCursor_ObjCCategoryDecl" },
    { CXCursor_ObjCProtocolDecl                                   , "CXCursor_ObjCProtocolDecl" },
    { CXCursor_ObjCPropertyDecl                                   , "CXCursor_ObjCPropertyDecl" },
    { CXCursor_ObjCIvarDecl                                       , "CXCursor_ObjCIvarDecl" },
    { CXCursor_ObjCInstanceMethodDecl                             , "CXCursor_ObjCInstanceMethodDecl" },
    { CXCursor_ObjCClassMethodDecl                                , "CXCursor_ObjCClassMethodDecl" },
    { CXCursor_ObjCImplementationDecl                             , "CXCursor_ObjCImplementationDecl" },
    { CXCursor_ObjCCategoryImplDecl                               , "CXCursor_ObjCCategoryImplDecl" },
    { CXCursor_TypedefDecl                                        , "CXCursor_TypedefDecl" },
    { CXCursor_CXXMethod                                          , "CXCursor_CXXMethod" },
    { CXCursor_Namespace                                          , "CXCursor_Namespace" },
    { CXCursor_LinkageSpec                                        , "CXCursor_LinkageSpec" },
    { CXCursor_Constructor                                        , "CXCursor_Constructor" },
    { CXCursor_Destructor                                         , "CXCursor_Destructor" },
    { CXCursor_ConversionFunction                                 , "CXCursor_ConversionFunction" },
    { CXCursor_TemplateTypeParameter                              , "CXCursor_TemplateTypeParameter" },
    { CXCursor_NonTypeTemplateParameter                           , "CXCursor_NonTypeTemplateParameter" },
    { CXCursor_TemplateTemplateParameter                          , "CXCursor_TemplateTemplateParameter" },
    { CXCursor_FunctionTemplate                                   , "CXCursor_FunctionTemplate" },
    { CXCursor_ClassTemplate                                      , "CXCursor_ClassTemplate" },
    { CXCursor_ClassTemplatePartialSpecialization                 , "CXCursor_ClassTemplatePartialSpecialization" },
    { CXCursor_NamespaceAlias                                     , "CXCursor_NamespaceAlias" },
    { CXCursor_UsingDirective                                     , "CXCursor_UsingDirective" },
    { CXCursor_UsingDeclaration                                   , "CXCursor_UsingDeclaration" },
    { CXCursor_TypeAliasDecl                                      , "CXCursor_TypeAliasDecl" },
    { CXCursor_ObjCSynthesizeDecl                                 , "CXCursor_ObjCSynthesizeDecl" },
    { CXCursor_ObjCDynamicDecl                                    , "CXCursor_ObjCDynamicDecl" },
    { CXCursor_CXXAccessSpecifier                                 , "CXCursor_CXXAccessSpecifier" },
    { CXCursor_FirstDecl                                          , "CXCursor_FirstDecl" },
    { CXCursor_LastDecl                                           , "CXCursor_LastDecl" },
    { CXCursor_FirstRef                                           , "CXCursor_FirstRef" },
    { CXCursor_ObjCSuperClassRef                                  , "CXCursor_ObjCSuperClassRef" },
    { CXCursor_ObjCProtocolRef                                    , "CXCursor_ObjCProtocolRef" },
    { CXCursor_ObjCClassRef                                       , "CXCursor_ObjCClassRef" },
    { CXCursor_TypeRef                                            , "CXCursor_TypeRef" },
    { CXCursor_CXXBaseSpecifier                                   , "CXCursor_CXXBaseSpecifier" },
    { CXCursor_TemplateRef                                        , "CXCursor_TemplateRef" },
    { CXCursor_NamespaceRef                                       , "CXCursor_NamespaceRef" },
    { CXCursor_MemberRef                                          , "CXCursor_MemberRef" },
    { CXCursor_LabelRef                                           , "CXCursor_LabelRef" },
    { CXCursor_OverloadedDeclRef                                  , "CXCursor_OverloadedDeclRef" },
    { CXCursor_VariableRef                                        , "CXCursor_VariableRef" },
    { CXCursor_LastRef                                            , "CXCursor_LastRef" },
    { CXCursor_FirstInvalid                                       , "CXCursor_FirstInvalid" },
    { CXCursor_InvalidFile                                        , "CXCursor_InvalidFile" },
    { CXCursor_NoDeclFound                                        , "CXCursor_NoDeclFound" },
    { CXCursor_NotImplemented                                     , "CXCursor_NotImplemented" },
    { CXCursor_InvalidCode                                        , "CXCursor_InvalidCode" },
    { CXCursor_LastInvalid                                        , "CXCursor_LastInvalid" },
    { CXCursor_FirstExpr                                          , "CXCursor_FirstExpr" },
    { CXCursor_UnexposedExpr                                      , "CXCursor_UnexposedExpr" },
    { CXCursor_DeclRefExpr                                        , "CXCursor_DeclRefExpr" },
    { CXCursor_MemberRefExpr                                      , "CXCursor_MemberRefExpr" },
    { CXCursor_CallExpr                                           , "CXCursor_CallExpr" },
    { CXCursor_ObjCMessageExpr                                    , "CXCursor_ObjCMessageExpr" },
    { CXCursor_BlockExpr                                          , "CXCursor_BlockExpr" },
    { CXCursor_IntegerLiteral                                     , "CXCursor_IntegerLiteral" },
    { CXCursor_FloatingLiteral                                    , "CXCursor_FloatingLiteral" },
    { CXCursor_ImaginaryLiteral                                   , "CXCursor_ImaginaryLiteral" },
    { CXCursor_StringLiteral                                      , "CXCursor_StringLiteral" },
    { CXCursor_CharacterLiteral                                   , "CXCursor_CharacterLiteral" },
    { CXCursor_ParenExpr                                          , "CXCursor_ParenExpr" },
    { CXCursor_UnaryOperator                                      , "CXCursor_UnaryOperator" },
    { CXCursor_ArraySubscriptExpr                                 , "CXCursor_ArraySubscriptExpr" },
    { CXCursor_BinaryOperator                                     , "CXCursor_BinaryOperator" },
    { CXCursor_CompoundAssignOperator                             , "CXCursor_CompoundAssignOperator" },
    { CXCursor_ConditionalOperator                                , "CXCursor_ConditionalOperator" },
    { CXCursor_CStyleCastExpr                                     , "CXCursor_CStyleCastExpr" },
    { CXCursor_CompoundLiteralExpr                                , "CXCursor_CompoundLiteralExpr" },
    { CXCursor_InitListExpr                                       , "CXCursor_InitListExpr" },
    { CXCursor_AddrLabelExpr                                      , "CXCursor_AddrLabelExpr" },
    { CXCursor_StmtExpr                                           , "CXCursor_StmtExpr" },
    { CXCursor_GenericSelectionExpr                               , "CXCursor_GenericSelectionExpr" },
    { CXCursor_GNUNullExpr                                        , "CXCursor_GNUNullExpr" },
    { CXCursor_CXXStaticCastExpr                                  , "CXCursor_CXXStaticCastExpr" },
    { CXCursor_CXXDynamicCastExpr                                 , "CXCursor_CXXDynamicCastExpr" },
    { CXCursor_CXXReinterpretCastExpr                             , "CXCursor_CXXReinterpretCastExpr" },
    { CXCursor_CXXConstCastExpr                                   , "CXCursor_CXXConstCastExpr" },
    { CXCursor_CXXFunctionalCastExpr                              , "CXCursor_CXXFunctionalCastExpr" },
    { CXCursor_CXXTypeidExpr                                      , "CXCursor_CXXTypeidExpr" },
    { CXCursor_CXXBoolLiteralExpr                                 , "CXCursor_CXXBoolLiteralExpr" },
    { CXCursor_CXXNullPtrLiteralExpr                              , "CXCursor_CXXNullPtrLiteralExpr" },
    { CXCursor_CXXThisExpr                                        , "CXCursor_CXXThisExpr" },
    { CXCursor_CXXThrowExpr                                       , "CXCursor_CXXThrowExpr" },
    { CXCursor_CXXNewExpr                                         , "CXCursor_CXXNewExpr" },
    { CXCursor_CXXDeleteExpr                                      , "CXCursor_CXXDeleteExpr" },
    { CXCursor_UnaryExpr                                          , "CXCursor_UnaryExpr" },
    { CXCursor_ObjCStringLiteral                                  , "CXCursor_ObjCStringLiteral" },
    { CXCursor_ObjCEncodeExpr                                     , "CXCursor_ObjCEncodeExpr" },
    { CXCursor_ObjCSelectorExpr                                   , "CXCursor_ObjCSelectorExpr" },
    { CXCursor_ObjCProtocolExpr                                   , "CXCursor_ObjCProtocolExpr" },
    { CXCursor_ObjCBridgedCastExpr                                , "CXCursor_ObjCBridgedCastExpr" },
    { CXCursor_PackExpansionExpr                                  , "CXCursor_PackExpansionExpr" },
    { CXCursor_SizeOfPackExpr                                     , "CXCursor_SizeOfPackExpr" },
    { CXCursor_LambdaExpr                                         , "CXCursor_LambdaExpr" },
    { CXCursor_ObjCBoolLiteralExpr                                , "CXCursor_ObjCBoolLiteralExpr" },
    { CXCursor_ObjCSelfExpr                                       , "CXCursor_ObjCSelfExpr" },
    { CXCursor_OMPArraySectionExpr                                , "CXCursor_OMPArraySectionExpr" },
    { CXCursor_ObjCAvailabilityCheckExpr                          , "CXCursor_ObjCAvailabilityCheckExpr" },
    { CXCursor_LastExpr                                           , "CXCursor_LastExpr" },
    { CXCursor_FirstStmt                                          , "CXCursor_FirstStmt" },
    { CXCursor_UnexposedStmt                                      , "CXCursor_UnexposedStmt" },
    { CXCursor_LabelStmt                                          , "CXCursor_LabelStmt" },
    { CXCursor_CompoundStmt                                       , "CXCursor_CompoundStmt" },
    { CXCursor_CaseStmt                                           , "CXCursor_CaseStmt" },
    { CXCursor_DefaultStmt                                        , "CXCursor_DefaultStmt" },
    { CXCursor_IfStmt                                             , "CXCursor_IfStmt" },
    { CXCursor_SwitchStmt                                         , "CXCursor_SwitchStmt" },
    { CXCursor_WhileStmt                                          , "CXCursor_WhileStmt" },
    { CXCursor_DoStmt                                             , "CXCursor_DoStmt" },
    { CXCursor_ForStmt                                            , "CXCursor_ForStmt" },
    { CXCursor_GotoStmt                                           , "CXCursor_GotoStmt" },
    { CXCursor_IndirectGotoStmt                                   , "CXCursor_IndirectGotoStmt" },
    { CXCursor_ContinueStmt                                       , "CXCursor_ContinueStmt" },
    { CXCursor_BreakStmt                                          , "CXCursor_BreakStmt" },
    { CXCursor_ReturnStmt                                         , "CXCursor_ReturnStmt" },
    { CXCursor_GCCAsmStmt                                         , "CXCursor_GCCAsmStmt" },
    { CXCursor_AsmStmt                                            , "CXCursor_AsmStmt" },
    { CXCursor_ObjCAtTryStmt                                      , "CXCursor_ObjCAtTryStmt" },
    { CXCursor_ObjCAtCatchStmt                                    , "CXCursor_ObjCAtCatchStmt" },
    { CXCursor_ObjCAtFinallyStmt                                  , "CXCursor_ObjCAtFinallyStmt" },
    { CXCursor_ObjCAtThrowStmt                                    , "CXCursor_ObjCAtThrowStmt" },
    { CXCursor_ObjCAtSynchronizedStmt                             , "CXCursor_ObjCAtSynchronizedStmt" },
    { CXCursor_ObjCAutoreleasePoolStmt                            , "CXCursor_ObjCAutoreleasePoolStmt" },
    { CXCursor_ObjCForCollectionStmt                              , "CXCursor_ObjCForCollectionStmt" },
    { CXCursor_CXXCatchStmt                                       , "CXCursor_CXXCatchStmt" },
    { CXCursor_CXXTryStmt                                         , "CXCursor_CXXTryStmt" },
    { CXCursor_CXXForRangeStmt                                    , "CXCursor_CXXForRangeStmt" },
    { CXCursor_SEHTryStmt                                         , "CXCursor_SEHTryStmt" },
    { CXCursor_SEHExceptStmt                                      , "CXCursor_SEHExceptStmt" },
    { CXCursor_SEHFinallyStmt                                     , "CXCursor_SEHFinallyStmt" },
    { CXCursor_MSAsmStmt                                          , "CXCursor_MSAsmStmt" },
    { CXCursor_NullStmt                                           , "CXCursor_NullStmt" },
    { CXCursor_DeclStmt                                           , "CXCursor_DeclStmt" },
    { CXCursor_OMPParallelDirective                               , "CXCursor_OMPParallelDirective" },
    { CXCursor_OMPSimdDirective                                   , "CXCursor_OMPSimdDirective" },
    { CXCursor_OMPForDirective                                    , "CXCursor_OMPForDirective" },
    { CXCursor_OMPSectionsDirective                               , "CXCursor_OMPSectionsDirective" },
    { CXCursor_OMPSectionDirective                                , "CXCursor_OMPSectionDirective" },
    { CXCursor_OMPSingleDirective                                 , "CXCursor_OMPSingleDirective" },
    { CXCursor_OMPParallelForDirective                            , "CXCursor_OMPParallelForDirective" },
    { CXCursor_OMPParallelSectionsDirective                       , "CXCursor_OMPParallelSectionsDirective" },
    { CXCursor_OMPTaskDirective                                   , "CXCursor_OMPTaskDirective" },
    { CXCursor_OMPMasterDirective                                 , "CXCursor_OMPMasterDirective" },
    { CXCursor_OMPCriticalDirective                               , "CXCursor_OMPCriticalDirective" },
    { CXCursor_OMPTaskyieldDirective                              , "CXCursor_OMPTaskyieldDirective" },
    { CXCursor_OMPBarrierDirective                                , "CXCursor_OMPBarrierDirective" },
    { CXCursor_OMPTaskwaitDirective                               , "CXCursor_OMPTaskwaitDirective" },
    { CXCursor_OMPFlushDirective                                  , "CXCursor_OMPFlushDirective" },
    { CXCursor_SEHLeaveStmt                                       , "CXCursor_SEHLeaveStmt" },
    { CXCursor_OMPOrderedDirective                                , "CXCursor_OMPOrderedDirective" },
    { CXCursor_OMPAtomicDirective                                 , "CXCursor_OMPAtomicDirective" },
    { CXCursor_OMPForSimdDirective                                , "CXCursor_OMPForSimdDirective" },
    { CXCursor_OMPParallelForSimdDirective                        , "CXCursor_OMPParallelForSimdDirective" },
    { CXCursor_OMPTargetDirective                                 , "CXCursor_OMPTargetDirective" },
    { CXCursor_OMPTeamsDirective                                  , "CXCursor_OMPTeamsDirective" },
    { CXCursor_OMPTaskgroupDirective                              , "CXCursor_OMPTaskgroupDirective" },
    { CXCursor_OMPCancellationPointDirective                      , "CXCursor_OMPCancellationPointDirective" },
    { CXCursor_OMPCancelDirective                                 , "CXCursor_OMPCancelDirective" },
    { CXCursor_OMPTargetDataDirective                             , "CXCursor_OMPTargetDataDirective" },
    { CXCursor_OMPTaskLoopDirective                               , "CXCursor_OMPTaskLoopDirective" },
    { CXCursor_OMPTaskLoopSimdDirective                           , "CXCursor_OMPTaskLoopSimdDirective" },
    { CXCursor_OMPDistributeDirective                             , "CXCursor_OMPDistributeDirective" },
    { CXCursor_OMPTargetEnterDataDirective                        , "CXCursor_OMPTargetEnterDataDirective" },
    { CXCursor_OMPTargetExitDataDirective                         , "CXCursor_OMPTargetExitDataDirective" },
    { CXCursor_OMPTargetParallelDirective                         , "CXCursor_OMPTargetParallelDirective" },
    { CXCursor_OMPTargetParallelForDirective                      , "CXCursor_OMPTargetParallelForDirective" },
    { CXCursor_OMPTargetUpdateDirective                           , "CXCursor_OMPTargetUpdateDirective" },
    { CXCursor_OMPDistributeParallelForDirective                  , "CXCursor_OMPDistributeParallelForDirective" },
    { CXCursor_OMPDistributeParallelForSimdDirective              , "CXCursor_OMPDistributeParallelForSimdDirective" },
    { CXCursor_OMPDistributeSimdDirective                         , "CXCursor_OMPDistributeSimdDirective" },
    { CXCursor_OMPTargetParallelForSimdDirective                  , "CXCursor_OMPTargetParallelForSimdDirective" },
    { CXCursor_OMPTargetSimdDirective                             , "CXCursor_OMPTargetSimdDirective" },
    { CXCursor_OMPTeamsDistributeDirective                        , "CXCursor_OMPTeamsDistributeDirective" },
    { CXCursor_OMPTeamsDistributeSimdDirective                    , "CXCursor_OMPTeamsDistributeSimdDirective" },
    { CXCursor_OMPTeamsDistributeParallelForSimdDirective         , "CXCursor_OMPTeamsDistributeParallelForSimdDirective" },
    { CXCursor_OMPTeamsDistributeParallelForDirective             , "CXCursor_OMPTeamsDistributeParallelForDirective" },
    { CXCursor_OMPTargetTeamsDirective                            , "CXCursor_OMPTargetTeamsDirective" },
    { CXCursor_OMPTargetTeamsDistributeDirective                  , "CXCursor_OMPTargetTeamsDistributeDirective" },
    { CXCursor_OMPTargetTeamsDistributeParallelForDirective       , "CXCursor_OMPTargetTeamsDistributeParallelForDirective" },
    { CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective   , "CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective" },
    { CXCursor_OMPTargetTeamsDistributeSimdDirective              , "CXCursor_OMPTargetTeamsDistributeSimdDirective" },
    { CXCursor_LastStmt                                           , "CXCursor_LastStmt" },
    { CXCursor_TranslationUnit                                    , "CXCursor_TranslationUnit" },
    { CXCursor_FirstAttr                                          , "CXCursor_FirstAttr" },
    { CXCursor_UnexposedAttr                                      , "CXCursor_UnexposedAttr" },
    { CXCursor_IBActionAttr                                       , "CXCursor_IBActionAttr" },
    { CXCursor_IBOutletAttr                                       , "CXCursor_IBOutletAttr" },
    { CXCursor_IBOutletCollectionAttr                             , "CXCursor_IBOutletCollectionAttr" },
    { CXCursor_CXXFinalAttr                                       , "CXCursor_CXXFinalAttr" },
    { CXCursor_CXXOverrideAttr                                    , "CXCursor_CXXOverrideAttr" },
    { CXCursor_AnnotateAttr                                       , "CXCursor_AnnotateAttr" },
    { CXCursor_AsmLabelAttr                                       , "CXCursor_AsmLabelAttr" },
    { CXCursor_PackedAttr                                         , "CXCursor_PackedAttr" },
    { CXCursor_PureAttr                                           , "CXCursor_PureAttr" },
    { CXCursor_ConstAttr                                          , "CXCursor_ConstAttr" },
    { CXCursor_NoDuplicateAttr                                    , "CXCursor_NoDuplicateAttr" },
    { CXCursor_CUDAConstantAttr                                   , "CXCursor_CUDAConstantAttr" },
    { CXCursor_CUDADeviceAttr                                     , "CXCursor_CUDADeviceAttr" },
    { CXCursor_CUDAGlobalAttr                                     , "CXCursor_CUDAGlobalAttr" },
    { CXCursor_CUDAHostAttr                                       , "CXCursor_CUDAHostAttr" },
    { CXCursor_CUDASharedAttr                                     , "CXCursor_CUDASharedAttr" },
    { CXCursor_VisibilityAttr                                     , "CXCursor_VisibilityAttr" },
    { CXCursor_DLLExport                                          , "CXCursor_DLLExport" },
    { CXCursor_DLLImport                                          , "CXCursor_DLLImport" },
    { CXCursor_LastAttr                                           , "CXCursor_LastAttr" },
    { CXCursor_PreprocessingDirective                             , "CXCursor_PreprocessingDirective" },
    { CXCursor_MacroDefinition                                    , "CXCursor_MacroDefinition" },
    { CXCursor_MacroExpansion                                     , "CXCursor_MacroExpansion" },
    { CXCursor_MacroInstantiation                                 , "CXCursor_MacroInstantiation" },
    { CXCursor_InclusionDirective                                 , "CXCursor_InclusionDirective" },
    { CXCursor_FirstPreprocessing                                 , "CXCursor_FirstPreprocessing" },
    { CXCursor_LastPreprocessing                                  , "CXCursor_LastPreprocessing" },
    { CXCursor_ModuleImportDecl                                   , "CXCursor_ModuleImportDecl" },
    { CXCursor_TypeAliasTemplateDecl                              , "CXCursor_TypeAliasTemplateDecl" },
    { CXCursor_StaticAssert                                       , "CXCursor_StaticAssert" },
    { CXCursor_FriendDecl                                         , "CXCursor_FriendDecl" },
    { CXCursor_FirstExtraDecl                                     , "CXCursor_FirstExtraDecl" },
    { CXCursor_LastExtraDecl                                      , "CXCursor_LastExtraDecl" },
    { CXCursor_OverloadCandidate                                  , "CXCursor_OverloadCandidate" },
  };

  std::string CXCursorKindToString( CXCursorKind kind )
  {
    return cursorKinds_[ kind ];
  }


  void TestCursor( std::string description, CXCursor cursor )
  {
    std::cout << "o "
              << description
              << " ("
              << (clang_Cursor_isNull( cursor ) ? "null" : "not null" )
              << ", "
              << (clang_isInvalid( clang_getCursorKind( cursor ) )
                  ? "invalid"
                  : "valid" )
              << "): "
              << CXCursorKindToString( clang_getCursorKind( cursor ) )
              << " at "
              << CXLocationToString( clang_getCursorLocation( cursor ) )
              << " with type '"
              << CXTypeToString( clang_getCursorType( cursor ) )
              << "' spelled '"
              << CXStringToString( clang_getCursorSpelling( cursor ) )
              << "'\n";
  }
}

int main( int argc, char ** argv )
{
  if ( argc < 4 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " filename line col\n";
    return 1;
  }

  clang_toggleCrashRecovery( true );

  auto index = clang_createIndex( 0, 0 );

  const char * flags[] = {
    "clang++",
    "-x", "c++",
    "-std=c++11",
    "-Wextra",
    "-Wall",
    "-Werror"
  };

  // Actually parse the translation unit.
  CXTranslationUnit unit;
  CXErrorCode failure = clang_parseTranslationUnit2FullArgv(
                          index,
                          "test.cc",
                          flags,
                          sizeof( flags ) / sizeof( const char * ),
                          nullptr,
                          0,
                          EditingOptions(),
                          &unit );
  if ( failure != CXError_Success ) {
    std::cerr << "Failed to parse unit\n";
    return 2;
  }

  failure = (CXErrorCode)clang_reparseTranslationUnit(
    unit,
    0,
    nullptr,
    clang_defaultReparseOptions( unit ) );

  if ( failure != CXError_Success ) {
    std::cerr << "Failed to reparse unit\n";
    return 2;
  }

  if ( clang_getNumDiagnostics( unit ) > 0 ) {
    std::cerr << "There were errors parsing the file\n";
    for ( unsigned i = 0; i < clang_getNumDiagnostics( unit ); ++i ) {
      auto d = clang_getDiagnostic( unit, i );

      std::cerr << CXLocationToString( clang_getDiagnosticLocation( d ) )
                << " "
                << CXStringToString( clang_getDiagnosticSpelling( d ) )
                << '\n';;

      clang_disposeDiagnostic( d );
    }
  }


  const char * fileName = argv[ 1 ];
  int line = atoi( argv[ 2 ] );
  int column = atoi( argv[ 3 ] );

  auto file = clang_getFile( unit, fileName );
  auto sourceLocation = clang_getLocation( unit,
                                           file,
                                           line,
                                           column );

  std::cout << "Checking location: "
            << CXLocationToString( sourceLocation )
            << '\n';

  auto cursor = clang_getCursor( unit, sourceLocation );
  TestCursor( "Cursor", cursor );
  auto ref = clang_getCursorReferenced( cursor );
  TestCursor( "Referenced", ref );
  auto can = clang_getCanonicalCursor( ref );
  TestCursor( "Canonical", can );
  auto def = clang_getCursorDefinition( cursor );
  TestCursor( "Definition", def );

  clang_disposeTranslationUnit( unit );
  clang_disposeIndex( index );

  return 0;
}
