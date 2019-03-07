#include "WriterDOT.h"
namespace HepMC3
{
WriterDOT::WriterDOT(const std::string &filename,shared_ptr<GenRunInfo> run): m_file(filename),
    m_stream(&m_file),
    m_buffer(nullptr),
    m_cursor(nullptr),
    m_buffer_size( 256*1024 ),
    m_style(0)
{
    if ( !m_file.is_open() ) {
        ERROR( "WriterDOT: could not open output file: "<<filename )
    }
}

WriterDOT::WriterDOT(std::ostream &stream, shared_ptr<GenRunInfo> run)
    : m_file(),
      m_stream(&stream),
      m_buffer(nullptr),
      m_cursor(nullptr),
      m_buffer_size( 256*1024 ),
      m_style(0)
{}


void WriterDOT::close() {
    std::ofstream* ofs = dynamic_cast<std::ofstream*>(m_stream);
    if (ofs && !ofs->is_open()) return;
    forced_flush();
    if (ofs) ofs->close();
}

void WriterDOT::write_event(const GenEvent &evt)
{
    allocate_buffer();
    if ( !m_buffer ) return;
    flush();
    m_cursor += sprintf(m_cursor, "digraph graphname%d {\n",evt.event_number());
    m_cursor += sprintf(m_cursor, "v0[label=\"Machine\"];\n");
    for(auto v: evt.vertices() ) {
        if (m_style!=0)
        {
            if (m_style==1) //paint decay and fragmentation vertices in green
            {
                if (v->status()==2) m_cursor += sprintf(m_cursor, "node [color=\"green\"];\n");
                else  m_cursor += sprintf(m_cursor, "node [color=\"black\"];\n");
            }
        }
        m_cursor += sprintf(m_cursor, "v%d[label=\"%d\"];\n", -v->id(),v->id());
        flush();
    }
    for(auto p: evt.beams() ) {
        if (!p->end_vertex()) continue;
        m_cursor += sprintf(m_cursor, "v0 -> v%d [label=\"%d(%d)\"];\n", -p->end_vertex()->id(),p->id(),p->pid());
    }

    for(auto v: evt.vertices() ) {
        for(auto p: v->particles_out() ) {
            {
                if (!p->end_vertex()) continue;
                if (m_style!=0)
                {
                    if (m_style==1) //paint suspected partons and 81/82 in red
                    {
                        bool parton=false;
                        if (p->pid()==81||p->pid()==82||std::abs(p->pid())<25) parton=true;
                        if (
                            (p->pid()/1000==1||p->pid()/1000==2||p->pid()/1000==3||p->pid()/1000==4||p->pid()/1000==5)
                            &&(p->pid()%1000/100==1||p->pid()%1000/100==2||p->pid()%1000/100==3||p->pid()%1000/100==4)
                            &&(p->pid()%100==1||p->pid()%100==3)
                        )
                            parton=true;
                        if (parton) m_cursor += sprintf(m_cursor, "edge [color=\"red\"];\n");
                        else        m_cursor +=sprintf(m_cursor, "edge [color=\"black\"];\n");
                    }
                }
                m_cursor += sprintf(m_cursor, "v%d -> v%d [label=\"%d(%d)\"];\n", -v->id(),-p->end_vertex()->id(),p->id(),p->pid());
                flush();
            }
        }
    }
    m_cursor += sprintf(m_cursor, "labelloc=\"t\";\nlabel=\"Event %d; Particles %lu; Vertices %lu;\";\n", evt.event_number(), evt.vertices().size(), evt.particles().size());
    m_cursor += sprintf(m_cursor,"}\n\n");
    forced_flush();
}
void WriterDOT::allocate_buffer() {
    if ( m_buffer ) return;
    while( m_buffer==nullptr && m_buffer_size >= 256 ) {
        try {
            m_buffer = new char[ m_buffer_size ]();
        }     catch (const std::bad_alloc& e) {
            delete[] m_buffer;
            m_buffer_size /= 2;
            WARNING( "WriterDOT::allocate_buffer: buffer size too large. Dividing by 2. New size: " << m_buffer_size )
        }
    }

    if ( !m_buffer ) {
        ERROR( "WriterDOT::allocate_buffer: could not allocate buffer!" )
        return;
    }
    m_cursor = m_buffer;
}
inline void WriterDOT::flush() {
    // The maximum size of single add to the buffer (other than by
    // using WriterDOT::write) is 32 bytes. This is a safe value as
    // we will not allow precision larger than 24 anyway
    unsigned long length = m_cursor - m_buffer;
    if ( m_buffer_size - length < 32 ) {
        // m_file.write( m_buffer, length );
        m_stream->write( m_buffer, length );
        m_cursor = m_buffer;
    }
}
inline void WriterDOT::forced_flush() {
    // m_file.write( m_buffer, m_cursor-m_buffer );
    m_stream->write( m_buffer, m_cursor - m_buffer );
    m_cursor = m_buffer;
}

} // namespace HepMC3
