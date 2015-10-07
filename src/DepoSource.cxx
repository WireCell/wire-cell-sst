#include "WireCellIface/SimpleDepo.h"
#include "WireCellSst/DepoSource.h"
#include "WireCellUtil/Units.h"

using namespace WireCell;

WireCellSst::DepoSource::DepoSource(const char* filename,
		       const char* treepath)
    : m_reader(new WireCellSst::DepoReader(filename, treepath))
    , m_cache(nullptr)
    , m_index(-1)
    , m_count(0)
{
}
    
WireCellSst::DepoSource::~DepoSource()
{
    delete m_reader;
    m_reader = nullptr;
}
bool WireCellSst::DepoSource::extract(WireCell::IDepo::pointer& depo)
{
    if (!m_cache) {
	m_cache = m_reader->next();
	if (!m_cache) {
	    depo = nullptr;	// EOS
	    return true;
	}
	m_index=-1;
	++m_count;
	return extract(depo);
    }
    ++m_index;
    if (m_index >= m_cache->x->size()) {
	m_cache = nullptr;
	return extract(depo);
    }
    SimpleDepo* mydepo = new SimpleDepo(m_count*units::second, // celltree has no actual time
					Point(m_cache->x->at(m_index),
					      m_cache->y->at(m_index),
					      m_cache->z->at(m_index)),
					m_cache->q->at(m_index));
    depo = IDepo::pointer(mydepo);
    return true;
}
