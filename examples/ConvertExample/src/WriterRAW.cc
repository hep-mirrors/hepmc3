#include "WriterRAW.h"
namespace HepMC3
{
WriterRAW::WriterRAW(const std::string &filename,std::shared_ptr<GenRunInfo> run): m_file(filename),
    m_stream(&m_file),
    m_buffer(nullptr),
    m_cursor(nullptr),
    m_buffer_size( 256*1024*1024 )
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR( "WriterRAW: could not open output file: "<<filename )
    }
    char buf[12];
	sprintf(buf,"HepMCBin0000");
	m_stream->write(buf,12);
	    set_run_info(run);
    if ( run_info() ) 
    {
    write_run_info();
    }
}

WriterRAW::WriterRAW(std::ostream &stream, std::shared_ptr<GenRunInfo> run)
    : m_file(),
      m_stream(&stream),
      m_buffer(nullptr),
      m_cursor(nullptr),
      m_buffer_size( 256*1024*1024 )
{   char buf[12];
	sprintf(buf,"HepMCBin0000");
	m_stream->write(buf,12);


    
    set_run_info(run);
    if ( run_info() ) 
    {
    write_run_info();
    }
	
	}

int  serialize(char* &cur, const std::vector<std::string>& some)
{
char* old_cur=cur;
     std::vector<int> as;
     for (auto a: some)    as.push_back(a.size());
     int n_as=as.size();

    memcpy(cur,&n_as,sizeof(int));
    cur+=sizeof(int);
    memcpy(cur,as.data(),sizeof(int)*n_as);
    cur+=sizeof(int)*n_as;
    for (auto a: some) {
		memcpy(cur,a.c_str(),sizeof(char)*a.size());
		cur+=sizeof(char)*a.size();
	}
	return cur-old_cur;
}

void WriterRAW::close() {
    std::ofstream* ofs = dynamic_cast<std::ofstream*>(m_stream);
    if (ofs && !ofs->is_open()) return;
    forced_flush();
    if (ofs) ofs->close();
}

void WriterRAW::write_event(const GenEvent &evt)
{
    allocate_buffer();
    if ( !m_buffer ) return;
    flush();
    
    
        bool refill = false;
    if ( evt.run_info()&&(!run_info() || (run_info() != evt.run_info())))  { set_run_info(evt.run_info()); refill = true;}
    if (refill)
    {
write_run_info();
    }
    
    
    char buf[12];
    
    buf[0]='E';
    buf[1]='0';
    buf[2]='0';
    buf[3]='0';
    int event_size=1;
    char* old_cursor=m_cursor;

    m_cursor+=12;
    
    GenEventData D;
    evt.write_data(D);
    memcpy(m_cursor,&D.event_number,sizeof(int));
    m_cursor+=sizeof(int);
    int mom=1;
    memcpy(m_cursor,&mom,sizeof(int));
    m_cursor+=sizeof(int);
    int len=1;
    memcpy(m_cursor,&len,sizeof(int));
    m_cursor+=sizeof(int);
    int n_particles=D.particles.size();
    memcpy(m_cursor,&n_particles,sizeof(int));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,D.particles.data(),sizeof(GenParticleData)*n_particles);
    m_cursor+=sizeof(GenParticleData)*n_particles;
    
    int n_vertices=D.vertices.size();
    memcpy(m_cursor,&n_vertices,sizeof(int));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,D.vertices.data(),sizeof(GenVertexData)*n_vertices);
    m_cursor+=sizeof(GenVertexData)*n_vertices;


    int n_weights=D.weights.size();
    memcpy(m_cursor,&n_weights,sizeof(double));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,D.weights.data(),sizeof(double)*n_weights);
    m_cursor+=sizeof(double)*n_weights;
    
    
    memcpy(m_cursor,&D.event_pos,sizeof(FourVector));
    m_cursor+= sizeof(FourVector);
     
    int n_links1=D.links1.size();
    memcpy(m_cursor,&n_links1,sizeof(int));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,D.links1.data(),sizeof(int)*n_links1);
    m_cursor+=sizeof(int)*n_links1;
    


    int n_links2=D.links2.size();
    memcpy(m_cursor,&n_links2,sizeof(int));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,D.links2.data(),sizeof(int)*n_links2);
    m_cursor+=sizeof(int)*n_links2;


    int n_attribute_id=D.attribute_id.size();
    memcpy(m_cursor,&n_attribute_id,sizeof(int));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,D.attribute_id.data(),sizeof(int)*n_attribute_id);
    m_cursor+=sizeof(int)*n_attribute_id;



     std::vector<int> an;
     for (auto a: D.attribute_name)    { 
		// printf("14->%i   %s %lu  ==%i\n", D.event_number,a.c_str(), a.size(),D.attribute_id.size()); 
		  an.push_back(a.size());}
     int n_an=an.size();


     
    memcpy(m_cursor,&n_an,sizeof(int));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,an.data(),sizeof(int)*n_an);
    m_cursor+=sizeof(int)*n_an;
    for (auto a: D.attribute_name) {
		memcpy(m_cursor,a.c_str(),sizeof(char)*a.size());
		m_cursor+=sizeof(char)*a.size();
	}


     std::vector<int> as;
     for (auto a: D.attribute_string)    as.push_back(a.size());
     int n_as=as.size();

    memcpy(m_cursor,&n_as,sizeof(int));
    m_cursor+=sizeof(int);
    memcpy(m_cursor,as.data(),sizeof(int)*n_as);
    m_cursor+=sizeof(int)*n_as;
    for (auto a: D.attribute_string) {
		memcpy(m_cursor,a.c_str(),sizeof(char)*a.size());
		m_cursor+=sizeof(char)*a.size();
	}
     
     
     event_size=m_cursor-old_cursor-12;
         memcpy(&(buf[4]),&event_size,sizeof(int));
    memcpy(old_cursor,buf,sizeof(char)*12);
     
     
    flush();
 
    forced_flush();
}

