/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "dictionary.h"
#include "utils.h"
#include <stdlib.h>

namespace hipo {

  void  schema::parse(std::string schString){
    std::vector<std::string>  entries;
    std::vector<std::string>  entry;
    hipo::utils::tokenize(schString, entries, ",");
    int offset = 0;
    for(int i = 0; i < entries.size(); i++){
      entry.clear();
      hipo::utils::tokenize(entries[i],entry, "/");
      schemaEntry_t e;
      e.name = entry[0];
      e.type = entry[1];
      e.typeId = getTypeByString(e.type);
      e.typeSize = getTypeSize(e.typeId);
      e.offset   = offset;
      offset += e.typeSize;
      schemaEntries.push_back(e);
    }
  }

  int   schema::getTypeByString(std::string &typeName){
      if(typeName=="B"){
        return 1;
      } else if(typeName=="S") {
        return 2;
      } else if(typeName=="I") {
        return 3;
      } else if(typeName=="F") {
        return 4;
      } else if(typeName=="D") {
        return 5;
      } else if(typeName=="L") {
        return 8;
      }
      return -1;
  }

  int  schema::getTypeSize(int id){
    switch(id){
      case 1: return 1;
      case 2: return 2;
      case 3: return 4;
      case 4: return 4;
      case 5: return 8;
      case 8: return 8;
      default: return 0;
    }
    return 0;
  }
  void  schema::show(){
    printf("schema : %14s , group = %6d, item = %3d\n",
       schemaName.c_str(),groupid,itemid);
    for(int i = 0; i < schemaEntries.size(); i++){
      printf("%16s : (%3s) %5d %5d , offset = %3d\n",
         schemaEntries[i].name.c_str(),schemaEntries[i].type.c_str(),
         schemaEntries[i].typeId,schemaEntries[i].typeSize, schemaEntries[i].offset
         );
    }
  }

  int   schema::getOffset(int item, int order, int rows){
      int offset = rows*schemaEntries[item].offset + order*schemaEntries[item].typeSize;
      return offset;
  }
}
