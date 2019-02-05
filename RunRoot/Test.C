#include <cstdlib>
#include <iostream>
#include <chrono>
#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TBenchmark.h>
#include "reader.h"
//#include "hallB_event.h"

using namespace clas12;

//just make the code a bit neater when using unique_ptr
using P4_=TLorentzVector;
using p4_uptr = std::unique_ptr<P4_>;

void HallBEvent(){
  // Record start time
  auto start = std::chrono::high_resolution_clock::now();


  /////////////////////////////////////
  //ignore this just getting file name!
   TString inputFile;
   TString outputFile;

   for(Int_t i=1;i<gApplication->Argc();i++){
    TString opt=gApplication->Argv(i);
    if((opt.Contains(".hipo"))){
      inputFile=opt(5,opt.Sizeof());
    }
   }
   if(inputFile==TString())  {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }
   /////////////////////////////////////

   
   cout<<"Analysing hipo file "<<inputFile<<endl;


   gBenchmark->Start("timer");
   int counter=0;

      //get the hipo data
   hipo::reader  reader;
   reader.open(inputFile.Data());
   hipo::dictionary  factory;
   reader.readDictionary(factory);

   //create the hallb event
   //hallB_event event(reader);
   hipo::event      event;

   //  clas12::particle  particles(factory);
   clas12::particle  particles(factory);
   hipo::bank PART(factory.getSchema("REC::Particle"));
         
   while(reader.next()==true){
     reader.read(event);
     event.getStructure(particles);
     event.getStructure(PART);
     // event.show();
     //particles.notify();
     //particles.show();
     int nrows = particles.getRows();
     int nrows0 = PART.getRows();
     cout<<nrows<<" "<<nrows0<<endl;
     //event.head()->getStartTime();
     
     //Loop over all particles to see how to access detector info.
     for(int i=0;i<nrows;i++){
       //  get predefined selected information
       cout<<i<<" "<<particles.getPid(i)<<" "<<particles.getPz(i)<<endl;
     }
     
     counter++;
     //if(counter==1E6) break;
     
   }
   gBenchmark->Stop("timer");
   gBenchmark->Print("timer");
   // hmiss->DrawCopy();
   
   auto finish = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = finish - start;
   std::cout << "Elapsed time: " << elapsed.count()<< " events = "<<counter<< " s\n";

}
