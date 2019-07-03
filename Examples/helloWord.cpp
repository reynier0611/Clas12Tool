#include <fstream>
#include <iostream>
#include "reader.h"
#include "particle.h"

using namespace std;

int main( int argc , char** argv){
	char inputFile[256];

	if(argc>1) {
		sprintf(inputFile,"%s",argv[1]);
		//sprintf(outputFile,"%s",argv[2]);
	} else {
		std::cout << " *** please provide a file name..." << std::endl;
		exit(0);
	}
	hipo::reader  reader;
	reader.open(inputFile);
	hipo::dictionary  factory;
	reader.readDictionary(factory);

	factory.show();

	hipo::bank  band_hits(factory.getSchema("BAND::hits"));
	hipo::bank  head(factory.getSchema("REC::Event"));

	clas12::particle particles(factory.getSchema("REC::Particle"));

	hipo::event      event;

	int counter = 0;

	while(reader.next()==true){
		reader.read(event);
		event.getStructure(band_hits);
		event.getStructure(head);
		event.getStructure(particles);
	
		if( particles.getPid(0) == 11 )
			particles.show();
		

		int nrows = band_hits.getRows();
		if(nrows != 0 )printf("---------- BAND_HITS BANK CONTENT -------\n");
		for(int row = 0; row < nrows; row++){
			int   pid = band_hits.getInt("sector",row);
			float  px = band_hits.getFloat("meantimeFadc",row);
			float  py = band_hits.getFloat("meantimeTdc",row);
			float  pz = band_hits.getFloat("difftimeTdc",row);
			printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
		}
	}
	printf("processed events = %d\n",counter);

	cout << "hi\n";
	return 1;
}
