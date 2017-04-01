#include "DxlMemMap.h"
#include <algorithm>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <string>
#include <iostream>
#include <fstream>

DxlMemMap::DxlMemMap(){
	this->fieldNames.reserve(MAX_CONTROL_TABLE_SIZE);
	for(unsigned int i = 0; i < sizeof(DxlMemMap::controlTable); i++) {
        DxlMemMap::controlTable[i]=0;
    }

}

DxlMemMap::DxlMemMap(const char* jsonFileName){

	this->fieldNames.reserve(MAX_CONTROL_TABLE_SIZE);

	Json::Value jsonObject;

	std::ifstream jsonFile(jsonFileName);

	jsonFile >> jsonObject;

	Json::Value defaultTable = jsonObject["defaultTable"];

	for (unsigned int i = 0; i < defaultTable.size(); ++i) {
		int regNumber = defaultTable[i]["regNumber"].asInt();
		char value = (char) defaultTable[i]["defaultValue"].asInt();
		std::string regName = defaultTable[i]["regName"].asString();;

		this->controlTable[regNumber] = value;
		this->fieldNames[regNumber] = regName;

	}

}



void DxlMemMap::set(int startAddress, int length, char *inValuesPtr){
    std::copy(inValuesPtr, inValuesPtr+length, &DxlMemMap::controlTable[startAddress]);
}

//TODO: verificar como tornar os 2 métodos a seguir um só

void DxlMemMap::set(int startAddress, std::vector<char> values){
    std::copy(values.begin(), values.end(), &DxlMemMap::controlTable[startAddress]);
}

void DxlMemMap::set(int startAddress, std::vector<short  int> values){
    std::copy(values.begin(), values.end(), (short int*) &DxlMemMap::controlTable[startAddress]);
}

void DxlMemMap::get(int startAddress, int length, char *outValuesPtr) {
    std::copy(&DxlMemMap::controlTable[startAddress],&DxlMemMap::controlTable[startAddress+length], outValuesPtr);
}

std::vector<char> DxlMemMap::getBytes(int startAddress, int length){
    std::vector <char> outputVector(length);

    std::copy(&DxlMemMap::controlTable[startAddress],&DxlMemMap::controlTable[startAddress+length],outputVector.begin());

    return outputVector;
}

std::vector<short int> DxlMemMap::getWords(int startAddress, int length) {
    std::vector <short int> outputVector(length);

    std::copy((short int*)&DxlMemMap::controlTable[startAddress],(short int*)&DxlMemMap::controlTable[startAddress+length*2],outputVector.begin());

    return outputVector;
}

std::string DxlMemMap::toString() {

    std::string str;

    for(unsigned int i=0; i < sizeof(DxlMemMap::controlTable); i++){

		str += "add" + std::to_string(i) + ": " + this->fieldNames[i] + " = " + std::to_string((int)controlTable[i]) + '\n';
    }
    return str;
}

std::string DxlMemMap::toString(int startAddress, int length){
    //TODO: teste de startAddress e length válidos

    std::string str;

    for(int i=startAddress; i < startAddress+length; i++){

		str += "add" + std::to_string(i) + ": " + this->fieldNames[i] + " = " + std::to_string((int)controlTable[i]) + '\n';
    }
    return str;
}