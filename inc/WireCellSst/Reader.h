#ifndef WIRECELLSST_READER
#define WIRECELLSST_READER

#include "TFile.h"
#include "TTree.h"

namespace WireCellSst {

    template<class RootDataClass>
    class Reader {
    public:
	typedef RootDataClass record_type;

	Reader(const char* filename, const char* treepath = "/Event/Sim")
	    : m_record(0)
	    , m_tfile(0)
	    , m_ttree(0)
	    , m_entry(-1)
	    , m_filename(filename)
	    , m_treepath(treepath) { }
	
	virtual ~Reader() {
	    clear();
	}
	

	/// Return the number of entries, may open file and set tree as side effect 
	int nentries() {
	    TTree* t = tree();
	    if (!t) { return -1; }
	    return t->GetEntries();
	}

	/// Load and return the n'th entry.  Return nullptr on end of file or error
	const record_type* load(int entry_number) {
	    if (entry_number < 0) { return nullptr; }
	    TTree* t = tree();
	    if (!t) { return nullptr; }
	    if (entry_number >= t->GetEntries()) { return nullptr; }
	    int siz = t->GetEntry(entry_number);
	    if (!siz) return nullptr;
	    m_entry = entry_number;
	    return m_record;
	}

	/// Load and return the next entry. Return nullptr on end of file or error.
	const record_type* next() {
	    return load(m_entry+1);
	}

	/// Clear buffers.  This invalidates any returned event data
	/// and closes the TFile but does not reset the entry count.
	/// Call this if you need to clear ROOT after extracting what
	/// is needed.
	virtual void clear() {
	    if (m_tfile) {
		m_tfile->Close();
		delete m_tfile;	// but not the ttree
		m_tfile = nullptr;
		m_ttree = nullptr;
	    }
	    if (m_record) {
		delete m_record;
		m_record = nullptr;
	    }
	}


    private:
	TTree* tree() {
	    if (m_ttree) { return m_ttree; }
	    m_tfile = TFile::Open(m_filename);
	    if (!m_tfile) { return nullptr; }
	    m_ttree = (TTree*)m_tfile->Get(m_treepath);
	    if (!m_ttree) {
		m_tfile->Close();
		delete m_tfile;
		m_tfile = nullptr;
		return nullptr;
	    }
	    m_record = new record_type;
	    m_ttree->SetBranchStatus("*",0);
	    m_record->attach(*m_ttree);
	    return m_ttree;
	}

	record_type* m_record;
	TFile* m_tfile;
	TTree* m_ttree;
	int m_entry;
	const char* m_filename;
	const char* m_treepath;
    };

}


#endif
