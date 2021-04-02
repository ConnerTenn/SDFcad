
float SignedDistance(vec3 p)
{
	//  p = sdRep( p, vec3( 4.0 ) );
	//  p = sdTwist( p, 3.0 );
	
	float d0 = sdBox(p, vec3(0.5));
	float d1 = sdSphere(p, 0.6);
	
	float d = sdInter(d1, d0) + 
        sin(p.y*2*PI*10.0f)/100.0f + 
        sin(p.x*2*PI*10.0f)/500.0f + 
        sin(p.z*2*PI*10.0f)/500.0f;

	return d;
	//return d + sfDisp( p * 2.5 );
	// return sdUnion_s( d + sfDip( p * 2.5 * sin( iTime * 1.01 ) ), d1, 0.1 );
}

