#include <fstream>
#include <iostream>

#include "TLorentzVector.h"

#include "reader.h"
#include "particle.h"
#include "clas12reader.h"
#include <TDatabasePDG.h>

#include "TCanvas.h"
#include "TH1.h"

using namespace std;

void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp);
// ============================================================================================================================================
int main( int argc , char** argv){
	char inputFile[256];

	if(argc>1){sprintf(inputFile,"%s",argv[1]);}
	else{std::cout << " *** please provide a file name..." << std::endl;	exit(0);}

	double me   = 0.0005110; // GeV (electron mass)
	double mp   = 0.9382723; // GeV (proton mass)
	double mpim = 0.139570 ; // GeV (charged pion mass)
	double mD   = 1.8756   ; // GeV (deuteron mass)

	TLorentzVector beam  (0,0,10.6,10.6);
	TLorentzVector target(0,0,0   ,mD  );
	TLorentzVector el    (0,0,0   ,me  );
	TLorentzVector pr1   (0,0,0   ,mp  );
	TLorentzVector pr2   (0,0,0   ,mp  );
	TLorentzVector pim   (0,0,0   ,mpim);

	clas12::clas12reader c12(inputFile);

	//Add some event Pid based selections
	//c12.AddAtLeastPid   ( 211,1);	//at least 1 pi+
	//c12.addExactPid     (  11,1);	//exactly 1 electron
	//c12.addExactPid     ( 211,1);	//exactly 1 pi+
	//c12.addExactPid     (-211,1);	//exactly 1 pi-
	//c12.addExactPid     (2212,1);	//exactly 1 proton
	//c12.addExactPid     (  22,2);	//exactly 2 gamma
	//c12.addZeroOfRestPid(      );	//nothing else
	//c12.useFTBased      (      );	//and use the Pids from RECFT

	// -------------------------------------------------------------------------------------------
	// Declaring histograms
	TH1D * h1_Mmiss  = new TH1D("h1_Mmiss" ,";m_{miss} [GeV]"        ,100,-2,2);
	TH1D * h1_Mmiss2 = new TH1D("h1_Mmiss2",";m_{miss}^{2} [GeV^{2}]",100,-2,2);

	// -------------------------------------------------------------------------------------------
	// Looping over all events in the hipo file
	while(c12.next()==true){

		// c12.event()->getStartTime(); //hipo4
		// c12.head()->getStartTime(); //hipo3

		//Loop over all particles in a given event
		for(auto& p : c12.getDetParticles()){
			// get predefined selected information
			p->getTime();
			p->getDetEnergy();
			p->getDeltaEnergy();

			// get any detector information (if exists for this particle)
			// there should be a get function for any entry in the bank
			switch(p->getRegion()) {
				case clas12::FD :
					p->cal(clas12::PCAL)->getEnergy();
					p->cal(clas12::ECIN)->getEnergy();
					p->cal(clas12::ECOUT)->getEnergy();
					p->sci(clas12::FTOF1A)->getEnergy();
					p->sci(clas12::FTOF1B)->getEnergy();
					p->sci(clas12::FTOF2)->getEnergy();
					p->trk(clas12::DC)->getSector();
					p->che(clas12::HTCC)->getNphe();
					p->che(clas12::LTCC)->getNphe();
					//trajectories
					p->traj(clas12::LTCC)->getX();
					// p->traj(DC,DC1)->getCx();; //First layer of DC, hipo4
					break;
				case clas12::FT :
					p->ft(clas12::FTCAL)->getEnergy();
					p->ft(clas12::FTHODO)->getEnergy();
					break;
				case clas12::CD:
					p->sci(clas12::CTOF)->getEnergy();
					p->sci(clas12::CND)->getEnergy();
					break;
			}

			// covariance matrix (comment in to see!)
			// p->covmat()->print();
			p->cmat();
		}

		// get particles by type
		auto electrons = c12.getByID(  11);
		auto protons   = c12.getByID(2212);
		auto pims      = c12.getByID(-211);

		if( electrons.size()==1 && protons.size()==2 && pims.size() == 1){

			// set the particle momenta
			SetLorentzVector(el,electrons[0]);
			SetLorentzVector(pr1,protons [0]);
			SetLorentzVector(pr2,protons [1]);
			SetLorentzVector(pim,pims    [0]);

			TLorentzVector miss=beam+target-el-pr1-pr2-pim;
			
			h1_Mmiss  -> Fill(miss.M ());
			h1_Mmiss2 -> Fill(miss.M2());
		}

	}

	// -------------------------------------------------------------------------------------------
	// Plotting histograms
	const int nCanvas = 20;
	TCanvas ** can = new TCanvas * [nCanvas];
	for(int i = 0 ; i < nCanvas ; i++) can[i] = new TCanvas(Form("can_%i",i));

	can[0] -> cd();
	can[0] -> Divide(2,1);
	can[0] -> cd(1);
	h1_Mmiss -> Draw();
	can[0] -> cd(2);
        h1_Mmiss2 -> Draw();

	// -------------------------------------------------------------------------------------------
	// Saving plots to pdf file
	TString out_pdf = "results.pdf";
	
	can[0] -> Print(out_pdf);

	return 0;
}
// ============================================================================================================================================
void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp){
	p4.SetXYZM(rp->par()->getPx(),rp->par()->getPy(),rp->par()->getPz(),p4.M());
}
