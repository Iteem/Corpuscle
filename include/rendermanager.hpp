#ifndef RENDERMANAGER_HPP_INCLUDED
#define RENDERMANAGER_HPP_INCLUDED

#include <atomic>
#include <condition_variable>
#include <string>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

#include "scene.hpp"

class RenderManager
{
	public:
		RenderManager( sf::Vector2u dimension, unsigned int numThreads );
		~RenderManager();

		void setUpdateImage( bool updateImage );
		bool getUpdateImage() const;

		sf::Vector2u getDimension() const;
		void setDimension( sf::Vector2u dimension );

		bool loadSceneFromFile( const std::string& path );

		void startRendering();
		void stopRendering();

		bool isRendering() const;

		unsigned int getSamples() const;
		sf::Image getImage() const;

		void reset();
	private:
		void workerFunction();
		int getNextJob();

		Scene m_scene;

		sf::Vector2u m_dimension;
		std::atomic_uint m_samples;
		std::vector< sf::Vector3f > m_pixels;
		sf::Image m_image;
		std::atomic_bool m_updateImage;

		std::vector<std::thread> m_threads;
		mutable std::mutex m_imageMutex;
		std::mutex m_mutex;
		std::condition_variable m_conVar;

		std::atomic_uint m_currentLine;
		std::atomic_uint m_numRunningThreads;
		std::atomic_bool m_isRunning;
		unsigned int m_numThreads;

};


#endif // RENDERMANAGER_HPP_INCLUDED
