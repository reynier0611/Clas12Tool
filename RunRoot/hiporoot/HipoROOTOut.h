#pragma once


#include "region_particle.h"
#include <TString.h>
#include <TChain.h>

namespace hiporoot {
  
  class HipoROOTOut {

    
  public :
    HipoROOTOut(TString filename);
    virtual ~HipoROOTOut();
    
    void AddAction(TString varExp,TString condExp="1");
    void CompileAction();
    void CleanAction();
    //  virtual TString ExpandVars(TString varExp0,TString seperator){return varExp0;};    
    virtual TString ExpandExpression(TString varExp0,TString seperator){return varExp0;};    
    virtual void Loop()=0;
    
    //  virtual Double_t Action()=0;
    virtual Bool_t Condition(){return kTRUE;}

    TString HipoFileName(){return _hipoFileName;}
    TString NextFile();
    Bool_t IsMoreFiles();
    TChain* GetChain(){return &_chain;}
    
  protected :
    
    TString _tempActionName;
    TString _actionClassName;

  private :

    TString _hipoFileName;
    TString _curMacro;
    TString _tempDir="~/.hiporoot";
    Int_t _Nactions=0;
    Int_t _Nruns=0;
    TChain _chain;
    Int_t _iHipoFile=0;
    
  };//class HipoROOTOut
  
}
