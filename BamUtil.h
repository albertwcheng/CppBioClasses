#include <sam.h>
#include <string>
#include <set>
using namespace std;

class BamReader{
public:
	
	samfile_t*bamfile;
	bam_index_t*idx;
	string bamfilename;
	set<string> chromsInBam;
	
	class fetch_count_data{
	public:
		bam_header_t* bamHeader;
		int count;
		int start0Constraint;
		int end1Constraint;
		inline void resetCount(){
			count=0;
		}
		inline fetch_count_data(bam_header_t* _bamHeader,int _start0Constraint=-1,int _end1Constraint=-1,int _count=0):bamHeader(_bamHeader),count(_count),start0Constraint(_start0Constraint),end1Constraint(_end1Constraint){}
		
	};
	
	
	inline void close(){
		if(bamfile){
			samclose(bamfile);	
			bamfile=NULL;	
		}
		if(idx){
			bam_index_destroy(idx);
			idx=NULL;
	
		}
	}
	
	inline ~BamReader(){
		close();
		
	}
	
	inline static int fetch_count(const bam1_t *b, void *data)
	{
		fetch_count_data* fdata=(fetch_count_data*)data;
		if(fdata->start0Constraint!=-1 && b->core.pos<fdata->start0Constraint)
			return 0;
		if(fdata->end1Constraint!=-1 && b->core.pos>=fdata->end1Constraint)
			return 0;
		fdata->count++;
		return 0;
	}
	
	inline BamReader(const string& _bamfilename):bamfilename(_bamfilename),idx(NULL),bamfile(NULL){
		
		bamfile=samopen(bamfilename.c_str(),"rb",0);
		
		if(!bamfile)
		{
			cerr<<"Fail to open BAM file "<<bamfilename<<endl;
			//return 1;
		}
		
		cerr<<"load bamfile index"<<endl;
		
		idx=bam_index_load(bamfilename.c_str());
		if(!idx){
			cerr<<"Fail to load index\n"<<endl;
			//return 1;
		}
		
		for(int i=0;i<bamfile->header->n_targets;i++){
			cerr<<"bam file has reference "<<bamfile->header->target_name[i]<<endl;
			chromsInBam.insert(bamfile->header->target_name[i]);
		}
		
	}
	
	
	
	inline static string getGenomeBrowserCoordinate(const string&chrom,int start0,int end1){
		char buf[256];
		sprintf(buf,"%s:%d-%d",chrom.c_str(),start0+1,end1);
		return buf;
	}
	inline static int countTotalNumOfReadsInBam(string bamfile){
		int total=0;
		samfile_t* bf=samopen(bamfile.c_str(),"rb",0);
		
		
		
		if(!bf){
			cerr<<"bam file "<<bamfile<<" cannot be open for counting"<<endl;
			return -1;
		}
		
		//now do the counting
		bam1_t *dummy=bam_init1();
		
		
		while(samread(bf,dummy)>=0){
			total++;
			if(total%1000000==1){
				cerr<<"passing through read "<<total<<endl;
			}
		}
		
		bam_destroy1(dummy);
		samclose(bf);
		return total;
	}
	
	inline int fetchCountOverlappingRegion(const string&chrom, int start0, int end1,bool constrainStart){
		string region=getGenomeBrowserCoordinate(chrom,start0,end1);
		int ref_id;
		int begin;
		int end;
		
		fetch_count_data counter(bamfile->header,(constrainStart?start0:-1));
		
		bam_parse_region(bamfile->header,region.c_str(),&ref_id,&begin,&end);
		bam_fetch(bamfile->x.bam,idx,ref_id,begin,end,&counter,BamReader::fetch_count);
		
		return counter.count;
	}
	
};
