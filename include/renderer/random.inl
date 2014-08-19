#include <cmath>

template< typename Generator >
glm::uvec3 uniformOnSphere( Generator& gen )
{
	// We use that the solid angle is constant here:
	// domega = sin(theta) * dtheta * dphi = const
	// Now u = cos(theta) -> du = sin(theta)*dtheta
	// ==> domega = du * dphi = const

	static std::uniform_real_distribution<float> phiDist( 0.f, 2.f * 6.2831853071795864769252f ); //[0, 2*Pi)
	static std::uniform_real_distribution<float> uDist( -1.f, 1.f ); //[-1, 1]

	float phi = phiDist( gen );
	float u = uDist( gen );

	// cos^2(phi) + sin^2(phi) = 1 (and u = cos(phi))
	// This way we can avoid to call arccos to calculate sin(phi)
	float sinephi = std::sqrt( 1 - u*u );

	// Return result in Cartesian coordinates.
	return glm::vec3( sinephi * std::cos( phi ), sinephi * std::sin( phi ), u);
}
