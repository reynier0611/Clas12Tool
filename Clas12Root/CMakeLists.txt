set(BUILD_SHARED_LIBS ON)

if(USE_HIPO3)
  set(CLAS12ROOT Clas12Root3)
  set(CLAS12BANKS Clas12Banks3)
  set(HIPO Hipo3)
endif(USE_HIPO3)
if(USE_HIPO4)
  set(CLAS12ROOT Clas12Root4)
  set(CLAS12BANKS Clas12Banks4)
  set(HIPO Hipo4)
endif(USE_HIPO4)

ROOT_GENERATE_DICTIONARY(G__${CLAS12ROOT}  BankHist.h HipoHist.h HipoTreeMaker.h HipoChain.h HipoRootAction.h   HipoROOTOut.h ParticleHist.h HipoSelector.h ParticleTree.h LINKDEF Clas12RootLinkDef.h)

add_library(${CLAS12ROOT} MODULE BankHist.cpp HipoHist.cpp HipoTreeMaker.cpp HipoChain.cpp HipoRootAction.cpp   HipoROOTOut.cpp ParticleHist.cpp  HipoSelector.cpp ParticleTree.cpp G__${CLAS12ROOT}.cxx)

target_link_libraries(${CLAS12ROOT} ${ROOT_LIBRARIES} ${HIPO} ${CLAS12BANKS})


install(TARGETS ${CLAS12ROOT} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_C12ROOT_PCM ${CMAKE_BINARY_DIR}/Clas12Root/*pcm)

install (FILES  ${ROOT_C12ROOT_PCM}    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)