void WriterRAW::write_run_info() {
	forced_flush();
	GenRunInfoData rid;
	run_info()->write_data(rid);
	
	   char buf[12];
    
    buf[0]='R';
    buf[1]='0';
    buf[2]='0';
    buf[3]='0';
    int run_size=0;
    char* old_cursor=m_cursor;
    m_cursor+=12;
	

	run_size+=serialize(m_cursor,rid.weight_names);     ///< Weight names

    run_size+=serialize(m_cursor,rid.tool_name);        ///< Tool names
    run_size+=serialize(m_cursor,rid.tool_version);     ///< Tool versions
    run_size+=serialize(m_cursor,rid.tool_description); ///< Tool descriptions

    run_size+=serialize(m_cursor,rid.attribute_name);   ///< Attribute name
    run_size+=serialize(m_cursor,rid.attribute_string);
	
	memcpy(&(buf[4]),&run_size,sizeof(int));
	printf("RS=%i   %i\n",run_size,m_cursor-old_cursor); 
	memcpy(old_cursor,buf,sizeof(char)*12);
	forced_flush();
	
	}


void WriterRAW::allocate_buffer() {
    if ( m_buffer ) return;
    while( m_buffer == nullptr && m_buffer_size >= 256 ) {
        try {
            m_buffer = new char[ m_buffer_size ]();
        }     catch (const std::bad_alloc& e) {
            delete[] m_buffer;
            m_buffer_size /= 2;
            HEPMC3_WARNING( "WriterRAW::allocate_buffer: buffer size too large. Dividing by 2. New size: " << m_buffer_size << e.what())
        }
    }

    if ( !m_buffer ) {
        HEPMC3_ERROR( "WriterRAW::allocate_buffer: could not allocate buffer!" )
        return;
    }
    m_cursor = m_buffer;
}
inline void WriterRAW::flush() {
    // The maximum size of single add to the buffer (other than by
    // using WriterRAW::write) is 32 bytes. This is a safe value as
    // we will not allow precision larger than 24 anyway
    if ( m_buffer + m_buffer_size < m_cursor + 256 ) {
        m_stream->write( m_buffer, m_cursor - m_buffer );
        m_cursor = m_buffer;
    }
}
inline void WriterRAW::forced_flush() {
    m_stream->write( m_buffer, m_cursor - m_buffer );
    m_cursor = m_buffer;
}

} // namespace HepMC3
