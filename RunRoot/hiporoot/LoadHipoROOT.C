#include <TSystem.h>
#include <TString.h>
#include <TInterpreter.h>
#include <TROOT.h>


void LoadHipoROOT(){

 
  gROOT->LoadMacro("$CLAS12TOOL/RunRoot/importToROOT.C");

  TString HIPOROOT=TString(gSystem->Getenv("CLAS12TOOL"))+"/RunRoot/hiporoot/";
  gSystem->AddIncludePath(TString("-I")+HIPOROOT);
  

  gROOT->LoadMacro(HIPOROOT+"HipoROOTOut.C+");
  gROOT->LoadMacro(HIPOROOT+"HipoRootAction.C+");
  gROOT->LoadMacro(HIPOROOT+"HipoHist.C+");
  gROOT->LoadMacro(HIPOROOT+"ParticleHist.C+");



}
