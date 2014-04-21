using namespace HepMC3;

/* Elementary test of the event content
   Typically executed e.g. before detector simulation
   Similar test was performed in Fortran */
void photos_check_momentum_conservation(GenEvent &evt)
{
        //cout<<"List of stable particles: "<<endl;

        FourVector sum;

        FindParticles search( evt, FIND_ALL, STATUS == 1 && VERSION_DELETED > evt.last_version());

        BOOST_FOREACH( GenParticle *p, search.results() ) {
            sum += p->momentum();
            //(*p)->print();
        }

        cout.precision(6);
        cout.setf(std::ios_base::floatfield);
        cout<<endl<<"Vector Sum: "<<sum.px()<<" "<<sum.py()<<" "<<sum.pz()<<" "<<sum.e()<<endl;
}