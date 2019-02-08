
#include "HipoRootAction.h"
#include "ParticleHist.h"
#include "clas12reader.h"
#include "clas12defs.h"
#include "region_particle.h"
#include <TH1.h>
#include <memory>

namespace hiporoot{
  using namespace clas12;

  class NNNNN : public HipoRootAction{
  
    void Action(hiporoot::HipoROOTOut *hiprout) final{
      auto phist=dynamic_cast<ParticleHist*>(hiprout);

      auto hists=phist->Hists();
      //      auto c12= phist->clas12().get();
      clas12::clas12reader c12(phist->HipoFileName().Data());
      std::vector<region_part_ptr>* particles=c12.getDetParticlesPtr();

      while(c12.next()==true){
	for(auto& p : *particles){
	  ////hists->at(IIII)->Fill(XXXX);
	}
     }
    }
 
    
  };
}
