#include <cstdlib>
#include <iostream>
#include <TBenchmark.h>
#include "reader.h"


void ReadEvents(){



   hipo::reader  reader;
   reader.open("/home/dglazier/fastdata/hipo4test/filtered/clas_004152.recon.hipo");

   hipo::dictionary  factory;

   reader.readDictionary(factory);

   factory.show();
   hipo::structure  particles;
   hipo::structure  detectors;

   hipo::event      event;
   int counter = 0;

   hipo::bank  dataPART;
   hipo::bank  dataCALO;

   // hipo::bank PART(factory.getSchema("REC::Particle"));
   // hipo::bank PART1(factory.getSchema("MC::Lund"));
   // hipo::bank PART2(factory.getSchema("REC::CovMat"));
   // hipo::bank PART3(factory.getSchema("REC::Event"));
   // hipo::bank PART4(factory.getSchema("REC::Calorimeter"));
   // hipo::bank PART5(factory.getSchema("REC::Scintillator"));
   // hipo::bank PART6(factory.getSchema("REC::Track"));
   // hipo::bank PART7(factory.getSchema("REC::Traj"));
   // hipo::bank PART8(factory.getSchema("REC::Cherenkov"));
   // hipo::bank PART9(factory.getSchema("REC::ForwardTagger"));

   auto PART_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::Particle"));
   auto PART1_ptr=std::make_shared<hipo::bank>(factory.getSchema("MC::LUND"));
   auto PART2_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::CovMat"));
   auto PART3_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::Event"));
   auto PART4_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::Calorimeter"));
   auto PART5_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::Scintillator"));
   auto PART6_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::Track"));
   auto PART7_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::Traj"));
   auto PART8_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::Cherenkov"));
   auto PART9_ptr=std::make_shared<hipo::bank>(factory.getSchema("REC::ForwardTagger"));
   gBenchmark->Start("timer");
   while(reader.next()==true){
      reader.read(event);
      //reader.next();
      //event.getStructure(dataBank,30,1);
      //dataBank.show();
      // event.getStructure(PART);
      // event.getStructure(PART1);
      // event.getStructure(PART2);
      // event.getStructure(PART3);
      // event.getStructure(PART4);
      // event.getStructure(PART5);
      // event.getStructure(PART6);
      // event.getStructure(PART7);
      // event.getStructure(PART8);
      // event.getStructure(PART9);
      event.getStructure(*PART_ptr.get());
      // event.getStructure(*PART1_ptr.get());
      // event.getStructure(*PART2_ptr.get());
      // event.getStructure(*PART3_ptr.get());
      // event.getStructure(*PART4_ptr.get());
      // event.getStructure(*PART5_ptr.get());
      // event.getStructure(*PART6_ptr.get());
      // event.getStructure(*PART7_ptr.get());
      // event.getStructure(*PART8_ptr.get());
      // event.getStructure(*PART9_ptr.get());
  //PART.show();
      int nrows = PART_ptr->getRows();
      for(int i = 0; i < nrows; i++){
	float  pz = PART_ptr->getFloat("pz",i);
	if(pz==0) counter++;
      }
      //     counter++;
   }
   printf("processed events = %d\n",counter);
   gBenchmark->Stop("timer");
   gBenchmark->Print("timer");


}
