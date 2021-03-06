# Add the include paths
include_directories(
  "${GDCM_BINARY_DIR}/Source/Common"
  "${GDCM_SOURCE_DIR}/Source/Common"
  "${GDCM_SOURCE_DIR}/Source/DataStructureAndEncodingDefinition"
  "${GDCM_SOURCE_DIR}/Source/DataDictionary"
  "${GDCM_SOURCE_DIR}/Source/InformationObjectDefinition"
  "${GDCM_SOURCE_DIR}/Source/MediaStorageAndFileFormat"

  "${GDCM_BINARY_DIR}/Testing/Source/Data"
  "${GDCM_SOURCE_DIR}/Testing/Source/Data"

  "${GDCM_SOURCE_DIR}/Utilities"
  "${GDCM_SOURCE_DIR}/Utilities/socketxx"
  "${GDCM_SOURCE_DIR}/Utilities/socketxx/socket++" # local.h
  "${GDCM_BINARY_DIR}/Utilities/socketxx/socket++" # config.h
  )

set(MessageExchangeDefinition_SRCS
  gdcmAAbortPDU.cxx
  gdcmRoleSelectionSub.cxx
  gdcmSOPClassExtendedNegociationSub.cxx
  gdcmPresentationContextGenerator.cxx
  gdcmAAssociateACPDU.cxx
  gdcmAAssociateRJPDU.cxx
  gdcmAAssociateRQPDU.cxx
  gdcmARTIMTimer.cxx
  gdcmAReleaseRPPDU.cxx
  gdcmAReleaseRQPDU.cxx
  gdcmAbstractSyntax.cxx
  gdcmApplicationContext.cxx
  gdcmAsynchronousOperationsWindowSub.cxx
  gdcmQueryBase.cxx
  gdcmBaseRootQuery.cxx
  gdcmCEchoMessages.cxx
  gdcmCFindMessages.cxx
  gdcmCMoveMessages.cxx
  gdcmCStoreMessages.cxx
  gdcmCommandDataSet.cxx
  gdcmCompositeMessageFactory.cxx
  gdcmCompositeNetworkFunctions.cxx
  gdcmImplementationClassUIDSub.cxx
  gdcmImplementationUIDSub.cxx
  gdcmImplementationVersionNameSub.cxx
  gdcmMaximumLengthSub.cxx
  gdcmPDUFactory.cxx
  gdcmPDataTFPDU.cxx
  gdcmFindPatientRootQuery.cxx
  gdcmMovePatientRootQuery.cxx
  gdcmPresentationContext.cxx
  gdcmPresentationContextRQ.cxx
  gdcmPresentationContextAC.cxx
  gdcmPresentationDataValue.cxx
  gdcmQueryFactory.cxx
  gdcmQueryImage.cxx
  gdcmQueryPatient.cxx
  gdcmQuerySeries.cxx
  gdcmQueryStudy.cxx
  gdcmServiceClassUser.cxx
  gdcmFindStudyRootQuery.cxx
  gdcmMoveStudyRootQuery.cxx
  gdcmTransferSyntaxSub.cxx
  gdcmULActionAA.cxx
  gdcmULActionAE.cxx
  gdcmULActionAR.cxx
  gdcmULActionDT.cxx
  gdcmULBasicCallback.cxx
  gdcmULConnection.cxx
  gdcmULConnectionInfo.cxx
  gdcmULConnectionManager.cxx
  gdcmULTransitionTable.cxx
  gdcmULWritingCallback.cxx
  gdcmUserInformation.cxx
  )

add_library(gdcmMEXD ${MessageExchangeDefinition_SRCS})
target_link_libraries(gdcmMEXD gdcmMSFF gdcmDICT gdcmDSED gdcmIOD socketxx)
set_target_properties(gdcmMEXD PROPERTIES ${GDCM_LIBRARY_PROPERTIES} LINK_INTERFACE_LIBRARIES "")
if(WIN32)
  target_link_libraries(gdcmMEXD ws2_32)
endif()

# libs
install_library(gdcmMEXD)
# PDB
install_pdb(gdcmMEXD)
# include files
install_includes("*.h")
