
float SignedDistance(vec3 p)
{
	// p = sdRep( p, vec3( 4.0 ) );
	// p = sdTwist( p, 3.0 );
	
	vec3 p1 = translate(p, vec3(0.6,0,0));
	vec3 p2 = translate(p, vec3(-0.6,0,0));

	float d0 = sdBox(p1, vec3(0.5));
	float d1 = sdSphere(p1, 0.6);
	float d2 = sdSphere(p2, 0.6);
	
	float d = sdInter(d1, d0) + 
		sin(p1.y*2*PI*10.0f)/100.0f + 
		sin(p1.x*2*PI*10.0f)/500.0f + 
		sin(p1.z*2*PI*10.0f)/500.0f;

	return sdUnion(d, d2);
	// return d + sfDisp( p * 2.5 );
	// return sdUnion_s( d + sfDip( p * 2.5 * sin( iTime * 1.01 ) ), d1, 0.1 );
}

