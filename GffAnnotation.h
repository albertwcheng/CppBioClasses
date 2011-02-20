#include <string>
#include <map>
#include <set>
using namespace std;

#include <StringUtil>


enum GffObjectType{
 GFFOBJECT_TYPE_EXON,
 GFFOBJECT_TYPE_INTRON,
 GFFOBJECT_TYPE_MRNA,
 GFFOBJECT_TYPE_GENE,
}

class GffObject{
private:
	string name;
	string ID;
	GffObject* parent;
	GffType type;
	string chrom;
	int start0;
	int end1;
	set<SmartPtr<GffObject>> children;  //sorted by left most coordinate
public:
	inline GffObject(string _id,string _name,GffType _type,GffObject* _parent):ID(_id),name(_name),type(_type),parent(_parent){}
	~GffObject();
	void addObject(GffObject*_child); //will set parent, and expand the coordinate
	
}


class GffAnnotation{
private:
	set<GffObject> genes;
	map<string,
public:
	void readGffFile(string filename);
	void writeGffFile(string filename);
	void readBedFile(string filename);
	void writeBedFile(string filename);

};