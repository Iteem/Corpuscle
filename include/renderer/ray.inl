inline Ray::Ray( glm::vec3 origin, glm::vec3 direction ) :
	m_origin( origin ),
	m_direction( direction ),
	m_inverseDirection( 1.f / direction )
{
}

inline glm::vec3 Ray::evaluate( float time ) const
{
	return m_origin + time * m_direction;
}

inline void Ray::setDirection( const glm::vec3& direction )
{
	m_direction = direction;
	m_inverseDirection = 1.f / direction;
}

inline glm::vec3 Ray::getDirection() const
{
	return m_direction;
}

inline glm::vec3 Ray::getInverseDirection() const
{
	return m_inverseDirection;
}

inline void Ray::setOrigin( const glm::vec3& origin )
{
	m_origin = origin;
}

inline glm::vec3 Ray::getOrigin() const
{
	return m_origin;
}
