/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "clas12reader.h"

namespace clas12 {

  clas12reader::clas12reader(string filename){
    cout<<" clas12reader::clas12reader reading "<<filename.data()<<endl;
    _reader.open(filename.data()); //keep a pointer to the reader

    //initialise banks pointers
    _bparts = std::make_shared<particle>();
    _bmcparts = std::make_shared<mcparticle>();
    _bcovmat= std::make_shared<covmatrix>();
    _bhead  = std::make_shared<header>();
    _bcal   = std::make_shared<calorimeter>();
    _bscint = std::make_shared<scintillator>();
    _btrck  = std::make_shared<tracker>();
    _btraj  = std::make_shared<traj>();
    _bcher  = std::make_shared<cherenkov>();
    _bft    = std::make_shared<forwardtagger>();
    _bvtp   = std::make_shared<clas12::vtp>();

    //initialise banks
    _bparts->init("REC::Particle",_reader);
    _bmcparts->init("MC::Lund",_reader);
    _bcovmat->init("REC::CovMat",_reader);
    _bhead->init("REC::Event",_reader);
    _bvtp->init("RAW::vtp",_reader);
    _bcal->init("REC::Calorimeter",_reader);
    _bscint->init("REC::Scintillator",_reader);
    _bcher->init("REC::Cherenkov",_reader);
    _bft->init("REC::ForwardTagger",_reader);
    _btrck->init("REC::Track",_reader);
    _btrck->init("REC::Traj",_reader);
 
    //add some detector regions to their vectors
    addARegionFDet();
    addARegionCDet();
    addARegionFT();
  
  }
  ////////////////////////////////////////////////////////
  ///initialise next event from the reader
  bool clas12reader::next(){
    if(!_reader.next())
      return false;

    sort();
    
    return true;
  }
  ////////////////////////////////////////////////////////
  ///initialise next event from the reader
  // bool clas12reader::nextInRecord(){
  //   if(!_reader.nextInRecord())
  //     return false;
    
  //   sort();
    
  //   return true;
  // }
  ////////////////////////////////////////////////////////
  /// Loop over particles and find their region
  /// Add appropriate region_partcle to event particle vector
  void clas12reader::sort(){

    
    _nparts=_bparts->getSize();
    _n_rfdets=0;
    _n_rcdets=0;
    _n_rfts=0;

    _detParticles.clear();
    _detParticles.reserve(_nparts);

    if(_nparts==0) return;
    
    //Loop over particles and find their region
    for(ushort i=0;i<_nparts;i++){ 
      _bparts->setEntry(i);
      
      //Check if FDet particle
      if(_rfdets[_n_rfdets]->sort()){
	//	add a FDet particle to the event list
	_detParticles.emplace_back(_rfdets[_n_rfdets]);
	_n_rfdets++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rfdets==_rfdets.size())
	  addARegionFDet();
	continue;
      }
      
     //Check if CDet particle
      if(_rcdets[_n_rcdets]->sort()){
	//	add a FDet particle to the event list
	_detParticles.emplace_back(_rcdets[_n_rcdets]);
	_n_rcdets++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rcdets==_rcdets.size())
	  addARegionCDet();
	continue;
      }
       //Check if FT particle
      if(_rfts[_n_rfts]->sort()){
	//add a FDet particle to the event list
	_detParticles.emplace_back(_rfts[_n_rfts]);
	_n_rfts++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rfts==_rfts.size())
	  addARegionFT();
	continue;
      }
    }
  }
  ////////////////////////////////////////////////////////
  ///Filter and return detParticles by given PID
  std::vector<region_part_ptr> clas12reader::getByID(int id){
    return container_filter(_detParticles, [id](region_part_ptr dr)
			    {return dr->getPid()==id;});
  }

}
