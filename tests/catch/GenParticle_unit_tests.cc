/**
 *  @file GenParticle_unit_tests.cc
 *  @brief Unit tests for HepMC3::GenParticle
 *
 *  @date Created       <b> 26th March 2014 </b>
 *  @date Last modified <b> 27th March 2014 </b>
 */
#include "catch.hpp"
#include "HepMC3/GenParticle.h"
using namespace HepMC3;

SCENARIO( "Generated mass set by the generator/tool", "[GenParticle]" ) {

    GIVEN( "GenParticle with some momentum data, but no generated mass set" ) {

        double px = 1., py = 2., pz = 3., e = 4.;
        FourVector m( px, py, pz, e );
        GenParticle  p( m, 0, 0 );

        WHEN( "Generated mass is calculated" ) {

            double generated_mass = p.generated_mass();

            THEN    ( "generated_mass() returns mass calculated from 4-vector" )
            AND_THEN( "is_generated_mass_set() flag is not set" ) {

                double m = e*e - px*px - py*py - pz*pz;
                if(m<0.0) m = -sqrt(-m);
                else      m =  sqrt( m);

                REQUIRE( m == Approx(generated_mass) );
                REQUIRE_FALSE( p.is_generated_mass_set() );
            }
        }

        WHEN( "Generated mass is set by the tool" ) {

            double generated_mass = 777.0;

            p.set_generated_mass( generated_mass );

            THEN    ( "When calling generated_mass(), previously set value is returned")
            AND_THEN( "is_generated_mass_set() flag is set" )            {

                REQUIRE      ( generated_mass   == Approx(p.generated_mass()) );
                REQUIRE_FALSE( p.momentum().m() == Approx(p.generated_mass()) );
                REQUIRE( p.is_generated_mass_set() );
            }

            AND_WHEN( "unset_generated_mass() is called" ) {

                p.unset_generated_mass();

                THEN    ( "Flag is_generated_mass_set() is reset" )
                AND_THEN( "Calling generated_mass() returns mass calculated from 4-vector" ) {

                    REQUIRE_FALSE( p.is_generated_mass_set() );
                    REQUIRE( p.momentum().m() == p.generated_mass() );
                }
            }
        }
    }
}