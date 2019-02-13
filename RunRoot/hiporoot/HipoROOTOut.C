#include "HipoROOTOut.h"
#include "HipoRootAction.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TBenchmark.h>
#include <TMacro.h>
#include <TObjString.h>
#include <iostream>

namespace hiporoot{
  
  HipoROOTOut::HipoROOTOut(TString filename): _hipoFileName(filename){
    //make a temp file directory
    TString HOME=TString(gSystem->Getenv("HOME"));
    _tempDir=HOME+"/.hiporoot/";
    if(gSystem->Exec(Form("mkdir -p %s",_tempDir.Data()))){
      std::cout<<"HipoROOTOut::HipoROOTOut will store temp files in current directory"<<std::endl;
      _tempDir="";
    }
 
  }
  HipoROOTOut::~HipoROOTOut(){
    gSystem->Exec(Form("rm %sasdf*",_tempDir.Data()));
   }


  void HipoROOTOut::AddAction(TString varExp,TString condExp){
    if(_curMacro==TString("")){
      TString HIPOROOT=TString(gSystem->Getenv("CLAS12TOOL"))+"/RunRoot/hiporoot/";
      TMacro macro(HIPOROOT+"template"+_tempActionName+".C");
      _curMacro=_tempDir+Form("asdf%d__%s__.C",_Nruns,_tempActionName.Data());
  
      TString strline=macro.GetLineWith("NNNN")->GetString();
      _actionClassName=_tempActionName+Form("%d",_Nruns);
      strline.ReplaceAll("NNNNN",_actionClassName);
      macro.GetLineWith("NNNNN")->SetString(strline);

      _Nruns++;
      macro.SaveSource(_curMacro);
    }

    TMacro macro(_curMacro);
    TString strline=macro.GetLineWith("XXXX")->GetString();
    strline.ReplaceAll("////","");
    strline.ReplaceAll("XXXX",varExp);
    strline.ReplaceAll("(IIII)",Form("(%d)",_Nactions++));
    strline.ReplaceAll("CCCC",condExp);
    
    TList *lines=macro.GetListOfLines();
    TObject* obj=macro.GetLineWith("XXXX");
    lines->AddAfter(obj,new TObjString(strline.Data()));

 
    macro.SaveSource(_curMacro);
 
  }
  void HipoROOTOut::CompileAction(){
      TMacro macro(_curMacro);
    macro.Print();
    
    auto result=gROOT->LoadMacro(Form("%s++",_curMacro.Data()));
  
    gROOT->ProcessLine(Form("hiporoot::%s::Create<hiporoot::%s>();",_actionClassName.Data(),_actionClassName.Data()));
  }
  
   void HipoROOTOut::CleanAction(){
     if(_curMacro!=TString("")){
       _curMacro="";
       _Nactions=0;
     }
   }
}
