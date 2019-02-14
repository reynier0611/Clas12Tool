#pragma once


#include "HipoHist.h"
#include "clas12reader.h"
#include <map>

namespace hiporoot {

  
  class ParticleHist : public HipoHist {


  public :
    ParticleHist(TString filename);
    virtual ~ParticleHist();


    void Loop() final;
    TString ExpandVars(TString varExp0,TString seperator);
    TString ExpandPart(TString exp);
    TString ExpandExpression(TString varExp0,TString seperator) final;
    TString ExpandParenthesis(TString varExp0,TString seperator);
    TString AddParenthesis(TString varExp0);
    
    vector<TString> RemoveArithmetic(TString& expr);
    
    using c12_uptr = std::unique_ptr<clas12::clas12reader>;
    
    c12_uptr clas12(){
      return std::move(c12_uptr
		       (new clas12::clas12reader(HipoFileName().Data())));
    }
    
  private :
 
    std::map<TString,TString> _mapOfParts;
 
  };
}
